#ifndef COMPONENTS_H
#define COMPONENTS_H

#pragma once

#include <string>
#include <conversions.h>

class Pipe {
private:
    int _id;
    std::string _name;
    int _fnode;
    int _tnode;
    double _diameter;
    double _length;
    double _friction_factor;
    int _num_discretization_segments;
    bool _standard;
    bool _si;
    bool _status;
    bool _per_unit;
    
public:
    Pipe(int id, std::string name, int fnode, int tnode,
         double diameter, double length, double friction_factor,
         int num_discretization_segments, int units);
    
    std::string get_name() const;
    int get_id() const;
    int get_fnode_id() const;
    int get_tnode_id() const;
    double get_diameter() const;
    double get_length() const;
    double get_friction_factor() const;
    int get_num_discretization_segments() const;
    bool is_standard() const;
    bool is_si() const;
    bool is_per_unit() const;
    bool get_status() const;
    
    void make_per_unit(const ConversionFactors &, const ScalingFactors &);
    void make_standard(const ConversionFactors &, const ScalingFactors &);
    void make_si(const ConversionFactors &, const ScalingFactors &);
    
    friend std::ostream& operator<<(std::ostream& os, const Pipe &pipe);
};

class Node {
private:
    int _id;
    std::string _name;
    std::pair<double, double> _coords;
    double _pmin;
    double _pmax;
    double _injection_min;
    double _injection_max;
    bool _slack;
    bool _standard;
    bool _si;
    bool _status;
    bool _per_unit;
    
public:
    Node(int id, std::string name, double x, double y,
         double pmin, double pmax, double injection_min,
         double injection_max, bool slack, int units);
    
    int get_id() const;
    std::string get_name() const;
    std::pair<double, double> get_coords() const;
    double get_pmin() const;
    double get_pmax() const;
    double get_injection_min() const;
    double get_injection_max() const;
    bool is_slack() const;
    bool is_si() const;
    bool is_standard() const;
    bool is_per_unit() const;
    bool get_status() const;
    
    void make_per_unit(const ConversionFactors &, const ScalingFactors &);
    void make_standard(const ConversionFactors &, const ScalingFactors &);
    void make_si(const ConversionFactors &, const ScalingFactors &);
    friend std::ostream& operator<<(std::ostream& os, const Node &node);
};

class Compressor {
private:
    int _id;
    std::string _name;
    int _fnode;
    int _tnode;
    double _cmin;
    double _cmax;
    double _power_max;
    double _flow_min;
    double _flow_max;
    double _diameter;
    double _length;
    double _friction_factor;
    bool _standard;
    bool _si;
    bool _status;
    bool _per_unit;
    
public:
    Compressor(int id, std::string name, int fnode, int tnode,
               double cmin, double cmax, double power_max,
               double flow_min, double flow_max, int units);
    
    int get_id() const;
    std::string get_name() const;
    int get_fnode_id() const;
    int get_tnode_id() const;
    double get_cmin() const;
    double get_cmax() const;
    double get_flow_min() const;
    double get_flow_max() const;
    double get_power_max() const;
    double get_diameter() const;
    double get_length() const;
    double get_friction_factor() const;
    bool is_standard() const;
    bool is_si() const;
    bool is_per_unit() const;
    bool get_status() const;
    
    void make_per_unit(const ConversionFactors &, const ScalingFactors &);
    void make_standard(const ConversionFactors &, const ScalingFactors &);
    void make_si(const ConversionFactors &, const ScalingFactors &);
    friend std::ostream& operator<<(std::ostream& os, const Compressor &compressor);
};

class Gnode {
private:
    int _id;
    std::string _name;
    int _node;
    bool _status;
    
public:
    Gnode();
    Gnode(int id, std::string name, int node);
    
    int get_id() const;
    std::string get_name() const;
    int get_node_id() const;
    bool get_status() const;

    friend std::ostream& operator<<(std::ostream& os, const Gnode &gnode);
};

#endif
