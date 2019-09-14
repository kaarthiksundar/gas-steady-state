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
        std::vector<double> _solution;
        double _objective_value;
        int _solve_status;

    public:
        Model();
        Model(std::string);

        std::shared_ptr<Variable> add_variable(std::string);
        std::shared_ptr<Variable> add_variable();
        std::vector<std::shared_ptr<Variable>> add_variables(int);
        std::vector<std::shared_ptr<Variable>> add_variables(int, std::vector<std::string>);
        
        std::shared_ptr<Constraint> add_constraint(std::string);
        std::shared_ptr<Constraint> add_constraint();

        std::shared_ptr<Func> add_objective(std::string);
        std::shared_ptr<Func> add_objective();

        std::vector<double> get_solution();
        double get_value(int);
        double get_objective_value();
        double get_value(std::shared_ptr<Variable>);
        int get_solve_status();
        int get_num_variables();
        int get_num_constraints();
        std::string get_name();
        std::tuple<double, double> get_variable_bounds(int i);
        std::tuple<double, double> get_variable_bounds(std::shared_ptr<Variable> var);
    double get_variable_start(int i);
    double get_variable_start(std::shared_ptr<Variable> var);
        std::tuple<double, double> get_constraint_bounds(int i);
        std::tuple<double, double> get_constraint_bounds(std::shared_ptr<Constraint> constraint);
        double evaluate_objective_function_value(const double *);
        double evaluate_constraint_function_value(int, const double *);
        double evaluate_constraint_function_value(std::shared_ptr<Constraint>, const double *);
        std::vector<std::tuple<int, double>> evaluate_objective_gradient(const double *);
        std::vector<std::tuple<int, double>> evaluate_constraint_gradient(int, const double *);
        std::vector<std::tuple<int, double>> evaluate_constraint_gradient(std::shared_ptr<Constraint>, const double *);
        std::vector<int> get_constraint_sparsity(int);
        std::vector<int> get_constraint_sparsity(std::shared_ptr<Constraint>);
        
    
        int get_nnz_jacobian();
        int get_nnz_hessian_lag();

        void set_solution(const double *);
        void set_objective_value(double);
        void set_solve_status(int);
        void set_name(std::string);
};

#endif 
