#ifndef UTILS_H
#define UTILS_H

#pragma once 

#include <set>
#include <unordered_map>
#include <iostream>
#include <string>
#include <iostream>

using Indices = std::set<int>;

std::ostream& operator<<(std::ostream& os, const Indices& indices); 

class Param {
    private:
        std::string _name;
        std::unordered_map<int, double> _values;

    public:
        Param();
        Param(std::string);

        void set_name(std::string);
        void in(Indices & indices);
        void in(Indices & indices, double value);
        void set_value(int key, double value);
        double get_value(int key);
        Indices get_indices();
        std::string get_name();
        bool has_key(int key);
};

#endif 