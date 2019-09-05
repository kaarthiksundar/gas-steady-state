#ifndef PROBLEM_H
#define PROBLEM_H

#include <gravity/solver.h>
#include <network.h>
using namespace gravity;

class Problem {
    public:
        /* parameters */
        param<double> p_min, p_max;
        param<double> phi_min_compressor, phi_max_compressor;
        param<double> phi_min_pipe, phi_max_pipe;
        param<double> c_ratio_min, c_ratio_max;
        param<double> length_pipe, diameter_pipe, friction_factor_pipe;
        param<double> length_compressor, diameter_compressor, friction_factor_compressor;
        param<double> cd, cs;
        param<double> pslack, cslack;
        param<double> qbar;
        param<double> gbar, smax, dmax;
        param<double> area_pipe, area_compressor;
        /* indices */
        indices nodes;
        indices pipes;
        indices compressors;
        indices gnodes;
        indices slack_nodes;
        indices non_slack_nodes;
        indices out_pipes_per_node, in_pipes_per_node;
        indices out_compressors_per_node, in_compressors_per_node;
        indices gnodes_per_node;
        
        /* model */ 
        Model<> model;
        /* variables */
        var<> p, phi_pipe, phi_compressor, s, d;
        /* constraints */
        Constraint<> pipe_physics, compressor_physics;
        Constraint<> nodal_balance;
        Constraint<> pressure_limits_compressor;
        

        Problem(const Network & network);
        ~Problem();

        void create_model();

    private: 
        void populate_parameters(const Network & network);
        void populate_indices(const Network & network);
        indices get_gnodes_per_node(const Network &, indices);

};

#endif 