#include <constraint.h>

#include <limits>

Constraint::Constraint() : 
    _lb(-std::numeric_limits<double>::infinity()), 
    _ub(std::numeric_limits<double>::infinity()) 
    {};

Constraint::Constraint(std::string name) :
    Func(name), 
    _lb(-std::numeric_limits<double>::infinity()), 
    _ub(std::numeric_limits<double>::infinity()) 
    {};

std::tuple<double, double> Constraint::get_bounds() { return std::make_tuple(_lb, _ub); };
        
void Constraint::equal_to(double bound) { _lb = bound; _ub = bound; };
void Constraint::less_than_equal_to(double bound) { _ub = bound; };
void Constraint::greater_than_equal_to(double bound) { _lb = bound; };
void Constraint::two_sided(double lb, double ub) { _lb = lb; _ub = ub; };
