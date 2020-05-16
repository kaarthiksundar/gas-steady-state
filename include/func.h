#ifndef FUNC_H
#define FUNC_H

#pragma once

#include <algorithm>
#include <set>
#include <string>
#include <term.h>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

class Func {
  protected:
    std::vector<Term> _terms;
    std::unordered_map<int, std::vector<int>> _variable_id_to_terms;
    std::string _name;
    std::set<int> _variable_ids_with_nz_second_derivative;
    std::set<std::pair<int, int>>
        _variable_id_pairs_with_nz_mixed_second_derivative;

  public:
    Func();
    Func(std::string);

    std::string get_name();
    double get_value(const double *);
    std::vector<std::tuple<int, double>> get_gradient(int, const double *);
    std::vector<int> get_gradient_sparsity(int);
    int get_num_variables();
    std::set<int> get_variable_ids_with_nz_second_derivative();
    std::set<std::pair<int, int>>
    get_variable_id_pairs_with_nz_mixed_second_derivative();
    std::set<std::pair<int, int>> get_hessian_sparsity();
    std::vector<std::tuple<int, int, double>> get_hessian(const double *);

    void set_name(std::string);
    void add_term(Term);
    bool has_variable_id(int);

    friend std::ostream &operator<<(std::ostream &os, const Func &func);
};

#endif
