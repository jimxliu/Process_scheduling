# Assignment 4

Milestone 0:  
    Due: Thursday, September 28 3:30 PM
- ALL Unit tests (function implementation not necessary)
    - CMake

    Milestone 1:  
    Due: Tuesday, October 3 3:30 PM
    - First Come First Served scheduling
    - PCB File Loading

    Milestone 2:  
    Due: Friday, October 6 5:00 PM
    - Priority scheduling
    - Scheduling statistics
    - Grad Students / Undergrad Bonus:
    - Round Robin scheduling

    ----  
    Add your scheduling algorithm analysis below this line  
    ----
    
### First Come First Serve        
    File: ../test/myPCBs.bin Algorithm: FCFS    
    Average wall lock time: 568.933350   
    Average latency time: 549.766663   
    Total run time: 1150    
    real   0m0.003s   
    user   0m0.001s   
    sys    0m0.002s

### Priority 
    File: ../test/myPCBs.bin Algorithm: Priority 
Average wall lock time: 600.233337
Average latency time: 581.066650
Total run time: 1150

real	0m0.003s
user	0m0.001s
sys		0m0.000s

###Round Robin
File: ../test/myPCBs.bin Algorithm: Round Robin Quantum: 10 
Average wall lock time: 800.716675
Average latency time: 781.549988
Total run time: 1150

real	0m0.002s
user	0m0.001s
sys	    0m0.001s



