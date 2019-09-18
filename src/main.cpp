#include <iostream>
#include <optionParser.hpp>

#include <network.h>
#include <converter.h>
#include <nondimensionalization.h>

#include <problem_data.h>
#include <model_data.h>
#include <ipopt_problem.h>
#include <solution.h>


int main (int argc, char * argv[]) {
    
    /* creating command line options */
    op::OptionParser opt;
    opt.add_option("h", "help", "shows option help"); 
    opt.add_option("n", "case_name", "case name", "model30ss_csv_si");
    opt.add_option("p", "case_path", "case file path", "/Users/kaarthik/Documents/research/gas-steady-state/data/" );

    /* parse options */
    bool correct_parsing = opt.parse_options(argc, argv);
    
    if(!correct_parsing){
        return EXIT_FAILURE;
    }

    Network net;
    net.populate_data(opt["p"] + opt["n"] + "/");
    std::cout << "number of pipes: " << net.pipes.size() << std::endl;
    std::cout << "number of nodes: " << net.nodes.size() << std::endl;
    std::cout << "number of compressors: " << net.compressors.size() << std::endl;
    std::cout << "number of gnodes: " << net.gnodes.size() << std::endl;
    std::cout << "number of slack nodes: " << net.num_slack_nodes << std::endl;
    std::cout << "number of non-slack nodes: " << net.num_non_slack_nodes << std::endl;
    if (net.input_params->units == 0) 
        std::cout << "all data in SI units" << std::endl;
    else 
        std::cout << "add data in standard units" << std::endl;
    
    Converter converter;
    converter.populate_mmscfd_conversion_factors(net.input_params.get());
    converter.convert_to_si(net);
    std::cout << "conversion done " << std::endl;
    if (net.si_units) 
        std::cout << "all data in SI units" << std::endl;
    else 
        std::cout << "add data in standard units" << std::endl;

    Nondimensionalization nd(net, converter);
    nd.non_dimensionalize(net);
    std::cout << "non-dimensionalization performed " << std::endl;
    std::cout << "is network dimensional: " << net.is_dimensional << std::endl;

    auto problem_data = std::make_unique<ProblemData>(net, nd);
    auto model_data = std::make_unique<SteadyStateModelData>();

    auto model = build_steady_state_model(problem_data.get(), model_data.get());

    std::cout << "number of variables : " << model->get_num_variables() << std::endl;
    std::cout << "number of constraints : " << model->get_num_constraints() << std::endl;
    
    solve_model(model.get());
    
    auto solution = populate_steady_state_solution_data(net, model_data.get(), problem_data.get(), model.get(), nd, converter);
    
    solution->write_output(net, opt["p"] + opt["n"] + "/");
    return 0;
}
