# HomeWork 1
Find the number of triangles at an Symmetrical Undericted Unweighted graph, and parallalezation with OpenMp and OpenCilk.

The C files were tested with the .mtx files in the folder,which were loaded with the mmio library.

Difference on the running time between the serial and the parallel code, was substant on the biggest Matrix File, "com-Youtube.mtx" containg 1,134,890 nodes and 5,975,248 Non zeros.\
It can be downloaded , at https://sparse.tamu.edu/SNAP/com-Youtube

The running time for the serial code, averaged at 13sec.
The OpenMP code, at 5.5sec.
The OpenCilk code, which was the most efficient at 5,2sec.


Compile Commands:

gcc -O3 -o main main.c mmio.c -Wno-unused-result
gcc -O3 -o main_mp main_mp.c mmio.c -fopenmp -Wno-unused-result
/usr/local/OpenCilk-9.0.1-Linux/bin/clang -O3 -o main_cilk main_cilk.c mmio.c -fcilkplus

Running Commands:

time ./main "com-Youtube.mtx"
time ./main_mp "com-Youtube.mtx"
time ./main_cilk "com-Youtube.mtx"

