#include<iostream>
#include <omp.h>

int main(){
    std::cout << "Max OpenMP threads available: " << omp_get_max_threads() << "\n";
    std::cout<< "version of c++: " << __cplusplus << std::endl;
    return 0;
}