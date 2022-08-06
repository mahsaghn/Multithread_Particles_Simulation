#!/bin/bash

rm constants.h


printf "
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants
{
     const  int N {%s};
     const  int NPARTICLE {%s};
     const  int  ENERGY {5};
}
#endif" $1 $2 > constants.h 

# g++ p_main.cpp p_particle.cpp -o p.out -fopenmp
# ./p.out