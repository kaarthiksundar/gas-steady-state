#include <iostream>
#include <optionParser.hpp>

#include <network.h>
#include <converter.h>
#include <nondimensionalization.h>
#include <problem.h>

using namespace gravity;


int main (int argc, char * argv[]) {
    
    /* creating command line options */
    op::OptionParser opt;
    opt.add_option("h", "help", "shows option help"); 
    opt.add_option("n", "case_name", "case name", "model30");
    opt.add_option("p", "case_path", "case file path", "../data/" );

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
    converter.populate_mmscfd_conversion_factors(net.input_params);
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

    Problem problem(net);
    problem.create_model();

    return 0;

}
