#include <pipe.h>

#include <iostream>

Pipe::Pipe() {
    _id = "";
    _name = "";
    _fnode = "";
    _tnode = "";
    _diameter = 0.0;
    _length = 0.0;
    _friction_factor = 0.0;
    _disc_seg = 0;
    _standard_units = false;
    _si_units = false;
    _active = true;
    _type_name = "pipes";
};

Pipe::~Pipe() {};

bool Pipe::is_empty() const { return _empty; };
void Pipe::set_filled() { _empty = false; };

std::ostream& operator<<(std::ostream& os, const Pipe &pipe) {
    if (!pipe.is_empty()) {
        os << "pipe(" <<
        "id: " << pipe._id << ", " <<
        "name: " << pipe._name << ", " << 
        "fnode: " << pipe._fnode << ", " <<
        "tnode: " << pipe._tnode << ", " << 
        "diameter: " << pipe._diameter << ", " << 
        "length: " << pipe._length << ", " << 
        "friction_factor: " << pipe._friction_factor << ", " <<
        "discretization_segments: " << pipe._disc_seg << ", " <<
        "standard_units: " << pipe._standard_units << ", " << 
        "si_units: " << pipe._si_units << ", " << 
        "status: " << pipe._active << ")" << std::endl;
    }
    else {
        os << "pipe is not initalized" << std::endl;
    }
    return os;
};

Pipe* create_pipe(std::string id, 
                std::string name, 
                std::string fnode, 
                std::string tnode, 
                double diameter, 
                double length, 
                double friction_factor, 
                int disc_seg,
                int units) {

    Pipe* pipe = new Pipe;
    pipe->_id = id; pipe->_name = name;
    pipe->_fnode = fnode; pipe->_tnode = tnode;
    pipe->_diameter = diameter; 
    pipe->_length = length;
    pipe->_friction_factor = friction_factor;
    pipe->_disc_seg = disc_seg;
    if (units == 0) pipe->_si_units = true;
    else pipe->_standard_units = true;
    pipe->set_filled();

    return pipe;
}