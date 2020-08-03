#include <algorithm>
#include <cmath>
#include <steady_state_data.h>

SteadyStateData::SteadyStateData(const Data &data, const ScalingFactors &sf) {
    _p_min = Param("p_min");
    _p_max = Param("p_max");
    _phi_min_compressor = Param("phi_min_comperssor");
    _phi_max_compressor = Param("phi_max_compressor");
    _phi_min_pipe = Param("phi_min_pipe");
    _phi_max_pipe = Param("phi_max_pipe");
    _c_ratio_min = Param("c_ratio_min");
    _c_ratio_max = Param("c_ratio_max");
    _length_pipe = Param("length_pipe");
    _diameter_pipe = Param("diameter_pipe");
    _friction_factor_pipe = Param("friction_factor_pipe");
    _length_compressor = Param("length_compressor");
    _diameter_compressor = Param("diameter_compressor");
    _friction_factor_compressor = Param("friction_factor_compressor");
    _power_max_compressor = Param("power_max_compressor");
    _cd = Param("cd");
    _cs = Param("cs");
    _pslack = Param("pslack");
    _cslack = Param("cslack");
    _qbar = Param("qbar");
    _gbar = Param("gbar");
    _smax = Param("smax");
    _dmax = Param("dmax");
    _area_pipe = Param("area_pipe");
    _area_compressor = Param("area_compressor");
    _resistance_pipe = Param("resistance_pipe");
    _resistance_compressor = Param("resistance_compressor");

    populate_indices(data);
    populate_parameters(data, sf);
};

const Param &SteadyStateData::get_p_min() const { return _p_min; };
const Param &SteadyStateData::get_p_max() const { return _p_max; };
const Param &SteadyStateData::get_phi_min_pipe() const {
    return _phi_min_pipe;
};
const Param &SteadyStateData::get_phi_max_pipe() const {
    return _phi_max_pipe;
};
const Param &SteadyStateData::get_phi_min_compressor() const {
    return _phi_min_compressor;
};
const Param &SteadyStateData::get_phi_max_compressor() const {
    return _phi_max_compressor;
};
const Param &SteadyStateData::get_c_ratio_min() const { return _c_ratio_min; };
const Param &SteadyStateData::get_c_ratio_max() const { return _c_ratio_max; };
const Param &SteadyStateData::get_length_pipe() const { return _length_pipe; };
const Param &SteadyStateData::get_diameter_pipe() const {
    return _diameter_pipe;
};
const Param &SteadyStateData::get_friction_factor_pipe() const {
    return _friction_factor_pipe;
};
const Param &SteadyStateData::get_length_compressor() const {
    return _length_compressor;
};
const Param &SteadyStateData::get_diameter_compressor() const {
    return _diameter_compressor;
};
const Param &SteadyStateData::get_friction_factor_compressor() const {
    return _friction_factor_compressor;
};
const Param &SteadyStateData::get_power_max_compressor() const {
    return _power_max_compressor;
};
const Param &SteadyStateData::get_cd() const { return _cd; };
const Param &SteadyStateData::get_cs() const { return _cs; };
const Param &SteadyStateData::get_pslack() const { return _pslack; };
const Param &SteadyStateData::get_cslack() const { return _cslack; };
const Param &SteadyStateData::get_qbar() const { return _qbar; };
const Param &SteadyStateData::get_gbar() const { return _gbar; };
const Param &SteadyStateData::get_smax() const { return _smax; };
const Param &SteadyStateData::get_dmax() const { return _dmax; };
const Param &SteadyStateData::get_area_pipe() const { return _area_pipe; };
const Param &SteadyStateData::get_area_compressor() const {
    return _area_compressor;
};
const Param &SteadyStateData::get_resistance_pipe() const {
    return _resistance_pipe;
};
const Param &SteadyStateData::get_resistance_compressor() const {
    return _resistance_compressor;
};

const Indices &SteadyStateData::get_node_indexes() const { return _nodes; };
const Indices &SteadyStateData::get_pipe_indexes() const { return _pipes; };
const Indices &SteadyStateData::get_compressor_indexes() const {
    return _compressors;
};
const Indices &SteadyStateData::get_gnode_indexes() const { return _gnodes; };
const Indices &SteadyStateData::get_slack_node_indexes() const {
    return _slack_nodes;
};
const Indices &SteadyStateData::get_non_slack_node_indexes() const {
    return _non_slack_nodes;
};
const Indices &SteadyStateData::get_out_pipe_indexes_in_node(int i) const {
    return _out_pipes_in_node.at(i);
};
const Indices &SteadyStateData::get_in_pipe_indexes_in_nodes(int i) const {
    return _in_pipes_in_node.at(i);
};
const Indices &
SteadyStateData::get_out_compressor_indexes_in_node(int i) const {
    return _out_compressors_in_node.at(i);
};
const Indices &SteadyStateData::get_in_compressor_indexes_in_node(int i) const {
    return _in_compressors_in_node.at(i);
};
const Indices &SteadyStateData::get_gnodes_in_node(int i) const {
    return _gnodes_in_node.at(i);
};
const Indices &SteadyStateData::get_disrupted_nodes() const {
    return _disrupted_nodes;
};
const Indices &SteadyStateData::get_disrupted_pipes() const {
    return _disrupted_pipes;
};
const Indices &SteadyStateData::get_disrupted_compressors() const {
    return _disrupted_compressors;
};
const Indices &SteadyStateData::get_disrupted_gnodes() const {
    return _disrupted_gnodes;
};
const Indices &SteadyStateData::get_dependent_disrupted_pipes() const {
    return _dependent_distrupted_pipes;
};
const Indices &SteadyStateData::get_dependent_disrupted_compressors() const {
    return _dependent_disrupted_compressors;
};
const Indices &SteadyStateData::get_dependent_disrupted_gnodes() const {
    return _dependent_disrupted_gnodes;
};
int SteadyStateData::get_fnode_index_of_pipe(int i) const {
    return _fnode_of_pipe.at(i);
};
int SteadyStateData::get_tnode_index_of_pipe(int i) const {
    return _tnode_of_pipe.at(i);
};
int SteadyStateData::get_fnode_index_of_compressor(int i) const {
    return _fnode_of_compressor.at(i);
};
int SteadyStateData::get_tnode_index_of_compressor(int i) const {
    return _tnode_of_compressor.at(i);
};

void SteadyStateData::populate_parameters(const Data &data,
                                          const ScalingFactors &sf) {
    _p_min.in(_nodes);
    _p_max.in(_nodes);
    for (auto node : data.get_nodes()) {
        auto node_index = node->get_id();
        _p_min.set_value(node_index, node->get_pmin());
        _p_max.set_value(node_index, node->get_pmax());
    }

    _gbar.in(_gnodes);
    _smax.in(_gnodes);
    _dmax.in(_gnodes);
    _cs.in(_gnodes);
    _cd.in(_gnodes);
    auto disrupted_gnode_ids = data.get_disruption_gnode_ids();
    for (size_t i = 0; i < data.get_gnodes().size(); ++i) {
        auto gnode_index = data.get_gnodes().at(i)->get_id();
        if (find(disrupted_gnode_ids.begin(), disrupted_gnode_ids.end(),
                 gnode_index) != disrupted_gnode_ids.end()) {
            _gbar.set_value(gnode_index, data.get_gbar().at(i));
            _smax.set_value(gnode_index, data.get_smax().at(i));
            _dmax.set_value(gnode_index, data.get_dmax().at(i));
        } else {
            _gbar.set_value(gnode_index, 0.0);
            _smax.set_value(gnode_index, 0.0);
            _dmax.set_value(gnode_index, 0.0);
        }
        _cs.set_value(gnode_index, data.get_cs().at(i));
        _cd.set_value(gnode_index, data.get_cd().at(i));
    }

    _pslack.in(_slack_nodes);
    for (size_t i = 0; i < data.get_num_slack_nodes(); ++i) {
        auto slack_node_index = data.get_pslack_ids().at(i);
        _pslack.set_value(slack_node_index, data.get_pslack().at(i));
    }

    _cslack.in(_slack_nodes);
    for (size_t i = 0; i < data.get_num_slack_nodes(); ++i) {
        auto slack_node_index = data.get_cslack_ids().at(i);
        _cslack.set_value(slack_node_index, data.get_cslack().at(i));
    }

    _qbar.in(_non_slack_nodes);
    for (size_t i = 0; i < data.get_num_non_slack_nodes(); ++i) {
        auto non_slack_node_index = data.get_qbar_ids().at(i);
        _qbar.set_value(non_slack_node_index, data.get_qbar().at(i));
    }

    _phi_min_pipe.in(_pipes);
    _phi_max_pipe.in(_pipes);
    _length_pipe.in(_pipes);
    _diameter_pipe.in(_pipes);
    _friction_factor_pipe.in(_pipes);
    _area_pipe.in(_pipes);
    _resistance_pipe.in(_pipes);
    for (auto pipe : data.get_pipes()) {
        auto pipe_index = pipe->get_id();
        _length_pipe.set_value(pipe_index, pipe->get_length());
        _diameter_pipe.set_value(pipe_index, pipe->get_diameter());
        _friction_factor_pipe.set_value(pipe_index,
                                        pipe->get_friction_factor());
        double area =
            3.14159 * pipe->get_diameter() * pipe->get_diameter() / 4.0;
        _area_pipe.set_value(pipe_index, area);
        double resistance = pipe->get_friction_factor() * pipe->get_length() *
                            sf.get_space_scaling() / pipe->get_diameter();
        _resistance_pipe.set_value(pipe_index, resistance);
        int fnode_index = pipe->get_fnode_id();
        int tnode_index = pipe->get_tnode_id();
        double fnode_p_min = _p_min.get_value(fnode_index);
        double fnode_p_max = _p_max.get_value(fnode_index);
        double tnode_p_min = _p_min.get_value(tnode_index);
        double tnode_p_max = _p_max.get_value(tnode_index);
        double phi_min =
            -std::sqrt((tnode_p_max * tnode_p_max - fnode_p_min * fnode_p_min) /
                       resistance);
        double phi_max =
            std::sqrt((fnode_p_max * fnode_p_max - tnode_p_min * tnode_p_min) /
                      resistance);
        _phi_min_pipe.set_value(pipe_index, phi_min);
        _phi_max_pipe.set_value(pipe_index, phi_max);
    }

    _phi_min_compressor.in(_compressors);
    _phi_max_compressor.in(_compressors);
    _c_ratio_min.in(_compressors);
    _c_ratio_max.in(_compressors);
    _length_compressor.in(_compressors);
    _diameter_compressor.in(_compressors);
    _friction_factor_compressor.in(_compressors);
    _area_compressor.in(_compressors);
    _resistance_compressor.in(_compressors);
    _power_max_compressor.in(_compressors);
    auto disrupted_compressor_ids = data.get_disrupted_compressor_ids();
    for (auto compressor : data.get_compressors()) {
        auto compressor_index = compressor->get_id();
        if (std::find(disrupted_compressor_ids.begin(),
                      disrupted_compressor_ids.end(),
                      compressor_index) != disrupted_compressor_ids.end()) {
            _c_ratio_min.set_value(compressor_index, compressor->get_cmin());
            _c_ratio_max.set_value(compressor_index, compressor->get_cmax());
        } else {
            _c_ratio_min.set_value(compressor_index, 1.0);
            _c_ratio_max.set_value(compressor_index, 1.0);
        }
        _length_compressor.set_value(compressor_index,
                                     compressor->get_length());
        _diameter_compressor.set_value(compressor_index,
                                       compressor->get_diameter());
        _friction_factor_compressor.set_value(
            compressor_index, compressor->get_friction_factor());
        _power_max_compressor.set_value(compressor_index,
                                        compressor->get_power_max());
        double area = 3.14159 * compressor->get_diameter() *
                      compressor->get_diameter() / 4.0;
        _phi_min_compressor.set_value(compressor_index,
                                      compressor->get_flow_min() / area);
        _phi_max_compressor.set_value(compressor_index,
                                      compressor->get_flow_max() / area);
        _area_compressor.set_value(compressor_index, area);
        double resistance = compressor->get_friction_factor() *
                            compressor->get_length() * sf.get_space_scaling() /
                            compressor->get_diameter();
        _resistance_compressor.set_value(compressor_index, resistance);
    }
};

void SteadyStateData::populate_indices(const Data &data) {
    auto disrupted_node_ids = data.get_disrupted_node_ids();
    auto disrupted_pipe_ids = data.get_disrupted_pipe_ids();
    auto disrupted_compressor_ids = data.get_disrupted_compressor_ids();
    auto disrupted_gnode_ids = data.get_disrupted_compressor_ids();
    std::set<int> disrupted_dependent_pipe_ids = {};
    std::set<int> disrupted_dependent_compressor_ids = {};
    std::set<int> disrupted_dependent_gnode_ids {};
    for (auto node_id : disrupted_node_ids) {
        auto in_pipe_ids = data.get_in_pipe_ids_of_disrupted_node(node_id);
        auto out_pipe_ids = data.get_out_pipe_ids_of_disrupted_node(node_id);
        disrupted_dependent_pipe_ids.insert(in_pipe_ids.begin(), in_pipe_ids.end());
        disrupted_dependent_pipe_ids.insert(out_pipe_ids.begin(), out_pipe_ids.end());
        auto in_compressor_ids =
            data.get_in_compressor_ids_of_disrupted_node(node_id);
        auto out_compressor_ids =
            data.get_out_compressor_ids_of_disrupted_node(node_id);
        disrupted_dependent_compressor_ids.insert(in_compressor_ids.begin(), in_compressor_ids.end());
        disrupted_dependent_compressor_ids.insert(out_compressor_ids.begin(), out_compressor_ids.end());
        auto gnode_ids = data.get_gnodes_of_node(node_id);
        disrupted_dependent_gnode_ids.insert(gnode_ids.begin(), gnode_ids.end());
    }

    for (auto node : data.get_nodes()) {
        auto node_index = node->get_id();
        _nodes.insert(node_index);
        if (node->is_slack())
            _slack_nodes.insert(node_index);
        else
            _non_slack_nodes.insert(node_index);
        _out_pipes_in_node[node_index] = Indices();
        _in_pipes_in_node[node_index] = Indices();
        _out_compressors_in_node[node_index] = Indices();
        _in_compressors_in_node[node_index] = Indices();
        _gnodes_in_node[node_index] = Indices();
    }

    for (auto pipe : data.get_pipes()) {
        auto pipe_index = pipe->get_id();

        _pipes.insert(pipe_index);
        auto fnode_index = pipe->get_fnode_id();
        auto tnode_index = pipe->get_tnode_id();
        _out_pipes_in_node[fnode_index].insert(pipe_index);
        _in_pipes_in_node[tnode_index].insert(pipe_index);
        _fnode_of_pipe[pipe_index] = fnode_index;
        _tnode_of_pipe[pipe_index] = tnode_index;
    }

    for (auto compressor : data.get_compressors()) {
        auto compressor_index = compressor->get_id();
        _compressors.insert(compressor_index);
        auto fnode_index = compressor->get_fnode_id();
        auto tnode_index = compressor->get_tnode_id();
        _out_compressors_in_node[fnode_index].insert(compressor_index);
        _in_compressors_in_node[tnode_index].insert(compressor_index);
        _fnode_of_compressor[compressor_index] = fnode_index;
        _tnode_of_compressor[compressor_index] = tnode_index;
    }

    for (auto gnode : data.get_gnodes()) {
        auto gnode_index = gnode->get_id();
        _gnodes.insert(gnode_index);
        auto node_index = gnode->get_node_id();
        _gnodes_in_node[node_index].insert(gnode_index);
    }

    return;
};
