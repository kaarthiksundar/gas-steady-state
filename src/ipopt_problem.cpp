#include <ipopt_problem.h>

#include <cassert>
#include <iostream>
#include <cmath>

#include <coin/IpIpoptApplication.hpp>

using namespace Ipopt;

GasNLP::GasNLP(Model * model) : _model(model) {};

GasNLP::~GasNLP() { };

bool GasNLP::get_nlp_info(Index &n, Index &m, Index &nnz_jac_g, Index &nnz_h_lag, IndexStyleEnum &index_style) {
    n = (Index)_model->get_num_variables();
    m = (Index)_model->get_num_constraints();
    nnz_jac_g = (Index)_model->get_nnz_jacobian();
    nnz_h_lag = (Index)_model->get_nnz_hessian_lag();
    index_style = TNLP::C_STYLE;
    return true;
};

bool GasNLP::get_bounds_info(Index n, Number *x_l, Number *x_u, Index m, Number *g_l, Number *g_u) {
    assert(n == _model->get_num_variables());
    assert(m == _model->get_num_constraints());
    for (Index i=0; i<n; ++i) {
        auto bounds = _model->get_variable_bounds(i);
        x_l[i] = (Number)std::get<0>(bounds);
        x_u[i] = (Number)std::get<1>(bounds);
    }
    for (Index i=0; i<m; ++i) {
        auto bounds = _model->get_constraint_bounds(i);
        double lb = std::get<0>(bounds), ub = std::get<1>(bounds);
        if (std::isinf(lb)) g_l[i] = -1e19;
        else g_l[i] = (Number)lb;
        if (std::isinf(ub)) g_u[i] = 1e19;
        else g_u[i] = (Number)ub;
    }
    return true;
};

bool GasNLP::get_starting_point(Index n, bool init_x, Number *x, bool init_z, Number *z_L, Number *z_U, Index m, bool init_lambda, Number *lambda) {
    assert(init_x == true);
    assert(init_z == false);
    assert(init_lambda == false);
    
    for (Index i=0; i<n; ++i) {
        double lb = std::get<0>(_model->get_variable_bounds(i));
        double ub = std::get<1>(_model->get_variable_bounds(i));
        double mid_point = (lb + ub)/2.0;
        x[i] = (Number)mid_point;
    }
    
    return true;
};

bool GasNLP::eval_f(Index n, const Number *x, bool new_x, Number &obj_value) {
    obj_value = (Number)_model->evaluate_objective_function_value(x);
    return true;
};

bool GasNLP::eval_grad_f(Index n, const Number *x, bool new_x, Number *grad_f) {
    auto gradients = _model->evaluate_objective_gradient(x);
    
    for (Index i=0; i<n; ++i)
        grad_f[i] = 0.0;
    for (auto element : gradients)
        grad_f[std::get<0>(element)] = (Number)std::get<1>(element);
    return true;
};

bool GasNLP::eval_g(Index n, const Number *x, bool new_x, Index m, Number *g) {
    for (Index i=0; i<m; ++i)
        g[i] = (Number)_model->evaluate_constraint_function_value(i, x);
    return true;
};

bool GasNLP::eval_jac_g(Index n, const Number *x, bool new_x, Index m, Index nele_jac, Index *iRow, Index *jCol, Number *values) {
    std::vector<int> rows, cols; std::vector<double> vals;
    /* populate sparsity structure */
    if (values == NULL) {
        for (Index i=0; i<m; ++i) {
            auto sparsity_pattern = _model->get_constraint_sparsity(i);
            for (auto element : sparsity_pattern) {
                rows.push_back(i);
                cols.push_back(element);
            }
        }
        for(Index i=0; i<rows.size(); ++i) {
            iRow[i] = rows[i]; jCol[i] = cols[i];
        }
    }
    /* populate Jacobian */
    else {
        for (Index i=0; i<m; ++i) {
            auto gradients = _model->evaluate_constraint_gradient(i, x);
            for (auto element : gradients) {
                rows.push_back(i);
                cols.push_back(std::get<0>(element));
                vals.push_back(std::get<1>(element));
            }
        }
        for (Index i=0; i<vals.size(); ++i)
            values[i] = vals[i];
    }
    return true;
};

bool GasNLP::eval_h(Index n, const Number *x, bool new_x, Number obj_factor, Index m, const Number *lambda, bool new_lambda, Index nele_hess, Index *iRow, Index *jCol, Number *values) {
    return false;
};

void GasNLP::finalize_solution(SolverReturn status, Index n, const Number *x, const Number *z_L, const Number *z_U, Index m, const Number *g, const Number *lambda, Number obj_value, const IpoptData *ip_data, IpoptCalculatedQuantities *ip_cq) {
    _model->set_solve_status((int)status);
    _model->set_objective_value(obj_value);
    _model->set_solution(x);
};

void solve_model(Model * model) {
    // Create a new instance of the GasNLP
    SmartPtr<TNLP> gas_nlp = new GasNLP(model);
    
    // Create a new instance of IpoptApplication
    // We are using the factory, since this allows us to compile this
    // example with an Ipopt Windows DLL
    SmartPtr<IpoptApplication> app = IpoptApplicationFactory();
    app->RethrowNonIpoptException(true);
    
    // Change some options
    app->Options()->SetNumericValue("tol", 1e-4);
    app->Options()->SetStringValue("mu_strategy", "adaptive");
    app->Options()->SetStringValue("output_file", "gas_ss_nlp.out");
//    app->Options()->SetStringValue("jacobian_approximation", "finite-difference-values");
//    app->Options()->SetStringValue("derivative_test", "first-order");
    app->Options()->SetIntegerValue("max_iter", 250);
    app->Options()->SetStringValue("hessian_approximation", "limited-memory");
    app->Options()->SetStringValue("limited_memory_update_type", "BFGS");
    app->Options()->SetStringValueIfUnset("linear_solver", "ma57");
    // The following overwrites the default name (ipopt.opt) of the
     app->Options()->SetStringValue("option_file_name", "gas_ss.opt");
    
    // Initialize the IpoptApplication and process the options
    ApplicationReturnStatus status;
    status = app->Initialize();
    std::cout << "initalized" << std::endl;
    if( status != Solve_Succeeded ) {
        std::cout << std::endl << std::endl << "*** Error during initialization!" << std::endl;
        std::exit(1);
    }
    
    // Ask Ipopt to solve the problem
    status = app->OptimizeTNLP(gas_nlp);
    std::cout << "optimized" << std::endl;
    if (status == Solve_Succeeded)
        std::cout << std::endl << std::endl << "*** The problem solved!" << std::endl;
    else {
        std::cout << std::endl << std::endl << "*** The problem FAILED!" << std::endl;
        std::exit(1);
    }
    
    // As the SmartPtrs go out of scope, the reference count
    // will be decremented and the objects will automatically
    // be deleted.
};
