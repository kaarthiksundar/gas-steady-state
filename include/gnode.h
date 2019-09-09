#ifndef GNODE_H
#define GNODE_H

#pragma once 

#include <string>
#include <iostream>
#include <memory>

class Gnode {
    private: 
        bool _empty = true;
    public: 
        std::string _id;
        std::string _name;
        std::string _node;
        bool _active;
        std::string _type_name;

        Gnode();

        bool is_empty() const;
        void set_filled();
        friend std::ostream& operator<<(std::ostream& os, const Gnode &gnode);
};

std::shared_ptr<Gnode> create_gnode(std::string id, 
                                    std::string name, 
                                    std::string node);

#endif 