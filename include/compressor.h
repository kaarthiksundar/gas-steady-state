#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <string>
#include <iostream>

class Compressor {
    private: 
        bool _empty = true;
    public: 
        std::string _id;
        std::string _name;
        std::string _fnode;
        std::string _tnode;
        double _cmin;
        double _cmax;
        double _power_max;
        double _flow_min;
        double _flow_max; 
        double _diameter;
        double _length;
        double _friction_factor;
        bool _standard_units;
        bool _si_units;
        bool _status;

        Compressor();
        ~Compressor();

        bool is_empty() const;
        void set_filled();
        friend std::ostream& operator<<(std::ostream& os, const Compressor &compressor);
};

Compressor* create_compressor(std::string id, 
                            std::string name, 
                            std::string fnode, 
                            std::string tnode, 
                            double cmin, double cmax, 
                            double power_max, 
                            double flow_min, double flow_max, 
                            int units);

#endif 