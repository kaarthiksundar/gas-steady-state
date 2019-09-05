#ifndef JUNCTION_H
#define JUNCTION_H

#include <string>
#include <iostream>
#include <utility>

class Junction {
    private: 
        bool _empty = true;
    public: 
        std::string _id;
        std::string _name;
        std::pair<double, double> _coords;
        double _pmin;
        double _pmax;
        double _injection_min;
        double _injection_max;
        bool _slack;
        bool _standard_units;
        bool _si_units;
        bool _active;
        std::string _type_name;

        Junction();
        ~Junction();

        bool is_empty() const;
        void set_filled();
        friend std::ostream& operator<<(std::ostream& os, const Junction &node);
};

Junction* create_node(std::string id, 
                    std::string name, 
                    double x, double y, 
                    double pmin, double pmax, 
                    double injection_min, 
                    double injection_max, 
                    bool slack, 
                    int units);

#endif 