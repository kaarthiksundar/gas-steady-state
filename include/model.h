#ifndef MODEL_H
#define MODEL_H

#pragma once 

#include <variable.h>
#include <constraint.h>
#include <func.h>
#include <term.h>
#include <string>


class Model {
private:
    std::string _name;
    std::vector<std::shared_ptr<Variable>> _variables;
    std::vector<std::shared_ptr<Constraint>> _constraints;
    std::shared_ptr<Func> _objective;
    int _num_variables;
    int _num_constraints;
    bool _has_objective;
    std::vector<double> _primal_solution;
    std::vector<double> _dual_solution;
    double _objective_value;
    int _solver_status;
    std::vector<std::pair<int, int>> _hessian_sparsity_pattern;
    
public:
    Model();
    Model(std::string);
    
    /* generic functions */
    std::string get_name();
    void set_name(std::string);
    bool has_objective();
    
    /* get_nlp_info functions */
    int get_num_variables();
    int get_num_constraints();
    int get_nnz_jacobian();
    int get_nnz_hessian_lag();
    
    /* get_bounds_info functions */
    std::tuple<double, double> get_variable_bounds(int);
    std::tuple<double, double> get_constraint_bounds(int);
    
    /* get_starting_point functions */
    double get_variable_start(int);
    
    /* eval_f functions */
    double evaluate_objective_function_value(const double *);
    
    /* eval_grad_f functions */
    std::vector<std::tuple<int, double>> evaluate_objective_gradient(const double *);
    
    /* eval_g functions */
    double evaluate_constraint_function_value(int, const double *);
    
    /* eval_jac_g functions */
    std::vector<int> get_constraint_sparsity(int);
    std::vector<std::tuple<int, double>> evaluate_constraint_gradient(int, const double *);
    
    /* eval_h functions (none so far - automatic hessian computations not implemented) */
    std::vector<std::pair<int, int>> get_hessian_sparsity_pattern();
    
    /* finalize_solution functions */
    void set_primal_solution(const double *);
    void set_solver_status(int);
    void set_dual_solution(const double *);
    void set_objective_value(double);
    
    /* final solution getters for non-ipopt classes */
    double get_primal_value(std::shared_ptr<Variable>);
    double get_dual_value(std::shared_ptr<Constraint>);
    double get_objective_value();
    int get_solver_status();
    
    /* model population functions */
    std::shared_ptr<Variable> add_variable(std::string);
    std::shared_ptr<Variable> add_variable();
    std::vector<std::shared_ptr<Variable>> add_variables(int);
    std::vector<std::shared_ptr<Variable>> add_variables(int, std::vector<std::string>);
    
    std::shared_ptr<Constraint> add_constraint(std::string);
    std::shared_ptr<Constraint> add_constraint();
    
    std::shared_ptr<Func> add_objective(std::string);
    std::shared_ptr<Func> add_objective();
};

#endif 
