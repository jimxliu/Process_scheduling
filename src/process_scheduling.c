#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"
#define QUANTUM 4 // Used for Robin Round for process as the run time limit

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) {
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) {
	if(ready_queue && result){
		size_t size = dyn_array_size(ready_queue);
      		uint32_t latency_total = 0; // sum of latency time of all processes
		unsigned long run_total = 0; // sum of run time of all processes
		while(dyn_array_size(ready_queue) > 0){
			// pull the first object from back
			ProcessControlBlock_t *pcb = dyn_array_back(ready_queue); 
			if(!pcb){ // PCB can't be NULL
				return false;
			}	
			latency_total = latency_total + run_total; // the run time of all the previous processes is the waiting time of next-to-run process 
  			if(pcb->remaining_burst_time < 0){ // the remaining_burst_time of a process must be non-negative  
				return false;
			} 
			run_total = run_total + pcb->remaining_burst_time; // add the burst_time of current process to the total run time
			while(pcb->remaining_burst_time > 0){ // simulate the process running by counting down the run time of the process
				virtual_cpu(pcb);
			}
			if(!dyn_array_pop_back(ready_queue)){ // remove the process from ready_queue if finshed running
				return false;
			}
		}	
		result->average_latency_time = (float) latency_total/size; // average latency(waiting) time 
		result->average_wall_clock_time = (float) (latency_total + run_total)/size; // the time from arrival to completion is equal to waiting time plus run time
		result->total_run_time = run_total; // total run time	
		return true;
	}
	return false;
}

int compare_reversed_priority(const void *a, const void *b){ // compares the priority number of two PCB, the bigger number, the lower priority
	ProcessControlBlock_t *pa = (ProcessControlBlock_t *)a;	
	ProcessControlBlock_t *pb = (ProcessControlBlock_t *)b;
	return (int) (pb->priority - pa->priority);	
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) {
	if(ready_queue && result){
		if(dyn_array_sort(ready_queue, compare_reversed_priority)){ // sort the ready_queue in an increasing order of priority
			if(first_come_first_serve(ready_queue, result)){ // re-use the code of FCFS to calculate waiting, wall clock and run time.
				return true;
			}		
		}	
	}
   	return false;   
}

 #ifdef GRAD_TESTS
bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) {
    	if(ready_queue && result){
		size_t size = dyn_array_size(ready_queue); // total number of processes in the ready_queue
		size_t run_total = 0; // total run time
		size_t wall_clock = 0; // total wall clock time
		size_t i=0;
		while(dyn_array_size(ready_queue) > 0){
			size_t count = 0;
			ProcessControlBlock_t *pcb = dyn_array_back(ready_queue);
			while(pcb->remaining_burst_time > 0 && count < quantum){ // simulat running the process
				virtual_cpu(pcb);
				count++;
			}
			run_total = run_total + count; // add to total run time after each context switch 
			if(pcb->remaining_burst_time <= 0){  // when the process finishes, remove it from ready queue
				if(!dyn_array_pop_back(ready_queue)){
					return false;
				}
				wall_clock = wall_clock + run_total; // the wall clock time is from when ps arrives (time 0) til it finishes 
			} else {
				ProcessControlBlock_t * updated_pcb = malloc(sizeof(ProcessControlBlock_t)); // update the PCB
				if(!dyn_array_extract_back(ready_queue, updated_pcb) || !dyn_array_push_front(ready_queue, updated_pcb)){
					free(updated_pcb);	// place the PCB to the last location (front) of the ready queue
					return false;
				} else {
					free(updated_pcb);	
				}
			}					
		}
		result->average_latency_time = (float) (wall_clock - run_total)/size;
		result->average_wall_clock_time = (float) wall_clock/size;
		result->total_run_time = run_total;	
		return true;	
	}
	
	return false;
}
#endif

dyn_array_t *load_process_control_blocks(const char *input_file) {
	int fd = open(input_file,O_RDONLY);
	if(fd < 0){
		return NULL; // Couldn't open the input file
	}
	if(!strrchr(input_file, '.')){
		return NULL; // File doesn't have format
	}
	uint32_t pcb_num; // variable to record the number of processes
	if(read(fd, &pcb_num, sizeof(uint32_t)) != sizeof(uint32_t)){
		return NULL; // the file content is empty or wrong
	}
	uint32_t pcbs[pcb_num][2]; // array to contain the data of all processes from the file
	if(read(fd, pcbs, pcb_num * sizeof(uint32_t) * 2) != (pcb_num * sizeof(uint32_t) * 2)){ // check if the file PCB data size is wrong.
		return NULL; 
	}
	uint32_t leftover = 0; // Check if file is empty
	if(read(fd, &leftover, sizeof(uint32_t)) > 0){ 
		close(fd);
		return NULL; // if not, meaning some leftover data, incorrect PCB file.
	} 
	dyn_array_t * da = dyn_array_create(0, sizeof(ProcessControlBlock_t),NULL); // create a ready_queue 
	int i;
	for(i = 0; i < pcb_num; ++i){ // push all PCBs to the ready_queue
		ProcessControlBlock_t p = {pcbs[i][0], pcbs[i][1], 0};
		if(!dyn_array_push_back(da, &p)){
			return NULL;
		}		
	}
	return da;
}
