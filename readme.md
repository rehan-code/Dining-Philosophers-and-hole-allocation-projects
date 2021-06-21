# Student Details
    Student Name: Rehan Nagoor Mohideen
    Student ID: 110592

## compilation
    Compile using: make all
    
### Dine
    Compile using: make dine
    Usage: ./dine [num. of philosophers] [num. of times each one eats]

### Holes
    Compile using: make holes
    Usage: ./holes [file name] [allocation stategy: first, best, next, worst]
    Notes: -Expect file to be formatted with letter space number in every line with no lines.
           -My next function start looking for a hole to place the next process based on the previous hole but if it couldnt find a hole large enough for the previous process then is swaps processes and start finding a hole from 0. 

           -I use a Process data structure. See structure at the start of the holes.c file.
           -I use a 128 array of pointers to Process structures to simulate the memory, where each memory location is either NULL(empty) or contains the pointer to the process using it.
           -I use a heap (same as last assignment) for my queue for processes to enter the memory. The queue is an array of pointers to Process structures as well.
           -The program basically puts all of the processes in the queue in the order it is in the file and then puts each one in memory indexes of the appropriate size and removes it and puts it back to the end of the queue when it is swapped.