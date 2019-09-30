#ifndef PROBLEM_DATA_H
#define PROBLEM_DATA_H

#include <utils.h>
#include <data.h>
#include <conversions.h>

class ProblemData {
public:
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
    std::unordered_map<int, Indices> _out_compressors_in_node, _in_compressors_in_node;
    std::unordered_map<int, Indices> _gnodes_in_node;
    std::unordered_map<int, int> _fnode_of_pipe, _tnode_of_pipe;
    std::unordered_map<int, int> _fnode_of_compressor, _tnode_of_compressor;
    
    ProblemData(const Data & data, const ScalingFactors & sf);
    
private:
    void populate_parameters(const Data & data, const ScalingFactors & sf);
    void populate_indices(const Data & data);
    
};

#endif 
