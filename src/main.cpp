#include <iostream>
#include <gravity/solver.h>
#include <optionParser.hpp>

#include <network.h>

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
    std::cout << net.input_params->units << std::endl;
    if (net.input_params->units == 0) 
        std::cout << "all data in SI units" << std::endl;
    else 
        std::cout << "add data in standard units" << std::endl;

}
