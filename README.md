# PES_5813_Final_Project

## Program Counter profiler

A program counter profiling implementation using Systick interrupt and UART on the target side. On Host PC side a python script handles parsing to interpret captured addresses.

## Usage 
With the serial logs captured using a utility like Putty, we can use profiler.py to gather statistics of the program run time.

```
python3 profiler.py <binary> <serial log>
```

For example when executed on the Profiler demo project binary we get the below results
![image](https://user-images.githubusercontent.com/13454264/166407056-215858f7-1ad0-4740-875a-4aa995b6973f.png)

Similarly for the unoptimized PBKDF project binary used in Assignment 5 we get below results
![image](https://user-images.githubusercontent.com/13454264/166407130-d88a6409-77cd-404f-b307-ea15a7b388fc.png)
