#ifdef FILENAMES_H
#define FILENAMES_H

#include <string>

struct Filenames {
    std::string nodes = "input_network_nodes.csv";
    std::string pipes = "input_network_pipes.csv";
    std::string compressors = "input_network_comps.csv";
    std::string gnodes = "input_network_gnodes.csv";

    std::string input_params = "input_param.csv";
    
    std::string demand_gnodes_price_bids = "input_int_cd.csv";
    std::string supply_gnodes_price_bids = "input_int_cs.csv";

    std::string slack_node_price = "input_cslack.csv";

    std::string slack_pressure = "input_pslack.csv";

    std::string demand_gnodes_max_offtakes = "input_dmax.csv";
    std::string supply_gnodes_max_injections = "input_smax.csv";

    std::string nonslack_physical_baseline_withdrawals = "input_qbar.csv";
    std::string gnodes_baseline_consumer_withdrawals = "input_gbar.csv";
};

const Filenames get_filenames() { 
    Filenames filenames;
    return filenames;
}

#endif 