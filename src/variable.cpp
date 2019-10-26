#include <variable.h>
#include <limits>

Variable::Variable() : 
_id(),
_name(""),
_lb(-std::numeric_limits<double>::infinity()),
_ub(std::numeric_limits<double>::infinity()),
_start(0.0)
{};

Variable::Variable(std::string name) : 
_id(),
_name(name),
_lb(-std::numeric_limits<double>::infinity()),
_ub(std::numeric_limits<double>::infinity()),
_start(0.0)
{};

Variable::Variable(std::string name, double lb, double ub) : 
_id(),
_name(name),
_lb(lb),
_ub(ub),
_start(0.0)
{};

void Variable::set_id(int id) { _id = id; };
void Variable::set_name(std::string name) { _name = name; };
void Variable::set_lb(double lb) { _lb = lb; };
void Variable::set_ub(double ub) { _ub = ub; };
void Variable::set_start(double start) { _start = start; };

int Variable::get_id() { return _id; };
std::string Variable::get_name() { return _name; };
double Variable::get_lb() { return _lb; };
double Variable::get_ub() { return _ub; };
double Variable::get_start() { return _start; };
std::tuple<double, double> Variable::get_bounds() { return std::make_tuple(_lb, _ub); };



