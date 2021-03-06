#include <term.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>

Term::Term()
    : _variables(), _variable_ids(), _power(), _coefficient(0.0),
      _type(TermType::constant){};

Term::Term(double coefficient)
    : _variables(), _variable_ids(), _power(), _coefficient(coefficient),
      _type(TermType::constant){};

Term::Term(std::shared_ptr<Variable> variable)
    : _variables({variable}), _variable_ids({variable->get_id()}),
      _power({1.0}), _coefficient(1.0), _type(TermType::linear){};

Term::Term(std::shared_ptr<Variable> variable, double coefficient)
    : _variables({variable}), _variable_ids({variable->get_id()}),
      _power({1.0}), _coefficient(coefficient), _type(TermType::linear){};

Term::Term(std::shared_ptr<Variable> variable, TermType type)
    : _variables({variable}), _variable_ids({variable->get_id()}),
      _power({1.0}), _coefficient(1.0), _type(type){};

Term::Term(std::shared_ptr<Variable> variable, double coefficient,
           TermType type)
    : _variables({variable}), _variable_ids({variable->get_id()}),
      _power({1.0}), _coefficient(coefficient), _type(type){};

Term::Term(std::vector<std::shared_ptr<Variable>> variables, TermType type) {
    if (type == TermType::xpowermminusone_absy) {
        std::cerr << "the value m must be specified when the term type is (x^m "
                     "- 1) * y"
                  << std::endl;
        exit(1);
    }
    _variables = variables;
    std::transform(variables.begin(), variables.end(),
                   std::back_inserter(_variable_ids),
                   [](std::shared_ptr<Variable> variable) -> int {
                       return variable->get_id();
                   });
    _power = std::vector<double>(variables.size(), 1.0);
    _coefficient = 1.0;
    _type = type;
};

Term::Term(std::vector<std::shared_ptr<Variable>> variables,
           std::vector<double> power, TermType type) {
    _variables = variables;
    _power = power;
    std::transform(variables.begin(), variables.end(),
                   std::back_inserter(_variable_ids),
                   [](std::shared_ptr<Variable> variable) -> int {
                       return variable->get_id();
                   });
    _coefficient = 1.0;
    _type = type;
};

Term::Term(std::vector<std::shared_ptr<Variable>> variables, double coefficient,
           TermType type) {
    _variables = variables;
    std::transform(variables.begin(), variables.end(),
                   std::back_inserter(_variable_ids),
                   [](std::shared_ptr<Variable> variable) -> int {
                       return variable->get_id();
                   });
    _power = std::vector<double>(variables.size(), 1.0);
    _coefficient = coefficient;
    _type = type;
};

Term::Term(std::vector<std::shared_ptr<Variable>> variables,
           std::vector<double> power, double coefficient, TermType type) {
    _variables = variables;
    std::transform(variables.begin(), variables.end(),
                   std::back_inserter(_variable_ids),
                   [](std::shared_ptr<Variable> variable) -> int {
                       return variable->get_id();
                   });
    _power = power;
    _coefficient = coefficient;
    _type = type;
};

void Term::set_coefficient(double coefficient) { _coefficient = coefficient; };
void Term::set_term_type(TermType type) { _type = type; };

const std::vector<int> &Term::get_variable_ids() const {
    return _variable_ids;
};
double Term::get_coefficient() { return _coefficient; };
TermType Term::get_term_type() { return _type; };

double Term::get_value(const std::vector<double> values) {
    if (values.size() != _variable_ids.size()) {
        std::cerr
            << "Term evaluation invoked with incorrect number of arguments"
            << std::endl;
        std::exit(1);
    }

    switch (_type) {
    case TermType::constant:
        return _coefficient;
        break;
    case TermType::linear:
        return _coefficient * values[0];
        break;
    case TermType::quadratic:
        return _coefficient * values[0] * values[0];
        break;
    case TermType::x_sq_y_sq:
        return _coefficient * std::pow(values[0], 2) * std::pow(values[1], 2);
        break;
    case TermType::x_abs_x:
        return _coefficient * values[0] * std::abs(values[0]);
        break;
    case TermType::xy:
        return _coefficient * values[0] * values[1];
        break;
    case TermType::xpowermminusone_absy:
        return _coefficient * (std::pow(values[0], _power[0]) - 1) *
               std::abs(values[1]);
        break;
    default:
        std::cerr << "term type not recognized " << std::endl;
        std::exit(1);
    }
    return 0.0;
};

double Term::get_derivative(int wrt, const std::vector<double> values) {
    if (wrt >= _variable_ids.size()) {
        std::cerr << "wrt argument out of bounds; has to be < "
                  << _variable_ids.size() << std::endl;
        std::exit(1);
    }

    if (values.size() != _variable_ids.size()) {
        std::cerr
            << "Term evaluation invoked with incorrect number of arguments"
            << std::endl;
        std::exit(1);
    }

    switch (_type) {
    case TermType::constant:
        return 0.0;
        break;
    case TermType::linear:
        return _coefficient;
        break;
    case TermType::quadratic:
        return 2.0 * _coefficient * values[0];
        break;
    case TermType::x_sq_y_sq:
        if (wrt == 0)
            return 2.0 * _coefficient * values[0] * std::pow(values[1], 2);
        else
            return 2.0 * _coefficient * values[1] * std::pow(values[0], 2);
        break;
    case TermType::x_abs_x:
        return _coefficient * 2.0 * std::abs(values[0]);
        break;
    case TermType::xy:
        if (wrt == 0)
            return _coefficient * values[1];
        else
            return _coefficient * values[0];
        break;
    case TermType::xpowermminusone_absy:
        if (wrt == 0)
            return _coefficient *
                   (_power[0] * std::pow(values[0], _power[0] - 1)) *
                   std::abs(values[1]);
        else {
            if (values[1] < 0.0)
                return (-1) * _coefficient *
                       (std::pow(values[0], _power[0]) - 1);
            else
                return _coefficient * (std::pow(values[0], _power[0]) - 1);
        }
        break;
    default:
        std::cerr << "term type not recognized " << std::endl;
        std::exit(1);
    }
    return 0.0;
};

double Term::get_derivative(const std::vector<double> values) {
    return get_derivative(0, values);
};

double Term::get_second_derivative(int wrt, const std::vector<double> values) {
    if (wrt >= _variable_ids.size()) {
        std::cerr << "wrt argument out of bounds; has to be < "
                  << _variable_ids.size() << std::endl;
        std::exit(1);
    }

    if (values.size() != _variable_ids.size()) {
        std::cerr
            << "Term evaluation invoked with incorrect number of arguments"
            << std::endl;
        std::exit(1);
    }

    switch (_type) {
    case TermType::constant:
        return 0.0;
        break;
    case TermType::linear:
        return 0.0;
        break;
    case TermType::quadratic:
        return 2.0 * _coefficient;
        break;
    case TermType::x_sq_y_sq:
        if (wrt == 0)
            return 2.0 * _coefficient * std::pow(values[1], 2);
        else
            return 2.0 * _coefficient * std::pow(values[0], 2);
        break;
    case TermType::x_abs_x:
        if (values[0] > 0.0)
            return 2.0 * _coefficient;
        else
            return (-1.0) * 2.0 * _coefficient;
        break;
    case TermType::xy:
        return 0.0;
        break;
    case TermType::xpowermminusone_absy:
        if (wrt == 0)
            return _coefficient *
                   (_power[0] * (_power[0] - 1) *
                    std::pow(values[0], _power[0] - 2.0)) *
                   std::abs(values[1]);
        else {
            return 0.0;
        }
        break;
    default:
        std::cerr << "term type not recognized " << std::endl;
        std::exit(1);
    }
    return 0.0;
};

double Term::get_second_derivative(const std::vector<double> values) {
    return get_second_derivative(0, values);
};

double Term::get_mixed_derivative(const std::vector<double> values) {
    if (values.size() != _variable_ids.size()) {
        std::cerr
            << "Term evaluation invoked with incorrect number of arguments"
            << std::endl;
        std::exit(1);
    }

    switch (_type) {
    case TermType::constant:
        return 0.0;
        break;
    case TermType::linear:
        return 0.0;
        break;
    case TermType::quadratic:
        return 0.0;
        break;
    case TermType::x_sq_y_sq:
        return 4.0 * _coefficient * values[0] * values[1];
        break;
    case TermType::x_abs_x:
        return 0.0;
        break;
    case TermType::xy:
        return _coefficient;
        break;
    case TermType::xpowermminusone_absy:
        if (values[1] > 0.0)
            return _coefficient *
                   (_power[0] * std::pow(values[0], _power[0] - 1.0));
        else
            return (-1.0) * _coefficient *
                   (_power[0] * std::pow(values[0], _power[0] - 1.0));

        break;
    default:
        std::cerr << "term type not recognized " << std::endl;
        std::exit(1);
    }
    return 0.0;
};

std::ostream &operator<<(std::ostream &os, const Term &term) {
    switch (term._type) {
    case TermType::constant:
        os << std::to_string(term._coefficient);
        break;
    case TermType::linear:
        if (term._coefficient != 1.0)
            os << "(" << std::to_string(term._coefficient) << ")"
               << " * " << term._variables[0]->get_name();
        else
            os << term._variables[0]->get_name();
        break;
    case TermType::quadratic:
        if (term._coefficient != 1.0)
            os << "(" << std::to_string(term._coefficient) << ")"
               << " * pow(" << term._variables[0]->get_name() << ", 2)";
        else
            os << "pow(" << term._variables[0]->get_name() << ", 2)";
        break;
    case TermType::x_sq_y_sq:
        if (term._coefficient != 1.0)
            os << "(" << std::to_string(term._coefficient) << ")"
               << " * pow(" << term._variables[0]->get_name() << ", 2)"
               << " * pow(" << term._variables[1]->get_name() << ", 2)";
        else
            os << " * pow(" << term._variables[0]->get_name() << ", 2)"
               << " * pow(" << term._variables[1]->get_name() << ", 2)";
        break;
    case TermType::x_abs_x:
        if (term._coefficient != 1.0)
            os << "(" << std::to_string(term._coefficient) << ")"
               << " * " << term._variables[0]->get_name() << " * |"
               << term._variables[0]->get_name() << "|";
        else
            os << term._variables[0]->get_name() << " * |"
               << term._variables[0]->get_name() << "|";
        break;
    case TermType::xy:
        if (term._coefficient != 1.0)
            os << "(" << std::to_string(term._coefficient) << ")"
               << " * " << term._variables[0]->get_name() << " * "
               << term._variables[1]->get_name();
        else
            os << term._variables[0]->get_name() << " * "
               << term._variables[1]->get_name();
        break;
    case TermType::xpowermminusone_absy:
        if (term._coefficient != 1.0)
            os << "(" << std::to_string(term._coefficient) << ")"
               << " * (pow(" << term._variables[0]->get_name() << ", "
               << term._power[0] << ") - 1) * |"
               << term._variables[1]->get_name() << "|";
        else
            os << "(pow(" << term._variables[0]->get_name() << ", "
               << term._power[0] << ") - 1) * |"
               << term._variables[1]->get_name() << "|";
        break;
    default:
        std::cerr << "term type not recognized " << std::endl;
        std::exit(1);
    }
    return os;
};
