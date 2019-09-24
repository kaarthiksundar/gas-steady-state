#include <input_params.h>
#include <csv.h>
#include <files.h>
#include <iostream>

InputParams::InputParams(double temperature, double gas_specific_gravity, 
                         double specific_heat_capacity_ratio,
                         double time_horizon, int eos, int units, int intervals,
                         int save_csv_output, int steady_state_solve_only,
                         double space_interval, double econ_weight, double max_iterations,
                         int tolerance_exponent, int objective_scale_exponent,
                         int extension_time_intervals,
                         int exit_if_steady_state_check_infeasible) {
    _temperature = temperature;
    _gas_specific_gravity = gas_specific_gravity;
    _specific_heat_capacity_ratio = specific_heat_capacity_ratio;
    _time_horizon = time_horizon;
    _eos = eos;
    _units = units;
    _intervals = intervals;
    _save_csv_output = save_csv_output;
    _steady_state_solve_only = steady_state_solve_only;
    _space_interval = space_interval;
    _econ_weight = econ_weight,
    _max_iterations = max_iterations;
    _tolerance_exponent = tolerance_exponent;
    _objective_scale_exponent = objective_scale_exponent;
    _extension_time_intervals = extension_time_intervals;
    _exit_if_steady_state_check_infeasible = exit_if_steady_state_check_infeasible;
};

double InputParams::get_temperature() const { return _temperature; };
double InputParams::get_gas_specific_gravity() const { return _gas_specific_gravity; };
double InputParams::get_specific_heat_capacity() const { return _specific_heat_capacity_ratio; };
double InputParams::get_time_horizon() const { return _time_horizon; };
int InputParams::get_eos() const { return _eos; };
int InputParams::get_units() const { return _units; };
int InputParams::save_output_csv() const { return _save_csv_output; };
int InputParams::solve_steady_state_only() const { return _steady_state_solve_only; };
double InputParams::get_space_interval() const { return _space_interval; };
double InputParams::get_econ_weight() const { return _econ_weight; };
double InputParams::get_max_iterations() const { return _max_iterations; };
int InputParams::get_tolerance_exponent() const { return _tolerance_exponent; };
int InputParams::get_objective_scale_exponent() const { return _objective_scale_exponent; };
int InputParams::get_extension_time_intervals() const { return _extension_time_intervals; };
int InputParams::should_exit_if_steady_state_infeasible() const { return _exit_if_steady_state_check_infeasible; };

InputParams build_input_params(std::string path) {
    io::CSVReader<2, io::trim_chars<' '>, io::double_quote_escape<',','\"'> > in(path + InputFilenames::input_params);
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
    std::string parameter; double value;
    while (in.read_row(parameter, value)) {
        if (parameter.find("Temperature") != std::string::npos)
            temperature = value;
        if (parameter.find("Gas") != std::string::npos)
            gas_specific_gravity = value;
        if (parameter.find("Specific") != std::string::npos)
            specific_heat_capacity_ratio = value;
        if (parameter.find("Time horizon") != std::string::npos)
            time_horizon = value;
        if (parameter.find("EOS") != std::string::npos)
            eos = (int) value;
        if (parameter.find("units") != std::string::npos)
            units = (int) value;
        if (parameter.find("intervals") != std::string::npos)
            intervals = (int) value;
        if (parameter.find("save csv output") != std::string::npos)
            save_csv_output = (int) value;
        if (parameter.find("steady-state solve") != std::string::npos)
            steady_state_solve_only = (int) value;
        if (parameter.find("Space interval") != std::string::npos)
            space_interval = value;
        if (parameter.find("Econ weight") != std::string::npos)
            econ_weight = value;
        if (parameter.find("Max iterations") != std::string::npos)
            max_iterations = (int) value;
        if (parameter.find("Tolerance exponent") != std::string::npos)
            tolerance_exponent = (int) value;
        if (parameter.find("Objective scale exponent") != std::string::npos)
            objective_scale_exponent = (int) value;
        if (parameter.find("time horizon extension") != std::string::npos)
            extension_time_intervals = (int) value;
        if (parameter.find("exit") != std::string::npos)
            exit_if_steady_state_check_infeasible = (int) value;
    }
    /* return the built input parameters */
    return InputParams(temperature, gas_specific_gravity, specific_heat_capacity_ratio,
                       time_horizon, eos, units, intervals, save_csv_output,
                       steady_state_solve_only, space_interval, econ_weight,
                       max_iterations, tolerance_exponent, objective_scale_exponent,
                       extension_time_intervals, exit_if_steady_state_check_infeasible);
};

std::ostream& operator<<(std::ostream& os, const InputParams &ip) {
    os << "input_parameters(" << std::endl <<
    "temperature: " << ip._temperature << std::endl <<
    "gas specific gravity: " << ip._gas_specific_gravity << std::endl <<
    "specfic heat capacity ratio: " << ip._specific_heat_capacity_ratio << std::endl <<
    "time horizon: " << ip._time_horizon << std::endl <<
    "EOS: " << ip._eos << std::endl <<
    "units: " << ip._units << std::endl <<
    "intervals: " << ip._intervals << std::endl <<
    "save csv outputs: " << ip._save_csv_output << std::endl <<
    "steady state solve only: " << ip._steady_state_solve_only << std::endl <<
    "space interval: " << ip._space_interval << std::endl <<
    "econ weight: " << ip._econ_weight << std::endl <<
    "max iterations: " << ip._max_iterations << std::endl <<
    "tolerance exponent: " << ip._tolerance_exponent << std::endl <<
    "objective scale exponent: " << ip._objective_scale_exponent << std::endl <<
    "extension time intervals: " << ip._extension_time_intervals << std::endl <<
    "exit if steady state check infeasible: " << ip._exit_if_steady_state_check_infeasible << ")" << std::endl;
    return os;
};

