#include <fcntl.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include <pthread.h>
#include "../include/processing_scheduling.h"
// Using a C library requires extern "C" to prevent function managling
extern "C" {
#include <dyn_array.h>
}


#define NUM_PCB 30
#define QUANTUM 4 // Used for Robin Round for process as the run time limit

unsigned int score;
unsigned int total;

class GradeEnvironment : public testing::Environment {
    public:
        virtual void SetUp() {
            score = 0;
#if GRAD_TESTS
            total = 280;

#else
            total = 220;
#endif
        }
        virtual void TearDown() {
            ::testing::Test::RecordProperty("points_given", score);
            ::testing::Test::RecordProperty("points_total", total);
            std::cout << "SCORE: " << score << '/' << total << std::endl;
        }
};

/*
 * * ROUND ROBIN TEST CASES
 * */

#if GRAD_TESTS

TEST (round_robin, nullInputProcessControlBlockDynArray) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = NULL;
    bool res = round_robin (pcbs,sr,QUANTUM);
    ASSERT_EQ(false,res);
    delete sr;

    score+=5;

}

TEST (round_robin, nullScheduleResult) {
    ScheduleResult_t *sr = NULL;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    bool res = round_robin (pcbs,sr,QUANTUM);
    ASSERT_EQ(false,res);
    dyn_array_destroy(pcbs);

    score+=5;

}

TEST (round_robin, goodInputA) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[3] = {
        [0] = {24,0,0},
        [1] = {3,0,0},
        [2] = {3,0,0}
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[0]);	
    bool res = round_robin (pcbs,sr,QUANTUM);	
    ASSERT_EQ(true,res);
    float answers[3] = {15.666667,5.666667,30};
    ASSERT_FLOAT_EQ(answers[0],sr->average_wall_clock_time);
    ASSERT_FLOAT_EQ(answers[1],sr->average_latency_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=10;
}

TEST (round_robin, goodInputB) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[3] = {
        [0] = {20,0,0},
        [1] = {5,0,0},
        [2] = {6,0,0}
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);		
    dyn_array_push_back(pcbs,&data[0]);	
    bool res = round_robin (pcbs,sr,QUANTUM);	
    ASSERT_EQ(true,res);
    float answers[3] = {22.333334,12,31};
    ASSERT_FLOAT_EQ(answers[0],sr->average_wall_clock_time);
    ASSERT_EQ(answers[1],sr->average_latency_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=10;
}

TEST (round_robin, goodInputC) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[4] = {
        [0] = {7,0,0},
        [1] = {4,0,0},
        [2] = {9,0,0},
		[3] = {11,0,0}
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[0]);	
    bool res = round_robin (pcbs,sr,QUANTUM);	
    ASSERT_EQ(true,res);
    float answers[3] = {21.5,13.75,31};
    ASSERT_FLOAT_EQ(answers[0],sr->average_wall_clock_time);
    ASSERT_FLOAT_EQ(answers[1],sr->average_latency_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=10;
}

TEST (round_robin, goodInputD) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[5] = {
        [0] = {15,0,0},
        [1] = {5,0,0},
        [2] = {11,0,0},
		[3] = {13,0,0},
		[4] = {8,0,0}
    };
    // back loading dyn_array, pull from the back
 	dyn_array_push_back(pcbs,&data[4]);    
	dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[0]);	
    bool res = round_robin (pcbs,sr,QUANTUM);	
    ASSERT_EQ(true,res);
    float answers[3] = {41.8,31.4,52};
    ASSERT_FLOAT_EQ(answers[0],sr->average_wall_clock_time);
    ASSERT_FLOAT_EQ(answers[1],sr->average_latency_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=10;
}

TEST (round_robin, goodInputE) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[5] = {
        [0] = {15,0,0},
        [1] = {5,0,0},
        [2] = {11,0,0},
		[3] = {13,0,0},
		[4] = {8,0,0}
    };
    // back loading dyn_array, pull from the back
 	dyn_array_push_back(pcbs,&data[4]);    
	dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[0]);	
    bool res = round_robin (pcbs,sr,6);	
    ASSERT_EQ(true,res);
    float answers[3] = {40.4,30,52};
    ASSERT_FLOAT_EQ(answers[0],sr->average_wall_clock_time);
    ASSERT_FLOAT_EQ(answers[1],sr->average_latency_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=10;
}


#endif 


/*
 *  * * Priority  TEST CASES
 *   * */

TEST (priority, nullInputProcessControlBlockDynArray) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = NULL;
    bool res = priority (pcbs,sr);
    ASSERT_EQ(false,res);
    delete sr;

    score+=5;
}

TEST (priority, nullScheduleResult) {
    ScheduleResult_t *sr = NULL;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    bool res = priority (pcbs,sr);
    ASSERT_EQ(false,res);
    dyn_array_destroy(pcbs);

    score+=5;
}

TEST (priority, goodInputA) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[3] = {
        [0] = {24,3,0},
        [1] = {3,1,0},
        [2] = {3,2,0}
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[0]);
    bool res = priority (pcbs,sr);
    ASSERT_EQ(true,res);
    float answers[3] = {13,3,30};
    ASSERT_EQ(answers[0],sr->average_wall_clock_time);
    ASSERT_EQ(answers[1],sr->average_latency_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=20;
}

TEST (priority, goodInputB) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[5] = {
        [0] = {10,3,0},
        [1] = {1,1,0},
        [2] = {2,4,0},
        [3] = {1,5,0},
        [4] = {5,2,0},
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[4]);
    dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[0]);
    bool res = priority (pcbs,sr);
    ASSERT_EQ(true,res);
    float answers[3] = {12,8.2,19};
    ASSERT_EQ(answers[0],sr->average_wall_clock_time);
    ASSERT_EQ(answers[1],sr->average_latency_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=20;
}
TEST (priority, goodInputC) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[3] = {
        [0] = {12,1,0},
        [1] = {3,3,0},
        [2] = {6,2,0}
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[0]);
    bool res = priority (pcbs,sr);
    ASSERT_EQ(true,res);
    float answers[3] = {17,10,21};
    ASSERT_EQ(answers[0],sr->average_wall_clock_time);
    ASSERT_EQ(answers[1],sr->average_latency_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=10;
}

TEST (priority, goodInputD) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[5] = {
        [0] = {15,2,0},
        [1] = {3,4,0},
        [2] = {7,1,0},
        [3] = {4,5,0},
        [4] = {8,3,0}
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[4]);
    dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[0]);
    bool res = priority (pcbs,sr);
    ASSERT_EQ(true,res);
    float answers[3] = {25.8,18.4,37};
    ASSERT_EQ(answers[0],sr->average_wall_clock_time);
    ASSERT_EQ(answers[1],sr->average_latency_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=10;
}

TEST (priority, goodInputE) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[10] = {
        [0] = {1,10,0},
        [1] = {2,9,0},
        [2] = {3,8,0},
        [3] = {4,7,0},
		[4] = {5,6,0},
		[5] = {6,5,0},
		[6] = {7,4,0},
		[7] = {8,3,0},
		[8] = {9,2,0},
		[9] = {10,1,0}
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[9]);
    dyn_array_push_back(pcbs,&data[8]);
    dyn_array_push_back(pcbs,&data[7]);
    dyn_array_push_back(pcbs,&data[6]);    
	dyn_array_push_back(pcbs,&data[5]);
    dyn_array_push_back(pcbs,&data[4]);
    dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[0]);
    bool res = priority (pcbs,sr);
    ASSERT_EQ(true,res);
    float answers[3] = {38.5,33,55};
    ASSERT_EQ(answers[0],sr->average_wall_clock_time);
    ASSERT_EQ(answers[1],sr->average_latency_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=10;
}

/*
 * * First Come First Serve  TEST CASES
 * */

TEST (first_come_first_serve, nullInputProcessControlBlockDynArray) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = NULL;
    bool res = first_come_first_serve (pcbs,sr);
    ASSERT_EQ(false,res);
    delete sr;

    score+=5;
}

TEST (first_come_first_serve, nullScheduleResult) {
    ScheduleResult_t *sr = NULL;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    bool res = first_come_first_serve (pcbs,sr);
    ASSERT_EQ(false,res);
    dyn_array_destroy(pcbs);

    score+=5;
}

TEST (first_come_first_serve, goodInputA) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[3] = {
        [0] = {24,0,0},
        [1] = {3,0,0},
        [2] = {3,0,0}
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);
    dyn_array_push_back(pcbs,&data[0]);	
    bool res = first_come_first_serve (pcbs,sr);	
    ASSERT_EQ(true,res);
    float answers[3] = {27,17,30};
    ASSERT_EQ(answers[0],sr->average_wall_clock_time);
    ASSERT_EQ(answers[1],sr->average_latency_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=20;
}

TEST (first_come_first_serve, goodInputB) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[4] = {
        [0] = {6,0,0},
        [1] = {8,0,0},
        [2] = {7,0,0},
        [3] = {3,0,0},
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);		
    dyn_array_push_back(pcbs,&data[0]);	
    bool res = first_come_first_serve (pcbs,sr);	
    ASSERT_EQ(true,res);
    float answers[3] = {16.25,10.25,24};
    ASSERT_EQ(answers[0],sr->average_wall_clock_time);
    ASSERT_EQ(answers[1],sr->average_latency_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=20;
}

TEST (first_come_first_serve, goodInputC) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[10] = {
        [0] = {1,0,0},
        [1] = {2,0,0},
        [2] = {3,0,0},
        [3] = {4,0,0},
		[4] = {5,0,0},
		[5] = {6,0,0},
        [6] = {7,0,0},
        [7] = {8,0,0},
		[8] = {9,0,0},
		[9] = {10,0,0}
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[9]);
    dyn_array_push_back(pcbs,&data[8]);    
	dyn_array_push_back(pcbs,&data[7]);
    dyn_array_push_back(pcbs,&data[6]);
    dyn_array_push_back(pcbs,&data[5]);		
    dyn_array_push_back(pcbs,&data[4]);
    dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);		
    dyn_array_push_back(pcbs,&data[0]);	
    bool res = first_come_first_serve (pcbs,sr);	
    ASSERT_EQ(true,res);
    float answers[3] = {22,16.5,55};
    ASSERT_EQ(answers[0],sr->average_wall_clock_time);
    ASSERT_EQ(answers[1],sr->average_latency_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=10;
}

TEST (first_come_first_serve, goodInputD) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[10] = {
        [0] = {10,0,0},
        [1] = {9,0,0},
        [2] = {8,0,0},
        [3] = {7,0,0},
		[4] = {6,0,0},
		[5] = {5,0,0},
        [6] = {4,0,0},
        [7] = {3,0,0},
		[8] = {2,0,0},
		[9] = {1,0,0}
    };
    // back loading dyn_array, pull from the back
    dyn_array_push_back(pcbs,&data[9]);
    dyn_array_push_back(pcbs,&data[8]);    
	dyn_array_push_back(pcbs,&data[7]);
    dyn_array_push_back(pcbs,&data[6]);
    dyn_array_push_back(pcbs,&data[5]);		
    dyn_array_push_back(pcbs,&data[4]);
    dyn_array_push_back(pcbs,&data[3]);
    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);		
    dyn_array_push_back(pcbs,&data[0]);	
    bool res = first_come_first_serve (pcbs,sr);	
    ASSERT_EQ(true,res);
    float answers[3] = {38.5,33,55};
    ASSERT_EQ(answers[0],sr->average_wall_clock_time);
    ASSERT_EQ(answers[1],sr->average_latency_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=10;
}

TEST (first_come_first_serve, goodInputE) {
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* pcbs = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    memset(sr,0,sizeof(ScheduleResult_t));
    // add PCBs now
    ProcessControlBlock_t data[10] = {
        [0] = {3,0,0},
        [1] = {3,0,0},
		[2] = {24,0,0}
    };
    // back loading dyn_array, pull from the back

    dyn_array_push_back(pcbs,&data[2]);
    dyn_array_push_back(pcbs,&data[1]);		
    dyn_array_push_back(pcbs,&data[0]);	
    bool res = first_come_first_serve (pcbs,sr);	
    ASSERT_EQ(true,res);
    float answers[3] = {13,3,30};
    ASSERT_EQ(answers[0],sr->average_wall_clock_time);
    ASSERT_EQ(answers[1],sr->average_latency_time);
    ASSERT_EQ(answers[2],sr->total_run_time);
    dyn_array_destroy(pcbs);
    delete sr;

    score+=10;
}

/*
 * LOAD PROCESS CONTROL BLOCKS TEST CASES
 */
TEST (load_process_control_blocks, nullFilePath) {
    dyn_array_t* da = load_process_control_blocks (NULL);
    ASSERT_EQ(da,(dyn_array_t*) NULL);

    score+=5;
}

TEST (load_process_control_blocks, notFoundFile) {

    dyn_array_t* da = load_process_control_blocks ("NotARealFile.Awesome");
    ASSERT_EQ(da,(dyn_array_t*)NULL);

    score+=5;
}

TEST (load_process_control_blocks, noFormatFile) {
	const char* fname = "noFormat";
 	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    int flags = O_CREAT | O_TRUNC | O_WRONLY;
    int fd = open(fname, flags, mode);
    close(fd);

    dyn_array_t* da = load_process_control_blocks (fname);
    ASSERT_EQ(da,(dyn_array_t*)NULL);

    score+=10;
}

TEST (load_process_control_blocks, emptyFoundFile) {
    const char* fname = "EMPTYFILE.DARN";
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    int flags = O_CREAT | O_TRUNC | O_WRONLY;
    int fd = open(fname, flags, mode);
    close(fd);
    dyn_array_t* da = load_process_control_blocks (fname);
    ASSERT_EQ(da,(dyn_array_t*)NULL);

    score+=5;
}

TEST (load_process_control_blocks, incorrectPCBFoundFile) {
    const char* fname = "CANYOUHANDLETHE.TRUTH";
    uint32_t pcb_num = 10;
    uint32_t pcbs[5][2] = {{1,1},{2,2},{3,3},{4,4},{5,5}};
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    int flags = O_CREAT | O_TRUNC | O_WRONLY;
    int fd = open(fname, flags, mode);
    write(fd,&pcb_num,sizeof(uint32_t));
    write(fd,pcbs,5 * sizeof(uint32_t)*2);
    close(fd);
    dyn_array_t* da = load_process_control_blocks (fname);
    ASSERT_EQ(da,(dyn_array_t*)NULL);

    score+=5;
}

TEST (load_process_control_blocks, overSizedPCBFoundFile) {
    const char* fname = "OVERFLOW.RULES";
    uint32_t pcb_num = 3;
    uint32_t pcbs[5][2] = {{1,1},{2,2},{3,3},{4,4},{5,5}};
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    int flags = O_CREAT | O_TRUNC | O_WRONLY;
    int fd = open(fname, flags, mode);
    write(fd,&pcb_num,sizeof(uint32_t));
    write(fd,pcbs,5 * sizeof(uint32_t)*2);
    close(fd);
    dyn_array_t* da = load_process_control_blocks (fname);
    ASSERT_EQ(da,(dyn_array_t*)NULL);

    score+=10;
}

TEST (load_process_control_blocks, fullFoundFile) {
    const char* fname = "../test/PCBs.bin";
    uint32_t pcb_num = NUM_PCB;
    uint32_t pcbs[NUM_PCB][2];
    for (uint32_t p = 0; p < pcb_num; ++p) {
        pcbs[p][0] = rand() % 15 + 1;
        pcbs[p][1] = p;
    }
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    int flags = O_CREAT | O_TRUNC | O_WRONLY;
    int fd = open(fname, flags, mode);
    write(fd,&pcb_num,sizeof(uint32_t));
    write(fd,pcbs,pcb_num * sizeof(uint32_t)*2);
    close(fd);
    dyn_array_t* da = load_process_control_blocks (fname);
    ASSERT_NE(da, (dyn_array_t*) NULL);
    for (size_t i = 0; i < dyn_array_size(da); ++i) {
        ProcessControlBlock_t * pPCB = (ProcessControlBlock_t *)dyn_array_at(da, i);
        ASSERT_EQ(pPCB->remaining_burst_time, pcbs[i][0]);
        ASSERT_EQ(pPCB->priority, pcbs[i][1]);
    }
    dyn_array_destroy(da);

    score+=10;
}

TEST (load_process_control_blocks, anotherFullFoundFile) {
    const char* fname = "../test/anotherPCBs.bin";
    uint32_t pcb_num = NUM_PCB*2;
    uint32_t pcbs[NUM_PCB*2][2];
    for (uint32_t p = 0; p < pcb_num; ++p) {
        pcbs[p][0] = rand() % 30 + 5;
        pcbs[p][1] = p;
    }
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    int flags = O_CREAT | O_TRUNC | O_WRONLY;
    int fd = open(fname, flags, mode);
    write(fd,&pcb_num,sizeof(uint32_t));
    write(fd,pcbs,pcb_num * sizeof(uint32_t)*2);
    close(fd);
    dyn_array_t* da = load_process_control_blocks (fname);
    ASSERT_NE(da, (dyn_array_t*) NULL);
    for (size_t i = 0; i < dyn_array_size(da); ++i) {
        ProcessControlBlock_t * pPCB = (ProcessControlBlock_t *)dyn_array_at(da, i);
        ASSERT_EQ(pPCB->remaining_burst_time, pcbs[i][0]);
        ASSERT_EQ(pPCB->priority, pcbs[i][1]);
    }
    dyn_array_destroy(da);

    score+=10;
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();

}


