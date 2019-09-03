#ifndef FILES_H
#define FILES_H

#pragma once

#include <string>

struct Filenames {
    const static std::string nodes;
    const static std::string pipes;
    const static std::string compressors;
    const static std::string gnodes;

    const static std::string input_params;
    
    const static std::string demand_gnodes_price_bids;
    const static std::string supply_gnodes_price_bids;

    const static std::string slack_node_price;

    const static std::string slack_pressure;

    const static std::string demand_gnodes_max_offtakes;
    const static std::string supply_gnodes_max_injections;

    const static std::string nonslack_physical_baseline_withdrawals;
    const static std::string gnodes_baseline_consumer_withdrawals;
};


#endif 