#include <input_params.h>

InputParams::InputParams(double temperature, double gas_specific_gravity, 
    double specific_heat_capacity_ratio, double fuel_factor, 
    double time_horizon, int eos, int units, int intervals, 
    int save_csv_output, int steady_state_solve_only, 
    double space_interval, double econ_weight, double max_iterations, 
    int tolerance_exponent, int objective_scale_exponent, 
    int extension_time_intervals, 
    int exit_if_steady_state_check_infeasible) {
        temperature = temperature;
        gas_specific_gravity = gas_specific_gravity;
        specific_heat_capacity_ratio = specific_heat_capacity_ratio;
        fuel_factor = fuel_factor;
        time_horizon = time_horizon;
        eos = eos; 
        units = units;
        intervals = intervals;
        save_csv_output = save_csv_output;
        steady_state_solve_only = steady_state_solve_only;
        space_interval = space_interval;
        econ_weight = econ_weight, 
        max_iterations = max_iterations;
        tolerance_exponent = tolerance_exponent;
        objective_scale_exponent = objective_scale_exponent;
        extension_time_intervals = extension_time_intervals;
        exit_if_steady_state_check_infeasible = exit_if_steady_state_check_infeasible;
};

InputParams::~InputParams() {};