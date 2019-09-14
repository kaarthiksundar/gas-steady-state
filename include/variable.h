#ifndef VARIABLE_H
#define VARIABLE_H

#pragma once 

#include <string>
#include <tuple>

class Variable {
    private:
        int _id;
        std::string _name;
        double _lb; 
        double _ub;
    double _start;
    
    public:
        Variable();
        Variable(std::string);
        Variable(std::string, double, double);

        void set_id(int);
        void set_lb(double);
        void set_ub(double);
    void set_start(double);
        void set_name(std::string);
        int get_id();
        double get_lb();
        double get_ub();
    double get_start();
        std::tuple<double, double> get_bounds();
        std::string get_name();
};

#endif 
