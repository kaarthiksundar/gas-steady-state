#include <iostream>
#include <optionParser.hpp>
#include <fstream>

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
    opt.add_option("n", "case_name", "case name", "model6");
    opt.add_option("p", "case_path", "case file path", "../data/model6/");
    opt.add_option("o", "output_path", "output folder path", "../output/");
    opt.add_option("c", "num_instances", "number of instances of the case apart from the base case", "10");
    opt.add_option("f", "data_format", "data format (csv/json)", "json");
    
    
    /* parse options */
    bool correct_parsing = opt.parse_options(argc, argv);
    
    if(!correct_parsing) {
        return EXIT_FAILURE;
    }
    
    bool has_help = op::str2bool(opt["h"]);
    if (has_help) {
        opt.show_help();
        std::exit(0);
    }
    
    if (opt["f"] != "csv" && opt["f"] != "json") {
        std::cerr << "The data format has to be either json or csv. The supplied data format is " << opt["f"] << std::endl;
        std::exit(0);
    }
    
    if (opt["f"] == "csv") {
        std::ifstream data_dir((opt["p"] + opt["n"]).c_str());
        std::ifstream out_dir((opt["o"] + opt["n"]).c_str());
        if (!data_dir.good()) {
            std::cerr << "The data directory specified is " << opt["p"] + opt["n"] << "." << std::endl <<
            "This directory does not exist." << std::endl <<
            "Enter the correct directory using the flags -p and -n. " << std::endl <<
            "Use the -h or --help flag to get a detailed list of command line options available. " << std::endl;
            std::exit(0);
        }
        if (!out_dir.good()) {
            std::cerr << "The output directory specified is " << opt["o"] + opt["n"] << "." << std::endl <<
            "This directory does not exist." << std::endl <<
            "Enter the correct directory using the flags -o and -n. " << std::endl <<
            "Use the -h or --help flag to get a detailed list of command line options available. " << std::endl;
            std::exit(0);
        }
    }
    else {
        std::ifstream data_dir((opt["p"]).c_str());
        std::ifstream out_dir((opt["o"]).c_str());
        if (!data_dir.good()) {
            std::cerr << "The data directory specified is " << opt["p"] << "." << std::endl <<
            "This directory does not exist." << std::endl <<
            "Enter the correct directory using the flags -p and -n. " << std::endl <<
            "Use the -h or --help flag to get a detailed list of command line options available. " << std::endl;
            std::exit(0);
        }
        if (!out_dir.good()) {
            std::cerr << "The output directory specified is " << opt["o"] << "." << std::endl <<
            "This directory does not exist." << std::endl <<
            "Enter the correct directory using the flags -o and -n. " << std::endl <<
            "Use the -h or --help flag to get a detailed list of command line options available. " << std::endl;
            std::exit(0);
        }
    }
    
    /* populate the filenames of the base case and other cases */
    std::string base_case = opt["n"] + "_0";
    std::vector<std::string> other_cases;
    for (auto i=1; i<=op::str2int(opt["c"]); ++i)
        other_cases.push_back(opt["n"] + "_" + std::to_string(i));
    
    /* create Ipopt TNLP object to be used for all solves */
    SmartPtr<GasNLP> gas_nlp = new GasNLP();
    
    /* populate and solve base case */
    std::cout << "*** Reading the input parameters and network data for the base case " << std::endl;
    InputParams ip_base_case = build_input_params(opt["p"], base_case, opt["f"]);
    ConversionFactors cf_base_case(ip_base_case);
    Data data_base_case(opt["p"], base_case, opt["f"], ip_base_case.get_units());
    ScalingFactors sf_base_case = build_scaling_factors(data_base_case.get_slack_pmin(), cf_base_case);
    data_base_case.make_per_unit(cf_base_case, sf_base_case);
    std::cout << "*** All data read successfully and converted to per unit " << std::endl;
    data_base_case.print_summary();
    
    SteadyStateData ssd_base_case(data_base_case, sf_base_case);
    std::cout << "*** Parameters required for formulating base case steady state model created " << std::endl;
    SteadyStateProblem ssp_base_case("Steady State NLP model", ssd_base_case);
    ssp_base_case.add_variables();
    ssp_base_case.add_constraints(ip_base_case);
    ssp_base_case.add_objective(ip_base_case);
    std::cout << "*** Steady state base case model created " << std::endl;
    
    gas_nlp->set_model(&ssp_base_case.get_model());
    assert(~gas_nlp->is_model_empty());
    solve_model(gas_nlp, ip_base_case, base_case + ".out");
    
    std::cout << "*** Steady state base case model solved " << std::endl;
    ssp_base_case.populate_solution();
    SteadyStateSolution sss_base_case(data_base_case, ssd_base_case, ssp_base_case, ip_base_case);
    if (ip_base_case.get_units() == 0)
        sss_base_case.make_si(cf_base_case, sf_base_case);
    else
        sss_base_case.make_standard(cf_base_case, sf_base_case);
    std::cout << "*** Steady state solution data structures populated " << std::endl;
    
    sss_base_case.write_output(data_base_case, ip_base_case, opt["o"], base_case, opt["f"]);
    
    std::cout << "*** Solution files saved in folder : " << opt["o"] << std::endl;
    
    gas_nlp->clear_model();
    
    for (auto case_name : other_cases) {
        std::cout << "*** Reading the input parameters and network data for the case:  " << case_name <<std::endl;
        InputParams ip = build_input_params(opt["p"], case_name, opt["f"]);
        ConversionFactors cf(ip);
        Data data(opt["p"], case_name, opt["f"], ip.get_units());
        ScalingFactors sf = build_scaling_factors(data.get_slack_pmin(), cf);
        data.make_per_unit(cf, sf);
        std::cout << "*** All data read successfully and converted to per unit " << std::endl;
        data.print_summary();
        
        SteadyStateData ssd(data, sf);
        std::cout << "*** Parameters required for formulating base case steady state model created " << std::endl;
        SteadyStateProblem ssp("Steady State NLP model", ssd);
        ssp.add_variables();
        ssp.add_constraints(ip);
        ssp.add_objective(ip);
        std::cout << "*** Steady state base case model created " << std::endl;
        
        gas_nlp->set_model(&ssp.get_model());
        assert(~gas_nlp->is_model_empty());
        solve_model(gas_nlp, ip_base_case, case_name + ".out");
        
        std::cout << "*** Steady state base case model solved " << std::endl;
        ssp.populate_solution();
        SteadyStateSolution sss(data, ssd, ssp, ip);
        if (ip.get_units() == 0)
            sss.make_si(cf, sf);
        else
            sss.make_standard(cf, sf);
        std::cout << "*** Steady state solution data structures populated " << std::endl;
        
        sss.write_output(data, ip, opt["o"], case_name, opt["f"]);
        
        std::cout << "*** Solution files saved in folder : " << opt["o"] << std::endl;
    }
    
    
   
    return 0;
}
