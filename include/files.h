#ifndef FILES_H
#define FILES_H

#pragma once

#include <string>

struct InputFilenames {
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

struct OutputFilenames {
    const static std::string pipe_flow_in;
    const static std::string pipe_flow_out;
    const static std::string pipe_pressure_in;
    const static std::string pipe_pressure_out;
    const static std::string slack_flows;
    const static std::string non_slack_flows;
    const static std::string nodal_pressure;
    const static std::string gnode_demand_flows;
    const static std::string gnode_supply_flows;
    const static std::string comp_ratios;
    const static std::string comp_discharge_pressure;
    const static std::string comp_flow_in;
    const static std::string comp_flow_out;
    const static std::string comp_pressure_in;
    const static std::string comp_pressure_out;
};


#endif 
