import logging
import csv
import json
import pandas as pd
import numpy as np

class NpEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, np.integer):
            return int(obj)
        elif isinstance(obj, np.floating):
            return float(obj)
        elif isinstance(obj, np.ndarray):
            return obj.tolist()
        else:
            return super(NpEncoder, self).default(obj)


class Data:
    def __init__(self):
        self.logger = logging.getLogger(__name__)
        logging.basicConfig(format='%(levelname)s : %(message)s', level=logging.INFO)
        self.input_param = {}
        self.nodes = {}
        self.pipes = {}
        self.compressors = {}
        self.gnodes = {}
        self.pslack = {}
        self.cslack = {}
        self.qbar = {}
        self.gbar = {}
        self.smax = {}
        self.dmax = {}
        self.cs = {}
        self.cd = {}

    def set_log_level(self, loglevel):
        self.logger.setLevel(loglevel)

    def read_input_parameters(self, filename):
        df = pd.read_csv(filename, header=None)
        for entry in df.values:
            self.input_param[entry[0]] = entry[1]

    def read_nodes(self, filename):
        df = pd.read_csv(filename)
        num_nodes = len(df)
        column_names = list(df.columns)
        for i in range(0, num_nodes):
            node_id = str(df['node_id'][i])
            self.nodes[node_id] = {}
            for column_name in column_names:
                self.nodes[node_id][column_name] = df[column_name][i]

    def read_pipes(self, filename):
        df = pd.read_csv(filename)
        num_pipes = len(df)
        column_names = list(df.columns)
        for i in range(0, num_pipes):
            pipe_id = str(df['pipe_id'][i])
            self.pipes[pipe_id] = {}
            for column_name in column_names:
                self.pipes[pipe_id][column_name] = df[column_name][i]

    def read_compressors(self, filename):
        df = pd.read_csv(filename)
        num_compressors = len(df)
        column_names = list(df.columns)
        for i in range(0, num_compressors):
            comp_id = str(df['comp_id'][i])
            self.compressors[comp_id] = {}
            for column_name in column_names:
                self.compressors[comp_id][column_name] = df[column_name][i]

    def read_gnodes(self, filename):
        df = pd.read_csv(filename)
        num_gnodes = len(df)
        column_names = list(df.columns)
        for i in range(0, num_gnodes):
            gnode_id = str(df['gnode_id'][i])
            self.gnodes[gnode_id] = {}
            for column_name in column_names:
                self.gnodes[gnode_id][column_name] = df[column_name][i]

    def read_pslack(self, filename):
        df = pd.read_csv(filename)
        for column_id in df.columns:
            self.pslack[column_id] = df[column_id][0]

    def read_cslack(self, filename):
        df = pd.read_csv(filename)
        for column_id in df.columns:
            self.cslack[column_id] = df[column_id][0]

    def read_qbar(self, filename):
        df = pd.read_csv(filename)
        for column_id in df.columns:
            self.qbar[column_id] = df[column_id][0]

    def read_gbar(self, filename):
        df = pd.read_csv(filename)
        for column_id in df.columns:
            self.gbar[column_id] = df[column_id][0]

    def read_smax(self, filename):
        df = pd.read_csv(filename)
        for column_id in df.columns:
            self.smax[column_id] = df[column_id][0]

    def read_dmax(self, filename):
        df = pd.read_csv(filename)
        for column_id in df.columns:
            self.dmax[column_id] = df[column_id][0]

    def read_cs(self, filename):
        df = pd.read_csv(filename)
        for column_id in df.columns:
            self.cs[column_id] = df[column_id][0]

    def read_cd(self, filename):
        df = pd.read_csv(filename)
        for column_id in df.columns:
            self.cd[column_id] = df[column_id][0]
    
    def read_data(self, input_data_folder, 
                    input_param_file='input_param.csv', 
                    nodes_file='input_network_nodes.csv', 
                    pipes_file='input_network_pipes.csv', 
                    compressors_file='input_network_comps.csv', 
                    gnodes_file='input_network_gnodes.csv', 
                    pslack_file='input_int_pslack.csv', 
                    cslack_file='input_int_cslack.csv', 
                    qbar_file='input_int_qbar.csv', 
                    gbar_file='input_int_gbar.csv', 
                    smax_file='input_int_smax.csv', 
                    dmax_file='input_int_dmax.csv',
                    cs_file='input_int_cs.csv',
                    cd_file='input_int_cd.csv'):
        # call all the read functions
        self.read_input_parameters(input_data_folder + input_param_file)
        self.read_nodes(input_data_folder + nodes_file)
        self.read_pipes(input_data_folder + pipes_file)
        self.read_compressors(input_data_folder + compressors_file)
        self.read_gnodes(input_data_folder + gnodes_file)
        self.read_pslack(input_data_folder + pslack_file)
        self.read_cslack(input_data_folder + cslack_file)
        self.read_qbar(input_data_folder + qbar_file)
        self.read_gbar(input_data_folder + gbar_file)
        self.read_smax(input_data_folder + smax_file)
        self.read_dmax(input_data_folder + dmax_file)
        self.read_cs(input_data_folder + cs_file)
        self.read_cd(input_data_folder + cd_file)
        

class Output:
    def __init__(self):
        self.output_dict = {}

    def add_field(self, data, name):
        self.output_dict[name] = {}
        for key, val in data.items():
            self.output_dict[name][key] = val

    def write_to_json_file(self, filename):
        with open(filename, 'w') as json_file:
            json.dump(self.output_dict, json_file, cls=NpEncoder, indent=2)