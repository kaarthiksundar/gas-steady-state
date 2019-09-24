#include <data.h>
#include <csv.h>
#include <files.h>
#include <fstream>
#include <sstream>

ComponentData::ComponentData(std::string path, int units) { populate_component_data(path, units); };

ComponentData::ComponentData(const ComponentData && cd) :
_pipes(std::move(cd._pipes)),
_nodes(std::move(cd._nodes)),
_compressors(std::move(cd._compressors)),
_gnodes(std::move(cd._gnodes)) {};

std::vector<std::shared_ptr<Pipe>> ComponentData::get_pipes() { return _pipes; };
std::vector<std::shared_ptr<Node>> ComponentData::get_nodes() { return _nodes; };
std::vector<std::shared_ptr<Compressor>> ComponentData::get_compressors() { return _compressors; };
std::vector<std::shared_ptr<Gnode>> ComponentData::get_gnodes() { return _gnodes; };
int ComponentData::get_num_slack_nodes() { return _num_slack_nodes; };
int ComponentData::get_num_non_slack_nodes() { return _num_non_slack_nodes; };

void ComponentData::populate_component_data(std::string path, int units) {
    populate_pipe_data(path, units); populate_node_data(path, units);
    populate_compressor_data(path, units); populate_gnode_data(path, units);
};

void ComponentData::populate_pipe_data(std::string path, int units) {
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
};

void ComponentData::populate_node_data(std::string path, int units) {
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
};

void ComponentData::populate_compressor_data(std::string path, int units) {
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
};

void ComponentData::populate_gnode_data(std::string path, int units) {
    io::CSVReader<3> in(path + InputFilenames::gnodes);
    in.read_header(io::ignore_extra_column, "gnode_id", "gnode_name", "node_id");
    std::string name;
    int gnode_id, node_id;
    while (in.read_row(gnode_id, name, node_id))
        _gnodes.push_back(std::make_shared<Gnode>(gnode_id, name, node_id));
};

ParameterData::ParameterData(std::string path, int units) {
    populate_slack_data(path, units);
    populate_slack_pressure_data(path, units);
    populate_slack_price_data(path, units);
    populate_non_slack_data(path, units);
    populate_gnode_data(path, units);
    populate_cost_data(path, units);
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
_gbar(std::move(pd._gbar)),
_smax(std::move(pd._smax)),
_dmax(std::move(pd._dmax)),
_cs(std::move(pd._cs)),
_si(pd._si),
_standard(pd._standard),
_per_unit(pd._per_unit) {};

void ParameterData::populate_slack_data(std::string path, int units) {
    populate_slack_pressure_data(path, units); populate_slack_price_data(path, units);
};

void ParameterData::populate_slack_pressure_data(std::string path, int units) {
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
};

void ParameterData::populate_slack_price_data(std::string path, int units) {
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
};

void ParameterData::populate_non_slack_data(std::string path, int units) {
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
};

void ParameterData::populate_gnode_data(std::string path, int units) {
    io::LineReader gbar_in(path + InputFilenames::gnodes_baseline_consumer_withdrawals);
    io::LineReader smax_in(path + InputFilenames::supply_gnodes_max_injections);
    io::LineReader dmax_in(path + InputFilenames::demand_gnodes_max_offtakes);
    std::string line, token, delimiter = ",";
    size_t position = 0;
    /* read gbar */
    line = std::string(gbar_in.next_line());
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        _gbar.push_back(std::stod(token));
        line.erase(0, position + delimiter.length());
    }
    _gbar.push_back(std::stod(line));
    /* read smax */
    position = 0;
    line = std::string(smax_in.next_line());
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        _smax.push_back(std::stod(token));
        line.erase(0, position + delimiter.length());
    }
    _smax.push_back(std::stod(line));
    /* read dmax */
    position = 0;
    line = std::string(dmax_in.next_line());
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        _dmax.push_back(std::stod(token));
        line.erase(0, position + delimiter.length());
    }
    _dmax.push_back(std::stod(line));
};

void ParameterData::populate_cost_data(std::string path, int units) {
    io::LineReader cd_in(path + InputFilenames::demand_gnodes_price_bids);
    io::LineReader cs_in(path + InputFilenames::supply_gnodes_price_bids);
    std::string line, token, delimiter = ",";
    size_t position = 0;
    /* read cd */
    line = std::string(cd_in.next_line());
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        _cd.push_back(std::stod(token));
        line.erase(0, position + delimiter.length());
    }
    _cd.push_back(std::stod(line));
    /* read cs */
    position = 0;
    line = std::string(cs_in.next_line());
    while ((position = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, position);
        _cs.push_back(std::stod(token));
        line.erase(0, position + delimiter.length());
    }
    _cs.push_back(std::stod(line));
};

const std::vector<int> & ParameterData::get_pslack_ids() const { return _pslack_ids; };
const std::vector<int> & ParameterData::get_cslack_ids() const { return _cslack_ids; };
const std::vector<double> & ParameterData::get_pslack() const { return _pslack; };
const std::vector<double> & ParameterData::get_cslack() const { return _cslack; };
const std::vector<int> & ParameterData::get_qbar_ids() const { return _qbar_ids; };
const std::vector<double> & ParameterData::get_qbar() const { return _qbar; };
const std::vector<double> & ParameterData::get_gbar() const { return _gbar; };
const std::vector<double> & ParameterData::get_smax() const { return _smax; };
const std::vector<double> & ParameterData::get_dmax() const { return _dmax; };
const std::vector<double> & ParameterData::get_cs() const { return _cs; };
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

Data::Data(std::string path, int units) : ComponentData(path, units), ParameterData(path, units) {};

double Data::get_slack_pmin() const {
    double pmin = std::numeric_limits<double>::max();
    for (auto node : _nodes)
        if (node->is_slack() && node->get_pmin() < pmin)
            pmin = node->get_pmin();
    double psi_to_pascal = 6894.75729;
    if (is_standard()) return (pmin * psi_to_pascal);
    return pmin;
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


