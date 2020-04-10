#include <func.h>

#include <iostream>

Func::Func() : 
_terms(),
_variable_id_to_terms(),
_name(""),
_variable_ids_with_nz_second_derivative(),
_variable_id_pairs_with_nz_mixed_second_derivative()
{};

Func::Func(std::string name) : 
_terms(),
_variable_id_to_terms(),
_name(name),
_variable_ids_with_nz_second_derivative(),
_variable_id_pairs_with_nz_mixed_second_derivative()
{};

std::string Func::get_name() { return _name; };

void Func::set_name(std::string name) { _name = name; };

void Func::add_term(Term term) {
    _terms.push_back(term);
    for (auto variable_id : term.get_variable_ids()) {
        if (has_variable_id(variable_id))
            _variable_id_to_terms[variable_id].push_back(_terms.size()-1);
        else
            _variable_id_to_terms[variable_id] = std::vector<int>(1, _terms.size()-1);
    }
    switch (term.get_term_type()) {
        case TermType::quadratic : {
            _variable_ids_with_nz_second_derivative.insert(term.get_variable_ids()[0]);
            break;
        }
        case TermType::x_abs_x : {
            _variable_ids_with_nz_second_derivative.insert(term.get_variable_ids()[0]);
            break;
        }
        case TermType::x_sq_y_sq : {
            int var_id_x = term.get_variable_ids()[0], var_id_y = term.get_variable_ids()[1];
            _variable_ids_with_nz_second_derivative.insert(var_id_x);
            _variable_ids_with_nz_second_derivative.insert(var_id_y);
            auto variable_id_pair = std::make_pair(std::min(var_id_x, var_id_y), std::max(var_id_x, var_id_y));
            _variable_id_pairs_with_nz_mixed_second_derivative.insert(variable_id_pair);
            break;
        }
        case TermType::xy : {
            int var_id_x = term.get_variable_ids()[0], var_id_y = term.get_variable_ids()[1];
            auto variable_id_pair = std::make_pair(std::min(var_id_x, var_id_y), std::max(var_id_x, var_id_y));
            _variable_id_pairs_with_nz_mixed_second_derivative.insert(variable_id_pair);
            break;
        }
        case TermType::xpowermminusone_absy : {
            int var_id_x = term.get_variable_ids()[0], var_id_y = term.get_variable_ids()[1];
            _variable_ids_with_nz_second_derivative.insert(var_id_x);
            auto variable_id_pair = std::make_pair(std::min(var_id_x, var_id_y), std::max(var_id_x, var_id_y));
            _variable_id_pairs_with_nz_mixed_second_derivative.insert(variable_id_pair);
            break;
        }
        default:
            break;
    }
};

double Func::get_value(const double* var_values) {
    double fcn_value = 0.0;
    for (auto term : _terms) {
        auto var_ids = term.get_variable_ids();
        std::vector<double> values;
        for (auto var_id : var_ids)
            values.push_back(var_values[var_id]);
        fcn_value += term.get_value(values);
    }
    return fcn_value;
};

std::vector<std::tuple<int, double>> Func::get_gradient(int num_variables, const double* var_values) {
    std::vector<std::tuple<int, double>> grad;
    for (auto i=0; i<num_variables; ++i) {
        double gradient = 0.0;
        if (has_variable_id(i)) {
            for (auto term_id : _variable_id_to_terms[i]) {
                auto var_ids = _terms[term_id].get_variable_ids();
                int wrt;
                std::vector<double> values;
                for (auto j=0; j<var_ids.size(); ++j) {
                    int var_id = var_ids[j];
                    if (var_id == i) wrt = j;
                    values.push_back(var_values[var_id]);
                }
                gradient += _terms[term_id].get_derivative(wrt, values);
            }
            grad.push_back(std::make_tuple(i, gradient));
        }
    }
    return grad;
};

std::vector<int> Func::get_gradient_sparsity(int num_variables) {
    std::vector<int> sparsity_pattern;
    for (auto i=0; i<num_variables; ++i)
        if (has_variable_id(i)) sparsity_pattern.push_back(i);
    return sparsity_pattern;
};

int Func::get_num_variables() { return _variable_id_to_terms.size(); };

std::set<int> Func::get_variable_ids_with_nz_second_derivative() { return _variable_ids_with_nz_second_derivative; };
std::set<std::pair<int, int>> Func::get_variable_id_pairs_with_nz_mixed_second_derivative() {
    return _variable_id_pairs_with_nz_mixed_second_derivative;
};

std::set<std::pair<int, int>> Func::get_hessian_sparsity() {
    std::set<std::pair<int, int>> sparsity_pattern;
    for (auto & id : _variable_ids_with_nz_second_derivative)
        sparsity_pattern.insert(std::make_pair(id, id));
    sparsity_pattern.insert(_variable_id_pairs_with_nz_mixed_second_derivative.begin(), _variable_id_pairs_with_nz_mixed_second_derivative.end());
    return sparsity_pattern;
};

bool Func::has_variable_id(int id) {
    if (_variable_id_to_terms.find(id) == _variable_id_to_terms.end())
        return false;
    return true;
};

std::vector<std::tuple<int, int, double>> Func::get_hessian(const double* var_values) {
    std::vector<std::pair<int, int>> indices;
    std::vector<double> hessian_values;
    for (auto term : _terms) {
        auto tt = term.get_term_type();
        if (tt == TermType::linear || tt == TermType::constant) continue;
        auto var_ids = term.get_variable_ids();
        std::vector<double> values;
        for (auto var_id : var_ids) values.push_back(var_values[var_id]);
        for (auto i=0; i<var_ids.size(); ++i) {
            double second_derivative = term.get_second_derivative(i, values);
            std::pair<int, int> var_id_pair = std::make_pair(var_ids[i], var_ids[i]);
            auto it = std::find(indices.begin(), indices.end(), var_id_pair);
            if (it != indices.end()) {
                int current_index = std::distance(indices.begin(), it);
                hessian_values[current_index] += second_derivative;
            }
            else {
                indices.push_back(var_id_pair);
                hessian_values.push_back(second_derivative);
            }
        }
        double mixed_derivative = term.get_mixed_derivative(values);
        if (mixed_derivative != 0.0) {
            int var_id_x = var_ids[0], var_id_y = var_ids[1];
            auto var_id_pair = std::make_pair(std::min(var_id_x, var_id_y), std::max(var_id_x, var_id_y));
            auto it = std::find(indices.begin(), indices.end(), var_id_pair);
            if (it != indices.end()) {
                int current_index = std::distance(indices.begin(), it);
                hessian_values[current_index] += mixed_derivative;
            }
            else {
                indices.push_back(var_id_pair);
                hessian_values.push_back(mixed_derivative);
            }
        }
    }
    
    std::vector<std::tuple<int, int, double>> hessian = {};
    for (auto i=0; i<indices.size(); ++i)
        hessian.push_back(std::make_tuple(indices[i].first, indices[i].second, hessian_values[i]));
    
    return hessian;
};
