#ifndef STEADY_STATE_DATA_H
#define STEADY_STATE_DATA_H

#include <conversions.h>
#include <data.h>
#include <utils.h>

class SteadyStateData {
  private:
    /* parameters */
    Param _p_min, _p_max;
    Param _phi_min_compressor, _phi_max_compressor;
    Param _phi_min_pipe, _phi_max_pipe;
    Param _c_ratio_min, _c_ratio_max;
    Param _length_pipe, _diameter_pipe, _friction_factor_pipe;
    Param _length_compressor, _diameter_compressor, _friction_factor_compressor;
    Param _resistance_pipe, _resistance_compressor;
    Param _power_max_compressor;
    Param _cd, _cs;
    Param _pslack, _cslack;
    Param _qbar;
    Param _gbar, _smax, _dmax;
    Param _area_pipe, _area_compressor;
    /* Indices */
    Indices _nodes;
    Indices _pipes;
    Indices _compressors;
    Indices _gnodes;
    Indices _slack_nodes;
    Indices _non_slack_nodes;
    std::unordered_map<int, Indices> _out_pipes_in_node, _in_pipes_in_node;
    std::unordered_map<int, Indices> _out_compressors_in_node,
        _in_compressors_in_node;
    std::unordered_map<int, Indices> _gnodes_in_node;
    std::unordered_map<int, int> _fnode_of_pipe, _tnode_of_pipe;
    std::unordered_map<int, int> _fnode_of_compressor, _tnode_of_compressor;
    /* disruption information */
    Indices _disrupted_nodes;
    Indices _disrupted_pipes;
    Indices _disrupted_compressors;
    Indices _disrupted_gnodes;
    Indices _dependent_distrupted_pipes;
    Indices _dependent_disrupted_compressors;
    Indices _dependent_disrupted_gnodes;

  public:
    SteadyStateData(const Data &data, const ScalingFactors &sf);
    const Param &get_p_min() const;
    const Param &get_p_max() const;
    const Param &get_phi_min_compressor() const;
    const Param &get_phi_max_compressor() const;
    const Param &get_phi_min_pipe() const;
    const Param &get_phi_max_pipe() const;
    const Param &get_c_ratio_min() const;
    const Param &get_c_ratio_max() const;
    const Param &get_length_pipe() const;
    const Param &get_diameter_pipe() const;
    const Param &get_friction_factor_pipe() const;
    const Param &get_length_compressor() const;
    const Param &get_diameter_compressor() const;
    const Param &get_friction_factor_compressor() const;
    const Param &get_resistance_pipe() const;
    const Param &get_resistance_compressor() const;
    const Param &get_power_max_compressor() const;
    const Param &get_cd() const;
    const Param &get_cs() const;
    const Param &get_pslack() const;
    const Param &get_cslack() const;
    const Param &get_qbar() const;
    const Param &get_gbar() const;
    const Param &get_smax() const;
    const Param &get_dmax() const;
    const Param &get_area_pipe() const;
    const Param &get_area_compressor() const;
    const Indices &get_node_indexes() const;
    const Indices &get_pipe_indexes() const;
    const Indices &get_compressor_indexes() const;
    const Indices &get_gnode_indexes() const;
    const Indices &get_slack_node_indexes() const;
    const Indices &get_non_slack_node_indexes() const;
    const Indices &get_out_pipe_indexes_in_node(int i) const;
    const Indices &get_in_pipe_indexes_in_nodes(int i) const;
    const Indices &get_out_compressor_indexes_in_node(int i) const;
    const Indices &get_in_compressor_indexes_in_node(int i) const;
    const Indices &get_gnodes_in_node(int i) const;
    const Indices &get_disrupted_nodes() const;
    const Indices &get_disrupted_pipes() const;
    const Indices &get_disrupted_compressors() const;
    const Indices &get_disrupted_gnodes() const;
    const Indices &get_dependent_disrupted_pipes() const;
    const Indices &get_dependent_disrupted_compressors() const;
    const Indices &get_dependent_disrupted_gnodes() const;
    int get_fnode_index_of_pipe(int i) const;
    int get_tnode_index_of_pipe(int i) const;
    int get_fnode_index_of_compressor(int i) const;
    int get_tnode_index_of_compressor(int i) const;

  private:
    void populate_parameters(const Data &data, const ScalingFactors &sf);
    void populate_indices(const Data &data);
};
#endif
