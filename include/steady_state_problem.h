#ifndef STEADY_STATE_PROBLEM_H
#define STEADY_STATE_PROBLEM_H

#pragma once 

#include <model.h>
#include <steady_state_data.h>
#include <data.h>
#include <memory>
#include <input_params.h>

class SteadyStateProblem {
private:
    Model _model;
    const SteadyStateData & _ssd;
    std::unordered_map<std::string, std::unordered_map<int, std::shared_ptr<Variable>>> _variables;
    std::unordered_map<std::string, std::unordered_map<int, std::shared_ptr<Constraint>>> _constraints;
    std::shared_ptr<Func> _objective;
    std::unordered_map<std::string, std::unordered_map<int, double>> _primal_values;
    std::unordered_map<std::string, std::unordered_map<int, double>> _dual_values;
    double _objective_value;

public:
    SteadyStateProblem(std::string, const SteadyStateData &);
    
    Model & get_model();
    std::shared_ptr<Variable> rho(int);
    std::shared_ptr<Variable> phi_p(int);
    std::shared_ptr<Variable> phi_c(int);
    std::shared_ptr<Variable> alpha(int);
    std::shared_ptr<Variable> s(int);
    std::shared_ptr<Variable> d(int);
    std::shared_ptr<Variable> slack_production(int);
    std::shared_ptr<Constraint> nodal_balance(int);
    std::shared_ptr<Constraint> pipe_physics(int);
    std::shared_ptr<Constraint> compressor_physics(int);
    std::shared_ptr<Constraint> compressor_bounds(int);
    std::shared_ptr<Constraint> compressor_power_bounds(int);
    std::shared_ptr<Func> objective();
    
    double get_rho(int);
    double get_phi_p(int);
    double get_phi_c(int);
    double get_alpha(int);
    double get_s(int);
    double get_d(int);
    double get_slack_production(int);
    double get_nodal_balance(int);
    double get_pipe_physics(int);
    double get_compressor_physics(int);
    double get_compressor_bounds(int);
    double get_compressor_power_bounds(int);
    double get_objective_value();
    
    
    void add_variables();
    void add_constraints(const InputParams &);
    void add_objective(const InputParams &);
    
private:
    /* variables */
    void add_rho_variables();
    void add_phi_variables();
    void add_alpha_variables();
    void add_production_consumption_variables();
    void add_slack_production_variables();
    /* constraints */
    void add_nodal_balance_constraints();
    void add_pipe_physics_constraints();
    void add_compressor_physics_constraints();
    void add_compressor_discharge_pressure_bounds();
    void add_compressor_power_bounds(const InputParams &);
};

#endif 
