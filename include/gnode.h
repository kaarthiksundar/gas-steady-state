#ifndef GNODE_H
#define GNODE_H

#include <string>
#include <iostream>

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
        ~Gnode();

        bool is_empty() const;
        void set_filled();
        friend std::ostream& operator<<(std::ostream& os, const Gnode &gnode);
};

Gnode* create_gnode(std::string id, 
                    std::string name, 
                    std::string node);

#endif 