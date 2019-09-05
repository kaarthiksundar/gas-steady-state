#include <problem.h>
#include <iterator>
#include <memory>

Problem::Problem(const Network & net) {
    this->p_min.set_name("p_min"); this->p_max.set_name("p_max");
    this->phi_min_compressor.set_name("phi_min_comperssor"); 
    this->phi_max_compressor.set_name("phi_max_compressor");
    this->phi_min_pipe.set_name("phi_min_pipe"); 
    this->phi_max_pipe.set_name("phi_max_pipe");
    this->c_ratio_min.set_name("c_ratio_min"); 
    this->c_ratio_max.set_name("c_ratio_max");
    this->length_pipe.set_name("length_pipe");
    this->diameter_pipe.set_name("diameter_pipe");
    this->friction_factor_pipe.set_name("friction_factor_pipe");
    this->length_compressor.set_name("length_compressor");
    this->diameter_compressor.set_name("diameter_compressor");
    this->friction_factor_compressor.set_name("friction_factor_compressor");
    this->cd.set_name("cd"); this->cs.set_name("cs");
    this->pslack.set_name("pslack"); this->cslack.set_name("cslack");
    this->qbar.set_name("qbar"); 
    this->gbar.set_name("gbar"); this->smax.set_name("smax"); this->dmax.set_name("dmax");
    this->area_pipe.set_name("area_pipe");
    this->area_compressor.set_name("area_compressor");

    this->nodes = indices(net.nodes);
    this->pipes = indices(net.pipes);
    this->compressors = indices(net.compressors);
    this->gnodes = indices(net.gnodes);
    for (auto node : net.nodes) {
        if (node->_slack) this->slack_nodes.add(node->_name);
        else this->non_slack_nodes.add(node->_name);
    }
    this->gnodes_per_node = this->get_gnodes_per_node(net, this->gnodes);

    this->populate_parameters(net);
    
}; 

Problem::~Problem() { };

void Problem::populate_parameters(const Network & net) {
    p_min.in(nodes); 
    p_max.in(nodes);
    for (auto node : net.nodes) {
        p_min.set_val(node->_name, node->_pmin);
        p_max.set_val(node->_name, node->_pmax);
    }

    gbar.in(gnodes); smax.in(gnodes); dmax.in(gnodes);
    cs.in(gnodes); cd.in(gnodes);
    for (size_t i = 0; i<net.gbar.size(); ++i) {
        gbar.set_val(i, net.gbar.at(i));
        smax.set_val(i, net.smax.at(i));
        dmax.set_val(i, net.dmax.at(i));
        cs.set_val(i, net.cs.at(i));
        cd.set_val(i, net.cd.at(i));
    }

    pslack.in(slack_nodes); cslack.in(slack_nodes);
    for (size_t i=0; i<net.num_slack_nodes; ++i) {
        pslack.set_val(i, net.pslack.at(i));
        cslack.set_val(i, net.cslack.at(i));
    }

    qbar.in(non_slack_nodes);
    for (size_t i=0; i<net.num_non_slack_nodes; ++i) 
        qbar.set_val(i, net.qbar.at(i));

    phi_min_pipe.in(pipes) = -10000.0;
    phi_max_pipe.in(pipes) = 10000.0;
    length_pipe.in(pipes);
    diameter_pipe.in(pipes);
    friction_factor_pipe.in(pipes);
    area_pipe.in(pipes);
    for (auto pipe : net.pipes) {
        length_pipe.set_val(pipe->_name, pipe->_length);
        diameter_pipe.set_val(pipe->_name, pipe->_diameter);
        friction_factor_pipe.set_val(pipe->_name, pipe->_friction_factor);
        double area = 3.14159 * pipe->_diameter * pipe->_diameter / 4.0;
        area_pipe.set_val(pipe->_name, area);
    }
    
    phi_min_compressor.in(compressors) = -10000.0;
    phi_max_compressor.in(compressors) = 10000.0;
    c_ratio_min.in(compressors);
    c_ratio_max.in(compressors);
    length_compressor.in(compressors);
    diameter_compressor.in(compressors);
    friction_factor_compressor.in(compressors);
    area_compressor.in(compressors);
    for (auto compressor : net.compressors) {
        c_ratio_min.set_val(compressor->_name, compressor->_cmin);
        c_ratio_max.set_val(compressor->_name, compressor->_cmax);
        length_compressor.set_val(compressor->_name, compressor->_length);
        diameter_compressor.set_val(compressor->_name, compressor->_diameter);
        friction_factor_compressor.set_val(compressor->_name, compressor->_friction_factor);
        double area = 3.14159 * compressor->_diameter * compressor->_diameter / 4.0;
        area_compressor.set_val(compressor->_name, area);
    }

};

indices Problem::get_gnodes_per_node(const Network & net, indices ind) {
    auto ids = indices(ind);
    ids.set_name("gnodes_per_node");
    ids._ids = make_shared<vector<vector<size_t>>>();
    ids._type = matrix_;
    ids._ids->resize(net.nodes.size());
    for (auto gnode : net.gnodes) {
        std::string node_id = gnode->_node;
        std::string gnode_name = gnode->_name;
        auto it = std::find_if(net.nodes.begin(), net.nodes.end(), 
            [&node_id] (const Junction* node) { return node->_id == node_id; });
        int index = std::distance(net.nodes.begin(), it);
        auto it1 = ids._keys_map->find(gnode_name);
        ids._ids->at(index).push_back(it1->second);
    }
    
    return ids;
};

void Problem::create_model() {
    model = Model<>("gas-steady-state");

    /* variable declaration */
    p = var<>("p", p_min, p_max);
    phi_pipe = var<>("phi_p", phi_min_pipe, phi_max_pipe);
    phi_compressor = var<>("phi_c", phi_min_compressor, phi_max_compressor);
    s = var<>("s", 0.0, smax);
    d = var<>("d", 0.0, dmax);
    
    /* add variables to model and index them */
    model.add(p.in(non_slack_nodes));
    model.add(phi_pipe.in(pipes));
    model.add(phi_compressor.in(compressors));
    model.add(s.in(gnodes));
    model.add(d.in(gnodes));

    
    Constraint<> balance("balance");
    gnodes_per_node.print();
    balance = //phi_pipe.sum_out(non_slack_nodes) - phi_pipe.sum_in(non_slack_nodes) 
        sum(s, gnodes_per_node) - sum(d, gnodes_per_node) - qbar; //- sum(gbar, gnodes_per_node) - qbar;
    model.add(balance.in(non_slack_nodes) == 0.0);
    // model.print_symbolic();
    // auto test = phi_pipe.sum_out(non_slack_nodes);
    // test.print();
    // this->p = var<>("nodal_pressure", p_min, p_max);
    // model.add(this->p.in(this->non_slack_nodes));
    // Constraint<> balance("balance");
    // balance = this->p;
    // model.add(balance == 0.0);
    model.print();

};