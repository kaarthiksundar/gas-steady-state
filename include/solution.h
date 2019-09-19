#ifndef solution_h
#define solution_h

#pragma once
#include <vector>
#include <model_data.h>
#include <network.h>
#include <model.h>
#include <files.h>
#include <nondimensionalization.h>
#include <converter.h>
#include <problem_data.h>

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
    bool is_dimensional;
    bool is_standard;
    bool is_si;
    
public:
    SteadyStateSolution(const Network &,  SteadyStateModelData*, ProblemData*, Model*);
    void write_output(const Network &, std::string);
    void dimensionalize(const Nondimensionalization &);
    void convert_to_standard_units(const Converter &);
};

SteadyStateSolution populate_steady_state_solution_data(const Network &,
                                                                         SteadyStateModelData*,
                                                                         ProblemData*,
                                                                         Model*,
                                                                         const Nondimensionalization &,
                                                                         const Converter &);

#endif
