#include <fstream>
#include <iostream>
#include <optionParser.hpp>

#include <conversions.h>
#include <data.h>
#include <input_params.h>
#include <ipopt_problem.h>
#include <steady_state_data.h>
#include <steady_state_problem.h>
#include <steady_state_solution.h>

int main(int argc, char *argv[]) {

    /* creating command line options */
    op::OptionParser opt;
    opt.add_option("h", "help", "shows option help");
    opt.add_option("n", "case_name", "case name", "model6ss_test_1");
    opt.add_option("p", "case_path", "case file path", "../data/");
    opt.add_option("o", "output_path", "output folder path", "../output/");
    opt.add_option("f", "data_format", "data format (csv/json)", "json");
    opt.add_option("s", "linear_solver", "linear solver to be used (mumps/ma27/ma57/ma97)", "mumps");

    /* parse options */
    bool correct_parsing = opt.parse_options(argc, argv);

    if (!correct_parsing) {
        return EXIT_FAILURE;
    }

    bool has_help = op::str2bool(opt["h"]);
    if (has_help) {
        opt.show_help();
        std::exit(0);
    }

    if (opt["f"] != "csv" && opt["f"] != "json") {
        std::cerr
            << "The data format has to be either json or csv. The supplied "
               "data format is "
            << opt["f"] << std::endl;
        std::exit(0);
    }

    if (opt["f"] == "csv") {
        std::ifstream data_dir((opt["p"] + opt["n"]).c_str());
        std::ifstream out_dir((opt["o"] + opt["n"]).c_str());
        if (!data_dir.good()) {
            std::cerr
                << "The data directory specified is " << opt["p"] + opt["n"]
                << "." << std::endl
                << "This directory does not exist." << std::endl
                << "Enter the correct directory using the flags -p and -n. "
                << std::endl
                << "Use the -h or --help flag to get a detailed list of "
                   "command line options available. "
                << std::endl;
            std::exit(0);
        }
        if (!out_dir.good()) {
            std::cerr
                << "The output directory specified is " << opt["o"] + opt["n"]
                << "." << std::endl
                << "This directory does not exist." << std::endl
                << "Enter the correct directory using the flags -o and -n. "
                << std::endl
                << "Use the -h or --help flag to get a detailed list of "
                   "command line options available. "
                << std::endl;
            std::exit(0);
        }
    } else {
        std::ifstream data_dir((opt["p"]).c_str());
        std::ifstream out_dir((opt["o"]).c_str());
        if (!data_dir.good()) {
            std::cerr
                << "The data directory specified is " << opt["p"] << "."
                << std::endl
                << "This directory does not exist." << std::endl
                << "Enter the correct directory using the flags -p and -n. "
                << std::endl
                << "Use the -h or --help flag to get a detailed list of "
                   "command line options available. "
                << std::endl;
            std::exit(0);
        }
        if (!out_dir.good()) {
            std::cerr
                << "The output directory specified is " << opt["o"] << "."
                << std::endl
                << "This directory does not exist." << std::endl
                << "Enter the correct directory using the flags -o and -n. "
                << std::endl
                << "Use the -h or --help flag to get a detailed list of "
                   "command line options available. "
                << std::endl;
            std::exit(0);
        }
    }

    std::cout << "*** Reading the input parameters and network data "
              << std::endl;
    InputParams ip = build_input_params(opt["p"], opt["n"], opt["f"], opt["s"]);
    ConversionFactors cf(ip);
    Data data(opt["p"], opt["n"], opt["f"], ip.get_units());
    ScalingFactors sf = build_scaling_factors(data.get_slack_pmin(), cf);
    data.make_per_unit(cf, sf);
    std::cout << "*** All data read successfully and converted to per unit "
              << std::endl;
    data.print_summary();

    SteadyStateData ssd(data, sf);
    std::cout
        << "*** Parameters required for formulating steady state model created "
        << std::endl;
    SteadyStateProblem ssp("Steady State NLP model", ssd);
    ssp.add_variables();
    ssp.add_constraints(ip);
    ssp.add_objective(ip);
    std::cout << "*** Steady state model created " << std::endl;

    solve_model(&ssp.get_model(), ip);
    std::cout << "*** Steady state model solved " << std::endl;
    ssp.populate_solution();
    SteadyStateSolution sss(data, ssd, ssp, ip, sf);
    if (ip.get_units() == 0)
        sss.make_si(cf, sf);
    else
        sss.make_standard(cf, sf);
    std::cout << "*** Steady state solution data structures populated "
              << std::endl;

    sss.write_output(data, ip, opt["o"], opt["n"], opt["f"]);

    std::cout << "*** Solution files saved in folder : " << opt["o"]
              << std::endl;

    return 0;
}
