#include <components.h>

#include <iostream>
#include <utility>

/* constructor, getters and functions for the Pipe class */
Pipe::Pipe(int id, std::string name, int fnode, int tnode,
           double diameter, double length, double friction_factor,
           int num_discretization_segments, int units) {
    _id = id; _name = name;
    _fnode = fnode; _tnode = tnode;
    _diameter = diameter; _length = length; _friction_factor = friction_factor;
    _num_discretization_segments = num_discretization_segments;
    _si = false; _standard = false; _per_unit = false;
    if (units == 0) _si = true;
    else _standard = true;
    _status = true;
};

int Pipe::get_id() const { return _id; };
std::string Pipe::get_name() const { return _name; };
int Pipe::get_fnode_id() const { return _fnode; };
int Pipe::get_tnode_id() const { return _tnode; };
double Pipe::get_diameter() const { return _diameter; };
double Pipe::get_length() const { return _length; };
double Pipe::get_friction_factor() const { return _friction_factor; };
int Pipe::get_num_discretization_segments() const { return _num_discretization_segments; };
bool Pipe::is_standard() const { return _standard; };
bool Pipe::is_si() const { return _si; };
bool Pipe::is_per_unit() const { return _per_unit; };
bool Pipe::get_status() const { return _status; };

std::ostream& operator<<(std::ostream& os, const Pipe &pipe) {
    os << "pipe(" <<
    "id: " << pipe._id << ", " <<
    "name: " << pipe._name << ", " <<
    "fnode: " << pipe._fnode << ", " <<
    "tnode: " << pipe._tnode << ", " <<
    "diameter: " << pipe._diameter << ", " <<
    "length: " << pipe._length << ", " <<
    "friction_factor: " << pipe._friction_factor << ", " <<
    "discretization_segments: " << pipe._num_discretization_segments << ", " <<
    "standard_units: " << pipe._standard << ", " <<
    "si_units: " << pipe._si << ", " <<
    "per_unit: " << pipe._per_unit << ", " <<
    "status: " << pipe._status << ")" << std::endl;
    return os;
};

void Pipe::make_per_unit(const ConversionFactors & cf, const ScalingFactors & sf) {
    if (_per_unit) return;
    if (_standard) make_si(cf, sf);
    if (_si) _length /= sf.get_space_scaling();
    _per_unit = true;
    _si = false;
    _standard = false;
};

void Pipe::make_standard(const ConversionFactors & cf, const ScalingFactors & sf) {
    if (_standard) return;
    if (_per_unit) make_si(cf, sf);
    if (_si) {
        _length *= cf.m_to_miles();
        _diameter *= cf.m_to_inches();
    }
    _per_unit = false;
    _si = false;
    _standard = true;
};

void Pipe::make_si(const ConversionFactors & cf, const ScalingFactors & sf) {
    if (_si) return;
    if (_per_unit) _length *= sf.get_space_scaling();
    if (_standard) {
        _length *= cf.miles_to_m();
        _diameter *= cf.inches_to_m();
    }
    _per_unit = false;
    _si = true;
    _standard = false;
};

/* constructor, getters and functions for the Node class */
Node::Node(int id, std::string name, double x, double y,
           double pmin, double pmax, double injection_min, double injection_max,
           bool slack, int units) {
    _id = id; _name = name;
    _coords = std::make_pair(x, y);
    _pmin = pmin; _pmax = pmax;
    _injection_min = injection_min; _injection_max = injection_max;
    _slack = slack;
    _si = false; _standard = false; _per_unit = false;
    if (units == 0) _si = true;
    else _standard = true;
    _status = true;
};

int Node::get_id() const { return _id; };
std::string Node::get_name() const { return _name; };
std::pair<double, double> Node::get_coords()  const { return _coords; };
double Node::get_pmin() const { return _pmin; };
double Node::get_pmax() const { return _pmax; };
double Node::get_injection_min() const { return _injection_min; };
double Node::get_injection_max() const { return _injection_max; };
bool Node::is_slack() const { return _slack; };
bool Node::is_standard() const { return _standard; };
bool Node::is_si() const { return _si; };
bool Node::is_per_unit() const { return _per_unit; };
bool Node::get_status() const { return _status; };

std::ostream& operator<<(std::ostream& os, const Node &node) {
    os << "node(" <<
    "id: " << node._id << ", " <<
    "name: " << node._name << ", " <<
    "x: " << node._coords.first << ", " <<
    "y: " << node._coords.second << ", " <<
    "pmin: " << node._pmin << ", " <<
    "pmax: " << node._pmax << ", " <<
    "injection_min: " << node._injection_min << ", " <<
    "injection_max: " << node._injection_max << ", " <<
    "slack: " << node._slack << ", " <<
    "standard_units: " << node._standard << ", " <<
    "si_units: " << node._si << ", " <<
    "per_unit: " << node._per_unit << ", " <<
    "status: " << node._status << ")" << std::endl;
    return os;
};

void Node::make_per_unit(const ConversionFactors & cf, const ScalingFactors & sf) {
    if (_per_unit) return;
    if (_standard) make_si(cf, sf);
    if (_si) {
        _pmin /= sf.get_pressure_scaling();
        _pmax /= sf.get_pressure_scaling();
        _injection_min /= sf.get_flow_scaling();
        _injection_max /= sf.get_flow_scaling();
    }
    _per_unit = true;
    _si = false;
    _standard = false;
};

void Node::make_standard(const ConversionFactors & cf, const ScalingFactors & sf) {
    if (_standard) return;
    if (_per_unit) make_si(cf, sf);
    if (_si) {
        _pmin *= cf.pascal_to_psi();
        _pmax *= cf.pascal_to_psi();
        _injection_min *= cf.kgps_to_mmscfd();
        _injection_max *= cf.kgps_to_mmscfd();
    }
    _per_unit = false;
    _si = false;
    _standard = true;
};

void Node::make_si(const ConversionFactors & cf, const ScalingFactors & sf) {
    if (_si) return;
    if (_per_unit) {
        _pmin *= sf.get_pressure_scaling();
        _pmax *= sf.get_pressure_scaling();
        _injection_min *= sf.get_flow_scaling();
        _injection_max *= sf.get_flow_scaling();
    }
    if (_standard) {
        _pmin *= cf.psi_to_pascal();
        _pmax *= cf.psi_to_pascal();
        _injection_min *= cf.mmscfd_to_kgps();
        _injection_max *= cf.mmscfd_to_kgps();
    }
    _per_unit = false;
    _si = true;
    _standard = false;
};

/* constructor, getters and functions for the Compressor class */
Compressor::Compressor(int id, std::string name, int fnode, int tnode,
           double cmin, double cmax, double power_max,
           double flow_min, double flow_max, int units) {
    _id = id; _name = name;
    _fnode = fnode; _tnode = tnode;
    _cmin = cmin; _cmax = cmax; _power_max = power_max;
    _flow_min = flow_min; _flow_max = flow_max;
    _si = false; _standard = false; _per_unit = false;
    if (units == 0) _si = true;
    else _standard = true;
    _status = true;
    if (_si) { _diameter = 1.0; _length = 250.0; _friction_factor = 0.001; }
    else { _diameter = 39.37; _length = 0.16; _friction_factor = 0.001; }
};

int Compressor::get_id() const { return _id; };
std::string Compressor::get_name() const { return _name; };
int Compressor::get_fnode_id() const { return _fnode; };
int Compressor::get_tnode_id() const { return _tnode; };
double Compressor::get_cmin() const { return _cmin; };
double Compressor::get_cmax() const { return _cmax; };
double Compressor::get_power_max() const { return _power_max; };
double Compressor::get_flow_min() const { return _flow_min; };
double Compressor::get_flow_max() const { return _flow_max; };
double Compressor::get_diameter() const { return _diameter; };
double Compressor::get_length() const { return _length; };
double Compressor::get_friction_factor() const { return _friction_factor; };
bool Compressor::is_standard() const { return _standard; };
bool Compressor::is_si() const { return _si; };
bool Compressor::is_per_unit() const { return _per_unit; };
bool Compressor::get_status() const { return _status; };

std::ostream& operator<<(std::ostream& os, const Compressor &compressor) {
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
    "diameter: " << compressor._diameter << ", " <<
    "length: " << compressor._length << ", " <<
    "friction factor: " << compressor._friction_factor << ", " <<
    "standard_units: " << compressor._standard << ", " <<
    "si_units: " << compressor._si << ", " <<
    "per_unit: " << compressor._per_unit << ", " <<
    "status: " << compressor._status << ")" << std::endl;
    return os;
};

void Compressor::make_per_unit(const ConversionFactors & cf, const ScalingFactors & sf) {
    if (_per_unit) return;
    if (_standard) make_si(cf, sf);
    if (_si) {
        _power_max /= sf.get_flow_scaling();
        _flow_min /= sf.get_flow_scaling();
        _flow_max /= sf.get_flow_scaling();
        _length /= sf.get_space_scaling();
    }
    _per_unit = true;
    _si = false;
    _standard = false;
};

void Compressor::make_standard(const ConversionFactors & cf, const ScalingFactors & sf) {
    if (_standard) return;
    if (_per_unit) make_si(cf, sf);
    if (_si) {
        _power_max *= cf.watts_to_hp();
        _flow_min *= cf.kgps_to_mmscfd();
        _flow_max *= cf.kgps_to_mmscfd();
        _length *= cf.m_to_miles();
        _diameter *= cf.m_to_inches();
    }
    _per_unit = false;
    _si = false;
    _standard = true;
};

void Compressor::make_si(const ConversionFactors & cf, const ScalingFactors & sf) {
    if (_si) return;
    if (_per_unit) {
        _power_max *= sf.get_flow_scaling();
        _flow_min *= sf.get_flow_scaling();
        _flow_max *= sf.get_flow_scaling();
        _length *= sf.get_space_scaling();
    }
    if (_standard) {
        _power_max *= cf.hp_to_watts();
        _flow_min *= cf.mmscfd_to_kgps();
        _flow_max *= cf.mmscfd_to_kgps();
        _length *= cf.miles_to_m();
        _diameter *= cf.inches_to_m();
    }
    _per_unit = false;
    _si = true;
    _standard = false;
};

/* constructor, getters and functions for the Gnode class */
Gnode::Gnode(int id, std::string name, int node) : _id(id), _name(name), _node(node), _status(true) {};

int Gnode::get_id() const { return _id; };
std::string Gnode::get_name() const { return _name; };
int Gnode::get_node_id() const { return _node; };
bool Gnode::get_status() const { return _status; };

std::ostream& operator<<(std::ostream& os, const Gnode &gnode) {
    os << "gnode(" <<
    "id: " << gnode._id << ", " <<
    "name: " << gnode._name << ", " <<
    "node: " << gnode._node << ", " <<
    "status: " << gnode._status << ")" << std::endl;
    return os;
};
