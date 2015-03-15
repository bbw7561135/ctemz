# OpenMP implementation overview #

[OpenMP](http://openmp.org/wp/) is a library to facilitate multi-threading in C++ and Fortran programs. `g++` and `gdb` have built in support for it, so it is fairly easy to incorporate into a program that is multi-thread-ready.

In short, implementing OpenMP in a C++ program amounts to adding '#pragma omp' statements to the code, around the loop to be parallelized.

# Necessary Changes to `ctemz` #

To parallelize certain (frequency) loops in the code, the code inside that loop had to be "fixed" so that it could be executed by multiple processors/threads simultaneously without corrupting data or doing a different calculation than the single threaded version. The issues fell into two main categories:

  * **Shared variables:** Some variables had to be made local and the associated adjustments had to be made so that memory would not get garbled as more than one thread tried to modify the same memory location at the same time.
  * **Dependency on consecutive execution:** Basically, the code as originally written (i.e. the single-threaded code) relied on the frequencies being processed in consecutive order (lowest to highest). Some changes had to be made to these frequency loops so that a given frequency was processed correctly regardless of which frequency that thread had processed just before it.

## Specifics ##

Note that the Fortran numerical labels from temz.f were transferred to C++ comments in ctemz to make it easier to find the corresponding code in each code base. Four loops labeled 129, 1129, 95 and 195 were modified in `BlzSim::run()` to facilitate parallelization. It is best to take a look at the changes made to [blzsim.cpp](https://code.google.com/p/ctemz/source/list?path=/trunk/blzsim.cpp), starting with version [r92](https://code.google.com/p/ctemz/source/detail?r=92&path=/trunk/blzsim.cpp) and going up to approximately version [r115](https://code.google.com/p/ctemz/source/detail?r=115&path=/trunk/blzsim.cpp)

# OpenMP usage details #

Basically, using OpenMP in the code looks like this

```
#include <omp.h>

#pragma omp parallel shared(var1, var2), private(var3, var4)
{
  // code or loop to parallelize goes here.
}

```

And when compiling, use the `-fopenmp` option, as is done in `Makefile`:

```
 g++ -fopenmp blzsim.cpp
```

OpenMP support is also built into GNU Fortran, so integrating OpenMP into a Fortran program such as `temz.f` would be similar to what is described above. However, note that the same changes to `blzsim.cpp` to make it multi-thread-ready would have to be made to `temz.f`

The Fortran equivalent of the above would be:

```
!$OMP PARALLEL SHARED(var1, var2), PRIVATE(var3, var4)
      ! code or loop to parallelize goes here.
!$OMP END PARALLEL
```