# matrix_multiplication
The aim of this project is to implement a matrix multiplier at the fastest rate. We will
attempt to maximize efficiency by implementing the matrix multiplication in
multithreading. We hope to build off of pd by also using blocking in order to maximize the
number of cache hits and increase the efficiency of the algorithm. 
We will also be measuring the efficiency of the program by comparing it against a naive
solution that does matrix multiplication on a row by column basis with a single thread. It
is also possible for us to compare our solution against a solution that uses
multithreading in a nonblocking manner (row by column basis), which we hypothesize will be
less efficient than a solution that uses both blocking and multithreading. 
