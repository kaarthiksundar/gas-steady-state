#include <cmath>
#include <csvwriter.h>
#include <files.h>
#include <functional>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <steady_state_solution.h>

using json = nlohmann::json;

SteadyStateSolution::SteadyStateSolution(const Data &data,
                                         const SteadyStateData &ssd,
                                         const SteadyStateProblem &ssp,
                                         const InputParams &ip) {
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
    double m = (ip.get_specific_heat_capacity_ratio() - 1) /
               ip.get_specific_heat_capacity_ratio();
    double Wc =
        286.76 * ip.get_temperature() / ip.get_gas_specific_gravity() / m;
    for (auto compressor : data.get_compressors()) {
        _comp_ratios.push_back(ssp.get_alpha(compressor->get_id()));
        _comp_flow_in.push_back(
            ssp.get_phi_c(compressor->get_id()) *
            area_compressor.get_value(compressor->get_id()));
        _comp_flow_out.push_back(
            ssp.get_phi_c(compressor->get_id()) *
            area_compressor.get_value(compressor->get_id()));
        _comp_discharge_pressure.push_back(
            ssp.get_rho(compressor->get_tnode_id()));
        _comp_pressure_out.push_back(ssp.get_rho(compressor->get_tnode_id()));
        if (ssd.get_slack_node_indexes().find(compressor->get_fnode_id()) !=
            ssd.get_slack_node_indexes().end())
            _comp_pressure_in.push_back(
                pslack.get_value(compressor->get_fnode_id()));
        else
            _comp_pressure_in.push_back(
                ssp.get_rho(compressor->get_fnode_id()));
        /* compressor power calculation */
        double alpha = ssp.get_alpha(compressor->get_id());
        double q = ssp.get_phi_c(compressor->get_id());
        double area = area_compressor.get_value(compressor->get_id());
        double power_consumption =
            Wc * (std::pow(alpha, m) - 1) * std::abs(q) * area;
        _comp_power.push_back(power_consumption);
    }
    _si = false;
    _standard = false;
    _per_unit = true;
};

const std::vector<double> &SteadyStateSolution::get_pipe_flow_in() const {
    return _pipe_flow_in;
};
const std::vector<double> &SteadyStateSolution::get_pipe_flow_out() const {
    return _pipe_flow_out;
};
const std::vector<double> &SteadyStateSolution::get_pipe_pressure_in() const {
    return _pipe_pressure_in;
};
const std::vector<double> &SteadyStateSolution::get_pipe_pressure_out() const {
    return _pipe_pressure_out;
};
const std::vector<double> &SteadyStateSolution::get_slack_flows() const {
    return _slack_flows;
};
const std::vector<double> &SteadyStateSolution::get_non_slack_flows() const {
    return _non_slack_flows;
};
const std::vector<double> &SteadyStateSolution::get_nodal_pressure() const {
    return _nodal_pressure;
};
const std::vector<double> &SteadyStateSolution::get_gnode_demand_flows() const {
    return _gnode_demand_flows;
};
const std::vector<double> &SteadyStateSolution::get_gnode_supply_flows() const {
    return _gnode_supply_flows;
};
const std::vector<double> &SteadyStateSolution::get_comp_ratios() const {
    return _comp_ratios;
};
const std::vector<double> &
SteadyStateSolution::get_comp_discharge_pressure() const {
    return _comp_discharge_pressure;
};
const std::vector<double> &SteadyStateSolution::get_comp_flow_in() const {
    return _comp_flow_in;
};
const std::vector<double> &SteadyStateSolution::get_comp_flow_out() const {
    return _comp_flow_out;
};
const std::vector<double> &SteadyStateSolution::get_comp_pressure_in() const {
    return _comp_pressure_in;
};
const std::vector<double> &SteadyStateSolution::get_comp_pressure_out() const {
    return _comp_pressure_out;
};
const std::vector<double> &SteadyStateSolution::get_comp_power() const {
    return _comp_power;
};

bool SteadyStateSolution::is_si() const { return _si; };
bool SteadyStateSolution::is_standard() const { return _standard; };
bool SteadyStateSolution::is_per_unit() const { return _per_unit; };

void SteadyStateSolution::make_si(const ConversionFactors &cf,
                                  const ScalingFactors &sf) {
    if (is_si())
        return;
    if (is_per_unit()) {
        /* all flows to si */
        std::transform(_pipe_flow_in.begin(), _pipe_flow_in.end(),
                       _pipe_flow_in.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_pipe_flow_out.begin(), _pipe_flow_out.end(),
                       _pipe_flow_out.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_slack_flows.begin(), _slack_flows.end(),
                       _slack_flows.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_non_slack_flows.begin(), _non_slack_flows.end(),
                       _non_slack_flows.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_gnode_demand_flows.begin(), _gnode_demand_flows.end(),
                       _gnode_demand_flows.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_gnode_supply_flows.begin(), _gnode_supply_flows.end(),
                       _gnode_supply_flows.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_comp_flow_in.begin(), _comp_flow_in.end(),
                       _comp_flow_in.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_comp_flow_out.begin(), _comp_flow_out.end(),
                       _comp_flow_out.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_comp_power.begin(), _comp_power.end(),
                       _comp_power.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, sf.get_flow_scaling()));
        /* all pressures to si */
        std::transform(_pipe_pressure_in.begin(), _pipe_pressure_in.end(),
                       _pipe_pressure_in.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1,
                                 sf.get_pressure_scaling()));
        std::transform(_pipe_pressure_out.begin(), _pipe_pressure_out.end(),
                       _pipe_pressure_out.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1,
                                 sf.get_pressure_scaling()));
        std::transform(_nodal_pressure.begin(), _nodal_pressure.end(),
                       _nodal_pressure.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1,
                                 sf.get_pressure_scaling()));
        std::transform(
            _comp_discharge_pressure.begin(), _comp_discharge_pressure.end(),
            _comp_discharge_pressure.begin(),
            std::bind(std::multiplies<double>(), std::placeholders::_1,
                      sf.get_pressure_scaling()));
        std::transform(_comp_pressure_in.begin(), _comp_pressure_in.end(),
                       _comp_pressure_in.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1,
                                 sf.get_pressure_scaling()));
        std::transform(_comp_pressure_out.begin(), _comp_pressure_out.end(),
                       _comp_pressure_out.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1,
                                 sf.get_pressure_scaling()));
    }
    if (is_standard()) {
        /* all flows to si */
        std::transform(_pipe_flow_in.begin(), _pipe_flow_in.end(),
                       _pipe_flow_in.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_pipe_flow_out.begin(), _pipe_flow_out.end(),
                       _pipe_flow_out.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_slack_flows.begin(), _slack_flows.end(),
                       _slack_flows.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_non_slack_flows.begin(), _non_slack_flows.end(),
                       _non_slack_flows.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_gnode_demand_flows.begin(), _gnode_demand_flows.end(),
                       _gnode_demand_flows.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_gnode_supply_flows.begin(), _gnode_supply_flows.end(),
                       _gnode_supply_flows.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_comp_flow_in.begin(), _comp_flow_in.end(),
                       _comp_flow_in.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_comp_flow_out.begin(), _comp_flow_out.end(),
                       _comp_flow_out.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_comp_power.begin(), _comp_power.end(),
                       _comp_power.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.hp_to_watts()));
        /* all pressures to si */
        std::transform(_pipe_pressure_in.begin(), _pipe_pressure_in.end(),
                       _pipe_pressure_in.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.psi_to_pascal()));
        std::transform(_pipe_pressure_out.begin(), _pipe_pressure_out.end(),
                       _pipe_pressure_out.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.psi_to_pascal()));
        std::transform(_nodal_pressure.begin(), _nodal_pressure.end(),
                       _nodal_pressure.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.psi_to_pascal()));
        std::transform(_comp_discharge_pressure.begin(),
                       _comp_discharge_pressure.end(),
                       _comp_discharge_pressure.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.psi_to_pascal()));
        std::transform(_comp_pressure_in.begin(), _comp_pressure_in.end(),
                       _comp_pressure_in.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.psi_to_pascal()));
        std::transform(_comp_pressure_out.begin(), _comp_pressure_out.end(),
                       _comp_pressure_out.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.psi_to_pascal()));
    }
    _per_unit = false;
    _si = true;
    _standard = false;
};

void SteadyStateSolution::make_standard(const ConversionFactors &cf,
                                        const ScalingFactors &sf) {
    if (is_standard())
        return;
    if (is_per_unit())
        make_si(cf, sf);
    if (is_si()) {
        /* all flows to standard */
        std::transform(_pipe_flow_in.begin(), _pipe_flow_in.end(),
                       _pipe_flow_in.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_pipe_flow_out.begin(), _pipe_flow_out.end(),
                       _pipe_flow_out.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_slack_flows.begin(), _slack_flows.end(),
                       _slack_flows.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_non_slack_flows.begin(), _non_slack_flows.end(),
                       _non_slack_flows.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_gnode_demand_flows.begin(), _gnode_demand_flows.end(),
                       _gnode_demand_flows.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_gnode_supply_flows.begin(), _gnode_supply_flows.end(),
                       _gnode_supply_flows.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_comp_flow_in.begin(), _comp_flow_in.end(),
                       _comp_flow_in.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_comp_flow_out.begin(), _comp_flow_out.end(),
                       _comp_flow_out.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_comp_power.begin(), _comp_power.end(),
                       _comp_power.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.watts_to_hp()));
        /* all pressures to standard */
        std::transform(_pipe_pressure_in.begin(), _pipe_pressure_in.end(),
                       _pipe_pressure_in.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.pascal_to_psi()));
        std::transform(_pipe_pressure_out.begin(), _pipe_pressure_out.end(),
                       _pipe_pressure_out.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.pascal_to_psi()));
        std::transform(_nodal_pressure.begin(), _nodal_pressure.end(),
                       _nodal_pressure.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.pascal_to_psi()));
        std::transform(_comp_discharge_pressure.begin(),
                       _comp_discharge_pressure.end(),
                       _comp_discharge_pressure.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.pascal_to_psi()));
        std::transform(_comp_pressure_in.begin(), _comp_pressure_in.end(),
                       _comp_pressure_in.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.pascal_to_psi()));
        std::transform(_comp_pressure_out.begin(), _comp_pressure_out.end(),
                       _comp_pressure_out.begin(),
                       std::bind(std::multiplies<double>(),
                                 std::placeholders::_1, cf.pascal_to_psi()));
    }
    _per_unit = false;
    _si = false;
    _standard = true;
};

void SteadyStateSolution::make_per_unit(const ConversionFactors &cf,
                                        const ScalingFactors &sf) {
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
    if (is_per_unit())
        return;
    if (is_standard())
        make_si(cf, sf);
    if (is_si()) {
        /* all flows to per_unit */
        std::transform(_pipe_flow_in.begin(), _pipe_flow_in.end(),
                       _pipe_flow_in.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1,
                                 sf.get_flow_scaling()));
        std::transform(_pipe_flow_out.begin(), _pipe_flow_out.end(),
                       _pipe_flow_out.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1,
                                 sf.get_flow_scaling()));
        std::transform(_slack_flows.begin(), _slack_flows.end(),
                       _slack_flows.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1,
                                 sf.get_flow_scaling()));
        std::transform(_non_slack_flows.begin(), _non_slack_flows.end(),
                       _non_slack_flows.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1,
                                 sf.get_flow_scaling()));
        std::transform(_gnode_demand_flows.begin(), _gnode_demand_flows.end(),
                       _gnode_demand_flows.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1,
                                 sf.get_flow_scaling()));
        std::transform(_gnode_supply_flows.begin(), _gnode_supply_flows.end(),
                       _gnode_supply_flows.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1,
                                 sf.get_flow_scaling()));
        std::transform(_comp_flow_in.begin(), _comp_flow_in.end(),
                       _comp_flow_in.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1,
                                 sf.get_flow_scaling()));
        std::transform(_comp_flow_out.begin(), _comp_flow_out.end(),
                       _comp_flow_out.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1,
                                 sf.get_flow_scaling()));
        std::transform(_comp_power.begin(), _comp_power.end(),
                       _comp_power.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1,
                                 sf.get_flow_scaling()));
        /* all pressures to per_unit */
        std::transform(_pipe_pressure_in.begin(), _pipe_pressure_in.end(),
                       _pipe_pressure_in.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1,
                                 sf.get_pressure_scaling()));
        std::transform(_pipe_pressure_out.begin(), _pipe_pressure_out.end(),
                       _pipe_pressure_out.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1,
                                 sf.get_pressure_scaling()));
        std::transform(_nodal_pressure.begin(), _nodal_pressure.end(),
                       _nodal_pressure.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1,
                                 sf.get_pressure_scaling()));
        std::transform(_comp_discharge_pressure.begin(),
                       _comp_discharge_pressure.end(),
                       _comp_discharge_pressure.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1,
                                 sf.get_pressure_scaling()));
        std::transform(_comp_pressure_in.begin(), _comp_pressure_in.end(),
                       _comp_pressure_in.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1,
                                 sf.get_pressure_scaling()));
        std::transform(_comp_pressure_out.begin(), _comp_pressure_out.end(),
                       _comp_pressure_out.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1,
                                 sf.get_pressure_scaling()));
    }
    _per_unit = true;
    _si = false;
    _standard = false;
};

void SteadyStateSolution::write_output(const Data &data, const InputParams &ip,
                                       std::string data_path,
                                       std::string case_name,
                                       std::string data_format) {
    std::string path;
    std::string filename;
    /* adjusting the path and file according for the data format provided */
    if (data_format == "csv") {
        path = data_path + case_name + "/";
        filename = "";
    } else {
        path = data_path + case_name;
        filename = path + "_output.json";
    }
    if (data_format == "csv") {
        /* write the csv files */
        CSVWriter pipe_flow_in(","), pipe_flow_out(","), pipe_pressure_in(","),
            pipe_pressure_out(",");
        pipe_flow_in.enableAutoNewRow(data.get_pipes().size());
        pipe_flow_out.enableAutoNewRow(data.get_pipes().size());
        pipe_pressure_in.enableAutoNewRow(data.get_pipes().size());
        pipe_pressure_out.enableAutoNewRow(data.get_pipes().size());
        for (auto pipe : data.get_pipes()) {
            pipe_flow_in << pipe->get_id();
            pipe_flow_out << pipe->get_id();
            pipe_pressure_in << pipe->get_id();
            pipe_pressure_out << pipe->get_id();
        }

        for (auto i = 0; i < data.get_pipes().size(); ++i) {
            pipe_flow_in << std::to_string(_pipe_flow_in[i]);
            pipe_flow_out << std::to_string(_pipe_flow_out[i]);
            pipe_pressure_in << std::to_string(_pipe_pressure_in[i]);
            pipe_pressure_out << std::to_string(_pipe_pressure_out[i]);
        }

        pipe_flow_in.writeToFile(path + OutputFilenames::pipe_flow_in);
        pipe_flow_out.writeToFile(path + OutputFilenames::pipe_flow_out);
        pipe_pressure_in.writeToFile(path + OutputFilenames::pipe_pressure_in);
        pipe_pressure_out.writeToFile(path +
                                      OutputFilenames::pipe_pressure_out);

        CSVWriter slack_flows(","), non_slack_flows(",");
        slack_flows.enableAutoNewRow(data.get_num_slack_nodes());
        non_slack_flows.enableAutoNewRow(data.get_num_non_slack_nodes());
        for (auto slack_id : data.get_pslack_ids())
            slack_flows << slack_id;
        for (auto non_slack_id : data.get_qbar_ids())
            non_slack_flows << non_slack_id;

        for (auto i = 0; i < data.get_num_slack_nodes(); ++i)
            slack_flows << std::to_string(_slack_flows[i]);

        for (auto i = 0; i < data.get_num_non_slack_nodes(); ++i)
            non_slack_flows << std::to_string(_non_slack_flows[i]);

        slack_flows.writeToFile(path + OutputFilenames::slack_flows);
        non_slack_flows.writeToFile(path + OutputFilenames::non_slack_flows);

        CSVWriter nodal_pressure(",");
        nodal_pressure.enableAutoNewRow(data.get_nodes().size());
        for (auto node : data.get_nodes())
            nodal_pressure << node->get_id();

        for (auto i = 0; i < data.get_nodes().size(); ++i)
            nodal_pressure << std::to_string(_nodal_pressure[i]);

        nodal_pressure.writeToFile(path + OutputFilenames::nodal_pressure);

        CSVWriter gnode_supply_flows(","), gnode_demand_flows(",");
        gnode_demand_flows.enableAutoNewRow(data.get_gnodes().size());
        gnode_supply_flows.enableAutoNewRow(data.get_gnodes().size());

        for (auto gnode : data.get_gnodes()) {
            gnode_supply_flows << gnode->get_id();
            gnode_demand_flows << gnode->get_id();
        }

        for (auto i = 0; i < data.get_gnodes().size(); ++i) {
            gnode_supply_flows << std::to_string(_gnode_supply_flows[i]);
            gnode_demand_flows << std::to_string(_gnode_demand_flows[i]);
        }

        gnode_supply_flows.writeToFile(path +
                                       OutputFilenames::gnode_supply_flows);
        gnode_demand_flows.writeToFile(path +
                                       OutputFilenames::gnode_demand_flows);

        CSVWriter comp_flow_in(","), comp_flow_out(",");
        CSVWriter comp_ratio(","), comp_power(",");
        CSVWriter comp_pressure_in(","), comp_pressure_out(","),
            comp_discharge_pressure(",");
        comp_flow_in.enableAutoNewRow(data.get_compressors().size());
        comp_flow_out.enableAutoNewRow(data.get_compressors().size());
        comp_pressure_in.enableAutoNewRow(data.get_compressors().size());
        comp_pressure_out.enableAutoNewRow(data.get_compressors().size());
        comp_ratio.enableAutoNewRow(data.get_compressors().size());
        comp_power.enableAutoNewRow(data.get_compressors().size());
        comp_discharge_pressure.enableAutoNewRow(data.get_compressors().size());
        for (auto compressor : data.get_compressors()) {
            comp_flow_in << compressor->get_id();
            comp_flow_out << compressor->get_id();
            comp_pressure_in << compressor->get_id();
            comp_pressure_out << compressor->get_id();
            comp_discharge_pressure << compressor->get_id();
            comp_ratio << compressor->get_id();
            comp_power << compressor->get_id();
        }

        for (auto i = 0; i < data.get_compressors().size(); ++i) {
            comp_flow_in << std::to_string(_comp_flow_in[i]);
            comp_flow_out << std::to_string(_comp_flow_out[i]);
            comp_pressure_in << std::to_string(_comp_pressure_in[i]);
            comp_pressure_out << std::to_string(_comp_pressure_out[i]);
            comp_discharge_pressure
                << std::to_string(_comp_discharge_pressure[i]);
            comp_ratio << std::to_string(_comp_ratios[i]);
            comp_power << std::to_string(_comp_power[i]);
        }

        comp_flow_in.writeToFile(path + OutputFilenames::comp_flow_in);
        comp_flow_out.writeToFile(path + OutputFilenames::comp_flow_out);
        comp_pressure_in.writeToFile(path + OutputFilenames::comp_pressure_in);
        comp_pressure_out.writeToFile(path +
                                      OutputFilenames::comp_pressure_out);
        comp_discharge_pressure.writeToFile(
            path + OutputFilenames::comp_discharge_pressure);
        comp_ratio.writeToFile(path + OutputFilenames::comp_ratios);
        comp_power.writeToFile(path + OutputFilenames::comp_power);
    } else {
        json json_stream;

        std::map<std::string, double> pipe_flow_in, pipe_flow_out,
            pipe_pressure_in, pipe_pressure_out;
        for (auto i = 0; i < data.get_pipes().size(); ++i) {
            std::string id = std::to_string(data.get_pipes()[i]->get_id());
            pipe_flow_in[id] = _pipe_flow_in[i];
            pipe_flow_out[id] = _pipe_flow_out[i];
            pipe_pressure_in[id] = _pipe_pressure_in[i];
            pipe_pressure_out[id] = _pipe_pressure_out[i];
        }

        std::map<std::string, double> slack_flows, non_slack_flows;
        std::vector<std::string> slack_ids, non_slack_ids;
        std::vector<double> slack_flow_values, non_slack_flow_values;
        for (auto slack_id : data.get_pslack_ids())
            slack_ids.push_back(std::to_string(slack_id));
        for (auto non_slack_id : data.get_qbar_ids())
            non_slack_ids.push_back(std::to_string(non_slack_id));
        for (auto i = 0; i < data.get_num_slack_nodes(); ++i)
            slack_flow_values.push_back(_slack_flows[i]);
        for (auto i = 0; i < data.get_num_non_slack_nodes(); ++i)
            non_slack_flow_values.push_back(_non_slack_flows[i]);
        for (auto i = 0; i < slack_ids.size(); ++i)
            slack_flows[slack_ids[i]] = slack_flow_values[i];
        for (auto i = 0; i < non_slack_ids.size(); ++i)
            non_slack_flows[non_slack_ids[i]] = non_slack_flow_values[i];

        std::map<std::string, double> nodal_pressure;
        for (auto i = 0; i < data.get_nodes().size(); ++i) {
            std::string id = std::to_string(data.get_nodes()[i]->get_id());
            nodal_pressure[id] = _nodal_pressure[i];
        }

        std::map<std::string, double> gnode_supply_flows, gnode_demand_flows;
        for (auto i = 0; i < data.get_gnodes().size(); ++i) {
            std::string id = std::to_string(data.get_gnodes()[i]->get_id());
            gnode_supply_flows[id] = _gnode_supply_flows[i];
            gnode_demand_flows[id] = _gnode_demand_flows[i];
        }

        std::map<std::string, double> comp_flow_in, comp_flow_out, comp_ratio,
            comp_power;
        std::map<std::string, double> comp_inlet_pressure,
            comp_discharge_pressure;
        for (auto i = 0; i < data.get_compressors().size(); ++i) {
            std::string id =
                std::to_string(data.get_compressors()[i]->get_id());
            comp_flow_in[id] = _comp_flow_in[i];
            comp_flow_out[id] = _comp_flow_out[i];
            comp_ratio[id] = _comp_ratios[i];
            comp_power[id] = _comp_power[i];
            comp_inlet_pressure[id] = _comp_pressure_in[i];
            comp_discharge_pressure[id] = _comp_pressure_out[i];
        }

        json_stream["units (SI=0, standard=1)"] = ip.get_units();

        json_stream["pipe_flow_in"] = pipe_flow_in;
        json_stream["pipe_flow_out"] = pipe_flow_out;
        json_stream["pipe_pressure_in"] = pipe_pressure_in;
        json_stream["pipe_pressure_out"] = pipe_pressure_out;

        json_stream["slack_node_flows"] = slack_flows;
        json_stream["non_slack_node_flows"] = non_slack_flows;
        json_stream["nodal_pressure"] = nodal_pressure;

        json_stream["gnode_supply_flows"] = gnode_supply_flows;
        json_stream["gnode_demand_flows"] = gnode_demand_flows;

        json_stream["comp_flow_in"] = comp_flow_in;
        json_stream["comp_flow_out"] = comp_flow_out;
        json_stream["comp_ratio"] = comp_ratio;
        json_stream["comp_power"] = comp_power;
        json_stream["comp_inlet_pressure"] = comp_inlet_pressure;
        json_stream["comp_discharge_pressure"] = comp_discharge_pressure;

        std::ofstream out(filename);
        out << std::setw(4) << json_stream << std::endl;
    }
};
