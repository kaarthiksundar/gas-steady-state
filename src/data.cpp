#include <data.h>
#include <files.h>
#include <fstream>
#include <sstream>
#include <input_params.h>
#include <csv.h>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ComponentData::ComponentData(std::string data_path, std::string case_name, std::string data_format, int units) { 
    populate_component_data(data_path, case_name, data_format, units);
};

ComponentData::ComponentData(const ComponentData && cd) :
_pipes(std::move(cd._pipes)),
_nodes(std::move(cd._nodes)),
_compressors(std::move(cd._compressors)),
_gnodes(std::move(cd._gnodes)) {};

std::vector<std::shared_ptr<Pipe>> ComponentData::get_pipes() const { return _pipes; };
std::vector<std::shared_ptr<Node>> ComponentData::get_nodes() const { return _nodes; };
std::vector<std::shared_ptr<Compressor>> ComponentData::get_compressors() const { return _compressors; };
std::vector<std::shared_ptr<Gnode>> ComponentData::get_gnodes() const { return _gnodes; };
int ComponentData::get_num_slack_nodes() const { return _num_slack_nodes; };
int ComponentData::get_num_non_slack_nodes() const { return _num_non_slack_nodes; };

void ComponentData::populate_component_data(std::string data_path, std::string case_name, std::string data_format, int units) {
    _num_slack_nodes = 0; _num_non_slack_nodes = 0;
    populate_pipe_data(data_path, case_name, data_format, units); 
    populate_node_data(data_path, case_name, data_format, units);
    populate_compressor_data(data_path, case_name, data_format, units); 
    populate_gnode_data(data_path, case_name, data_format, units);
};

void ComponentData::populate_pipe_data(std::string data_path, std::string case_name, std::string data_format, int units) {
    std::string path; 
    std::string filename;
    /* adjusting the path and file according for the data format provided */
    if (data_format == "csv") { path = data_path + case_name + "/"; filename = ""; }
    else { path = data_path + case_name; filename = path + ".json"; }
    if (data_format == "csv") {
        io::CSVReader<8> in(path + InputFilenames::pipes);
        in.read_header(io::ignore_extra_column, "pipe_id", "pipe_name",
                   "from_node", "to_node",
                   "diameter", "length",
                   "friction_factor", "disc_seg");
        int pipe_id, fnode, tnode, num_discretization_segments;
        std::string pipe_name;
        double diameter, length, friction_factor;
        while (in.read_row(pipe_id, pipe_name, fnode, tnode, diameter, length, friction_factor, num_discretization_segments))
            _pipes.push_back(std::make_shared<Pipe>(Pipe(pipe_id, pipe_name, fnode, tnode,
                                                     diameter, length, friction_factor, num_discretization_segments, units)));
    }
    else {
        std::ifstream in(filename);
        json json_stream;
        in >> json_stream;
        auto pipes = json_stream["pipes"];
        for (auto & pipe : pipes) {
            int pipe_id = pipe["pipe_id"];
            int fnode = pipe["from_node"];
            int tnode = pipe["to_node"];
            int num_discretization_segments = pipe["disc_seg"];
            std::string pipe_name = pipe["pipe_name"];
            double diameter = pipe["diameter"];
            double length = pipe["length"];
            double friction_factor = pipe["friction_factor"];
            _pipes.push_back(std::make_shared<Pipe>(Pipe(pipe_id, pipe_name, fnode, tnode,
                                                     diameter, length, friction_factor, num_discretization_segments, units)));
        }
        in.close();
    }
};

void ComponentData::populate_node_data(std::string data_path, std::string case_name, std::string data_format, int units) {
    std::string path; 
    std::string filename;
    /* adjusting the path and file according for the data format provided */
    if (data_format == "csv") { path = data_path + case_name + "/"; filename = ""; }
    else { path = data_path + case_name; filename = path + ".json"; }
    if (data_format == "csv") {
        io::CSVReader<9> in(path + InputFilenames::nodes);
        in.read_header(io::ignore_extra_column,
                   "node_id", "node_name", "x_coord", "y_coord",
                   "min_pressure", "max_pressure", "min_injection",
                   "max_injection", "slack_bool");
        int node_id, slack_bool;
        std::string node_name;
        double node_x, node_y, pmax, pmin, injection_min, injection_max;
        while (in.read_row(node_id, node_name, node_x, node_y, pmin, pmax, injection_min, injection_max, slack_bool)) {
            _nodes.push_back(std::make_shared<Node>(Node(node_id, node_name, node_x, node_y, pmin, pmax, injection_min, injection_max, (bool)slack_bool, units)));
            if (_nodes.back()->is_slack()) _num_slack_nodes++;
            else _num_non_slack_nodes++;
        }
    }
    else {
        std::ifstream in(filename);
        json json_stream;
        in >> json_stream;
        auto nodes = json_stream["nodes"];
        for (auto & node : nodes) {
            int node_id = node["node_id"];
            std::string node_name = node["node_name"];
            double node_x = node["x_coord"];
            double node_y = node["y_coord"];
            double pmin = node["min_pressure"];
            double pmax = node["max_pressure"];
            double injection_min = node["min_injection"];
            double injection_max = node["max_injection"];
            int slack_bool = node["slack_bool"];
            _nodes.push_back(std::make_shared<Node>(Node(node_id, node_name, node_x, node_y, pmin, pmax, injection_min, injection_max, (bool)slack_bool, units)));
            if (_nodes.back()->is_slack()) _num_slack_nodes++;
            else _num_non_slack_nodes++;
        }
        in.close();
    }
};

void ComponentData::populate_compressor_data(std::string data_path, std::string case_name, std::string data_format, int units) {
    std::string path; 
    std::string filename;
    /* adjusting the path and file according for the data format provided */
    if (data_format == "csv") { path = data_path + case_name + "/"; filename = ""; }
    else { path = data_path + case_name; filename = path + ".json"; }
    if (data_format == "csv") {
        io::CSVReader<9> in(path + InputFilenames::compressors);
        in.read_header(io::ignore_extra_column,
                   "comp_id", "comp_name", "from_node", "to_node",
                   "c_min", "c_max", "max_power", "min_flow", "max_flow");
        int id, fnode, tnode;
        std::string name;
        double cmin, cmax, power_max, flow_min, flow_max;
        while (in.read_row(id, name, fnode, tnode, cmin, cmax,
                       power_max, flow_min, flow_max))
            _compressors.push_back(std::make_shared<Compressor>(id, name, fnode, tnode, cmin, cmax, power_max, flow_min, flow_max, units));
    }
    else {
        std::ifstream in(filename);
        json json_stream;
        in >> json_stream;
        auto compressors = json_stream["compressors"];
        for (auto & compressor : compressors) {
            int id = compressor["comp_id"];
            std::string name = compressor["comp_name"];
            int fnode = compressor["from_node"];
            int tnode = compressor["to_node"];
            double cmin = compressor["c_min"];
            double cmax = compressor["c_max"];
            double power_max = compressor["max_power"];
            double flow_min = compressor["min_flow"];
            double flow_max = compressor["max_flow"];
            _compressors.push_back(std::make_shared<Compressor>(id, name, fnode, tnode, cmin, cmax, power_max, flow_min, flow_max, units));
        }
        in.close();
    }
};

void ComponentData::populate_gnode_data(std::string data_path, std::string case_name, std::string data_format, int units) {
    std::string path; 
    std::string filename;
    /* adjusting the path and file according for the data format provided */
    if (data_format == "csv") { path = data_path + case_name + "/"; filename = ""; }
    else { path = data_path + case_name; filename = path + ".json"; }
    if (data_format == "csv") {
        io::CSVReader<3> in(path + InputFilenames::gnodes);
        in.read_header(io::ignore_extra_column, "gnode_id", "gnode_name", "node_id");
        std::string name;
        int gnode_id, node_id;
        while (in.read_row(gnode_id, name, node_id))
            _gnodes.push_back(std::make_shared<Gnode>(gnode_id, name, node_id));
    }
    else {
        std::ifstream in(filename);
        json json_stream;
        in >> json_stream;
        auto gnodes = json_stream["gnodes"];
        for (auto & gnode : gnodes) {
            int gnode_id = gnode["gnode_id"];
            std::string name = gnode["gnode_name"];
            int node_id = gnode["node_id"];
            _gnodes.push_back(std::make_shared<Gnode>(gnode_id, name, node_id));
        }
        in.close();
    }
};

void ComponentData::print_summary() const {
    std::cout << "------------------------------------" << std::endl;
    std::cout << "        Summary of data read        " << std::endl;
    std::cout << " Number of nodes : " << _nodes.size() << std::endl;
    std::cout << " Number of pipes : " << _pipes.size() << std::endl;
    std::cout << " Number of compressors : " << _compressors.size() << std::endl;
    std::cout << " Number of gnodes : " << _gnodes.size() << std::endl;
    std::cout << "------------------------------------" << std::endl;
}

ParameterData::ParameterData(std::string data_path, std::string case_name, std::string data_format, int units) {
    populate_slack_data(data_path, case_name, data_format, units);
    populate_non_slack_data(data_path, case_name, data_format, units);
    populate_gnode_data(data_path, case_name, data_format, units);
    populate_cost_data(data_path, case_name, data_format, units);
    _si = false; _standard = false; _per_unit = false;
    if (units == 0) _si = true;
    else _standard = true;
};

ParameterData::ParameterData(const ParameterData &&pd) :
_pslack_ids(std::move(pd._pslack_ids)),
_cslack_ids(std::move(pd._cslack_ids)),
_pslack(std::move(pd._pslack)),
_cslack(std::move(pd._cslack)),
_qbar_ids(std::move(pd._qbar_ids)),
_qbar(std::move(pd._qbar)),
_gbar_ids(std::move(pd._gbar_ids)),
_gbar(std::move(pd._gbar)),
_smax_ids(std::move(pd._smax_ids)),
_smax(std::move(pd._smax)),
_dmax_ids(std::move(pd._dmax_ids)),
_dmax(std::move(pd._dmax)),
_cs_ids(std::move(pd._cs_ids)),
_cs(std::move(pd._cs)),
_cd_ids(std::move(pd._cd_ids)),
_cd(std::move(pd._cd)),
_si(pd._si),
_standard(pd._standard),
_per_unit(pd._per_unit) {};

void ParameterData::populate_slack_data(std::string data_path, std::string case_name, std::string data_format, int units) {
    populate_slack_pressure_data(data_path, case_name, data_format, units);
    populate_slack_price_data(data_path, case_name, data_format, units);
};

void ParameterData::populate_slack_pressure_data(std::string data_path, std::string case_name, std::string data_format, int units) {
    std::string path; 
    std::string filename;
    /* adjusting the path and file according for the data format provided */
    if (data_format == "csv") { path = data_path + case_name + "/"; filename = ""; }
    else { path = data_path + case_name; filename = path + ".json"; }
    if (data_format == "csv") {
        io::LineReader in(path + InputFilenames::slack_pressure);
        std::string line = std::string(in.next_line());
        std::string delimiter = ",";
        size_t position = 0;
        std::string token;
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _pslack_ids.push_back(std::stoi(token));
            line.erase(0, position + delimiter.length());
        }
        _pslack_ids.push_back(std::stoi(line));
    
        /* reads only one line (assumes steady state) */
        line = std::string(in.next_line());
        position = 0;
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _pslack.push_back(std::stod(token));
            line.erase(0, position + delimiter.length());
        }
        _pslack.push_back(std::stod(line));
    }
    else {
        std::ifstream in(filename);
        json json_stream;
        in >> json_stream;
        auto pslack = json_stream["pslack"];
        for (auto & element : pslack.items()) {
            _pslack_ids.push_back(std::stoi(element.key().c_str()));
            _pslack.push_back(element.value());
        }
        in.close();
    }
};

void ParameterData::populate_slack_price_data(std::string data_path, std::string case_name, std::string data_format, int units) {
    std::string path; 
    std::string filename;
    /* adjusting the path and file according for the data format provided */
    if (data_format == "csv") { path = data_path + case_name + "/"; filename = ""; }
    else { path = data_path + case_name; filename = path + ".json"; }
    if (data_format == "csv") {
        io::LineReader in(path + InputFilenames::slack_node_price);
        std::string line = std::string(in.next_line());
        std::string delimiter = ",";
        size_t position = 0;
        std::string token;
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _cslack_ids.push_back(std::stoi(token));
            line.erase(0, position + delimiter.length());
        }
        _cslack_ids.push_back(std::stoi(line));
    
        /* reads only one line (assumes steady state) */
        line = std::string(in.next_line());
        position = 0;
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _cslack.push_back(std::stod(token));
            line.erase(0, position + delimiter.length());
        }
        _cslack.push_back(std::stod(line));
    }
    else {
        std::ifstream in(filename);
        json json_stream;
        in >> json_stream;
        auto cslack = json_stream["cslack"];
        for (auto & element : cslack.items()) {
            _cslack_ids.push_back(std::stoi(element.key().c_str()));
            _cslack.push_back(element.value());
        }
        in.close();
    }
};

void ParameterData::populate_non_slack_data(std::string data_path, std::string case_name, std::string data_format, int units) {
    std::string path; 
    std::string filename;
    /* adjusting the path and file according for the data format provided */
    if (data_format == "csv") { path = data_path + case_name + "/"; filename = ""; }
    else { path = data_path + case_name; filename = path + ".json"; }
    if (data_format == "csv") {
        io::LineReader in(path + InputFilenames::nonslack_physical_baseline_withdrawals);
        std::string line = std::string(in.next_line());
        std::string delimiter = ",";
        size_t position = 0;
        std::string token;
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _qbar_ids.push_back(std::stoi(token));
            line.erase(0, position + delimiter.length());
        }
        _qbar_ids.push_back(std::stoi(line));
        
        /* reads only one line (assumes steady state) */
        line = std::string(in.next_line());
        position = 0;
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _qbar.push_back(std::stod(token));
            line.erase(0, position + delimiter.length());
        }
        _qbar.push_back(std::stod(line));
    }
    else {
        std::ifstream in(filename);
        json json_stream;
        in >> json_stream;
        auto qbar = json_stream["qbar"];
        for (auto & element : qbar.items()) {
            _qbar_ids.push_back(std::stoi(element.key().c_str()));
            _qbar.push_back(element.value());
        }
        in.close();
    }
};

void ParameterData::populate_gnode_data(std::string data_path, std::string case_name, std::string data_format, int units) {
    std::string path; 
    std::string filename;
    /* adjusting the path and file according for the data format provided */
    if (data_format == "csv") { path = data_path + case_name + "/"; filename = ""; }
    else { path = data_path + case_name; filename = path + ".json"; }
    if (data_format == "csv") {
        io::LineReader gbar_in(path + InputFilenames::gnodes_baseline_consumer_withdrawals);
        io::LineReader smax_in(path + InputFilenames::supply_gnodes_max_injections);
        io::LineReader dmax_in(path + InputFilenames::demand_gnodes_max_offtakes);
        std::string line, token, delimiter = ",";
        size_t position = 0;
        /* read gbar ids */
        line = std::string(gbar_in.next_line());
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _gbar_ids.push_back(std::stoi(token));
            line.erase(0, position + delimiter.length());
        }
        _gbar_ids.push_back(std::stoi(line));

        /* read gbar values */
        line = std::string(gbar_in.next_line());
        position = 0;
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _gbar.push_back(std::stod(token));
            line.erase(0, position + delimiter.length());
        }
        _gbar.push_back(std::stod(line));

        /* read smax ids */
        position = 0;
        line = std::string(smax_in.next_line());
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _smax_ids.push_back(std::stoi(token));
            line.erase(0, position + delimiter.length());
        }
        _smax_ids.push_back(std::stoi(line));

        /* read smax values */
        line = std::string(smax_in.next_line());
        position = 0;
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _smax.push_back(std::stod(token));
            line.erase(0, position + delimiter.length());
        }
        _smax.push_back(std::stod(line));

        /* read dmax ids */
        position = 0;
        line = std::string(dmax_in.next_line());
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _dmax_ids.push_back(std::stoi(token));
            line.erase(0, position + delimiter.length());
        }
        _dmax_ids.push_back(std::stoi(line));

        /* read dmax values */
        position = 0;
        line = std::string(dmax_in.next_line());
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _dmax.push_back(std::stod(token));
            line.erase(0, position + delimiter.length());
        }
        _dmax.push_back(std::stod(line));
    }
    else {
        std::ifstream in(filename);
        json json_stream;
        in >> json_stream;
        auto gbar = json_stream["gbar"];
        auto smax = json_stream["smax"];
        auto dmax = json_stream["dmax"];
        for (auto & element : gbar.items()) {
            _gbar_ids.push_back(std::stoi(element.key().c_str()));
            _gbar.push_back(element.value());
        }
        for (auto & element : smax.items()) {
            _smax_ids.push_back(std::stoi(element.key().c_str()));
            _smax.push_back(element.value());
        }
        for (auto & element : dmax.items()) {
            _dmax_ids.push_back(std::stoi(element.key().c_str()));
            _dmax.push_back(element.value());
        }
        in.close();
    }
};

void ParameterData::populate_cost_data(std::string data_path, std::string case_name, std::string data_format, int units) {
    std::string path; 
    std::string filename;
    /* adjusting the path and file according for the data format provided */
    if (data_format == "csv") { path = data_path + case_name + "/"; filename = ""; }
    else { path = data_path + case_name; filename = path + ".json"; }
    if (data_format == "csv") {
        io::LineReader cd_in(path + InputFilenames::demand_gnodes_price_bids);
        io::LineReader cs_in(path + InputFilenames::supply_gnodes_price_bids);
        std::string line, token, delimiter = ",";
        size_t position = 0;
        /* read cd ids*/
        line = std::string(cd_in.next_line());
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _cd_ids.push_back(std::stoi(token));
            line.erase(0, position + delimiter.length());
        }
        _cd_ids.push_back(std::stoi(line));

        /* read cd values */
        position = 0;
        line = std::string(cd_in.next_line());
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _cd.push_back(std::stod(token));
            line.erase(0, position + delimiter.length());
        }
        _cd.push_back(std::stod(line));

        /* read cs ids */
        position = 0;
        line = std::string(cs_in.next_line());
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _cs_ids.push_back(std::stoi(token));
            line.erase(0, position + delimiter.length());
        }
        _cs_ids.push_back(std::stoi(line));

        /* read cs values */
        position = 0;
        line = std::string(cs_in.next_line());
        while ((position = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, position);
            _cs.push_back(std::stod(token));
            line.erase(0, position + delimiter.length());
        }
        _cs.push_back(std::stod(line));
    }
    else {
        std::ifstream in(filename);
        json json_stream;
        in >> json_stream;
        auto cs = json_stream["cs"];
        auto cd = json_stream["cd"];
        for (auto & element : cs.items()) {
            _cs_ids.push_back(std::stoi(element.key().c_str()));
            _cs.push_back(element.value());
        }
        for (auto & element : cd.items()) {
            _cd_ids.push_back(std::stoi(element.key().c_str()));
            _cd.push_back(element.value());
        }
        in.close();
    }
};

const std::vector<int> & ParameterData::get_pslack_ids() const { return _pslack_ids; };
const std::vector<int> & ParameterData::get_cslack_ids() const { return _cslack_ids; };
const std::vector<double> & ParameterData::get_pslack() const { return _pslack; };
const std::vector<double> & ParameterData::get_cslack() const { return _cslack; };
const std::vector<int> & ParameterData::get_qbar_ids() const { return _qbar_ids; };
const std::vector<double> & ParameterData::get_qbar() const { return _qbar; };
const std::vector<int> & ParameterData::get_gbar_ids() const { return _gbar_ids; };
const std::vector<double> & ParameterData::get_gbar() const { return _gbar; };
const std::vector<int> & ParameterData::get_smax_ids() const { return _smax_ids; };
const std::vector<double> & ParameterData::get_smax() const { return _smax; };
const std::vector<int> & ParameterData::get_dmax_ids() const { return _dmax_ids; };
const std::vector<double> & ParameterData::get_dmax() const { return _dmax; };
const std::vector<int> & ParameterData::get_cs_ids() const { return _cs_ids; };
const std::vector<double> & ParameterData::get_cs() const { return _cs; };
const std::vector<int> & ParameterData::get_cd_ids() const { return _cs_ids; };
const std::vector<double> & ParameterData::get_cd() const { return _cd; };
bool ParameterData::is_si() const { return _si; };
bool ParameterData::is_standard() const { return _standard; };
bool ParameterData::is_per_unit() const {return _per_unit; };

void ParameterData::make_parameters_per_unit(const ConversionFactors & cf, const ScalingFactors & sf) {
    if (is_per_unit()) return;
    if (is_standard()) make_parameters_si(cf, sf);
    if (is_si()) {
        std::transform(_pslack.begin(), _pslack.end(), _pslack.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_pressure_scaling()));
        std::transform(_qbar.begin(), _qbar.end(), _qbar.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_gbar.begin(), _gbar.end(), _gbar.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_smax.begin(), _smax.end(), _smax.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_dmax.begin(), _dmax.end(), _dmax.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, sf.get_flow_scaling()));
    }
    _per_unit = true;
    _si = false;
    _standard = false;
};

void ParameterData::make_parameters_standard(const ConversionFactors & cf, const ScalingFactors & sf) {
    if (is_standard()) return;
    if (is_per_unit()) make_parameters_si(cf, sf);
    if (is_si()) {
        std::transform(_pslack.begin(), _pslack.end(), _pslack.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.pascal_to_psi()));
        std::transform(_cslack.begin(), _cslack.end(), _cslack.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_qbar.begin(), _qbar.end(), _qbar.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_gbar.begin(), _gbar.end(), _gbar.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_smax.begin(), _smax.end(), _smax.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_dmax.begin(), _dmax.end(), _dmax.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_cs.begin(), _cs.end(), _cs.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, cf.kgps_to_mmscfd()));
        std::transform(_cd.begin(), _cd.end(), _cd.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, cf.kgps_to_mmscfd()));
    }
    _per_unit = false;
    _si = false;
    _standard = true;
};

void ParameterData::make_parameters_si(const ConversionFactors & cf, const ScalingFactors & sf) {
    if (is_si()) return;
    if (is_per_unit()) {
        std::transform(_pslack.begin(), _pslack.end(), _pslack.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_pressure_scaling()));
        std::transform(_qbar.begin(), _qbar.end(), _qbar.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_gbar.begin(), _gbar.end(), _gbar.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_smax.begin(), _smax.end(), _smax.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_flow_scaling()));
        std::transform(_dmax.begin(), _dmax.end(), _dmax.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, sf.get_flow_scaling()));
    }
    if (is_standard()) {
        std::transform(_pslack.begin(), _pslack.end(), _pslack.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.psi_to_pascal()));
        std::transform(_cslack.begin(), _cslack.end(), _cslack.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_qbar.begin(), _qbar.end(), _qbar.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_gbar.begin(), _gbar.end(), _gbar.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_smax.begin(), _smax.end(), _smax.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_dmax.begin(), _dmax.end(), _dmax.begin(),
                       std::bind(std::multiplies<double>(), std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_cs.begin(), _cs.end(), _cs.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, cf.mmscfd_to_kgps()));
        std::transform(_cd.begin(), _cd.end(), _cd.begin(),
                       std::bind(std::divides<double>(), std::placeholders::_1, cf.mmscfd_to_kgps()));
    }
    _per_unit = false;
    _si = true;
    _standard = false;
};

Data::Data(std::string data_path, std::string case_name, std::string data_format, int units) : 
ComponentData(data_path, case_name, data_format, units), 
ParameterData(data_path, case_name, data_format, units) 
{};

double Data::get_slack_pmin() const {
    double pmin = std::numeric_limits<double>::max();
    for (auto node : _nodes)
        if (node->is_slack() && node->get_pmin() < pmin)
            pmin = node->get_pmin();
    double psi_to_pascal = 6894.75729;
    if (is_standard()) return (pmin * psi_to_pascal);
    return pmin;
};

void Data::fix_parameter_ordering() {

};

void Data::make_si(const ConversionFactors &cf, const ScalingFactors &sf) {
    for (auto pipe : _pipes) pipe->make_si(cf, sf);
    for (auto node : _nodes) node->make_si(cf, sf);
    for (auto compressor : _compressors) compressor->make_si(cf, sf);
    make_parameters_si(cf, sf);
    
};

void Data::make_standard(const ConversionFactors &cf, const ScalingFactors &sf) {
    for (auto pipe : _pipes) pipe->make_standard(cf, sf);
    for (auto node : _nodes) node->make_standard(cf, sf);
    for (auto compressor : _compressors) compressor->make_standard(cf, sf);
    make_parameters_standard(cf, sf);
};

void Data::make_per_unit(const ConversionFactors &cf, const ScalingFactors &sf) {
    for (auto pipe : _pipes) pipe->make_per_unit(cf, sf);
    for (auto node : _nodes) node->make_per_unit(cf, sf);
    for (auto compressor : _compressors) compressor->make_per_unit(cf, sf);
    make_parameters_per_unit(cf, sf);
};


InputParams build_input_params(std::string data_path, std::string case_name, std::string data_format) {
    std::string path; 
    std::string filename;
    /* adjusting the path and file according for the data format provided */
    if (data_format == "csv") { path = data_path + case_name + "/"; filename = ""; }
    else { path = data_path + case_name; filename = path + ".json"; }
    /* initializing the parameter variables */
    double temperature;
    double gas_specific_gravity;
    double specific_heat_capacity_ratio;
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
    /* if data format is csv, read from csv and populate parameters */
    if (data_format == "csv") {
        io::CSVReader<2, io::trim_chars<' '>, io::double_quote_escape<',','\"'> > in(path + InputFilenames::input_params);
        std::string parameter; double value;
        while (in.read_row(parameter, value)) {
            if (parameter.find("Temperature") != std::string::npos)
                temperature = value;
            if (parameter.find("Gas") != std::string::npos)
                gas_specific_gravity = value;
            if (parameter.find("Specific") != std::string::npos)
                specific_heat_capacity_ratio = value;
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
    }
    /* if data format is json, read from json and populate parameters */
    else {
        std::ifstream in(filename);
        json json_stream;
        in >> json_stream;
        auto input_params = json_stream["input_param"];
        for (auto & element : input_params.items()) {
            std::string parameter = element.key();
            double value = element.value();
            if (parameter.find("Temperature") != std::string::npos)
                temperature = value;
            if (parameter.find("Gas") != std::string::npos)
                gas_specific_gravity = value;
            if (parameter.find("Specific") != std::string::npos)
                specific_heat_capacity_ratio = value;
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
        in.close();
    }

    /* return the built input parameters */
    return InputParams(temperature, gas_specific_gravity, specific_heat_capacity_ratio,
                       time_horizon, eos, units, intervals, save_csv_output,
                       steady_state_solve_only, space_interval, econ_weight,
                       max_iterations, tolerance_exponent, objective_scale_exponent,
                       extension_time_intervals, exit_if_steady_state_check_infeasible);
};
