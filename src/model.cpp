#include <model.h>

#include <iostream>

Model::Model() :
    _name(""), 
    _variables(), 
    _constraints(), 
    _objective(),
    _num_variables(0), 
    _num_constraints(0), 
    _has_objective(false), 
    _solution(), 
    _objective_value(), 
    _solve_status()
    {};

Model::Model(std::string name) :
    _name(name), 
    _variables(), 
    _constraints(), 
    _objective(),
    _num_variables(0), 
    _num_constraints(0), 
    _has_objective(false), 
    _solution(), 
    _objective_value(), 
    _solve_status()
    {};

std::shared_ptr<Variable> Model::add_variable(std::string name) {
    auto var = std::make_shared<Variable>(name);
    var->set_id(_variables.size());
    _variables.push_back(var);
    _num_variables = _variables.size();
    return var;
};

std::shared_ptr<Variable> Model::add_variable() {
    auto var = std::make_shared<Variable>();
    var->set_id(_variables.size());
    _variables.push_back(var);
    _num_variables = _variables.size();
    return var;
};

std::vector<std::shared_ptr<Variable>> Model::add_variables(int n) {
    std::vector<std::shared_ptr<Variable>> vars;
    for (auto i=0; i<n; ++i) {
        auto var = std::make_shared<Variable>();
        var->set_id(_variables.size());
        vars.push_back(var);
        _variables.push_back(var);
    }
    _num_variables = _variables.size();
    return vars;
};      

std::vector<std::shared_ptr<Variable>> Model::add_variables(int n, std::vector<std::string> names) {
    if (names.size() != n) {
        std::cerr << "variable names count not matching number of variables" << std::endl;
        std::exit(1);
    }
    std::vector<std::shared_ptr<Variable>> vars;
    for (auto i=0; i<n; ++i) {
        auto var = std::make_shared<Variable>(names[i]);
        var->set_id(_variables.size());
        vars.push_back(var);
        _variables.push_back(var);
    }
    _num_variables = _variables.size();
    return vars;
};

std::shared_ptr<Constraint> Model::add_constraint() { 
    auto constraint = std::make_shared<Constraint>();
    _constraints.push_back(constraint);
    _num_constraints = _constraints.size();
    return constraint;
};

std::shared_ptr<Constraint> Model::add_constraint(std::string name) {
    auto constraint = std::make_shared<Constraint>(name);
    _constraints.push_back(constraint);
    _num_constraints = _constraints.size();
    return constraint;
};

std::shared_ptr<Func> Model::add_objective(std::string name) {
    _objective = std::make_shared<Func>(name);
    _has_objective = true;
    return _objective;
};

std::shared_ptr<Func> Model::add_objective() {
    _objective = std::make_shared<Func>();
    _has_objective = true;
    return _objective;
};

std::vector<double> Model::get_solution() {
    if (_solution.size() == 0) {
        std::cerr << "solution not populated " << std::endl;
        std::exit(1);
    }
    return _solution;
};

double Model::get_value(int i) {
    if (_solution.size() == 0) {
        std::cerr << "solution not populated " << std::endl;
        std::exit(1);
    }
    return _solution.at(i);
};

double Model::get_objective_value() {
    if (_solution.size() == 0) {
        std::cerr << "solution not populated " << std::endl;
        std::exit(1);
    }
    return _objective_value;
};

double Model::get_value(std::shared_ptr<Variable> var) {
    if (_solution.size() == 0) {
        std::cerr << "solution not populated " << std::endl;
        std::exit(1);
    }
    return _solution.at(var->get_id());
};

int Model::get_num_variables() { return _num_variables; };
int Model::get_num_constraints() { return _num_constraints; };
std::string Model::get_name() { return _name; };
int Model::get_solve_status() { return _solve_status; };
double Model::get_variable_start(int i) { return _variables[i]->get_start(); };
double Model::get_variable_start(std::shared_ptr<Variable> var) { return var->get_start(); };
std::tuple<double, double> Model::get_variable_bounds(int i) { return _variables[i]->get_bounds(); };
std::tuple<double, double> Model::get_variable_bounds(std::shared_ptr<Variable> var) { return var->get_bounds(); };
std::tuple<double, double> Model::get_constraint_bounds(int i) { return _constraints[i]->get_bounds(); };
std::tuple<double, double> Model::get_constraint_bounds(std::shared_ptr<Constraint> constraint) {
    return constraint->get_bounds();
};

double Model::evaluate_objective_function_value(const double * vals) { return _objective->get_value(vals); };
double Model::evaluate_constraint_function_value(int i, const double * vals) {
    return _constraints[i]->get_value(vals);
};

double Model::evaluate_constraint_function_value(std::shared_ptr<Constraint> constraint, const double * vals) {
    return constraint->get_value(vals);
};

std::vector<std::tuple<int, double>> Model::evaluate_objective_gradient(const double * vals) {
    return _objective->get_gradient(get_num_variables(), vals);
};

std::vector<std::tuple<int, double>> Model::evaluate_constraint_gradient(int i, const double * vals) {
    return _constraints[i]->get_gradient(get_num_variables(), vals);
};

std::vector<std::tuple<int, double>> Model::evaluate_constraint_gradient(std::shared_ptr<Constraint> constraint, const double * vals) {
    return constraint->get_gradient(get_num_variables(), vals);
};

std::vector<int> Model::get_constraint_sparsity(int i) {
    return _constraints[i]->get_gradient_sparsity(get_num_variables());
};

std::vector<int> Model::get_constraint_sparsity(std::shared_ptr<Constraint> constraint) {
    return constraint->get_gradient_sparsity(get_num_variables());
};

int Model::get_nnz_jacobian() {
    int nnz = 0;
    for (auto constraint : _constraints)
        nnz += constraint->get_num_variables();
    return nnz;
};

int Model::get_nnz_hessian_lag() {
    int nnz;
    auto variables_with_nz_second_derivative = _objective->get_variable_ids_with_nz_second_derivative();
    auto variable_pairs_with_nz_mixed_second_derivative = _objective->get_variable_id_pairs_with_nz_mixed_second_derivative();
    for (auto constraint : _constraints) {
        auto s1 = constraint->get_variable_ids_with_nz_second_derivative();
        auto s2 = constraint->get_variable_id_pairs_with_nz_mixed_second_derivative();
        variables_with_nz_second_derivative.insert(s1.begin(), s1.end());
        variable_pairs_with_nz_mixed_second_derivative.insert(s2.begin(), s2.end());
    }
    nnz = variables_with_nz_second_derivative.size() + variable_pairs_with_nz_mixed_second_derivative.size();
    return nnz;
};

void Model::set_solution(const double * solution) {
    for (auto i=0; i<get_num_variables(); ++i)
        _solution.push_back(solution[i]);
};

void Model::set_objective_value(double obj_value) { _objective_value = obj_value; };
void Model::set_solve_status(int status) { _solve_status = status; };
void Model::set_name(std::string name) { _name = name; };
