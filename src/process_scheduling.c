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
      		uint32_t latency_total = 0;
		unsigned long run_total = 0;
		while(dyn_array_size(ready_queue) > 0){
			// pull the first object from back
			ProcessControlBlock_t *pcb = dyn_array_back(ready_queue); 
			if(!pcb){
				return false;
			}	
			latency_total = latency_total + run_total;
  			if(pcb->remaining_burst_time < 0){
				return false;
			}
			run_total = run_total + pcb->remaining_burst_time;
			while(pcb->remaining_burst_time > 0){
				virtual_cpu(pcb);
			}
			if(!dyn_array_pop_back(ready_queue)){
				return false;
			}
		}	
		result->average_latency_time = (float) latency_total/size;
		result->average_wall_clock_time = (float) (latency_total + run_total)/size;
		result->total_run_time = run_total;	
		return true;
	}
	return false;
}

int compare_reversed_priority(const void *a, const void *b){
	ProcessControlBlock_t *pa = (ProcessControlBlock_t *)a;	
	ProcessControlBlock_t *pb = (ProcessControlBlock_t *)b;
	return (int) (pb->priority - pa->priority);	
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) {
	if(ready_queue && result){
		if(dyn_array_sort(ready_queue, compare_reversed_priority)){
			if(first_come_first_serve(ready_queue, result)){
				return true;
			}		
		}	
	}
   	return false;   
}

#ifdef GRAD_TESTS
bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) {
    	if(ready_queue && result){
		size_t size = dyn_array_size(ready_queue);
		size_t run_total = 0;
		size_t wall_clock = 0;
		size_t i=0;
		while(dyn_array_size(ready_queue) > 0){
			size_t count = 0;
			ProcessControlBlock_t *pcb = dyn_array_back(ready_queue);
			while(pcb->remaining_burst_time > 0 && count < quantum){
				virtual_cpu(pcb);
				count++;
			}
			run_total = run_total + count;
			if(pcb->remaining_burst_time <= 0){
				if(!dyn_array_pop_back(ready_queue)){
					return false;
				}
				wall_clock = wall_clock + run_total;
			} else {
				ProcessControlBlock_t * updated_pcb = malloc(sizeof(ProcessControlBlock_t));
				if(!dyn_array_extract_back(ready_queue, updated_pcb) || !dyn_array_push_front(ready_queue, updated_pcb)){
					free(updated_pcb);	
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
	uint32_t pcb_num;
	if(read(fd, &pcb_num, sizeof(uint32_t)) != sizeof(uint32_t)){
		return NULL; // the file content is empty or wrong
	}
	uint32_t pcbs[pcb_num][2];
	if(read(fd, pcbs, pcb_num * sizeof(uint32_t) * 2) != (pcb_num * sizeof(uint32_t) * 2)){ // the file PCB data size is wrong.
		return NULL; 
	}
	uint32_t leftover = 0; // Check if file is empty
	if(read(fd, &leftover, sizeof(uint32_t)) > 0){
		close(fd);
		return NULL; // if not, meaning some leftover data, incorrect PCB file.
	}
	dyn_array_t * da = dyn_array_create(0, sizeof(ProcessControlBlock_t),NULL); 
	int i;
	for(i = 0; i < pcb_num; ++i){
		ProcessControlBlock_t p = {pcbs[i][0], pcbs[i][1], 0};
		if(!dyn_array_push_back(da, &p)){
			return NULL;
		}		
	}
	return da;
}
