#ifndef MODEL_DATA_H
#define MODEL_DATA_H

#pragma once 

#include <model.h>
#include <problem_data.h>
#include <memory>

struct SteadyStateModelData {
    std::unordered_map<int, std::shared_ptr<Variable>> rho;
    std::unordered_map<int, std::shared_ptr<Variable>> phi_p;
    std::unordered_map<int, std::shared_ptr<Variable>> phi_c;
    std::unordered_map<int, std::shared_ptr<Variable>> alpha;
    std::unordered_map<int, std::shared_ptr<Variable>> s;
    std::unordered_map<int, std::shared_ptr<Variable>> d;
    std::unordered_map<int, std::shared_ptr<Variable>> slack_production;
    
    std::unordered_map<int, std::shared_ptr<Constraint>> nodal_balance;
    std::unordered_map<int, std::shared_ptr<Constraint>> pipe_physics;
    std::unordered_map<int, std::shared_ptr<Constraint>> compressor_physics;
    std::unordered_map<int, std::shared_ptr<Constraint>> compressor_bounds;

    std::shared_ptr<Func> objective;
};

std::unique_ptr<Model> build_steady_state_model(ProblemData *, SteadyStateModelData*);

#endif 
