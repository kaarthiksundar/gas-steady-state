#include <problem_data.h>
#include <algorithm>

ProblemData::ProblemData(const Network & net, const Nondimensionalization & nd) {
    p_min = Param("p_min"); p_max = Param("p_max");
    phi_min_compressor = Param("phi_min_comperssor"); 
    phi_max_compressor = Param("phi_max_compressor");
    phi_min_pipe = Param("phi_min_pipe"); 
    phi_max_pipe = Param("phi_max_pipe");
    c_ratio_min = Param("c_ratio_min"); 
    c_ratio_max = Param("c_ratio_max");
    length_pipe = Param("length_pipe");
    diameter_pipe = Param("diameter_pipe");
    friction_factor_pipe = Param("friction_factor_pipe");
    length_compressor = Param("length_compressor");
    diameter_compressor = Param("diameter_compressor");
    friction_factor_compressor = Param("friction_factor_compressor");
    cd = Param("cd"); cs = Param("cs");
    pslack = Param("pslack"); cslack = Param("cslack");
    qbar = Param("qbar"); 
    gbar = Param("gbar"); smax = Param("smax"); dmax = Param("dmax");
    area_pipe = Param("area_pipe");
    area_compressor = Param("area_compressor");

    populate_indices(net);
    populate_parameters(net, nd);
}; 

void ProblemData::populate_parameters(const Network & net, const Nondimensionalization & nd) {
    p_min.in(nodes); 
    p_max.in(nodes);
    for (auto node : net.nodes) {
        auto node_index = std::stoi(node->_id);
        p_min.set_value(node_index, node->_pmin);
        p_max.set_value(node_index, node->_pmax);
    }

    gbar.in(gnodes); smax.in(gnodes); dmax.in(gnodes);
    cs.in(gnodes); cd.in(gnodes);
    for (size_t i = 0; i<net.gbar.size(); ++i) {
        auto gnode_index = std::stoi(net.gnodes[i]->_id);
        gbar.set_value(gnode_index, net.gbar.at(i));
        smax.set_value(gnode_index, net.smax.at(i));
        dmax.set_value(gnode_index, net.dmax.at(i));
        cs.set_value(gnode_index, net.cs.at(i));
        cd.set_value(gnode_index, net.cd.at(i));
    }

    pslack.in(slack_nodes); 
    for (size_t i=0; i<net.num_slack_nodes; ++i) {
        auto slack_node_index = std::stoi(net.pslack_ids[i]);
        pslack.set_value(slack_node_index, net.pslack.at(i));
    }

    cslack.in(slack_nodes);
    for (size_t i=0; i<net.num_slack_nodes; ++i) {
        auto slack_node_index = std::stoi(net.cslack_ids[i]);
        cslack.set_value(slack_node_index, net.cslack.at(i));
    }

    qbar.in(non_slack_nodes);
    for (size_t i=0; i<net.num_non_slack_nodes; ++i) {
        auto non_slack_node_index = std::stoi(net.qbar_ids[i]);
        qbar.set_value(non_slack_node_index, net.qbar.at(i));
    }

    phi_min_pipe.in(pipes, -10000.0);
    phi_max_pipe.in(pipes, 10000.0);
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
            nd.space_factor / 2.0 / pipe->_diameter;
        resistance_pipe.set_value(pipe_index, resistance);
    }
    
    phi_min_compressor.in(compressors, -10000.0);
    phi_max_compressor.in(compressors, 10000.0);
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
        area_compressor.set_value(compressor_index, area);
        double resistance = compressor->_friction_factor * compressor->_length * 
            nd.space_factor / 2.0 / compressor->_diameter;
        resistance_compressor.set_value(compressor_index, resistance);
    }
};

void ProblemData::populate_indices(const Network & net) {
    for (auto node : net.nodes) {
        auto node_index = std::stoi(node->_id);
        nodes.insert(node_index);
        if (node->_slack) slack_nodes.insert(node_index);
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
