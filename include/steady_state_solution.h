#ifndef STEADY_STATE_SOLUTION_H
#define STEADY_STATE_SOLUTION_H

#pragma once
#include <vector>
#include <data.h>
#include <conversions.h>
#include <steady_state_problem.h>
#include <steady_state_data.h>

class SteadyStateSolution {
private:
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
    bool _si;
    bool _standard;
    bool _per_unit;
public:
    SteadyStateSolution(const Data &,
                        const SteadyStateData &,
                        const SteadyStateProblem &,
                        const InputParams &);
    
    const std::vector<double> & get_pipe_flow_in() const;
    const std::vector<double> & get_pipe_flow_out() const;
    const std::vector<double> & get_pipe_pressure_in() const;
    const std::vector<double> & get_pipe_pressure_out() const;
    const std::vector<double> & get_slack_flows() const;
    const std::vector<double> & get_non_slack_flows() const;
    const std::vector<double> & get_nodal_pressure() const;
    const std::vector<double> & get_gnode_demand_flows() const;
    const std::vector<double> & get_gnode_supply_flows() const;
    const std::vector<double> & get_comp_ratios() const;
    const std::vector<double> & get_comp_discharge_pressure() const;
    const std::vector<double> & get_comp_flow_in() const;
    const std::vector<double> & get_comp_flow_out() const;
    const std::vector<double> & get_comp_pressure_in() const;
    const std::vector<double> & get_comp_pressure_out() const;
    const std::vector<double> & get_comp_power() const;
    
    bool is_si() const;
    bool is_standard() const;
    bool is_per_unit() const;
    
    void make_si(const ConversionFactors &, const ScalingFactors &);
    void make_standard(const ConversionFactors &, const ScalingFactors &);
    void make_per_unit(const ConversionFactors &, const ScalingFactors &);
    
    void write_output(const Data &, std::string);
    void write_output(const Data &, const InputParams &, std::string data_path, std::string case_name, std::string data_format);
    
};

#endif
