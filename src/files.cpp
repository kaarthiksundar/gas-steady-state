#include <files.h>

#include <string>

const std::string Filenames::nodes = "input_network_nodes.csv";
const std::string Filenames::pipes = "input_network_pipes.csv";
const std::string Filenames::compressors = "input_network_comps.csv";
const std::string Filenames::gnodes = "input_network_gnodes.csv";

const std::string Filenames::input_params = "input_param.csv";

const std::string Filenames::demand_gnodes_price_bids = "input_int_cd.csv";
const std::string Filenames::supply_gnodes_price_bids = "input_int_cs.csv";

const std::string Filenames::slack_node_price = "input_int_cslack.csv";

const std::string Filenames::slack_pressure = "input_int_pslack.csv";

const std::string Filenames::demand_gnodes_max_offtakes = "input_int_dmax.csv";
const std::string Filenames::supply_gnodes_max_injections = "input_int_smax.csv";

const std::string Filenames::nonslack_physical_baseline_withdrawals = "input_int_qbar.csv";
const std::string Filenames::gnodes_baseline_consumer_withdrawals = "input_int_gbar.csv";