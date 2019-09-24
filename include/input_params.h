#ifndef INPUT_PARAMS_H
#define INPUT_PARAMS_H

#pragma once
#include <ostream>

class InputParams {
private:
    double _temperature;
    double _gas_specific_gravity;
    double _specific_heat_capacity_ratio;
    double _time_horizon;
    int _eos;
    int _units;
    int _intervals;
    int _save_csv_output;
    int _steady_state_solve_only;
    double _space_interval;
    double _econ_weight;
    double _max_iterations;
    int _tolerance_exponent;
    int _objective_scale_exponent;
    int _extension_time_intervals;
    int _exit_if_steady_state_check_infeasible;
    
public:
    InputParams(double temperature, double gas_specific_gravity,
                double specific_heat_capacity_ratio,
                double time_horizon, int eos, int units, int intervals,
                int save_csv_output, int steady_state_solve_only,
                double space_interval, double econ_weight, double max_iterations,
                int tolerance_exponent, int objective_scale_exponent,
                int extension_time_intervals,
                int exit_if_steady_state_check_infeasible);
    
    double get_temperature() const;
    double get_gas_specific_gravity() const;
    double get_specific_heat_capacity() const;
    double get_time_horizon() const;
    int get_eos() const;
    int get_units() const;
    int get_intervals() const;
    int save_output_csv() const;
    int solve_steady_state_only() const;
    double get_space_interval() const;
    double get_econ_weight() const;
    double get_max_iterations() const;
    int get_tolerance_exponent() const;
    int get_objective_scale_exponent() const;
    int get_extension_time_intervals() const;
    int should_exit_if_steady_state_infeasible() const;
    
    friend std::ostream& operator<<(std::ostream& os, const InputParams &ip);
};

InputParams build_input_params(std::string path);

#endif 
