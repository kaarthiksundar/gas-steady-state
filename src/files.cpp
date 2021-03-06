#include <files.h>
#include <string>

const std::string InputFilenames::nodes = "input_network_nodes.csv";
const std::string InputFilenames::pipes = "input_network_pipes.csv";
const std::string InputFilenames::compressors = "input_network_comps.csv";
const std::string InputFilenames::gnodes = "input_network_gnodes.csv";

const std::string InputFilenames::input_params = "input_param.csv";

const std::string InputFilenames::demand_gnodes_price_bids = "input_int_cd.csv";
const std::string InputFilenames::supply_gnodes_price_bids = "input_int_cs.csv";

const std::string InputFilenames::slack_node_price = "input_int_cslack.csv";

const std::string InputFilenames::slack_pressure = "input_int_pslack.csv";

const std::string InputFilenames::demand_gnodes_max_offtakes =
    "input_int_dmax.csv";
const std::string InputFilenames::supply_gnodes_max_injections =
    "input_int_smax.csv";

const std::string InputFilenames::nonslack_physical_baseline_withdrawals =
    "input_int_qbar.csv";
const std::string InputFilenames::gnodes_baseline_consumer_withdrawals =
    "input_int_gbar.csv";

const std::string InputFilenames::disruption_pipes =
    "input_disrupt_pipe.csv";
const std::string InputFilenames::disruption_nodes =
    "input_disrupt_node.csv";
const std::string InputFilenames::disruption_compressors =
    "input_disrupt_comp.csv";
const std::string InputFilenames::disruption_gnodes =
    "input_disrupt_gnode.csv";

const std::string OutputFilenames::pipe_flow_in = "output_ss_pipe_flow_in.csv";
const std::string OutputFilenames::pipe_flow_out =
    "output_ss_pipe_flow_out.csv";

const std::string OutputFilenames::pipe_pressure_in =
    "output_ss_pipe_pressure_in.csv";
const std::string OutputFilenames::pipe_pressure_out =
    "output_ss_pipe_pressure_out.csv";

const std::string OutputFilenames::slack_flows = "output_ss_slack_flows.csv";
const std::string OutputFilenames::non_slack_flows =
    "output_ss_non_slack_flows.csv";

const std::string OutputFilenames::nodal_pressure =
    "output_ss_nodal_pressure.csv";
const std::string OutputFilenames::gnode_demand_flows =
    "output_ss_gnode_demand_flows.csv";
const std::string OutputFilenames::gnode_supply_flows =
    "output_ss_gnode_supply_flows.csv";

const std::string OutputFilenames::comp_ratios = "output_ss_comp_ratios.csv";
const std::string OutputFilenames::comp_discharge_pressure =
    "ouput_ss_comp_discharge_pressure.csv";
const std::string OutputFilenames::comp_flow_in = "output_ss_comp_flows_in.csv";
const std::string OutputFilenames::comp_flow_out =
    "output_ss_comp_flows_out.csv";
const std::string OutputFilenames::comp_pressure_in =
    "output_ss_comp_pressure_in.csv";
const std::string OutputFilenames::comp_pressure_out =
    "output_ss_comp_pressure_out.csv";
const std::string OutputFilenames::comp_power = "output_ss_comp_power.csv";
