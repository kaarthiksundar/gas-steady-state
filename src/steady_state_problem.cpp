#include <cmath>
#include <steady_state_problem.h>

SteadyStateProblem::SteadyStateProblem(std::string name,
                                       const SteadyStateData &ssd)
: _model(Model(name)), _ssd(ssd), _variables(), _constraints(),
_objective(), _primal_values(), _dual_values(), _objective_value(){};

Model &SteadyStateProblem::get_model() { return _model; };
std::shared_ptr<Variable> SteadyStateProblem::rho(int i) {
    return _variables.at("rho").at(i);
};
std::shared_ptr<Variable> SteadyStateProblem::phi_p(int i) {
    return _variables.at("phi_pipe").at(i);
};
std::shared_ptr<Variable> SteadyStateProblem::phi_c(int i) {
    return _variables.at("phi_compressor").at(i);
};
std::shared_ptr<Variable> SteadyStateProblem::alpha(int i) {
    return _variables.at("alpha").at(i);
};
std::shared_ptr<Variable> SteadyStateProblem::s(int i) {
    return _variables.at("s").at(i);
};
std::shared_ptr<Variable> SteadyStateProblem::d(int i) {
    return _variables.at("d").at(i);
};
std::shared_ptr<Variable> SteadyStateProblem::slack_production(int i) {
    return _variables.at("slack_production").at(i);
};
std::shared_ptr<Constraint> SteadyStateProblem::nodal_balance(int i) {
    return _constraints.at("nodal_balance").at(i);
};
std::shared_ptr<Constraint> SteadyStateProblem::pipe_physics(int i) {
    return _constraints.at("pipe_physics").at(i);
};
std::shared_ptr<Constraint> SteadyStateProblem::compressor_physics(int i) {
    return _constraints.at("compressor_physics").at(i);
};
std::shared_ptr<Constraint> SteadyStateProblem::compressor_bounds(int i) {
    return _constraints.at("compressor_discharge_bounds").at(i);
};
std::shared_ptr<Constraint> SteadyStateProblem::compressor_power_bounds(int i) {
    return _constraints.at("compressor_power_bounds").at(i);
};
std::shared_ptr<Func> SteadyStateProblem::objective() { return _objective; };

double SteadyStateProblem::get_rho(int i) const {
    return _primal_values.at("rho").at(i);
};
double SteadyStateProblem::get_phi_p(int i) const {
    return _primal_values.at("phi_pipe").at(i);
};
double SteadyStateProblem::get_phi_c(int i) const {
    return _primal_values.at("phi_compressor").at(i);
};
double SteadyStateProblem::get_alpha(int i) const {
    return _primal_values.at("alpha").at(i);
};
double SteadyStateProblem::get_s(int i) const {
    return _primal_values.at("s").at(i);
};
double SteadyStateProblem::get_d(int i) const {
    return _primal_values.at("d").at(i);
};
double SteadyStateProblem::get_slack_production(int i) const {
    return _primal_values.at("slack_production").at(i);
};
double SteadyStateProblem::get_nodal_balance(int i) const {
    return _dual_values.at("nodal_balance").at(i);
};
double SteadyStateProblem::get_pipe_physics(int i) const {
    return _dual_values.at("pipe_physics").at(i);
};
double SteadyStateProblem::get_compressor_physics(int i) const {
    return _dual_values.at("compressor_physics").at(i);
};
double SteadyStateProblem::get_compressor_bounds(int i) const {
    return _dual_values.at("compressor_discharge_bounds").at(i);
};
double SteadyStateProblem::get_compressor_power_bounds(int i) const {
    return _dual_values.at("compressor_power_bounds").at(i);
};
double SteadyStateProblem::get_objective_value() const {
    return _objective_value;
};

void SteadyStateProblem::add_variables() {
    add_rho_variables();
    add_phi_variables();
    add_alpha_variables();
    add_production_consumption_variables();
    add_slack_production_variables();
};

void SteadyStateProblem::add_constraints(const InputParams &ip, const ScalingFactors &sf) {
    add_nodal_balance_constraints();
    add_pipe_physics_constraints(ip, sf);
    add_compressor_physics_constraints(ip, sf);
    add_compressor_discharge_pressure_bounds();
    add_compressor_power_bounds(ip);
};

void SteadyStateProblem::populate_solution() {
    std::vector<std::string> variable_keys{
        "rho", "phi_pipe", "phi_compressor",  "alpha",
        "s",   "d",        "slack_production"};
    std::vector<std::string> constraint_keys{
        "nodal_balance", "pipe_physics", "compressor_physics",
        "compressor_discharge_bounds", "compressor_power_bounds"};
    for (auto key : variable_keys)
        for (auto i : _variables.at(key))
            _primal_values.at(key).at(i.first) =
            _model.get_primal_value(i.second);
    
    for (auto key : constraint_keys)
        for (auto i : _constraints.at(key))
            _dual_values.at(key).at(i.first) = _model.get_dual_value(i.second);
    if (_model.has_objective())
        _objective_value = _model.get_objective_value();
};

void SteadyStateProblem::add_rho_variables() {
    /* nodal rho variables for all non-slack nodes */
    auto p_min = _ssd.get_p_min();
    auto p_max = _ssd.get_p_max();
    _variables["rho"] = std::unordered_map<int, std::shared_ptr<Variable>>();
    _primal_values["rho"] = std::unordered_map<int, double>();
    for (auto index : _ssd.get_non_slack_node_indexes()) {
        auto var = _model.add_variable("rho_" + std::to_string(index));
        var->set_lb(p_min.get_value(index));
        var->set_ub(p_max.get_value(index));
        var->set_start(p_min.get_value(index));
        _variables["rho"][index] = var;
        _primal_values["rho"][index] = std::nan("");
    }
};

void SteadyStateProblem::add_phi_variables() {
    /* pipe phi variables */
    auto phi_min_pipe = _ssd.get_phi_min_pipe();
    auto phi_max_pipe = _ssd.get_phi_max_pipe();
    _variables["phi_pipe"] =
    std::unordered_map<int, std::shared_ptr<Variable>>();
    _primal_values["phi_pipe"] = std::unordered_map<int, double>();
    for (auto index : _ssd.get_pipe_indexes()) {
        auto var = _model.add_variable("phi_pipe_" + std::to_string(index));
        var->set_lb(phi_min_pipe.get_value(index));
        var->set_ub(phi_max_pipe.get_value(index));
        var->set_start((var->get_lb() + var->get_ub()) / 2.0);
        _variables["phi_pipe"][index] = var;
        _primal_values["phi_pipe"][index] = std::nan("");
    }
    
    /* phi compressor variables */
    auto phi_min_compressor = _ssd.get_phi_min_compressor();
    auto phi_max_compressor = _ssd.get_phi_max_compressor();
    _variables["phi_compressor"] =
    std::unordered_map<int, std::shared_ptr<Variable>>();
    _primal_values["phi_compressor"] = std::unordered_map<int, double>();
    for (auto index : _ssd.get_compressor_indexes()) {
        auto var =
        _model.add_variable("phi_compressor_" + std::to_string(index));
        var->set_lb(phi_min_compressor.get_value(index));
        var->set_ub(phi_max_compressor.get_value(index));
        var->set_start((var->get_lb() + var->get_ub()) / 2.0);
        _variables["phi_compressor"][index] = var;
        _primal_values["phi_compressor"][index] = std::nan("");
    }
};

void SteadyStateProblem::add_alpha_variables() {
    /* compressor alpha variables */
    auto c_ratio_min_compressor = _ssd.get_c_ratio_min();
    auto c_ratio_max_compressor = _ssd.get_c_ratio_max();
    _variables["alpha"] = std::unordered_map<int, std::shared_ptr<Variable>>();
    _primal_values["alpha"] = std::unordered_map<int, double>();
    for (auto index : _ssd.get_compressor_indexes()) {
        auto var = _model.add_variable("alpha_" + std::to_string(index));
        var->set_lb(c_ratio_min_compressor.get_value(index));
        var->set_ub(c_ratio_max_compressor.get_value(index));
        var->set_start(c_ratio_max_compressor.get_value(index));
        _variables["alpha"][index] = var;
        _primal_values["alpha"][index] = std::nan("");
    }
};

void SteadyStateProblem::add_production_consumption_variables() {
    /* gnode s variables */
    auto smax = _ssd.get_smax();
    _variables["s"] = std::unordered_map<int, std::shared_ptr<Variable>>();
    _primal_values["s"] = std::unordered_map<int, double>();
    for (auto index : _ssd.get_gnode_indexes()) {
        auto var = _model.add_variable("s_" + std::to_string(index));
        var->set_lb(0.0);
        var->set_ub(smax.get_value(index));
        var->set_start(0.0);
        _variables["s"][index] = var;
        _primal_values["s"][index] = std::nan("");
    }
    
    /* gnode d variables */
    auto dmax = _ssd.get_dmax();
    _variables["d"] = std::unordered_map<int, std::shared_ptr<Variable>>();
    _primal_values["d"] = std::unordered_map<int, double>();
    for (auto index : _ssd.get_gnode_indexes()) {
        auto var = _model.add_variable("d_" + std::to_string(index));
        var->set_lb(0.0);
        var->set_ub(dmax.get_value(index));
        var->set_start(dmax.get_value(index));
        _variables["d"][index] = var;
        _primal_values["d"][index] = std::nan("");
    }
};

void SteadyStateProblem::add_slack_production_variables() {
    /* calculation of maximum withdrawal in the system = sum (g in gnodes)
     * (dmax(g) + gbar(g)) + sum (i in non-slack) qbar(i) */
    auto dmax = _ssd.get_dmax();
    auto gbar = _ssd.get_gbar();
    auto qbar = _ssd.get_qbar();
    double max_withdrawal = 0.0;
    for (auto index : _ssd.get_gnode_indexes())
        max_withdrawal += dmax.get_value(index) + gbar.get_value(index);
    for (auto index : _ssd.get_non_slack_node_indexes())
        max_withdrawal += qbar.get_value(index);
    /* slack production variables */
    _variables["slack_production"] =
    std::unordered_map<int, std::shared_ptr<Variable>>();
    _primal_values["slack_production"] = std::unordered_map<int, double>();
    for (auto index : _ssd.get_slack_node_indexes()) {
        auto var =
        _model.add_variable("slack_production_" + std::to_string(index));
        var->set_lb(0.0);
        var->set_ub(max_withdrawal);
        var->set_start(max_withdrawal);
        _variables["slack_production"][index] = var;
        _primal_values["slack_production"][index] = std::nan("");
    }
};

void SteadyStateProblem::add_nodal_balance_constraints() {
    auto area_pipe = _ssd.get_area_pipe();
    auto area_compressor = _ssd.get_area_compressor();
    _constraints["nodal_balance"] =
    std::unordered_map<int, std::shared_ptr<Constraint>>();
    _dual_values["nodal_balance"] = std::unordered_map<int, double>();
    
    /* nodal balance constraints for slack node */
    for (auto node_index : _ssd.get_slack_node_indexes()) {
        auto constraint = _model.add_constraint("nodal_balance_" +
                                                std::to_string(node_index));
        auto out_pipes_in_node = _ssd.get_out_pipe_indexes_in_node(node_index);
        auto in_pipes_in_node = _ssd.get_in_pipe_indexes_in_nodes(node_index);
        auto out_compressors_in_node =
        _ssd.get_out_compressor_indexes_in_node(node_index);
        auto in_compressors_in_node =
        _ssd.get_in_compressor_indexes_in_node(node_index);
        for (auto out_pipe_index : out_pipes_in_node)
            constraint->add_term(Term(phi_p(out_pipe_index),
                                      area_pipe.get_value(out_pipe_index)));
        for (auto out_compressor_index : out_compressors_in_node)
            constraint->add_term(
                                 Term(phi_c(out_compressor_index),
                                      area_compressor.get_value(out_compressor_index)));
        for (auto in_pipe_index : in_pipes_in_node)
            constraint->add_term(Term(phi_p(in_pipe_index),
                                      -area_pipe.get_value(in_pipe_index)));
        for (auto in_compressor_index : in_compressors_in_node)
            constraint->add_term(
                                 Term(phi_c(in_compressor_index),
                                      -area_compressor.get_value(in_compressor_index)));
        constraint->add_term(Term(slack_production(node_index), -1.0));
        constraint->equal_to(0.0);
        _constraints["nodal_balance"][node_index] = constraint;
        _dual_values["nodal_balance"][node_index] = std::nan("");
    }
    
    /* nodal balance constraints for non-slack nodes */
    auto qbar = _ssd.get_qbar();
    auto gbar = _ssd.get_gbar();
    for (auto node_index : _ssd.get_non_slack_node_indexes()) {
        auto constraint = _model.add_constraint("nodal_balance_" +
                                                std::to_string(node_index));
        auto out_pipes_in_node = _ssd.get_out_pipe_indexes_in_node(node_index);
        auto in_pipes_in_node = _ssd.get_in_pipe_indexes_in_nodes(node_index);
        auto out_compressors_in_node =
        _ssd.get_out_compressor_indexes_in_node(node_index);
        auto in_compressors_in_node =
        _ssd.get_in_compressor_indexes_in_node(node_index);
        auto gnodes_in_node = _ssd.get_gnodes_in_node(node_index);
        if (qbar.get_value(node_index) != 0.0)
            constraint->add_term(Term(qbar.get_value(node_index)));
        for (auto out_pipe_index : out_pipes_in_node)
            constraint->add_term(Term(phi_p(out_pipe_index),
                                      area_pipe.get_value(out_pipe_index)));
        for (auto out_compressor_index : out_compressors_in_node)
            constraint->add_term(
                                 Term(phi_c(out_compressor_index),
                                      area_compressor.get_value(out_compressor_index)));
        for (auto in_pipe_index : in_pipes_in_node)
            constraint->add_term(Term(phi_p(in_pipe_index),
                                      -area_pipe.get_value(in_pipe_index)));
        for (auto in_compressor_index : in_compressors_in_node)
            constraint->add_term(
                                 Term(phi_c(in_compressor_index),
                                      -area_compressor.get_value(in_compressor_index)));
        for (auto gnode_index : gnodes_in_node) {
            constraint->add_term(Term(s(gnode_index), -1.0));
            constraint->add_term(Term(d(gnode_index)));
            if (gbar.get_value(gnode_index) != 0.0)
                constraint->add_term(Term(gbar.get_value(gnode_index)));
        }
        constraint->equal_to(0.0);
        _constraints["nodal_balance"][node_index] = constraint;
        _dual_values["nodal_balance"][node_index] = std::nan("");
    }
};

void SteadyStateProblem::add_pipe_physics_constraints(const InputParams &ip, const ScalingFactors &sf) {
    auto coeffs = get_coeffs(ip, sf);
    auto b_1 = coeffs.first, b_2 = coeffs.second;
    auto slack_nodes = _ssd.get_slack_node_indexes();
    auto resistance = _ssd.get_resistance_pipe();
    auto pslack = _ssd.get_pslack();
    _constraints["pipe_physics"] =
    std::unordered_map<int, std::shared_ptr<Constraint>>();
    _dual_values["pipe_physics"] = std::unordered_map<int, double>();
    /* pipe physics */
    for (auto pipe_index : _ssd.get_pipe_indexes()) {
        auto constraint =
        _model.add_constraint("pipe_physics_" + std::to_string(pipe_index));
        auto fnode_index = _ssd.get_fnode_index_of_pipe(pipe_index);
        auto tnode_index = _ssd.get_tnode_index_of_pipe(pipe_index);
        bool is_fnode_slack =
        slack_nodes.find(fnode_index) != slack_nodes.end();
        if (is_fnode_slack) {
            constraint->add_term(
                                 Term(b_1 * std::pow(pslack.get_value(fnode_index), 2)));
            constraint->add_term(
                                 Term(2.0 * b_2 * std::pow(pslack.get_value(fnode_index), 3) / 3.0));
        }
        else {
            constraint->add_term(
                                 Term(rho(fnode_index), b_1, TermType::quadratic));
            constraint->add_term(
                                 Term(rho(fnode_index), 2 * b_2 / 3.0, TermType::cubic));
        }
        constraint->add_term(Term(rho(tnode_index), -b_1, TermType::quadratic));
        constraint->add_term(Term(rho(tnode_index), -2.0 * b_2 / 3.0, TermType::cubic));
        constraint->add_term(Term(phi_p(pipe_index),
                                  -resistance.get_value(pipe_index),
                                  TermType::x_abs_x));
        constraint->equal_to(0.0);
        _constraints["pipe_physics"][pipe_index] = constraint;
        _dual_values["pipe_physics"][pipe_index] = std::nan("");
    }
};

void SteadyStateProblem::add_compressor_physics_constraints(const InputParams &ip, const ScalingFactors &sf) {
    auto coeffs = get_coeffs(ip, sf);
    auto b_1 = coeffs.first, b_2 = coeffs.second;
    auto slack_nodes = _ssd.get_slack_node_indexes();
    auto resistance = _ssd.get_resistance_compressor();
    auto pslack = _ssd.get_pslack();
    _constraints["compressor_physics"] =
    std::unordered_map<int, std::shared_ptr<Constraint>>();
    _dual_values["compressor_physics"] = std::unordered_map<int, double>();
    /* compressor physics */
    for (auto compressor_index : _ssd.get_compressor_indexes()) {
        auto constraint = _model.add_constraint(
                                                "compressor_physics_" + std::to_string(compressor_index));
        auto fnode_index = _ssd.get_fnode_index_of_compressor(compressor_index);
        auto tnode_index = _ssd.get_tnode_index_of_compressor(compressor_index);
        bool is_fnode_slack =
        slack_nodes.find(fnode_index) != slack_nodes.end();
        if (is_fnode_slack) {
            constraint->add_term(
                                 Term(alpha(compressor_index),
                                      b_1 * std::pow(pslack.get_value(fnode_index), 2),
                                      TermType::quadratic));
            constraint->add_term(
                                 Term(alpha(compressor_index),
                                      2.0 * b_2 * std::pow(pslack.get_value(fnode_index), 2) / 3.0,
                                      TermType::quadratic));
            
        }
        else {
            constraint->add_term(
                                 Term({alpha(compressor_index), rho(fnode_index)}, b_1,
                                      TermType::x_sq_y_sq));
            constraint->add_term(
                                 Term({alpha(compressor_index), rho(fnode_index)}, 2 * b_2 / 3.0,
                                      TermType::x_cube_y_cube));
        }
        constraint->add_term(Term(rho(tnode_index), -b_1, TermType::quadratic));
        constraint->add_term(Term(rho(tnode_index), -2.0 * b_2 / 3.0, TermType::cubic));
        constraint->add_term(Term(phi_c(compressor_index),
                                  -resistance.get_value(compressor_index),
                                  TermType::x_abs_x));
        constraint->equal_to(0.0);
        _constraints["compressor_physics"][compressor_index] = constraint;
        _dual_values["compressor_physics"][compressor_index] = std::nan("");
    }
};

void SteadyStateProblem::add_compressor_discharge_pressure_bounds() {
    auto slack_nodes = _ssd.get_slack_node_indexes();
    auto pslack = _ssd.get_pslack();
    auto p_max = _ssd.get_p_max();
    _constraints["compressor_discharge_bounds"] =
    std::unordered_map<int, std::shared_ptr<Constraint>>();
    _dual_values["compressor_discharge_bounds"] =
    std::unordered_map<int, double>();
    /* discharge pressure from a compressor bounds */
    for (auto compressor_index : _ssd.get_compressor_indexes()) {
        auto constraint = _model.add_constraint(
                                                "compressor_discharge_bounds_" + std::to_string(compressor_index));
        auto fnode_index = _ssd.get_fnode_index_of_compressor(compressor_index);
        bool is_fnode_slack =
        slack_nodes.find(fnode_index) != slack_nodes.end();
        if (is_fnode_slack)
            constraint->add_term(
                                 Term(alpha(compressor_index), pslack.get_value(fnode_index)));
        else
            constraint->add_term(
                                 Term({alpha(compressor_index), rho(fnode_index)}, 1.0,
                                      TermType::xy));
        constraint->two_sided(0.0, p_max.get_value(fnode_index));
        _constraints["compressor_discharge_bounds"][compressor_index] =
        constraint;
        _dual_values["compressor_discharge_bounds"][compressor_index] =
        std::nan("");
    }
};

void SteadyStateProblem::add_compressor_power_bounds(const InputParams &ip) {
    double m = (ip.get_specific_heat_capacity_ratio() - 1) /
    ip.get_specific_heat_capacity_ratio();
    double Wc =
    286.76 * ip.get_temperature() / ip.get_gas_specific_gravity() / m;
    auto power_max = _ssd.get_power_max_compressor();
    _constraints["compressor_power_bounds"] =
    std::unordered_map<int, std::shared_ptr<Constraint>>();
    _dual_values["compressor_power_bounds"] = std::unordered_map<int, double>();
    auto area_compressor = _ssd.get_area_compressor();
    /* compressor power bounds */
    for (auto compressor_index : _ssd.get_compressor_indexes()) {
        double area = area_compressor.get_value(compressor_index);
        auto constraint = _model.add_constraint(
                                                "compressor_power_bounds_" + std::to_string(compressor_index));
        constraint->add_term(
                             Term({alpha(compressor_index), phi_c(compressor_index)}, {m, 1.0},
                                  Wc * area, TermType::xpowermminusone_absy));
        constraint->less_than_equal_to(power_max.get_value(compressor_index));
        _constraints["compressor_power_bounds"][compressor_index] = constraint;
        _dual_values["compressor_power_bounds"][compressor_index] =
        std::nan("");
    }
};

void SteadyStateProblem::add_objective(const InputParams &ip) {
    double m = (ip.get_specific_heat_capacity_ratio() - 1) /
    ip.get_specific_heat_capacity_ratio();
    double econ_weight = ip.get_econ_weight();
    double objective_scaling = std::pow(10, ip.get_objective_scale_exponent());
    auto cd = _ssd.get_cd();
    auto cs = _ssd.get_cs();
    auto cslack = _ssd.get_cslack();
    /* objective term 1: sum (g in gnodes) (- d(g) cd(g) + s(g) cs(g)) + sum (i
     * in slack) slack_production(i) cslack(i)*/
    _objective = _model.add_objective("social_welfare_compressor_power");
    _objective_value = std::nan("");
    for (auto index : _ssd.get_gnode_indexes()) {
        _objective->add_term(Term(d(index), -cd.get_value(index) * econ_weight *
                                  objective_scaling));
        _objective->add_term(Term(s(index), cs.get_value(index) * econ_weight *
                                  objective_scaling));
    }
    for (auto index : _ssd.get_slack_node_indexes())
        _objective->add_term(
                             Term(slack_production(index),
                                  cslack.get_value(index) * econ_weight * objective_scaling));
    
    auto area_compressor = _ssd.get_area_compressor();
    /* objective term 2 : (1.0 - econ_weight) * sum(i in compressors)
     * compressor_power(i) */
    for (auto compressor_index : _ssd.get_compressor_indexes())
        _objective->add_term(Term(
                                  {alpha(compressor_index), phi_c(compressor_index)}, {m, 1.0},
                                  objective_scaling * area_compressor.get_value(compressor_index) *
                                  (1.0 - econ_weight),
                                  TermType::xpowermminusone_absy));
};

std::pair<double, double> SteadyStateProblem::get_coeffs(const InputParams &ip, const ScalingFactors &sf) {
    if (ip.get_model_type() == "cnga") {
        return std::make_pair(1.00300865, 2.96848838E-8 * sf.get_pressure_scaling());
    }
    if (ip.get_model_type() == "fullcnga") {
        auto p_atm = 101350.0;
        auto a_1 = 344400.0;
        auto a_2 = 1.785;
        auto a_3 = 3.825;
        auto b_1 = 1.0 + a_1 * (p_atm/6894.75729) * std::pow(10, a_2 * ip.get_gas_specific_gravity()) /
        std::pow(1.8 * ip.get_temperature(), a_3);
        auto b_2 = a_1 * std::pow(10.0, a_2 * ip.get_gas_specific_gravity()) /
        (6894.75729 * std::pow(1.8 * ip.get_temperature(), a_3));
        return std::make_pair(b_1, b_2 * sf.get_pressure_scaling());
    }
    return std::make_pair(1.0, 0.0);
};
