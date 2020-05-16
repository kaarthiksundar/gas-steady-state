#ifndef UTILS_H
#define UTILS_H

#pragma once

#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

using Indices = std::set<int>;

std::ostream &operator<<(std::ostream &os, const Indices &indices);

class Param {
  private:
    std::string _name;
    Indices _indices;
    std::unordered_map<int, double> _values;

  public:
    Param();
    Param(std::string);

    void set_name(std::string);
    void in(Indices &indices);
    void in(Indices &indices, double value);
    void set_value(int key, double value);
    double get_value(int key);
    const Indices &get_indices() const;
    std::string get_name();
    bool has_key(int key);
};

#endif
