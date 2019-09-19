#include <model_data.h>

std::unique_ptr<Model> build_steady_state_model(ProblemData * pd,  SteadyStateModelData* md) {
    auto model = std::make_unique<Model>("Steady State NLP model");
    
    /* nodal rho variables for all non-slack nodes */
    for (auto index : pd->non_slack_nodes) {
        auto var = model->add_variable("rho_" + std::to_string(index));
        var->set_lb(pd->p_min.get_value(index));
        var->set_ub(pd->p_max.get_value(index));
        var->set_start(pd->p_min.get_value(index));
        md->rho[index] = var;
    }
    
    /* pipe phi variables */
    for (auto index : pd->pipes) {
        auto var = model->add_variable("phi_pipe_" + std::to_string(index));
        var->set_lb(pd->phi_min_pipe.get_value(index));
        var->set_ub(pd->phi_max_pipe.get_value(index));
        md->phi_p[index] = var;
    }
    
    /* phi compressor variables */
    for (auto index : pd->compressors) {
        auto var = model->add_variable("phi_compressor_" + std::to_string(index));
        var->set_lb(pd->phi_min_compressor.get_value(index));
        var->set_ub(pd->phi_max_compressor.get_value(index));
        md->phi_c[index] = var;
    }
    
    /* compressor alpha variables */
    for (auto index : pd->compressors) {
        auto var = model->add_variable("alpha_" + std::to_string(index));
        var->set_lb(pd->c_ratio_min.get_value(index));
        var->set_ub(pd->c_ratio_max.get_value(index));
        var->set_start(pd->c_ratio_max.get_value(index));
        md->alpha[index] = var;
    }
    
    /* gnode s variables */
    for (auto index : pd->gnodes) {
        auto var = model->add_variable("s_" + std::to_string(index));
        var->set_lb(0.0);
        var->set_ub(pd->smax.get_value(index));
        md->s[index] = var;
    }
    
    /* gnode d variables */
    for (auto index : pd->gnodes) {
        auto var = model->add_variable("d_" + std::to_string(index));
        var->set_lb(0.0);
        var->set_ub(pd->dmax.get_value(index));
        var->set_start(pd->dmax.get_value(index));
        md->d[index] = var;
    }
    
    for (auto index : pd->slack_nodes) {
        auto var = model->add_variable("slack_production_" + std::to_string(index));
        var->set_lb(0.0);
        var->set_ub(10000.0);
        md->slack_production[index] = var;
    }
    
    /* objective : sum (g in gnodes) (- d(g) cd(g) + s(g) cs(g)) + sum (i in slack) slack_production(i) cslack(i)*/
    auto objective = model->add_objective("social_welfare");
    for (auto index : pd->gnodes) {
        objective->add_term(Term(md->d[index], -pd->cd.get_value(index)));
        objective->add_term(Term(md->s[index], pd->cs.get_value(index)));
    }
    for (auto index : pd->slack_nodes)
        objective->add_term(Term(md->slack_production[index], pd->cslack.get_value(index)));
    
    md->objective = objective;
    
    /* nodal balance constraint */
    for (auto node_index : pd->slack_nodes) {
        auto constraint = model->add_constraint("nodal_balance_" + std::to_string(node_index));
        auto out_pipes_in_node = pd->out_pipes_in_node[node_index];
        auto in_pipes_in_node = pd->in_pipes_in_node[node_index];
        auto out_compressors_in_node = pd->out_compressors_in_node[node_index];
        auto in_compressors_in_node = pd->in_compressors_in_node[node_index];
        for (auto out_pipe_index : out_pipes_in_node)
            constraint->add_term(Term(md->phi_p[out_pipe_index], pd->area_pipe.get_value(out_pipe_index)));
        for (auto out_compressor_index : out_compressors_in_node)
            constraint->add_term(Term(md->phi_c[out_compressor_index], pd->area_compressor.get_value(out_compressor_index)));
        for (auto in_pipe_index : in_pipes_in_node)
            constraint->add_term(Term(md->phi_p[in_pipe_index], -pd->area_pipe.get_value(in_pipe_index)));
        for (auto in_compressor_index : in_compressors_in_node)
            constraint->add_term(Term(md->phi_c[in_compressor_index], -pd->area_compressor.get_value(in_compressor_index)));
        constraint->add_term(Term(md->slack_production[node_index], -1.0));
        constraint->equal_to(0.0);
        md->nodal_balance[node_index] = constraint;
        
    }
    for (auto node_index : pd->non_slack_nodes) {
        auto constraint = model->add_constraint("nodal_balance_" + std::to_string(node_index));
        auto out_pipes_in_node = pd->out_pipes_in_node[node_index];
        auto in_pipes_in_node = pd->in_pipes_in_node[node_index];
        auto out_compressors_in_node = pd->out_compressors_in_node[node_index];
        auto in_compressors_in_node = pd->in_compressors_in_node[node_index];
        auto gnodes_in_node = pd->gnodes_in_node[node_index];
        if (pd->qbar.get_value(node_index) != 0.0)
            constraint->add_term(Term(pd->qbar.get_value(node_index)));
        for (auto out_pipe_index : out_pipes_in_node)
            constraint->add_term(Term(md->phi_p[out_pipe_index], pd->area_pipe.get_value(out_pipe_index)));
        for (auto out_compressor_index : out_compressors_in_node)
            constraint->add_term(Term(md->phi_c[out_compressor_index], pd->area_compressor.get_value(out_compressor_index)));
        for (auto in_pipe_index : in_pipes_in_node)
            constraint->add_term(Term(md->phi_p[in_pipe_index], -pd->area_pipe.get_value(in_pipe_index)));
        for (auto in_compressor_index : in_compressors_in_node)
            constraint->add_term(Term(md->phi_c[in_compressor_index], -pd->area_compressor.get_value(in_compressor_index)));
        for (auto gnode_index : gnodes_in_node) {
            constraint->add_term(Term(md->s[gnode_index], -1.0));
            constraint->add_term(Term(md->d[gnode_index]));
            if (pd->gbar.get_value(gnode_index) != 0.0)
                constraint->add_term(Term(pd->gbar.get_value(gnode_index)));
        }
        constraint->equal_to(0.0);
        md->nodal_balance[node_index] = constraint;
    }
    
    /* pipe physics */
    for (auto pipe_index : pd->pipes) {
        auto constraint = model->add_constraint("pipe_physics_" + std::to_string(pipe_index));
        auto fnode_index = pd->fnode_of_pipe[pipe_index];
        auto tnode_index = pd->tnode_of_pipe[pipe_index];
        bool is_fnode_slack = pd->slack_nodes.find(fnode_index) != pd->slack_nodes.end();
        if (is_fnode_slack)
            constraint->add_term(Term(std::pow(pd->pslack.get_value(fnode_index), 2)));
        else
            constraint->add_term(Term(md->rho[fnode_index], 1.0, TermType::quadratic));
        constraint->add_term(Term(md->rho[tnode_index], -1.0, TermType::quadratic));
        constraint->add_term(Term(md->phi_p[pipe_index], -pd->resistance_pipe.get_value(pipe_index), TermType::x_abs_x));
        constraint->equal_to(0.0);
        md->pipe_physics[pipe_index] = constraint;
    }
    
    /* compressor physics */
    for (auto compressor_index : pd->compressors) {
        auto constraint = model->add_constraint("compressor_physics_" + std::to_string(compressor_index));
        auto fnode_index = pd->fnode_of_compressor[compressor_index];
        auto tnode_index = pd->tnode_of_compressor[compressor_index];
        bool is_fnode_slack = pd->slack_nodes.find(fnode_index) != pd->slack_nodes.end();
        if (is_fnode_slack)
            constraint->add_term(Term(md->alpha[compressor_index], std::pow(pd->pslack.get_value(fnode_index), 2), TermType::quadratic));
        else
            constraint->add_term(Term({md->alpha[compressor_index], md->rho[fnode_index]}, 1.0, TermType::x_sq_y_sq));
        constraint->add_term(Term(md->rho[tnode_index], -1.0, TermType::quadratic));
        constraint->add_term(Term(md->phi_c[compressor_index], -pd->resistance_compressor.get_value(compressor_index), TermType::x_abs_x));
        constraint->equal_to(0.0);
        md->pipe_physics[compressor_index] = constraint;
    }
    
    /* discharge pressure from a compressor bounds */
    for (auto compressor_index : pd->compressors) {
        auto constraint = model->add_constraint("compressor_bounds_" + std::to_string(compressor_index));
        auto fnode_index = pd->fnode_of_compressor[compressor_index];
        bool is_fnode_slack = pd->slack_nodes.find(fnode_index) != pd->slack_nodes.end();
        if (is_fnode_slack)
            constraint->add_term(Term(md->alpha[compressor_index], pd->pslack.get_value(fnode_index)));
        else
            constraint->add_term(Term({ md->alpha[compressor_index], md->rho[fnode_index] }, 1.0, TermType::xy));
        constraint->two_sided(0.0, pd->p_max.get_value(fnode_index));
    }
    
    return model;
};
