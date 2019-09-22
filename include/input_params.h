#ifndef INPUT_PARAMS_H
#define INPUT_PARAMS_H

#pragma once 

class InputParams {
private:
    double temperature;
    double gas_specific_gravity;
    double specific_heat_capacity_ratio;
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
    
public:
    InputParams(double temperature, double gas_specific_gravity,
                double specific_heat_capacity_ratio,
                double time_horizon, int eos, int units, int intervals,
                int save_csv_output, int steady_state_solve_only,
                double space_interval, double econ_weight, double max_iterations,
                int tolerance_exponent, int objective_scale_exponent,
                int extension_time_intervals,
                int exit_if_steady_state_check_infeasible);
    
    double get_temperature();
    double get_gas_specific_gravity();
    double get_specific_heat_capacity();
    double get_time_horizon();
    int get_eos();
    int get_units();
    int get_intervals();
    int save_output_csv();
    int solve_steady_state_only();
    double get_space_interval();
    double get_econ_weight();
    double get_max_iterations();
    int get_tolerance_exponent();
    int get_objective_scale_exponent();
    int get_extension_time_intervals();
    int should_exit_if_steady_state_infeasible();
};




#endif 
