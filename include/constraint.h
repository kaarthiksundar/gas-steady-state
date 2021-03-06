#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#pragma once

#include <func.h>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class Constraint : public Func {
  protected:
    int _id;
    double _lb;
    double _ub;

  public:
    Constraint();
    Constraint(std::string);

    std::tuple<double, double> get_bounds();

    void equal_to(double);
    void less_than_equal_to(double);
    void greater_than_equal_to(double);
    void two_sided(double, double);

    void set_id(int);
    int get_id();

    friend std::ostream &operator<<(std::ostream &os,
                                    const Constraint &constraint);
};

#endif
