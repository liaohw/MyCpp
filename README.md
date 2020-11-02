## my c++ codes

=======================
### common 
* base function of c++

### split xdr

### cpu_mem
* Brief:
    > Get CPU and memory information by C++

* Compile:
    > `g++ GetCpuMem.cpp -o test -pthread`

    > trace_log Compile: 

    > `g++ GetCpuMem.cpp -o test -pthread -DTRACE_LOG`

* Running:
```
---------------------process:
[PID:32539]GetCpuMem: %CPU:0.0  MEM:1MB
[PID:32539]GetCpuMem: %CPU:0.0  MEM:2MB
[PID:32539]GetCpuMem: %CPU:0.0  MEM:3MB

---------------------thread:
PID(32539) create thread ThreadTest with TID(32543)
PID(32539) create thread ThreadTest with TID(32544)
PID(32539) create thread ThreadTest with TID(32545)
[TID:32543]GetCpuMem: %CPU:0.6  MEM:13MB
[TID:32544]GetCpuMem: %CPU:1.0  MEM:23MB
[TID:32545]GetCpuMem: %CPU:0.6  MEM:33MB
[TID:32543]GetCpuMem: %CPU:1.2  MEM:43MB
[TID:32544]GetCpuMem: %CPU:1.2  MEM:3MB
[TID:32545]GetCpuMem: %CPU:1.0  MEM:63MB
[TID:32543]GetCpuMem: %CPU:1.4  MEM:73MB
[TID:32544]GetCpuMem: %CPU:1.4  MEM5:83MB
[TID:32545]GetCpuMem: %CPU:1.2  MEM:93MB
[TID:32543]GetCpuMem: %CPU:1.4  MEM:103MB
[TID:32544]GetCpuMem: %CPU:1.4  MEM:113MB
[TID:32545]GetCpuMem: %CPU:1.2  MEM:123MB
[TID:32543]GetCpuMem: %CPU:1.4  MEM:133MB
TID(32543) pthread_exit!
[TID:32544]GetCpuMem: %CPU:1.5  MEM:143MB
TID(32544) pthread_exit!
[TID:32545]GetCpuMem: %CPU:1.3  MEM:153MB
TID(32545) pthread_exit!

---------------------ending
```

