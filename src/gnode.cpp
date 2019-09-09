#include <gnode.h>

#include <iostream>

Gnode::Gnode() {
    _id = "";
    _name = "";
    _node = "";
    _active = true;
    _type_name = "gnodes";
};

bool Gnode::is_empty() const { return _empty; };
void Gnode::set_filled() { _empty = false; };

std::ostream& operator<<(std::ostream& os, const Gnode &gnode) {
    if (!gnode.is_empty()) {
        os << "gnode(" <<
        "id: " << gnode._id << ", " <<
        "name: " << gnode._name << ", " << 
        "node: " << gnode._node << ", " << 
        "status: " << gnode._active << ")" << std::endl;
    }
    else {
        os << "gnode is not initalized" << std::endl;
    }
    return os;
};

std::shared_ptr<Gnode> create_gnode(std::string id, std::string name, std::string node) {
    auto gnode = std::make_shared<Gnode>();
    gnode->_id = id;
    gnode->_name = name;
    gnode->_node = node;
    gnode->set_filled();
    return gnode;
}
