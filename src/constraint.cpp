#include <constraint.h>

#include <cmath>
#include <iostream>
#include <limits>

Constraint::Constraint()
    : Func(), _id(), _lb(-std::numeric_limits<double>::infinity()),
      _ub(std::numeric_limits<double>::infinity()){};

Constraint::Constraint(std::string name)
    : Func(name), _id(), _lb(-std::numeric_limits<double>::infinity()),
      _ub(std::numeric_limits<double>::infinity()){};

std::tuple<double, double> Constraint::get_bounds() {
    return std::make_tuple(_lb, _ub);
};

void Constraint::equal_to(double bound) {
    _lb = bound;
    _ub = bound;
};
void Constraint::less_than_equal_to(double bound) { _ub = bound; };
void Constraint::greater_than_equal_to(double bound) { _lb = bound; };
void Constraint::two_sided(double lb, double ub) {
    _lb = lb;
    _ub = ub;
};

void Constraint::set_id(int id) { _id = id; };
int Constraint::get_id() { return _id; };

std::ostream &operator<<(std::ostream &os, const Constraint &constraint) {
    if (constraint._lb == constraint._ub) {
        os << "(";
        for (auto &term : constraint._terms)
            os << "+ " << term << " ";
        os << ") == " << constraint._lb << std::endl;
        return os;
    }
    std::string lb;
    std::string ub;

    if (std::isinf(constraint._lb))
        lb = "-inf";
    else
        lb = std::to_string(constraint._lb);

    if (std::isinf(constraint._ub))
        ub = "inf";
    else
        ub = std::to_string(constraint._ub);

    os << lb << " <= (";
    for (auto &term : constraint._terms)
        os << "+ " << term << " ";
    os << ") <= " << ub << std::endl;
    return os;
};
