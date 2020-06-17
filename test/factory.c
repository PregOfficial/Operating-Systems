#include "factory.h"

pthread_mutex_t mutex;

int current_doors = 0;
int current_knobs = 0;

int main(int argc, char* argv[]) {
	if(argc != 3) {
		fprintf(stderr, "Usage: %s <enable load balancing> <numbers of doors>\n", argv[0]);
		return EXIT_FAILURE;
	}

	int load_balancing = atoi(argv[1]);
	int num_doors = atoi(argv[2]);

	int current_door_workers = WORKERS / 2;
	int current_knob_workers = WORKERS / 2;

	pthread_t threads[WORKERS];
	worker_t workers[WORKERS];

	if(pthread_mutex_init(&mutex, NULL) < 0) {
		perror("error mutex");
		return EXIT_FAILURE;
	}

	for(int i = 0; i < WORKERS; i++) {
		if(i < WORKERS / 2) {
			worker_t worker = { .create_door = CREATE_DOOR };
			workers[i] = worker;
		} else {
			worker_t worker = { .create_door = CREATE_KNOB };
			workers[i] = worker;
		}

		pthread_mutex_lock(&mutex);
		pthread_create(&threads[i], NULL, worker_code, &workers[i]);
		pthread_mutex_unlock(&mutex);
	}

	int elapsed_time = 0;
	while(current_doors < num_doors) {
		sleep(1);
		elapsed_time++;

		printf("producing %.2f knobs/s, %.2f doors/s\n", (double)current_knobs / elapsed_time,
		       (double)current_doors / elapsed_time);

		if(load_balancing) {
			pthread_mutex_lock(&mutex);
			if((double)current_doors / (double)current_knobs > 1.1) {
				worker_t* worker = find_worker(workers, CREATE_DOOR);
				if(worker != NULL) {
					worker->create_door = CREATE_KNOB;
					current_door_workers--;
					current_knob_workers++;
					printf("\tWorkers reassigned: %d making knobs, %d making doors\n",
					       current_knob_workers, current_door_workers);
				}
			} else if((double)current_knobs / (double)current_doors > 1.1) {
				worker_t* worker = find_worker(workers, CREATE_KNOB);
				if(worker != NULL) {
					worker->create_door = CREATE_DOOR;
					current_door_workers++;
					current_knob_workers--;
					printf("\tWorkers reassigned: %d making knobs, %d making doors\n",
					       current_knob_workers, current_door_workers);
				}
			}
			pthread_mutex_unlock(&mutex);
		}
	}

	for(int i = 0; i < WORKERS; i++) {
		pthread_cancel(threads[i]);
	}

	printf("%d doors produced in %d seconds (%.2f doors/s)\n", current_doors, elapsed_time,
	       (double)current_doors / elapsed_time);
	printf("%d knobs produced in %d seconds (%.2f knbos/s)\n", current_knobs, elapsed_time,
	       (double)current_knobs / elapsed_time);

	if(pthread_mutex_destroy(&mutex) < 0) {
		perror("error destroy mutex");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

static worker_t* find_worker(worker_t workers[], int type) {
	for(int i = 0; i < WORKERS; i++) {
		if(workers[i].create_door == type) {
			return &workers[i];
		}
	}

	return NULL;
}

static void* worker_code(void* input) {
	worker_t* worker = (worker_t*)input;
	unsigned seed = (unsigned)pthread_self();
	double lazyness = (double)(rand_r(&seed) % 11) / 10.0;

	while(1) {
		pthread_testcancel();

		if(worker->create_door == CREATE_DOOR) {
			// create door
			double time = (10 + lazyness * 90) * 1000.0;
			usleep(time);
			pthread_mutex_lock(&mutex);
			if(current_knobs - current_doors > 0) {
				current_doors++;
			}
		} else if(worker->create_door == CREATE_KNOB) {
			// create knob
			double time = (10 + lazyness * 30) * 1000.0;
			usleep(time);
			pthread_mutex_lock(&mutex);
			current_knobs++;
		}

		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(NULL);
}