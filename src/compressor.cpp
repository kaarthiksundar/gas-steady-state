#include <compressor.h>

#include <iostream>

Compressor::Compressor() {
    _id = "";
    _name = "";
    _fnode = "";
    _tnode = "";
    _cmin = 0.0;
    _cmax = 0.0;
    _power_max = 0.0;
    _flow_min = 0.0;
    _flow_max = 0.0;
    _diameter = 1.0; // meters
    _length = 250.0; // meters
    _friction_factor = 0.0001;
    _standard_units = false;
    _si_units = true;
    _active = true;
    _type_name = "compressors";
};

Compressor::~Compressor() {};

bool Compressor::is_empty() const { return _empty; };
void Compressor::set_filled() { _empty = false; };

std::ostream& operator<<(std::ostream& os, const Compressor &compressor) {
    if (!compressor.is_empty()) {
        os << "compressor(" <<
        "id: " << compressor._id << ", " <<
        "name: " << compressor._name << ", " << 
        "fnode: " << compressor._fnode << ", " <<
        "tnode: " << compressor._tnode << ", " << 
        "cmin: " << compressor._cmin << ", " << 
        "cmax: " << compressor._cmax << ", " << 
        "power_max: " << compressor._power_max << ", " << 
        "flow_min: " << compressor._flow_min << ", " << 
        "flow_max: " << compressor._flow_max << ", " << 
        "standard_units: " << compressor._standard_units << ", " << 
        "si_units: " << compressor._si_units << ", " << 
        "status: " << compressor._active << ")" << std::endl;
    }
    else {
        os << "compressor is not initalized" << std::endl;
    }
    return os;
};

Compressor* create_compressor(std::string id, 
                            std::string name, 
                            std::string fnode, 
                            std::string tnode, 
                            double cmin, double cmax, 
                            double power_max, 
                            double flow_min, double flow_max, 
                            int units) {
    
    Compressor* compressor = new Compressor;
    compressor->_id = id; 
    compressor->_name = name;
    compressor->_fnode = fnode;
    compressor->_tnode = tnode;
    compressor->_cmin = cmin;
    compressor->_cmax = cmax;
    compressor->_power_max = power_max; 
    compressor->_flow_min = flow_min; 
    compressor->_flow_max = flow_max;
    if (units == 0) compressor->_si_units = true;
    else compressor->_standard_units = true;
    compressor->set_filled();

    return compressor;
}