#include <network.h>

#include <files.h>
#include <gravity/csv.h>
#include <fstream>
#include <sstream>

Network::Network() { 
    standard_units = false;
    si_units = false;
    num_slack_nodes = 0;
    num_non_slack_nodes = 0;
};

Network::~Network() { 
    delete input_params;
    for (auto pipe : pipes)
        delete pipe;
    for (auto node : nodes) 
        delete node;
    for (auto compressor : compressors) 
        delete compressor;
};

void Network::populate_data(std::string path) {
    populate_params(path);
    if (input_params->units == 0) si_units = true;
    else standard_units = true;
    populate_pipes(path);
    populate_nodes(path);
    populate_compressors(path);
    populate_gnodes(path);
    populate_slack_data(path);
    populate_non_slack_data(path);
    populate_gnode_data(path);
    populate_cost_data(path);
};

void Network::populate_params(std::string path) {
    io::CSVReader<2, 
        io::trim_chars<' '>, 
        io::double_quote_escape<',','\"'> > in(path + Filenames::input_params);
    double temperature;
    double gas_specific_gravity;
    double specific_heat_capacity_ratio; 
    double fuel_factor;
    double time_horizon;
    int eos;
    int units;
    int intervals; 
    int save_csv_output;
    int steady_state_solve_only;
    double space_interval;
    double econ_weight;
    double max_iterations;
    int tolerance_exponent;
    int objective_scale_exponent;
    int extension_time_intervals;
    int exit_if_steady_state_check_infeasible;
    std::string parameter; double value;
    while (in.read_row(parameter, value)) {
        std::cout << parameter << " -> " << value << std::endl;
        if (parameter.find("Temperature") != std::string::npos)
            temperature = value; 
        if (parameter.find("Gas") != std::string::npos) 
            gas_specific_gravity = value;
        if (parameter.find("Specific") != std::string::npos) 
            specific_heat_capacity_ratio = value;
        if (parameter.find("Fuel") != std::string::npos) 
            fuel_factor = value;
        if (parameter.find("Time horizon") != std::string::npos) 
            time_horizon = value;
        if (parameter.find("EOS") != std::string::npos) 
            eos = (int) value;
        if (parameter.find("units") != std::string::npos) 
            units = (int) value;
        if (parameter.find("intervals") != std::string::npos) 
            intervals = (int) value;
        if (parameter.find("save csv output") != std::string::npos) 
            save_csv_output = (int) value;
        if (parameter.find("steady-state solve") != std::string::npos) 
            steady_state_solve_only = (int) value;
        if (parameter.find("Space interval") != std::string::npos) 
            space_interval = value;
        if (parameter.find("Econ weight") != std::string::npos) 
            econ_weight = value;
        if (parameter.find("Max iterations") != std::string::npos) 
            max_iterations = (int) value;
        if (parameter.find("Tolerance exponent") != std::string::npos) 
            tolerance_exponent = (int) value;
        if (parameter.find("Objective scale exponent") != std::string::npos) 
            objective_scale_exponent = (int) value;
        if (parameter.find("time horizon extension") != std::string::npos) 
            extension_time_intervals = (int) value;
        if (parameter.find("exit") != std::string::npos) 
            exit_if_steady_state_check_infeasible = (int) value;
    }
    std::cout << units << std::endl;
    input_params = new InputParams(temperature, gas_specific_gravity, 
        specific_heat_capacity_ratio, fuel_factor, 
        time_horizon, eos, units, intervals, 
        save_csv_output, steady_state_solve_only, 
        space_interval, econ_weight, max_iterations, 
        tolerance_exponent, objective_scale_exponent, 
        extension_time_intervals, 
        exit_if_steady_state_check_infeasible);
    std::cout << input_params->temperature << std::endl;
};

void Network::populate_pipes(std::string path) {
    io::CSVReader<8> in(path + Filenames::pipes);
    in.read_header(io::ignore_extra_column, 
        "pipe_id", "pipe_name", "from_node", "to_node", 
        "diameter", "length", "friction_factor", "disc_seg");
    std::string pipe_id, pipe_name, from_node, to_node;
    double diameter, length, friction_factor; 
    int disc_seg;
    while (in.read_row(pipe_id, pipe_name, from_node, to_node, 
        diameter, length, friction_factor, disc_seg)) {
        auto pipe = create_pipe(pipe_id, pipe_name, 
                                from_node, to_node, 
                                diameter, length, 
                                friction_factor, disc_seg, 
                                input_params->units);
        pipes.push_back(pipe);
    }
};

void Network::populate_nodes(std::string path) {
    io::CSVReader<9> in(path + Filenames::nodes);
    in.read_header(io::ignore_extra_column, 
        "node_id", "node_name", "x_coord", "y_coord", 
        "min_pressure", "max_pressure", "min_injection", 
        "max_injection", "slack_bool");
    std::string node_id, node_name; 
    double node_x, node_y, pmax, pmin, injection_min, 
    injection_max; 
    int slack_bool;
    while (in.read_row(node_id, node_name, node_x, node_y, 
        pmax, pmin, injection_min, injection_max, slack_bool)) {
            auto node = create_node(node_id, node_name, node_x, 
                                    node_y, pmin, pmax, 
                                    injection_min, injection_max, 
                                    (bool) slack_bool, 
                                    input_params->units);
            if (node->_slack) num_slack_nodes++;
            else num_non_slack_nodes++;
            nodes.push_back(node);
        }
};

void Network::populate_compressors(std::string path) {
    io::CSVReader<9> in(path + Filenames::compressors);
    in.read_header(io::ignore_extra_column, 
        "comp_id", "comp_name", "from_node", "to_node", 
        "c_min", "c_max", "max_power", "min_flow", "max_flow");
    std::string id, name, fnode, tnode;
    double cmin, cmax, power_max, flow_min, flow_max;
    while (in.read_row(id, name, fnode, tnode, cmin, cmax, 
        power_max, flow_min, flow_max)) {
            auto compressor = create_compressor(id, name, fnode, tnode, 
                                                cmin, cmax, power_max, 
                                                flow_min, flow_max, 
                                                input_params->units);
            compressors.push_back(compressor);
        }
};

void Network::populate_gnodes(std::string path) {
    io::CSVReader<3> in(path + Filenames::gnodes);
    in.read_header(io::ignore_extra_column, 
        "gnode_id", "gnode_name", "node_id");
    std::string id, name, node_id;
    while (in.read_row(id, name, node_id)) {
        auto gnode = create_gnode(id, name, node_id);
        gnodes.push_back(gnode);
    }
};

void Network::populate_slack_data(std::string path) {
    populate_slack_pressure_data(path);
    populate_slack_price_data(path);
};

void Network::populate_slack_pressure_data(std::string path) {  
    io::LineReader in(path + Filenames::slack_pressure);
    std::string line = std::string(in.next_line());
    std::string delimiter = ",";
    size_t position = 0;
    std::string token;
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        pslack_ids.push_back(token);
        line.erase(0, position + delimiter.length());
    }
    pslack_ids.push_back(line);

    /* reads only one line (assumes steady state) */
    line = std::string(in.next_line());
    position = 0;
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        pslack.push_back(std::stod(token));
        line.erase(0, position + delimiter.length());
    }
    pslack.push_back(std::stod(line));
};

void Network::populate_slack_price_data(std::string path) {
    io::LineReader in(path + Filenames::slack_node_price);
    std::string line = std::string(in.next_line());
    std::string delimiter = ",";
    size_t position = 0;
    std::string token;
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        cslack_ids.push_back(token);
        line.erase(0, position + delimiter.length());
    }
    cslack_ids.push_back(line);

    /* reads only one line (assumes steady state) */
    line = std::string(in.next_line());
    position = 0;
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        cslack.push_back(std::stod(token));
        line.erase(0, position + delimiter.length());
    }
    cslack.push_back(std::stod(line));
};

void Network::populate_non_slack_data(std::string path) {
    io::LineReader in(path + Filenames::nonslack_physical_baseline_withdrawals);
    std::string line = std::string(in.next_line());
    std::string delimiter = ",";
    size_t position = 0;
    std::string token;
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        qbar_ids.push_back(token);
        line.erase(0, position + delimiter.length());
    }
    qbar_ids.push_back(line);
    
    /* reads only one line (assumes steady state) */
    line = std::string(in.next_line());
    position = 0;
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        qbar.push_back(std::stod(token));
        line.erase(0, position + delimiter.length());
    }
    qbar.push_back(std::stod(line));
};

void Network::populate_gnode_data(std::string path) {
    io::LineReader gbar_in(path + Filenames::gnodes_baseline_consumer_withdrawals);
    io::LineReader smax_in(path + Filenames::supply_gnodes_max_injections);
    io::LineReader dmax_in(path + Filenames::demand_gnodes_max_offtakes);
    std::string line, token, delimiter = ",";
    size_t position = 0;
    /* read gbar */
    line = std::string(gbar_in.next_line());
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        gbar.push_back(std::stod(token));
        line.erase(0, position + delimiter.length());
    }
    gbar.push_back(std::stod(line));
    /* read smax */
    position = 0;
    line = std::string(smax_in.next_line());
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        smax.push_back(std::stod(token));
        line.erase(0, position + delimiter.length());
    }
    smax.push_back(std::stod(line));
    /* read dmax */
    position = 0;
    line = std::string(dmax_in.next_line());
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        dmax.push_back(std::stod(token));
        line.erase(0, position + delimiter.length());
    }
    dmax.push_back(std::stod(line));
};

void Network::populate_cost_data(std::string path) {
    io::LineReader cd_in(path + Filenames::demand_gnodes_price_bids);
    io::LineReader cs_in(path + Filenames::supply_gnodes_price_bids);
    std::string line, token, delimiter = ",";
    size_t position = 0;
    /* read cd */
    line = std::string(cd_in.next_line());
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        cd.push_back(std::stod(token));
        line.erase(0, position + delimiter.length());
    }
    cd.push_back(std::stod(line));
    /* read cs */
    position = 0;
    line = std::string(cs_in.next_line());
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        cs.push_back(std::stod(token));
        line.erase(0, position + delimiter.length());
    }
    cs.push_back(std::stod(line));
};