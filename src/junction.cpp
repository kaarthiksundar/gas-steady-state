#include <junction.h>

#include <iostream>

Junction::Junction() {
    _id = "";
    _name = "";
    _coords = std::make_pair(0.0, 0.0);
    _pmin = 0.0; _pmax = 0.0;
    _injection_min = 0.0; _injection_max = 0.0;
    _slack = false;
    _standard_units = false;
    _si_units = false;
    _active = true;
    _type_name = "junctions";
};


bool Junction::is_empty() const { return _empty; };
void Junction::set_filled() { _empty = false; };


std::ostream& operator<<(std::ostream& os, const Junction &junction) {
    if (!junction.is_empty()) {
        os << "junction(" <<
        "id: " << junction._id << ", " <<
        "name: " << junction._name << ", " << 
        "x: " << junction._coords.first << ", " <<
        "y: " << junction._coords.second << ", " << 
        "pmin: " << junction._pmin << ", " << 
        "pmax: " << junction._pmax << ", " << 
        "injection_min: " << junction._injection_min << ", " <<
        "injection_max: " << junction._injection_max << ", " << 
        "slack: " << junction._slack << ", " << 
        "standard_units: " << junction._standard_units << ", " << 
        "si_units: " << junction._si_units << ", " << 
        "status: " << junction._active << ")" << std::endl;
    }
    else {
        os << "junction is not initalized" << std::endl;
    }
    return os;
};

std::shared_ptr<Junction> create_node(std::string id, 
                                    std::string name, 
                                    double x, double y, 
                                    double pmin, double pmax, 
                                    double injection_min, 
                                    double injection_max, 
                                    bool slack, 
                                    int units) {
    
    auto node = std::make_shared<Junction>();
    node->_id = id;
    node->_name = name;
    node->_coords = std::make_pair(x, y);
    node->_pmin = pmin; 
    node->_pmax = pmax;
    node->_injection_min = injection_min;
    node->_injection_max = injection_max;
    node->_slack = slack;
    if (units == 0) node->_si_units = true;
    else node->_standard_units = true;
    node->set_filled();

    return node;

}