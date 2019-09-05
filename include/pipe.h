#ifndef PIPE_H
#define PIPE_H

#include <string>
#include <iostream>

class Pipe {
    private: 
        bool _empty = true;
    public: 
        std::string _id;
        std::string _name;
        std::string _fnode;
        std::string _tnode;
        double _diameter;
        double _length;
        double _friction_factor;
        int _disc_seg;
        bool _standard_units;
        bool _si_units;
        bool _active;
        std::string _type_name;

        Pipe();
        ~Pipe();

        bool is_empty() const;
        void set_filled();
        friend std::ostream& operator<<(std::ostream& os, const Pipe &pipe);
};

Pipe* create_pipe(std::string id, 
                std::string name, 
                std::string fnode, 
                std::string tnode, 
                double diameter, 
                double length, 
                double friction_factor, 
                int disc_seg, 
                int units);

#endif 