#ifndef TERM_H
#define TERM_H

#pragma once 

#include <term_type.h>
#include <variable.h>
#include <vector>
#include <memory>

class Term {
private:
    std::vector<std::shared_ptr<Variable>> _variables;
    std::vector<int> _variable_ids;
    std::vector<double> _power;
    double _coefficient;
    TermType _type;

public:
    Term();
    Term(double);
    Term(std::shared_ptr<Variable>);
    Term(std::shared_ptr<Variable>, double);
    Term(std::shared_ptr<Variable>, TermType);
    Term(std::shared_ptr<Variable>, double, TermType);
    Term(std::vector<std::shared_ptr<Variable>>, TermType);
    Term(std::vector<std::shared_ptr<Variable>>, std::vector<double>, TermType);
    Term(std::vector<std::shared_ptr<Variable>>, double, TermType);
    Term(std::vector<std::shared_ptr<Variable>>, std::vector<double>, double, TermType);
    
    void set_term_type(TermType);
    void set_coefficient(double);
    std::vector<std::shared_ptr<Variable>> get_variables();
    const std::vector<int> & get_variable_ids() const;
    double get_coefficient();
    TermType get_term_type();
    
    double get_value(const std::vector<double>);
    double get_derivative(int wrt, const std::vector<double>);
    double get_derivative(const std::vector<double>);
    double get_second_derivative(int wrt, const std::vector<double>);
    double get_second_derivative(const std::vector<double>);
    double get_mixed_derivative(int wrt_x, int wrt_y, const std::vector<double>);
};

#endif 
