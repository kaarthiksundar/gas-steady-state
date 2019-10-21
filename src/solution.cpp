#include <solution.h>
#include <csvwriter.h>
#include <files.h>
#include <iomanip>

//SteadyStateSolution::SteadyStateSolution(const Data & data,
//                                         SteadyStateModelData* md,
//                                         ProblemData *pd,
//                                         Model* m) {
//    for (auto pipe : net.pipes) {
//        auto pipe_id = std::stoi(pipe->_id);
//        auto pipe_flow = m->get_value(md->phi_p[pipe_id]);
//        _pipe_flow_in.push_back(pipe_flow * pd->area_pipe.get_value(pipe_id));
//        _pipe_flow_out.push_back(pipe_flow* pd->area_pipe.get_value(pipe_id));
//        auto fnode_id = std::stoi(pipe->_fnode);
//        auto tnode_id = std::stoi(pipe->_tnode);
//        _pipe_pressure_in.push_back(m->get_value(md->rho[fnode_id]));
//        _pipe_pressure_out.push_back(m->get_value(md->rho[tnode_id]));
//    }
//
//    for (auto slack_node_id : net.pslack_ids) {
//        auto id = std::stoi(slack_node_id);
//        _slack_flows.push_back(m->get_value(md->slack_production[id]));
//    }
//
//    for (auto non_slack_node_id : net.qbar_ids) {
//        auto id = std::stoi(non_slack_node_id);
//        double val = pd->qbar.get_value(id);
//        auto gnodes_in_node = pd->gnodes_in_node[id];
//        for (auto gnode_id : gnodes_in_node) {
//            val += m->get_value(md->d[gnode_id]);
//            val -= m->get_value(md->s[gnode_id]);
//            val += pd->gbar.get_value(gnode_id);
//        }
//        _non_slack_flows.push_back(val);
//    }
//
//    for (auto node : net.nodes) {
//        auto node_id = std::stoi(node->_id);
//        if (node->_slack == true)
//            _nodal_pressure.push_back(pd->pslack.get_value(node_id));
//        else
//            _nodal_pressure.push_back(m->get_value(md->rho[node_id]));
//    }
//
//    for (auto gnode : net.gnodes) {
//        auto gnode_id = std::stoi(gnode->_id);
//        _gnode_demand_flows.push_back(m->get_value(md->d[gnode_id]));
//        _gnode_supply_flows.push_back(m->get_value(md->s[gnode_id]));
//    }
//
//    for (auto compressor : net.compressors) {
//        auto compressor_id = std::stoi(compressor->_id);
//        auto fnode_id = std::stoi(compressor->_fnode);
//        auto tnode_id = std::stoi(compressor->_tnode);
//        _comp_ratios.push_back(m->get_value(md->alpha[compressor_id]));
//        _comp_flow_in.push_back(m->get_value(md->phi_c[compressor_id]) * pd->area_compressor.get_value(compressor_id));
//        _comp_flow_out.push_back(m->get_value(md->phi_c[compressor_id]) * pd->area_compressor.get_value(compressor_id));
//        _comp_discharge_pressure.push_back(m->get_value(md->rho[tnode_id]));
//        _comp_pressure_out.push_back(m->get_value(md->rho[tnode_id]));
//        if (pd->slack_nodes.find(fnode_id) != pd->slack_nodes.end())
//            _comp_pressure_in.push_back(pd->pslack.get_value(fnode_id));
//        else
//            _comp_pressure_in.push_back(m->get_value(md->rho[fnode_id]));
//    }
//
//    is_dimensional = false;
//    is_standard = false;
//    is_si = false;
//};

/*
void SteadyStateSolution::dimensionalize(const Nondimensionalization & nd) {
    std::transform(_pipe_flow_in.begin(), _pipe_flow_in.end(), _pipe_flow_in.begin(),
                   std::bind(std::multiplies<double>(), std::placeholders::_1, nd.flow_factor));
    
    std::transform(_pipe_flow_out.begin(), _pipe_flow_out.end(), _pipe_flow_out.begin(),
                   std::bind(std::multiplies<double>(), std::placeholders::_1, nd.flow_factor));
    
    std::transform(_slack_flows.begin(), _slack_flows.end(), _slack_flows.begin(),
                   std::bind(std::multiplies<double>(), std::placeholders::_1, nd.flow_factor));
    
    std::transform(_non_slack_flows.begin(), _non_slack_flows.end(), _slack_flows.begin(),
                   std::bind(std::multiplies<double>(), std::placeholders::_1, nd.flow_factor));
    
    std::transform(_gnode_demand_flows.begin(), _gnode_demand_flows.end(), _gnode_demand_flows.begin(),
                   std::bind(std::multiplies<double>(), std::placeholders::_1, nd.flow_factor));
    
    std::transform(_gnode_supply_flows.begin(), _gnode_supply_flows.end(), _gnode_supply_flows.begin(),
                   std::bind(std::multiplies<double>(), std::placeholders::_1, nd.flow_factor));
    
    std::transform(_comp_flow_in.begin(), _comp_flow_in.end(), _comp_flow_in.begin(),
                   std::bind(std::multiplies<double>(), std::placeholders::_1, nd.flow_factor));
    
    std::transform(_comp_flow_out.begin(), _comp_flow_out.end(), _comp_flow_out.begin(),
                   std::bind(std::multiplies<double>(), std::placeholders::_1, nd.flow_factor));
    
    std::transform(_pipe_pressure_in.begin(), _pipe_pressure_in.end(), _pipe_pressure_in.begin(),
                   std::bind(std::multiplies<double>(), std::placeholders::_1, nd.p_factor));
    
    std::transform(_pipe_pressure_out.begin(), _pipe_pressure_out.end(), _pipe_pressure_out.begin(),
                   std::bind(std::multiplies<double>(), std::placeholders::_1, nd.p_factor));
    
    std::transform(_nodal_pressure.begin(), _nodal_pressure.end(), _nodal_pressure.begin(),
                   std::bind(std::multiplies<double>(), std::placeholders::_1, nd.p_factor));
    
    std::transform(_comp_discharge_pressure.begin(), _comp_discharge_pressure.end(), _comp_discharge_pressure.begin(),
                   std::bind(std::multiplies<double>(), std::placeholders::_1, nd.p_factor));
    
    std::transform(_comp_pressure_in.begin(), _comp_pressure_in.end(), _comp_pressure_in.begin(),
                   std::bind(std::multiplies<double>(), std::placeholders::_1, nd.p_factor));
    
    std::transform(_comp_pressure_out.begin(), _comp_pressure_out.end(), _comp_pressure_out.begin(),
                   std::bind(std::multiplies<double>(), std::placeholders::_1, nd.p_factor));
    
    is_dimensional = true;
    is_si = true;
    return;
}

void SteadyStateSolution::convert_to_standard_units(const Converter & converter) {
    if (is_standard) return;
    is_si = false;
    is_standard = true;
    std::transform(_pipe_flow_in.begin(), _pipe_flow_in.end(), _pipe_flow_in.begin(),
                   std::bind(std::divides<double>(), std::placeholders::_1, converter.mmscfd_to_kgps));
    
    std::transform(_pipe_flow_out.begin(), _pipe_flow_out.end(), _pipe_flow_out.begin(),
                   std::bind(std::divides<double>(), std::placeholders::_1, converter.mmscfd_to_kgps));
    
    std::transform(_slack_flows.begin(), _slack_flows.end(), _slack_flows.begin(),
                   std::bind(std::divides<double>(), std::placeholders::_1, converter.mmscfd_to_kgps));
    
    std::transform(_non_slack_flows.begin(), _non_slack_flows.end(), _non_slack_flows.begin(),
                   std::bind(std::divides<double>(), std::placeholders::_1, converter.mmscfd_to_kgps));
    
    std::transform(_gnode_demand_flows.begin(), _gnode_demand_flows.end(), _gnode_demand_flows.begin(),
                   std::bind(std::divides<double>(), std::placeholders::_1, converter.mmscfd_to_kgps));
    
    std::transform(_gnode_supply_flows.begin(), _gnode_supply_flows.end(), _gnode_supply_flows.begin(),
                   std::bind(std::divides<double>(), std::placeholders::_1, converter.mmscfd_to_kgps));
    
    std::transform(_comp_flow_in.begin(), _comp_flow_in.end(), _comp_flow_in.begin(),
                   std::bind(std::divides<double>(), std::placeholders::_1, converter.mmscfd_to_kgps));
    
    std::transform(_comp_flow_out.begin(), _comp_flow_out.end(), _comp_flow_out.begin(),
                   std::bind(std::divides<double>(), std::placeholders::_1, converter.mmscfd_to_kgps));
    
    std::transform(_pipe_pressure_in.begin(), _pipe_pressure_in.end(), _pipe_pressure_in.begin(),
                   std::bind(std::divides<double>(), std::placeholders::_1, converter.psi_to_pascal));
    
    std::transform(_pipe_pressure_out.begin(), _pipe_pressure_out.end(), _pipe_pressure_out.begin(),
                   std::bind(std::divides<double>(), std::placeholders::_1, converter.psi_to_pascal));
    
    std::transform(_nodal_pressure.begin(), _nodal_pressure.end(), _nodal_pressure.begin(),
                   std::bind(std::divides<double>(), std::placeholders::_1, converter.psi_to_pascal));
    
    std::transform(_comp_discharge_pressure.begin(), _comp_discharge_pressure.end(), _comp_discharge_pressure.begin(),
                   std::bind(std::divides<double>(), std::placeholders::_1, converter.psi_to_pascal));
    
    std::transform(_comp_pressure_in.begin(), _comp_pressure_in.end(), _comp_pressure_in.begin(),
                   std::bind(std::divides<double>(), std::placeholders::_1, converter.psi_to_pascal));
    
    std::transform(_comp_pressure_out.begin(), _comp_pressure_out.end(), _comp_pressure_out.begin(),
                   std::bind(std::divides<double>(), std::placeholders::_1, converter.psi_to_pascal));
};
*/

//void SteadyStateSolution::write_output(const Network & net, std::string path) {
//    /* write the csv files */
//    CSVWriter pipe_flow_in(",", net.pipes.size()), pipe_flow_out(",", net.pipes.size());
//    CSVWriter pipe_pressure_in(",", net.pipes.size()), pipe_pressure_out(",", net.pipes.size());
//    for (auto pipe : net.pipes) {
//        pipe_flow_in << pipe->_id;
//        pipe_flow_out << pipe->_id;
//        pipe_pressure_in << pipe->_id;
//        pipe_pressure_out << pipe->_id;
//    }
//
//    for (auto i=0; i<net.pipes.size(); ++i) {
//        pipe_flow_in << std::to_string(_pipe_flow_in[i]);
//        pipe_flow_out << std::to_string(_pipe_flow_out[i]);
//        pipe_pressure_in << std::to_string(_pipe_pressure_in[i]);
//        pipe_pressure_out << std::to_string(_pipe_pressure_out[i]);
//    }
//
//    pipe_flow_in.writeToFile(path + OutputFilenames::pipe_flow_in);
//    pipe_flow_out.writeToFile(path + OutputFilenames::pipe_flow_out);
//    pipe_pressure_in.writeToFile(path + OutputFilenames::pipe_pressure_in);
//    pipe_pressure_out.writeToFile(path + OutputFilenames::pipe_pressure_out);
//
//    CSVWriter slack_flows(",", net.num_slack_nodes), non_slack_flows(",", net.num_non_slack_nodes);
//    for (auto slack_id : net.pslack_ids)
//        slack_flows << slack_id;
//    for (auto non_slack_id : net.qbar_ids)
//        non_slack_flows << non_slack_id;
//
//    for (auto i=0; i<net.pslack_ids.size(); ++i)
//        slack_flows << std::to_string(_slack_flows[i]);
//
//    for (auto i=0; i<net.qbar_ids.size(); ++i)
//        non_slack_flows << std::to_string(_non_slack_flows[i]);
//
//    slack_flows.writeToFile(path + OutputFilenames::slack_flows);
//    non_slack_flows.writeToFile(path + OutputFilenames::non_slack_flows);
//
//    CSVWriter nodal_pressure(",", net.nodes.size());
//    for (auto node : net.nodes)
//        nodal_pressure << node->_id;
//
//    for (auto i=0; i<net.nodes.size(); ++i)
//        nodal_pressure << std::to_string(_nodal_pressure[i]);
//
//    nodal_pressure.writeToFile(path + OutputFilenames::nodal_pressure);
//
//
//};

//SteadyStateSolution populate_steady_state_solution_data(const Network & net,
//                                                        SteadyStateModelData* md,
//                                                        ProblemData* pd,
//                                                        Model* m,
//                                                        const Nondimensionalization & nd,
//                                                        const Converter & converter) {
//    SteadyStateSolution solution(net, md, pd, m);
//    solution.dimensionalize(nd);
//
//    if (net.input_params->get_units() == 1)
//        solution.convert_to_standard_units(converter);
//
//    return solution;
//};
