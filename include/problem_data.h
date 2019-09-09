#ifndef PROBLEM_DATA_H
#define PROBLEM_DATA_H

#include <utils.h>
#include <network.h>
#include <nondimensionalization.h>

class ProblemData {
    public:
        /* parameters */
        Param p_min, p_max;
        Param phi_min_compressor, phi_max_compressor;
        Param phi_min_pipe, phi_max_pipe;
        Param c_ratio_min, c_ratio_max;
        Param length_pipe, diameter_pipe, friction_factor_pipe;
        Param length_compressor, diameter_compressor, friction_factor_compressor;
        Param resistance_pipe, resistance_compressor;
        Param cd, cs;
        Param pslack, cslack;
        Param qbar;
        Param gbar, smax, dmax;
        Param area_pipe, area_compressor;
        /* Indices */
        Indices nodes;
        Indices pipes;
        Indices compressors;
        Indices gnodes;
        Indices slack_nodes;
        Indices non_slack_nodes;
        std::unordered_map<int, Indices> out_pipes_in_node, in_pipes_in_node;
        std::unordered_map<int, Indices> out_compressors_in_node, in_compressors_in_node;
        std::unordered_map<int, Indices> gnodes_in_node;
        std::unordered_map<int, int> fnode_of_pipe, tnode_of_pipe;
        std::unordered_map<int, int> fnode_of_compressor, tnode_of_compressor;
        
        ProblemData(const Network & network, const Nondimensionalization & nd);

    private: 
        void populate_parameters(const Network & network, const Nondimensionalization & nd);
        void populate_indices(const Network & network);

};

#endif 