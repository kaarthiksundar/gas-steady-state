#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#pragma once

#include <input_params.h>

class ConversionFactors {
private:
    double _psi_to_pascal;
    double _km_to_m;
    double _universal_R;
    double _molecular_weight_air;
    double _gas_R;
    double _one_atm;
    double _mmscfd_to_kgps;
    double _hp_to_watts;
    double _a;
    double _miles_to_m;
    double _inches_to_m;
public:
    ConversionFactors(const InputParams &);
    
    double psi_to_pascal() const;
    double pascal_to_psi() const;
    double km_to_m() const;
    double m_to_km() const;
    double get_universal_R() const;
    double get_molecular_weight_air() const;
    double get_one_atm() const;
    double get_gas_R() const;
    double mmscfd_to_kgps() const;
    double kgps_to_mmscfd() const;
    double hp_to_watts() const;
    double watts_to_hp() const;
    double get_sound_speed() const;
    double miles_to_m() const;
    double m_to_miles() const;
    double inches_to_m() const;
    double m_to_inches() const;
};

class ScalingFactors {
private:
    double _pressure_scaling;
    double _flow_scaling;
    double _time_scaling;
    double _space_scaling;
public:
    ScalingFactors(double, double, double, double);
    
    double get_pressure_scaling() const;
    double get_flow_scaling() const;
    double get_time_scaling() const;
    double get_space_scaling() const;
};

ScalingFactors build_scaling_factors(double, const ConversionFactors &);



#endif


