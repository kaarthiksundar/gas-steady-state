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
_primal_solution(),
_dual_solution(),
_objective_value(),
_solver_status()
{};

Model::Model(std::string name) :
_name(name),
_variables(),
_constraints(),
_objective(),
_num_variables(0),
_num_constraints(0),
_has_objective(false),
_primal_solution(),
_dual_solution(),
_objective_value(),
_solver_status()
{};

/* generic functions */
std::string Model::get_name() { return _name; };
void Model::set_name(std::string name) { _name = name; };

/* get_nlp_info functions */
int Model::get_num_variables() { return _num_variables; };
int Model::get_num_constraints() { return _num_constraints; };

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

/* get_bounds_info functions */
std::tuple<double, double> Model::get_variable_bounds(int i) { return _variables[i]->get_bounds(); };
std::tuple<double, double> Model::get_constraint_bounds(int i) { return _constraints[i]->get_bounds(); };

/* get_starting_point functions */
double Model::get_variable_start(int i) { return _variables[i]->get_start(); };

/* eval_f functions */
double Model::evaluate_objective_function_value(const double * vals) { return _objective->get_value(vals); };

/* eval_grad_f functions */
std::vector<std::tuple<int, double>> Model::evaluate_objective_gradient(const double * vals) {
    return _objective->get_gradient(get_num_variables(), vals);
};

/* eval_g functions */
double Model::evaluate_constraint_function_value(int i, const double * vals) { return _constraints[i]->get_value(vals); };

/* eval_jac_g functions */
std::vector<int> Model::get_constraint_sparsity(int i) { return _constraints[i]->get_gradient_sparsity(get_num_variables()); };
std::vector<std::tuple<int, double>> Model::evaluate_constraint_gradient(int i, const double * vals) {
    return _constraints[i]->get_gradient(get_num_variables(), vals);
};

/* eval_h functions (none so far - automatic hessian computations not implemented) */

/* finalize_solution functions */
void Model::set_primal_solution(const double * vals) {
    for (auto i=0; i<get_num_variables(); ++i)
        _primal_solution.push_back(vals[i]);
};
void Model::set_solver_status(int status) { _solver_status = status; };
void Model::set_dual_solution(const double * vals) {
    for (auto i=0; i<get_num_constraints(); ++i)
        _dual_solution.push_back(vals[i]);
};
void Model::set_objective_value(double objective_value) { _objective_value = objective_value; };

/* final solution getters for non-ipopt classes */
double Model::get_primal_value(std::shared_ptr<Variable> var) { return _primal_solution[var->get_id()]; };
double Model::get_dual_value(std::shared_ptr<Constraint> constraint) { return _dual_solution[constraint->get_id()]; };
double Model::get_objective_value() { return _objective_value; };
int Model::get_solver_status() { return _solver_status; };

/* model population functions */
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
    constraint->set_id(_constraints.size());
    _constraints.push_back(constraint);
    _num_constraints = _constraints.size();
    return constraint;
};

std::shared_ptr<Constraint> Model::add_constraint(std::string name) {
    auto constraint = std::make_shared<Constraint>(name);
    constraint->set_id(_constraints.size());
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
