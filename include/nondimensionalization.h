#ifndef NONDIMENSIONALIZATION_H
#define NONDIMENSIONALIZATION_H

#pragma once 

#include <string>
#include <network.h>
#include <converter.h>

class Nondimensionalization {
public:
    double p_factor;
    double flow_factor;
    double space_factor;
    double time_factor;
    
    Nondimensionalization(const Network & net, const Converter & converter);
    
    void non_dimensionalize(Network & net);
    void dimensionalize(Network & net);
};



#endif 
