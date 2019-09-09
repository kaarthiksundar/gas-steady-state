#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#pragma once 

#include <func.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <tuple>

class Constraint : public Func {
    protected: 
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
};

#endif 