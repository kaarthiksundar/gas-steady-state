#ifndef NETWORK_H
#define NETWORK_H

#pragma once 

#include <vector>
#include <unordered_map>
#include <input_params.h>
#include <pipe.h>
#include <junction.h>
#include <compressor.h>
#include <gnode.h>
#include <memory>

class Network {
    public: 
        std::unique_ptr<InputParams> input_params;
        std::vector<std::shared_ptr<Pipe>> pipes;
        std::vector<std::shared_ptr<Junction>> nodes;
        std::vector<std::shared_ptr<Compressor>> compressors;
        std::vector<std::shared_ptr<Gnode>> gnodes;
        std::vector<std::string> pslack_ids;
        std::vector<std::string> cslack_ids;
        std::vector<double> pslack;
        std::vector<double> cslack;
        std::vector<std::string> qbar_ids;
        std::vector<double> qbar;
        std::vector<double> gbar;
        std::vector<double> smax;
        std::vector<double> dmax;
        std::vector<double> cs;
        std::vector<double> cd;
        int num_slack_nodes;
        int num_non_slack_nodes;
        bool standard_units;
        bool si_units;
        bool is_dimensional;

        Network();

        void populate_data(std::string path);

    private: 
        void populate_params(std::string path);
        void populate_pipes(std::string path);
        void populate_nodes(std::string path);
        void populate_compressors(std::string path);
        void populate_gnodes(std::string path);
        void populate_slack_data(std::string path);
        void populate_slack_pressure_data(std::string path);
        void populate_slack_price_data(std::string path);
        void populate_non_slack_data(std::string path);
        void populate_gnode_data(std::string path);
        void populate_cost_data(std::string path);

};

#endif 