#include <steady_state_solution.h>
#include <csvwriter.h>
#include <files.h>
#include <iomanip>

SteadyStateSolution::SteadyStateSolution(const Data & data,
                                         const SteadyStateData & ssd,
                                         const SteadyStateProblem & ssp,
                                         const InputParams & ip) {
    auto area_pipe = ssd.get_area_pipe();
    for (auto pipe : data.get_pipes()) {
        auto pipe_id = pipe->get_id();
        auto pipe_flow = ssp.get_phi_p(pipe_id);
        _pipe_flow_in.push_back(pipe_flow * area_pipe.get_value(pipe_id));
        _pipe_flow_out.push_back(pipe_flow * area_pipe.get_value(pipe_id));
        auto fnode_id = pipe->get_fnode_id();
        auto tnode_id = pipe->get_tnode_id();
        _pipe_pressure_in.push_back(ssp.get_rho(fnode_id));
        _pipe_pressure_out.push_back(ssp.get_rho(tnode_id));
    }
    
    for (auto slack_node_id : data.get_pslack_ids())
        _slack_flows.push_back(ssp.get_slack_production(slack_node_id));
    
    auto qbar = ssd.get_qbar();
    auto gbar = ssd.get_gbar();
    for (auto non_slack_node_id : data.get_qbar_ids()) {
        double val = qbar.get_value(non_slack_node_id);
        for (auto gnode_id : ssd.get_gnodes_in_node(non_slack_node_id)) {
            val += ssp.get_d(gnode_id);
            val -= ssp.get_s(gnode_id);
            val += gbar.get_value(gnode_id);
        }
        _non_slack_flows.push_back(val);
    }
    
    auto pslack = ssd.get_pslack();
    for (auto node : data.get_nodes()) {
        if (node->is_slack())
            _nodal_pressure.push_back(pslack.get_value(node->get_id()));
        else
            _nodal_pressure.push_back(ssp.get_rho(node->get_id()));
    }
    
    for (auto gnode : data.get_gnodes()) {
        _gnode_demand_flows.push_back(ssp.get_d(gnode->get_id()));
        _gnode_supply_flows.push_back(ssp.get_s(gnode->get_id()));
    }
    
    auto area_compressor = ssd.get_area_compressor();
    double m = (ip.get_specific_heat_capacity_ratio() - 1) / ip.get_specific_heat_capacity_ratio();
    double Wc = 286.76 * ip.get_temperature() / ip.get_gas_specific_gravity() / m;
    for (auto compressor : data.get_compressors()) {
        _comp_ratios.push_back(ssp.get_alpha(compressor->get_id()));
        _comp_flow_in.push_back(ssp.get_phi_c(compressor->get_id()) * area_compressor.get_value(compressor->get_id()));
        _comp_flow_out.push_back(ssp.get_phi_c(compressor->get_id()) * area_compressor.get_value(compressor->get_id()));
        _comp_discharge_pressure.push_back(ssp.get_rho(compressor->get_tnode_id()));
        _comp_pressure_out.push_back(ssp.get_rho(compressor->get_tnode_id()));
        if (ssd.get_slack_node_indexes().find(compressor->get_fnode_id()) != ssd.get_slack_node_indexes().end())
            _comp_pressure_in.push_back(pslack.get_value(compressor->get_fnode_id()));
        else
            _comp_pressure_in.push_back(ssp.get_rho(compressor->get_fnode_id()));
        /* compressor power calculation */
        double alpha = ssp.get_alpha(compressor->get_id());
        double q = ssp.get_phi_c(compressor->get_id());
        double area = area_compressor.get_value(compressor->get_id());
        double power_consumption = Wc * (std::pow(alpha, m) - 1) * std::fabs(q) * area;
        _comp_power.push_back(power_consumption);
    }
    
    _si = false;
    _standard = false;
    _per_unit = true;
};

const std::vector<double> & SteadyStateSolution::get_pipe_flow_in() const { return _pipe_flow_in; };
const std::vector<double> & SteadyStateSolution::get_pipe_flow_out() const { return _pipe_flow_out; };
const std::vector<double> & SteadyStateSolution::get_pipe_pressure_in() const { return _pipe_pressure_in; };
const std::vector<double> & SteadyStateSolution::get_pipe_pressure_out() const { return _pipe_pressure_out; };
const std::vector<double> & SteadyStateSolution::get_slack_flows() const { return _slack_flows; };
const std::vector<double> & SteadyStateSolution::get_non_slack_flows() const { return _non_slack_flows; };
const std::vector<double> & SteadyStateSolution::get_nodal_pressure() const { return _nodal_pressure; };
const std::vector<double> & SteadyStateSolution::get_gnode_demand_flows() const { return _gnode_demand_flows; };
const std::vector<double> & SteadyStateSolution::get_gnode_supply_flows() const { return _gnode_supply_flows; };
const std::vector<double> & SteadyStateSolution::get_comp_ratios() const { return _comp_ratios; };
const std::vector<double> & SteadyStateSolution::get_comp_discharge_pressure() const { return _comp_discharge_pressure; };
const std::vector<double> & SteadyStateSolution::get_comp_flow_in() const { return _comp_flow_in; };
const std::vector<double> & SteadyStateSolution::get_comp_flow_out() const { return _comp_flow_out; };
const std::vector<double> & SteadyStateSolution::get_comp_pressure_in() const { return _comp_pressure_in; };
const std::vector<double> & SteadyStateSolution::get_comp_pressure_out() const { return _comp_pressure_out; };
const std::vector<double> & SteadyStateSolution::get_comp_power() const { return _comp_power; };

bool SteadyStateSolution::is_si() const { return _si; };
bool SteadyStateSolution::is_standard() const { return _standard; };
bool SteadyStateSolution::is_per_unit() const { return _per_unit; };

void SteadyStateSolution::make_si(const ConversionFactors & cf, const ScalingFactors & sf) {
    if (is_si()) return;
    if (is_per_unit()) {
        /* all flows to si */
        std::transform(_pipe_flow_in.begin(), _pipe_flow_in.end(), _pipe_flow_in.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_pipe_flow_out.begin(), _pipe_flow_out.end(), _pipe_flow_out.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_slack_flows.begin(), _slack_flows.end(), _slack_flows.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_non_slack_flows.begin(), _non_slack_flows.end(), _non_slack_flows.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_gnode_demand_flows.begin(), _gnode_demand_flows.end(), _gnode_demand_flows.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_gnode_supply_flows.begin(), _gnode_supply_flows.end(), _gnode_supply_flows.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_comp_flow_in.begin(), _comp_flow_in.end(), _comp_flow_in.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_comp_flow_out.begin(), _comp_flow_out.end(), _comp_flow_out.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_comp_power.begin(), _comp_power.end(), _comp_power.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        /* all pressures to si */
        std::transform(_pipe_pressure_in.begin(), _pipe_pressure_in.end(), _pipe_pressure_in.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_pressure_scaling()));
        std::transform(_pipe_pressure_out.begin(), _pipe_pressure_out.end(), _pipe_pressure_out.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_pressure_scaling()));
        std::transform(_nodal_pressure.begin(), _nodal_pressure.end(), _nodal_pressure.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_pressure_scaling()));
        std::transform(_comp_discharge_pressure.begin(), _comp_discharge_pressure.end(), _comp_discharge_pressure.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_pressure_scaling()));
        std::transform(_comp_pressure_in.begin(), _comp_pressure_in.end(), _comp_pressure_in.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_pressure_scaling()));
        std::transform(_comp_pressure_out.begin(), _comp_pressure_out.end(), _comp_pressure_out.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_pressure_scaling()));
    }
    if (is_standard()) {
        /* all flows to si */
        std::transform(_pipe_flow_in.begin(), _pipe_flow_in.end(), _pipe_flow_in.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_pipe_flow_out.begin(), _pipe_flow_out.end(), _pipe_flow_out.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_slack_flows.begin(), _slack_flows.end(), _slack_flows.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_non_slack_flows.begin(), _non_slack_flows.end(), _non_slack_flows.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_gnode_demand_flows.begin(), _gnode_demand_flows.end(), _gnode_demand_flows.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_gnode_supply_flows.begin(), _gnode_supply_flows.end(), _gnode_supply_flows.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_comp_flow_in.begin(), _comp_flow_in.end(), _comp_flow_in.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_comp_flow_out.begin(), _comp_flow_out.end(), _comp_flow_out.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_comp_power.begin(), _comp_power.end(), _comp_power.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.hp_to_watts()));
        /* all pressures to si */
        std::transform(_pipe_pressure_in.begin(), _pipe_pressure_in.end(), _pipe_pressure_in.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.psi_to_pascal()));
        std::transform(_pipe_pressure_out.begin(), _pipe_pressure_out.end(), _pipe_pressure_out.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.psi_to_pascal()));
        std::transform(_nodal_pressure.begin(), _nodal_pressure.end(), _nodal_pressure.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.psi_to_pascal()));
        std::transform(_comp_discharge_pressure.begin(), _comp_discharge_pressure.end(), _comp_discharge_pressure.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.psi_to_pascal()));
        std::transform(_comp_pressure_in.begin(), _comp_pressure_in.end(), _comp_pressure_in.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.psi_to_pascal()));
        std::transform(_comp_pressure_out.begin(), _comp_pressure_out.end(), _comp_pressure_out.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.psi_to_pascal()));
    }
    _per_unit = false;
    _si = true;
    _standard = false;
};

void SteadyStateSolution::make_standard(const ConversionFactors & cf, const ScalingFactors & sf) {
    if (is_standard()) return;
    if (is_per_unit()) make_si(cf, sf);
    if (is_si()) {
        /* all flows to standard */
        std::transform(_pipe_flow_in.begin(), _pipe_flow_in.end(), _pipe_flow_in.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_pipe_flow_out.begin(), _pipe_flow_out.end(), _pipe_flow_out.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_slack_flows.begin(), _slack_flows.end(), _slack_flows.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_non_slack_flows.begin(), _non_slack_flows.end(), _non_slack_flows.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_gnode_demand_flows.begin(), _gnode_demand_flows.end(), _gnode_demand_flows.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_gnode_supply_flows.begin(), _gnode_supply_flows.end(), _gnode_supply_flows.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_comp_flow_in.begin(), _comp_flow_in.end(), _comp_flow_in.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_comp_flow_out.begin(), _comp_flow_out.end(), _comp_flow_out.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_comp_power.begin(), _comp_power.end(), _comp_power.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.watts_to_hp()));
        /* all pressures to standard */
        std::transform(_pipe_pressure_in.begin(), _pipe_pressure_in.end(), _pipe_pressure_in.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.pascal_to_psi()));
        std::transform(_pipe_pressure_out.begin(), _pipe_pressure_out.end(), _pipe_pressure_out.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.pascal_to_psi()));
        std::transform(_nodal_pressure.begin(), _nodal_pressure.end(), _nodal_pressure.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.pascal_to_psi()));
        std::transform(_comp_discharge_pressure.begin(), _comp_discharge_pressure.end(), _comp_discharge_pressure.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.pascal_to_psi()));
        std::transform(_comp_pressure_in.begin(), _comp_pressure_in.end(), _comp_pressure_in.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.pascal_to_psi()));
        std::transform(_comp_pressure_out.begin(), _comp_pressure_out.end(), _comp_pressure_out.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.pascal_to_psi()));
    }
    _per_unit = false;
    _si = false;
    _standard = true;
};

void SteadyStateSolution::make_per_unit(const ConversionFactors & cf, const ScalingFactors & sf) {
    std::vector<double> _pipe_flow_in, _pipe_flow_out;
    std::vector<double> _pipe_pressure_in, _pipe_pressure_out;
    std::vector<double> _slack_flows;
    std::vector<double> _non_slack_flows;
    std::vector<double> _nodal_pressure;
    std::vector<double> _gnode_demand_flows, _gnode_supply_flows;
    std::vector<double> _comp_ratios, _comp_discharge_pressure;
    std::vector<double> _comp_flow_in, _comp_flow_out;
    std::vector<double> _comp_pressure_in, _comp_pressure_out;
    std::vector<double> _comp_power;
    if (is_per_unit()) return;
    if (is_standard()) make_si(cf, sf);
    if (is_si()) {
        /* all flows to per_unit */
        std::transform(_pipe_flow_in.begin(), _pipe_flow_in.end(), _pipe_flow_in.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_pipe_flow_out.begin(), _pipe_flow_out.end(), _pipe_flow_out.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_slack_flows.begin(), _slack_flows.end(), _slack_flows.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_non_slack_flows.begin(), _non_slack_flows.end(), _non_slack_flows.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_gnode_demand_flows.begin(), _gnode_demand_flows.end(), _gnode_demand_flows.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_gnode_supply_flows.begin(), _gnode_supply_flows.end(), _gnode_supply_flows.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_comp_flow_in.begin(), _comp_flow_in.end(), _comp_flow_in.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_comp_flow_out.begin(), _comp_flow_out.end(), _comp_flow_out.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_comp_power.begin(), _comp_power.end(), _comp_power.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        /* all pressures to per_unit */
        std::transform(_pipe_pressure_in.begin(), _pipe_pressure_in.end(), _pipe_pressure_in.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_pressure_scaling()));
        std::transform(_pipe_pressure_out.begin(), _pipe_pressure_out.end(), _pipe_pressure_out.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_pressure_scaling()));
        std::transform(_nodal_pressure.begin(), _nodal_pressure.end(), _nodal_pressure.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_pressure_scaling()));
        std::transform(_comp_discharge_pressure.begin(), _comp_discharge_pressure.end(), _comp_discharge_pressure.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_pressure_scaling()));
        std::transform(_comp_pressure_in.begin(), _comp_pressure_in.end(), _comp_pressure_in.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_pressure_scaling()));
        std::transform(_comp_pressure_out.begin(), _comp_pressure_out.end(), _comp_pressure_out.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_pressure_scaling()));
        
        
    }
    _per_unit = true;
    _si = false;
    _standard = false;
    
};


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
