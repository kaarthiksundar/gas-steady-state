#include <iostream>
#include <optionParser.hpp>

#include <input_params.h>
#include <conversions.h>
#include <data.h>
#include <steady_state_data.h>
#include <steady_state_problem.h>
#include <ipopt_problem.h>
#include <steady_state_solution.h>


int main (int argc, char * argv[]) {
    
    /* creating command line options */
    op::OptionParser opt;
    opt.add_option("h", "help", "shows option help"); 
    opt.add_option("n", "case_name", "case name", "model6ss_test_1");
    opt.add_option("p", "case_path", "case file path", "/Users/kaarthik/Documents/research/gas-steady-state/data/");
    opt.add_option("o", "output_path", "output folder path", "/Users/kaarthik/Documents/research/gas-steady-state/output/");
    
    /* parse options */
    bool correct_parsing = opt.parse_options(argc, argv);
    
    if(!correct_parsing){
        return EXIT_FAILURE;
    }
    
    InputParams ip = build_input_params(opt["p"] + opt["n"] + "/");
    ConversionFactors cf(ip);
    Data data(opt["p"] + opt["n"] + "/", ip.get_units());
    ScalingFactors sf = build_scaling_factors(data.get_slack_pmin(), cf);
    data.make_per_unit(cf, sf);
    
    SteadyStateData ssd(data, sf);
    SteadyStateProblem ssp("Steady State NLP model", ssd);
    ssp.add_variables();
    ssp.add_constraints(ip);
    ssp.add_objective(ip);
    
    solve_model(&ssp.get_model());
    ssp.populate_solution();
    SteadyStateSolution sss(data, ssd, ssp, ip);
    if (ip.get_units() == 0)
        sss.make_si(cf, sf);
    else
        sss.make_standard(cf, sf);
    
    
    
    /*
     solve_model(model.get());
     
     auto solution = populate_steady_state_solution_data(net, model_data.get(), problem_data.get(), model.get(), nd, converter);
     
     solution.write_output(net, opt["p"] + opt["n"] + "/");
     return 0;
     */
    return 0;
}
