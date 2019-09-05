#include <nondimensionalization.h>

#include <limits>
#include <algorithm>
#include <functional>

Nondimensionalization::Nondimensionalization(const Network & net, const Converter & converter) {
    this->p_factor = std::numeric_limits<double>::max();
    for (auto node : net.nodes) {
        if (node->_slack && node->_pmin < p_factor)
            this->p_factor = node->_pmin;
    }
    
    this->space_factor = 1000.0;
    this->time_factor = this->space_factor / converter.a;
    this->flow_factor = this->p_factor / converter.a;
};

Nondimensionalization::~Nondimensionalization() { };

void Nondimensionalization::non_dimensionalize(Network & net) {
    if (net.is_dimensional == false) return;
    net.is_dimensional = false;
    for (auto pipe : net.pipes) 
        pipe->_length /= space_factor;
    
    for (auto node : net.nodes) {
        node->_pmin /= p_factor;
        node->_pmax /= p_factor;
        node->_injection_min /= flow_factor;
        node->_injection_max /= flow_factor;
    }

    for (auto compressor : net.compressors) 
        compressor->_length /= space_factor;

    std::transform(net.pslack.begin(), net.pslack.end(), net.pslack.begin(),
        std::bind(std::divides<double>(), std::placeholders::_1, p_factor));

    std::transform(net.qbar.begin(), net.qbar.end(), net.qbar.begin(), 
        std::bind(std::divides<double>(), std::placeholders::_1, flow_factor));

    std::transform(net.gbar.begin(), net.gbar.end(), net.gbar.begin(), 
        std::bind(std::divides<double>(), std::placeholders::_1, flow_factor));

    std::transform(net.smax.begin(), net.smax.end(), net.smax.begin(), 
        std::bind(std::divides<double>(), std::placeholders::_1, flow_factor));

    std::transform(net.dmax.begin(), net.dmax.end(), net.dmax.begin(), 
        std::bind(std::divides<double>(), std::placeholders::_1, flow_factor));

};