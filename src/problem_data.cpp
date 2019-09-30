#include <problem_data.h>
#include <algorithm>

ProblemData::ProblemData(const Data & data, const ScalingFactors & sf) {
    _p_min = Param("p_min"); _p_max = Param("p_max");
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
    _cd = Param("cd"); _cs = Param("cs");
    _pslack = Param("pslack"); _cslack = Param("cslack");
    _qbar = Param("qbar");
    _gbar = Param("gbar"); _smax = Param("smax"); _dmax = Param("dmax");
    _area_pipe = Param("area_pipe");
    _area_compressor = Param("area_compressor");
    
    populate_indices(data);
    populate_parameters(data, sf);
}; 

void ProblemData::populate_parameters(const Data & data, const ScalingFactors & sf) {
    _p_min.in(_nodes);
    _p_max.in(_nodes);
    for (auto node : data.get_nodes()) {
        auto node_index = node->get_id();
        _p_min.set_value(node_index, node->get_pmin());
        _p_max.set_value(node_index, node->get_pmax());
    }
    
    _gbar.in(_gnodes); _smax.in(_gnodes); _dmax.in(_gnodes);
    _cs.in(_gnodes); _cd.in(_gnodes);
    for (size_t i=0; i<data.get_gnodes().size(); ++i) {
        auto gnode_index = data.get_nodes().at(i)->get_id();
        _gbar.set_value(gnode_index, data.get_gbar().at(i));
        _smax.set_value(gnode_index, data.get_smax().at(i));
        _dmax.set_value(gnode_index, data.get_dmax().at(i));
        _cs.set_value(gnode_index, data.get_cs().at(i));
        _cd.set_value(gnode_index, data.get_cd().at(i));
    }
    
    _pslack.in(_slack_nodes);
    for (size_t i=0; i<data.get_num_slack_nodes(); ++i) {
        auto slack_node_index = data.get_pslack_ids().at(i);
        _pslack.set_value(slack_node_index, data.get_pslack().at(i));
    }
    
    _cslack.in(_slack_nodes);
    for (size_t i=0; i<data.get_num_slack_nodes(); ++i) {
        auto slack_node_index = data.get_cslack_ids().at(i);
        _cslack.set_value(slack_node_index, data.get_cslack().at(i));
    }
    
    _qbar.in(_non_slack_nodes);
    for (size_t i=0; i<data.get_num_non_slack_nodes(); ++i) {
        auto non_slack_node_index = data.get_qbar_ids().at(i);
        _qbar.set_value(non_slack_node_index, data.get_qbar().at(i));
    }
    
    phi_min_pipe.in(pipes, -100.0);
    phi_max_pipe.in(pipes, 200.0);
    length_pipe.in(pipes);
    diameter_pipe.in(pipes);
    friction_factor_pipe.in(pipes);
    area_pipe.in(pipes);
    resistance_pipe.in(pipes);
    for (auto pipe : net.pipes) {
        auto pipe_index = std::stoi(pipe->_id);
        length_pipe.set_value(pipe_index, pipe->_length);
        diameter_pipe.set_value(pipe_index, pipe->_diameter);
        friction_factor_pipe.set_value(pipe_index, pipe->_friction_factor);
        double area = 3.14159 * pipe->_diameter * pipe->_diameter / 4.0;
        area_pipe.set_value(pipe_index, area);
        double resistance = pipe->_friction_factor * pipe->_length *
        nd.space_factor / pipe->_diameter;
        resistance_pipe.set_value(pipe_index, resistance);
    }
    
    phi_min_compressor.in(compressors);
    phi_max_compressor.in(compressors);
    c_ratio_min.in(compressors);
    c_ratio_max.in(compressors);
    length_compressor.in(compressors);
    diameter_compressor.in(compressors);
    friction_factor_compressor.in(compressors);
    area_compressor.in(compressors);
    resistance_compressor.in(compressors);
    for (auto compressor : net.compressors) {
        auto compressor_index = std::stoi(compressor->_id);
        c_ratio_min.set_value(compressor_index, compressor->_cmin);
        c_ratio_max.set_value(compressor_index, compressor->_cmax);
        length_compressor.set_value(compressor_index, compressor->_length);
        diameter_compressor.set_value(compressor_index, compressor->_diameter);
        friction_factor_compressor.set_value(compressor_index, compressor->_friction_factor);
        double area = 3.14159 * compressor->_diameter * compressor->_diameter / 4.0;
        phi_min_compressor.set_value(compressor_index, compressor->_flow_min/area);
        phi_max_compressor.set_value(compressor_index, compressor->_flow_max/area);
        area_compressor.set_value(compressor_index, area);
        double resistance = compressor->_friction_factor * compressor->_length *
        nd.space_factor / compressor->_diameter;
        resistance_compressor.set_value(compressor_index, resistance);
    }
};

void ProblemData::populate_indices(const Data & data) {
    for (auto node : data.get_nodes()) {
        auto node_index = node->get_id();
        _nodes.insert(node_index);
        if (node->is_slack()) slack_nodes.insert(node_index);
        else non_slack_nodes.insert(node_index);
        out_pipes_in_node[node_index] = Indices();
        in_pipes_in_node[node_index] = Indices();
        out_compressors_in_node[node_index] = Indices();
        in_compressors_in_node[node_index] = Indices();
        gnodes_in_node[node_index] = Indices();
    }
    
    for (auto pipe : net.pipes) {
        auto pipe_index = std::stoi(pipe->_id);
        pipes.insert(pipe_index);
        auto fnode_index = std::stoi(pipe->_fnode);
        auto tnode_index = std::stoi(pipe->_tnode);
        out_pipes_in_node[fnode_index].insert(pipe_index);
        in_pipes_in_node[tnode_index].insert(pipe_index);
        fnode_of_pipe[pipe_index] = fnode_index;
        tnode_of_pipe[pipe_index] = tnode_index;
    }
    
    for (auto compressor : net.compressors) {
        auto compressor_index = std::stoi(compressor->_id);
        compressors.insert(compressor_index);
        auto fnode_index = std::stoi(compressor->_fnode);
        auto tnode_index = std::stoi(compressor->_tnode);
        out_compressors_in_node[fnode_index].insert(compressor_index);
        in_compressors_in_node[tnode_index].insert(compressor_index);
        fnode_of_compressor[compressor_index] = fnode_index;
        tnode_of_compressor[compressor_index] = tnode_index;
    }
    
    for (auto gnode : net.gnodes) {
        auto gnode_index = std::stoi(gnode->_id);
        gnodes.insert(gnode_index);
        auto node_index = std::stoi(gnode->_node);
        gnodes_in_node[node_index].insert(gnode_index);
    }
    
    return;
};
