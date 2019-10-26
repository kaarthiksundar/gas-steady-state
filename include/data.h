#ifndef DATA_H
#define DATA_H

#pragma once

#include <components.h>
#include <conversions.h>
#include <vector>
#include <memory>
#include <string>

class ComponentData {
protected:
    std::vector<std::shared_ptr<Pipe>> _pipes;
    std::vector<std::shared_ptr<Node>> _nodes;
    std::vector<std::shared_ptr<Compressor>> _compressors;
    std::vector<std::shared_ptr<Gnode>> _gnodes;
    int _num_slack_nodes;
    int _num_non_slack_nodes;
    
    void populate_component_data(std::string path, int units);
    void populate_pipe_data(std::string path, int units);
    void populate_node_data(std::string path, int units);
    void populate_compressor_data(std::string path, int units);
    void populate_gnode_data(std::string path, int units);
    
public:
    ComponentData(std::string, int units);
    ComponentData(const ComponentData &&);
    
    std::vector<std::shared_ptr<Pipe>> get_pipes() const;
    std::vector<std::shared_ptr<Node>> get_nodes() const;
    std::vector<std::shared_ptr<Compressor>> get_compressors() const;
    std::vector<std::shared_ptr<Gnode>> get_gnodes() const;
    int get_num_slack_nodes() const;
    int get_num_non_slack_nodes() const;
};

class ParameterData {
protected:
    std::vector<int> _pslack_ids;
    std::vector<int> _cslack_ids;
    std::vector<double> _pslack;
    std::vector<double> _cslack;
    std::vector<int> _qbar_ids;
    std::vector<double> _qbar;
    std::vector<double> _gbar;
    std::vector<double> _smax;
    std::vector<double> _dmax;
    std::vector<double> _cs;
    std::vector<double> _cd;
    bool _si;
    bool _standard;
    bool _per_unit;
    
    void populate_slack_data(std::string path, int units);
    void populate_slack_pressure_data(std::string path, int units);
    void populate_slack_price_data(std::string path, int units);
    void populate_non_slack_data(std::string path, int units);
    void populate_gnode_data(std::string path, int units);
    void populate_cost_data(std::string path, int units);
    
public:
    ParameterData(std::string path, int units);
    ParameterData(const ParameterData &&);
    
    const std::vector<int> & get_pslack_ids() const;
    const std::vector<int> & get_cslack_ids() const;
    const std::vector<double> & get_pslack() const;
    const std::vector<double> & get_cslack() const;
    const std::vector<int> & get_qbar_ids() const;
    const std::vector<double> & get_qbar() const;
    const std::vector<double> & get_gbar() const;
    const std::vector<double> & get_smax() const;
    const std::vector<double> & get_dmax() const;
    const std::vector<double> & get_cs() const;
    const std::vector<double> & get_cd() const;
    bool is_si() const;
    bool is_standard() const;
    bool is_per_unit() const;
    void make_parameters_si(const ConversionFactors &, const ScalingFactors &);
    void make_parameters_standard(const ConversionFactors &, const ScalingFactors &);
    void make_parameters_per_unit(const ConversionFactors &, const ScalingFactors &);
};

class Data : public ComponentData, public ParameterData {
public:
    Data(std::string path, int units);
    Data(const Data &&);
    
    double get_slack_pmin() const;
    
    void make_si(const ConversionFactors &, const ScalingFactors &);
    void make_standard(const ConversionFactors &, const ScalingFactors &);
    void make_per_unit(const ConversionFactors &, const ScalingFactors &);
};


#endif
