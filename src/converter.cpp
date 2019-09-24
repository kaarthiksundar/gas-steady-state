#include <converter.h>
#include <cmath>
#include <algorithm>
#include <functional>


Converter::Converter() { 
    this->psi_to_pascal = 6894.75729;
    this->km_to_m = 1000.0;
    this->universal_R = 8314.472;
    this->molecular_weight_air = 28.9626;
    this->one_atm = 101.325; /* pascal */
    this->miles_to_m = 1609.64;
    this->inches_to_m = 2.54/100.0;
};

void Converter::populate_mmscfd_conversion_factors(const InputParams* ip) {
    mmscfd_to_kgps = 1000*0.02832/86400*(one_atm/(universal_R * ip->get_temperature())*100*100*100)*
        ip->get_gas_specific_gravity()*molecular_weight_air;
    gas_R = universal_R / molecular_weight_air /ip->get_gas_specific_gravity();
    mmscfd_to_hp = 1000.0/0.0593807; /* fix later - should depend on calorific value */
    a = std::sqrt(gas_R * ip->get_temperature());
};

void Converter::convert_to_si(Network & net) {
    if (net.si_units) 
        return;
    for (auto pipe : net.pipes) {
        pipe->_diameter *= inches_to_m;
        pipe->_length *= miles_to_m;
        pipe->_si_units = true;
        pipe->_standard_units = false;
    }

    for (auto node : net.nodes) {
        node->_pmin *= psi_to_pascal;
        node->_pmax *= psi_to_pascal;
        node->_injection_min *= mmscfd_to_kgps;
        node->_injection_max *= mmscfd_to_kgps;
        node->_si_units = true;
        node->_standard_units = false;
    }
    
    for (auto compressor : net.compressors) {
        compressor->_flow_max *= mmscfd_to_kgps;
        compressor->_si_units = true;
        compressor->_standard_units = false;
    }

    std::transform(net.pslack.begin(), net.pslack.end(), net.pslack.begin(),
        std::bind(std::multiplies<double>(), std::placeholders::_1, psi_to_pascal));
    
    std::transform(net.cslack.begin(), net.cslack.end(), net.cslack.begin(),
                   std::bind(std::divides<double>(), std::placeholders::_1, mmscfd_to_kgps));

    std::transform(net.qbar.begin(), net.qbar.end(), net.qbar.begin(), 
        std::bind(std::multiplies<double>(), std::placeholders::_1, mmscfd_to_kgps));

    std::transform(net.gbar.begin(), net.gbar.end(), net.gbar.begin(), 
        std::bind(std::multiplies<double>(), std::placeholders::_1, mmscfd_to_kgps));

    std::transform(net.smax.begin(), net.smax.end(), net.smax.begin(), 
        std::bind(std::multiplies<double>(), std::placeholders::_1, mmscfd_to_kgps));

    std::transform(net.dmax.begin(), net.dmax.end(), net.dmax.begin(), 
        std::bind(std::multiplies<double>(), std::placeholders::_1, mmscfd_to_kgps));
    
    std::transform(net.cs.begin(), net.cs.end(), net.cs.begin(),
        std::bind(std::divides<double>(), std::placeholders::_1, mmscfd_to_kgps));
    
    std::transform(net.cd.begin(), net.cd.end(), net.cd.begin(),
        std::bind(std::divides<double>(), std::placeholders::_1, mmscfd_to_kgps));
    
    net.standard_units = false;
    net.si_units = true;
}
