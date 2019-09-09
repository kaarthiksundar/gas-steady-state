#include <input_params.h>

InputParams::InputParams(double temperature, double gas_specific_gravity, 
    double specific_heat_capacity_ratio, double fuel_factor, 
    double time_horizon, int eos, int units, int intervals, 
    int save_csv_output, int steady_state_solve_only, 
    double space_interval, double econ_weight, double max_iterations, 
    int tolerance_exponent, int objective_scale_exponent, 
    int extension_time_intervals, 
    int exit_if_steady_state_check_infeasible) {
        this->temperature = temperature;
        this->gas_specific_gravity = gas_specific_gravity;
        this->specific_heat_capacity_ratio = specific_heat_capacity_ratio;
        this->fuel_factor = fuel_factor;
        this->time_horizon = time_horizon;
        this->eos = eos; 
        this->units = units;
        this->intervals = intervals;
        this->save_csv_output = save_csv_output;
        this->steady_state_solve_only = steady_state_solve_only;
        this->space_interval = space_interval;
        this->econ_weight = econ_weight, 
        this->max_iterations = max_iterations;
        this->tolerance_exponent = tolerance_exponent;
        this->objective_scale_exponent = objective_scale_exponent;
        this->extension_time_intervals = extension_time_intervals;
        this->exit_if_steady_state_check_infeasible = exit_if_steady_state_check_infeasible;
};
