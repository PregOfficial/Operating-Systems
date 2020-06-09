#include "trains.h"

#define TRAINS 7
#define STATIONS 3

station_t stations[STATIONS];
time_t startTime;
pthread_mutex_t mutex;
pthread_t threads[TRAINS];

bool quit = false;

int main(void) {

	for(int i = 0; i < STATIONS; i++) {
		station_t station = { .number = i + 1, .train = NULL };
		stations[i] = station;
	}

	startTime = time(NULL);
	train_t trains[TRAINS] = { { .number = 1, .arrivalTime = 0, .stopTime = 8 },
		                       { .number = 2, .arrivalTime = 0, .stopTime = 4 },
		                       { .number = 3, .arrivalTime = 2, .stopTime = 3 },
		                       { .number = 4, .arrivalTime = 3, .stopTime = 7 },
		                       { .number = 5, .arrivalTime = 5, .stopTime = 3 },
		                       { .number = 6, .arrivalTime = 6, .stopTime = 5 },
		                       { .number = 7, .arrivalTime = 7, .stopTime = 2 } };

	signal(SIGINT, sigint_handler);

	if(pthread_mutex_init(&mutex, NULL) != 0) {
		perror("error init mutex");
		exit(1);
	}
	for(int i = 0; i < TRAINS; i++) {
		pthread_mutex_lock(&mutex);
		pthread_create(&threads[i], NULL, train_code, &trains[i]);
		pthread_mutex_unlock(&mutex);
	}

	for(int i = 0; i < TRAINS; i++) {
		pthread_join(threads[i], NULL);
	}

	if(pthread_mutex_destroy(&mutex) != 0) {
		perror("error destroy mutex");
		exit(1);
	}

	if(quit) {
		printf("\n");
		for(int i = 0; i < STATIONS; i++) {
			if(stations[i].train != NULL)
				printf("Platform %d: Train %d\n", stations[i].number, stations[i].train->number);
		}
	}

	return EXIT_SUCCESS;
}

void* train_code(void* input) {
	train_t* train = (train_t*)input;

	sleep(train->arrivalTime);
	int counter = 0;
	station_t* current_station = NULL;

	while(1) {
		pthread_mutex_lock(&mutex);
		station_t* station = &stations[counter];
		if(station->train == NULL) {
			station->train = train;
			current_station = station;
			break;
		}
		counter = (counter + 1) % STATIONS;
		pthread_mutex_unlock(&mutex);
	}
	pthread_mutex_unlock(&mutex);
	time_t now = time(NULL);

	printf("%2.f seconds after the start: train %d moves into station at platform %d\n",
	       difftime(now, startTime), train->number, current_station->number);

	sleep(train->stopTime);

	now = time(NULL);

	pthread_mutex_lock(&mutex);
	printf("%2.f seconds after the start: train %d leaves the station at platform %d\n",
	       difftime(now, startTime), train->number, current_station->number);
	current_station->train = NULL;
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}

static void sigint_handler() {
	for(int i = 0; i < TRAINS; i++) {
		pthread_cancel(threads[i]);
	}
	quit = true;
}