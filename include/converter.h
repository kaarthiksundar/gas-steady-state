#ifndef CONVERTER_H
#define CONVERTER_H

#pragma once 

#include <string>
#include <input_params.h>
#include <network.h>

class Converter {
    public: 
        double psi_to_pascal;
        double km_to_m;
        double universal_R;
        double molecular_weight_air;
        double gas_R;
        double one_atm; 
        double mmscfd_to_kgps;
        double mmscfd_to_hp;
        double a;
        double miles_to_m;
        double inches_to_m;

        Converter();
        void populate_mmscfd_conversion_factors(const InputParams* ip);
        void convert_to_si(Network & net);
};



#endif 
