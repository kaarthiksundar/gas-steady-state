#include <utils.h>

std::ostream &operator<<(std::ostream &os, const Indices &indices) {
    os << "[";
    for (auto it : indices) {
        os << it;
        if (it != *indices.rbegin())
            os << ", ";
    }
    os << "]" << std::endl;
    return os;
};

Param::Param() : _name(""), _indices(), _values(){};
Param::Param(std::string name) : _name(name), _indices(), _values(){};

void Param::set_name(std::string name) { _name = name; };
std::string Param::get_name() { return _name; };
void Param::in(Indices &indices) {
    for (auto i : indices) {
        _indices.insert(i);
        _values[i] = 0.0;
    }
};
void Param::in(Indices &indices, double value) {
    for (auto i : indices) {
        _indices.insert(i);
        _values[i] = value;
    }
};

void Param::set_value(int key, double value) {
    if (has_key(key) == false) {
        std::cerr << "parameter has no value in key " << key << std::endl;
        std::exit(1);
    }
    _values[key] = value;
};

double Param::get_value(int key) {
    if (has_key(key) == false) {
        std::cerr << "parameter has no value in key " << key << std::endl;
        std::exit(1);
    }
    return _values[key];
};

const Indices &Param::get_indices() const { return _indices; };

bool Param::has_key(int key) {
    if (_indices.find(key) == _indices.end())
        return false;
    return true;
};
