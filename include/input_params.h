#ifndef INPUT_PARAMS_H
#define INPUT_PARAMS_H

#pragma once 

#include <string>

class InputParams {
    public: 
        double temperature;
        double gas_specific_gravity;
        double specific_heat_capacity_ratio; 
        double fuel_factor;
        double time_horizon;
        int eos;
        int units;
        int intervals; 
        int save_csv_output;
        int steady_state_solve_only;
        double space_interval;
        double econ_weight;
        double max_iterations;
        int tolerance_exponent;
        int objective_scale_exponent;
        int extension_time_intervals;
        int exit_if_steady_state_check_infeasible;
        
        InputParams(double temperature, double gas_specific_gravity, 
            double specific_heat_capacity_ratio, double fuel_factor, 
            double time_horizon, int eos, int units, int intervals, 
            int save_csv_output, int steady_state_solve_only, 
            double space_interval, double econ_weight, double max_iterations, 
            int tolerance_exponent, int objective_scale_exponent, 
            int extension_time_intervals, 
            int exit_if_steady_state_check_infeasible);
        ~InputParams();
};




#endif 