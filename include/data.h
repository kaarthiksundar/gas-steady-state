#ifndef DATA_H
#define DATA_H

#pragma once

#include <components.h>
#include <conversions.h>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class ComponentData {
  protected:
    std::vector<std::shared_ptr<Pipe>> _pipes;
    std::vector<std::shared_ptr<Node>> _nodes;
    std::vector<std::shared_ptr<Compressor>> _compressors;
    std::vector<std::shared_ptr<Gnode>> _gnodes;
    int _num_slack_nodes;
    int _num_non_slack_nodes;

    void populate_component_data(std::string data_path, std::string case_name,
                                 std::string data_format, int units);
    void populate_pipe_data(std::string data_path, std::string case_name,
                            std::string data_format, int units);
    void populate_node_data(std::string data_path, std::string case_name,
                            std::string data_format, int units);
    void populate_compressor_data(std::string data_path, std::string case_name,
                                  std::string data_format, int units);
    void populate_gnode_data(std::string data_path, std::string case_name,
                             std::string data_format, int units);

  public:
    ComponentData(std::string data_path, std::string case_name,
                  std::string data_format, int units);
    ComponentData(const ComponentData &&);

    std::vector<std::shared_ptr<Pipe>> get_pipes() const;
    std::vector<std::shared_ptr<Node>> get_nodes() const;
    std::vector<std::shared_ptr<Compressor>> get_compressors() const;
    std::vector<std::shared_ptr<Gnode>> get_gnodes() const;
    int get_num_slack_nodes() const;
    int get_num_non_slack_nodes() const;
    void print_summary() const;
};

class ParameterData {
  protected:
    std::vector<int> _pslack_ids;
    std::vector<int> _cslack_ids;
    std::vector<double> _pslack;
    std::vector<double> _cslack;
    std::vector<int> _qbar_ids;
    std::vector<double> _qbar;
    std::vector<int> _gbar_ids;
    std::vector<double> _gbar;
    std::vector<int> _smax_ids;
    std::vector<double> _smax;
    std::vector<int> _dmax_ids;
    std::vector<double> _dmax;
    std::vector<int> _cs_ids;
    std::vector<double> _cs;
    std::vector<int> _cd_ids;
    std::vector<double> _cd;
    bool _si;
    bool _standard;
    bool _per_unit;

    void populate_slack_data(std::string data_path, std::string case_name,
                             std::string data_format, int units);
    void populate_slack_pressure_data(std::string data_path,
                                      std::string case_name,
                                      std::string data_format, int units);
    void populate_slack_price_data(std::string data_path, std::string case_name,
                                   std::string data_format, int units);
    void populate_non_slack_data(std::string data_path, std::string case_name,
                                 std::string data_format, int units);
    void populate_gnode_data(std::string data_path, std::string case_name,
                             std::string data_format, int units);
    void populate_cost_data(std::string data_path, std::string case_name,
                            std::string data_format, int units);

  public:
    ParameterData(std::string data_path, std::string case_name,
                  std::string data_format, int units);
    ParameterData(const ParameterData &&);

    const std::vector<int> &get_pslack_ids() const;
    const std::vector<int> &get_cslack_ids() const;
    const std::vector<double> &get_pslack() const;
    const std::vector<double> &get_cslack() const;
    const std::vector<int> &get_qbar_ids() const;
    const std::vector<double> &get_qbar() const;
    const std::vector<int> &get_gbar_ids() const;
    const std::vector<double> &get_gbar() const;
    const std::vector<int> &get_smax_ids() const;
    const std::vector<double> &get_smax() const;
    const std::vector<int> &get_dmax_ids() const;
    const std::vector<double> &get_dmax() const;
    const std::vector<int> &get_cs_ids() const;
    const std::vector<double> &get_cs() const;
    const std::vector<int> &get_cd_ids() const;
    const std::vector<double> &get_cd() const;
    bool is_si() const;
    bool is_standard() const;
    bool is_per_unit() const;
    void make_parameters_si(const ConversionFactors &, const ScalingFactors &);
    void make_parameters_standard(const ConversionFactors &,
                                  const ScalingFactors &);
    void make_parameters_per_unit(const ConversionFactors &,
                                  const ScalingFactors &);
};

class DisruptionData {
  protected:
    std::vector<int> _pipe_ids;
    std::vector<int> _node_ids;
    std::vector<int> _compressor_ids;
    std::vector<int> _gnode_ids;
    std::unordered_map<int, int> _fnode_id_of_pipe, _tnode_id_of_pipe;
    std::unordered_map<int, int> _fnode_id_of_compressor,
        _tnode_id_of_compressor;
    std::unordered_map<int, std::set<int>> _in_pipe_ids_of_node,
        _out_pipe_ids_of_node;
    std::unordered_map<int, std::set<int>> _in_compressor_ids_of_node,
        _out_compressor_ids_of_node;
    std::unordered_map<int, std::set<int>> _gnodes_of_node;

    void populate_disrupted_pipes(std::string data_path, std::string case_name,
                                  std::string data_format);
    void populate_disrupted_nodes(std::string data_path, std::string case_name,
                                  std::string data_format);
    void populate_disrupted_compressors(std::string data_path,
                                        std::string case_name,
                                        std::string data_format);
    void populate_disrupted_gnodes(std::string data_path, std::string case_name,
                                   std::string data_format);

  public:
    DisruptionData(std::string data_path, std::string case_name,
                   std::string data_format);
    DisruptionData(const DisruptionData &&);

    const std::vector<int> &get_disrupted_pipe_ids() const;
    const std::vector<int> &get_disrupted_node_ids() const;
    const std::vector<int> &get_disrupted_compressor_ids() const;
    const std::vector<int> &get_disruption_gnode_ids() const;
    const int &get_fnode_id_of_disrupted_pipe(int) const;
    const int &get_tnode_id_of_disrupted_pipe(int) const;
    const int &get_fnode_id_of_disrupted_compressor(int) const;
    const int &get_tnode_id_of_disrupted_compressor(int) const;
    const std::set<int> &get_in_pipe_ids_of_disrupted_node(int) const;
    const std::set<int> &get_out_pipe_ids_of_disrupted_node(int) const;
    const std::set<int> &get_in_compressor_ids_of_disrupted_node(int) const;
    const std::set<int> &get_out_compressor_ids_of_disrupted_node(int) const;
    const std::set<int> &get_gnodes_of_node(int) const;
};

class Data : public ComponentData, public ParameterData, public DisruptionData {
  public:
    Data(std::string data_path, std::string case_name, std::string data_format,
         int units);
    Data(const Data &&);

    double get_slack_pmin() const;

    void make_si(const ConversionFactors &, const ScalingFactors &);
    void make_standard(const ConversionFactors &, const ScalingFactors &);
    void make_per_unit(const ConversionFactors &, const ScalingFactors &);

  private:
    void fix_parameter_ordering();
    void populate_dependent_disruptions();
};

#endif
