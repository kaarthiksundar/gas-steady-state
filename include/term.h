#ifndef TERM_H
#define TERM_H

#pragma once 

#include <term_type.h>
#include <variable.h>
#include <vector>
#include <memory>

/*
class Term {
private:
    std::vector<std::shared_ptr<Variable>> _variables;
    std::vector<int> _variable_ids;
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
    Term(std::vector<std::shared_ptr<Variable>>, double, TermType);
    
    void set_term_type(TermType);
    void set_coefficient(double);
    std::vector<std::shared_ptr<Variable>> get_variables();
    std::vector<int> get_variable_ids();
    double get_coefficient();
    TermType get_term_type();
    
    double get_value(const std::vector<double>);
    double get_derivative(int wrt, const std::vector<double>);
    double get_derivative(const std::vector<double>);
};
*/

class Term {
    private:
        std::vector<int> _variable_ids;
        double _coefficient;
        TermType _type;
    
    public:
        Term();
        Term(double);
        Term(int);
        Term(int, double);
        Term(int, TermType);
        Term(int, double, TermType);
        Term(std::vector<int>, TermType);
        Term(std::vector<int>, double, TermType);
        Term(std::shared_ptr<Variable>);
        Term(std::shared_ptr<Variable>, double);
        Term(std::shared_ptr<Variable>, TermType);
        Term(std::shared_ptr<Variable>, double, TermType);
        Term(std::vector<std::shared_ptr<Variable>>, TermType);
        Term(std::vector<std::shared_ptr<Variable>>, double, TermType);

        void set_term_type(TermType);
        void set_coefficient(double);
        std::vector<int> get_variable_ids();
        double get_coefficient();
        TermType get_term_type();

        double get_value(const std::vector<double>);
        double get_derivative(int wrt, const std::vector<double>);
        double get_derivative(const std::vector<double>);
};

#endif 
