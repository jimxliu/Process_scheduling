#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS" // First come first serve
#define P "P"  // Priority
#define RR "RR" // Round Robin

// Add and comment your analysis code in this function.
int main(int argc, char **argv) {
    	if (argc < 3) {
        	printf("Missing parameters \n %s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
        	return EXIT_FAILURE;
	} else if (argc == 3){ //If two parameters, assume either FCFS or P algorithm will be used.
		dyn_array_t * pcbs = load_process_control_blocks(argv[1]);
		if(!pcbs){
			printf("Input_File_Error");
			return EXIT_FAILURE;
		} else {	
			ScheduleResult_t *sr = malloc(sizeof(ScheduleResult_t));
			if(strcmp( argv[2],FCFS)==0){  // First come first serve
				if(first_come_first_serve(pcbs,sr)){
					printf("File: %s Algorithm: FCFS \n", argv[1]);
					printf("Average wall lock time: %f\n", sr->average_wall_clock_time);
					printf("Average latency time: %f\n", sr->average_latency_time);
					printf("Total run time: %lu \n", sr->total_run_time);
					free(sr);
					dyn_array_destroy(pcbs);
					return EXIT_SUCCESS;	
				} else {
					printf("FCFS_Error\n");
					free(sr);
					dyn_array_destroy(pcbs);
					return EXIT_FAILURE;
				}
			} else if(strcmp(argv[2],P)==0){ // Priority
				if(priority(pcbs,sr)){
					printf("File: %s Algorithm: Priority \n",argv[1]);
					printf("Average wall lock time: %f\n",sr->average_wall_clock_time);
					printf("Average latency time: %f\n",sr->average_latency_time);
					printf("Total run time: %lu\n", sr->total_run_time);
					free(sr);
					dyn_array_destroy(pcbs);
					return EXIT_SUCCESS;	
				} else {
					printf("Priority_Error\n");
					free(sr);
					dyn_array_destroy(pcbs);
					return EXIT_FAILURE;
				}
			} else {	
				free(sr);
				dyn_array_destroy(pcbs);
				if(strcmp(argv[2],RR)==0){
				    printf("Round Robin needs parameter 'quantum'\n %s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
				} else {
				    printf("Algorithm not found\n  %s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
				}
				return EXIT_FAILURE;	
			}
		}
	} else if (argc == 4){ // If three parameters, assume using RR.
		if(strcmp(argv[2],RR) == 0){ // Round Robin
			dyn_array_t * pcbs = load_process_control_blocks(argv[1]);
			ScheduleResult_t *sr = malloc(sizeof(ScheduleResult_t));
			if(round_robin(pcbs,sr,(size_t)atoi(argv[3]))){
				printf("File: %s Algorithm: Round Robin Quantum: %s \n",argv[1],argv[3]);
				printf("Average wall lock time: %f\n",sr->average_wall_clock_time);
				printf("Average latency time: %f\n",sr->average_latency_time);
				printf("Total run time: %lu\n", sr->total_run_time);
				free(sr);
				dyn_array_destroy(pcbs);
				return EXIT_SUCCESS;	
			} else {	
				free(sr);
				dyn_array_destroy(pcbs);
				printf("Input_File_Error");
				return EXIT_FAILURE;
			}	
		} else {
			printf("Algorithm not found\n  %s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
			return EXIT_FAILURE;
		}
	} else {
		printf("Too many parameters \n %s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
		return EXIT_FAILURE;
	}
		

    return EXIT_FAILURE;
}
