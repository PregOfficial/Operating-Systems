#include <math.h>
#include <stdio.h>

#include "scheduling_sim.h"
#include "scheduling_utility.h"

// ---------------------------------------------------------------------------
// Example schedulers
// ---------------------------------------------------------------------------

process_t* fcfs(int timestep, my_array* processes) {
	process_t* selected = my_array_get(processes, 0);

	return selected;
}

process_t* round_robin(int timestep, my_array* processes) {
	process_t* selected = my_array_get(processes, 0);
	// We are allowed to delete the scheduled process from the list.
	// It will be re-appended at the end (after any newly arrived processes).
	my_array_delete(processes, 0);
	return selected;
}

// ---------------------------------------------------------------------------
// Implement your schedulers here
// ---------------------------------------------------------------------------

process_t* srtn(int timestep, my_array* processes) {
	int shortest = 0;

	for(int i = 1; i < (int)my_array_size(processes); i++) {
		process_t* shortest_process = my_array_get(processes, shortest);
		process_t* current = my_array_get(processes, i);
		if(current->remaining_time < shortest_process->remaining_time) {
			shortest = i;
		}
	}

	process_t* selected = my_array_get(processes, shortest);

	return selected;
}

process_t* round_robin_4(int timestep, my_array* processes) {
	process_t* selected = my_array_get(processes, 0);

	selected->user1++;
	if(selected->user1 == 4) {
		my_array_delete(processes, 0);
		selected->user1 = 0;
	}
	return selected;
}

process_t* priority_preemption(int timestep, my_array* processes) {
	int highest_priority = 0;

	for(int i = 1; i < (int)my_array_size(processes); i++) {
		process_t* highest_priority_process = my_array_get(processes, highest_priority);
		process_t* current = my_array_get(processes, i);
		if(current->priority > highest_priority_process->priority) {
			highest_priority = i;
		}
	}

	process_t* selected = my_array_get(processes, highest_priority);

	return selected;
}

process_t* priority(int timestep, my_array* processes) {
	for(int i = 0; i < (int)my_array_size(processes); i++) {
		process_t* current = my_array_get(processes, i);

		if(current->user1) {
			return my_array_get(processes, i);
		}
	}

	int highest_priority = 0;

	for(int i = 1; i < (int)my_array_size(processes); i++) {
		process_t* highest_priority_process = my_array_get(processes, highest_priority);
		process_t* current = my_array_get(processes, i);

		if(current->priority > highest_priority_process->priority) {
			highest_priority = i;
		}
	}

	process_t* selected = my_array_get(processes, highest_priority);
	selected->user1 = 1;

	return selected;
}

// ---------------------------------------------------------------------------

int main(int argc, char** argv) {
	if(argc < 2) {
		fprintf(stderr, "Error: Usage: %s <filename>\nExample: %s input.csv\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	simulation_data_t* data = read_simulation_data(argv[1]);
	if(!data) {
		fprintf(stderr, "Error: Unable to open the file '%s' for reading!\n", argv[1]);
		return EXIT_FAILURE;
	}
	print_simulation_data(stdout, data);

	// -----------------------------------------------------------------------
	// Insert calls to your schedulers here
	// -----------------------------------------------------------------------

	print_schedule(stdout, compute_schedule(data, fcfs), data);
	print_schedule(stdout, compute_schedule(data, round_robin), data);
	print_schedule(stdout, compute_schedule(data, srtn), data);
	print_schedule(stdout, compute_schedule(data, round_robin_4), data);
	print_schedule(stdout, compute_schedule(data, priority_preemption), data);
	print_schedule(stdout, compute_schedule(data, priority), data);

	// -----------------------------------------------------------------------

	free_simulation_data(data);

	return EXIT_SUCCESS;
}
