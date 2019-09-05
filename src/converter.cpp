#include <converter.h>
#include <cmath>
#include <algorithm>
#include <functional>


Converter::Converter() { 
    this->psi_to_pascal = 6894.75729;
    this->km_to_m = 1000.0;
    this->R = 8314.472;
    this->one_atm = 101.325; /* pascal */
    this->miles_to_m = 1609.64;
    this->inches_to_m = 2.54/100.0;
};

Converter::~Converter() { };

void Converter::populate_mmscfd_conversion_factors(const InputParams* ip) {
    mmscfd_to_kgps = 1000*0.2832/86400*(one_atm/(R*ip->temperature)*100*100*100)*
        ip->gas_specific_gravity*28.9626;
    mmscfd_to_hp = 1000.0/0.0593807; /* fix later - should depend on calorific value */
    a = std::sqrt(R * ip->temperature);
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
        node->_si_units = true;
        node->_standard_units = false;
    }

    std::transform(net.pslack.begin(), net.pslack.end(), net.pslack.begin(),
        std::bind(std::multiplies<double>(), std::placeholders::_1, psi_to_pascal));

    std::transform(net.qbar.begin(), net.qbar.end(), net.qbar.begin(), 
        std::bind(std::multiplies<double>(), std::placeholders::_1, mmscfd_to_kgps));

    std::transform(net.gbar.begin(), net.gbar.end(), net.gbar.begin(), 
        std::bind(std::multiplies<double>(), std::placeholders::_1, mmscfd_to_kgps));

    std::transform(net.smax.begin(), net.smax.end(), net.smax.begin(), 
        std::bind(std::multiplies<double>(), std::placeholders::_1, mmscfd_to_kgps));

    std::transform(net.dmax.begin(), net.dmax.end(), net.dmax.begin(), 
        std::bind(std::multiplies<double>(), std::placeholders::_1, mmscfd_to_kgps));
    
    net.standard_units = false;
    net.si_units = true;
}