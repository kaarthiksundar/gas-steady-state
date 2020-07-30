import logging
import argparse 

from data import Data
from data import Output


def setup_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument('--datapath', '-p', type=str,
                        default='../model6ss_test_0/', help='path to folder containing csv files')
    parser.add_argument('--outputfolder', '-o', type=str, 
                        default='../', help='folder where json file has to be saved')
    parser.add_argument('--jsonfile', type=str, default='model6ss_test_0.json', help='json file name')
    parser.add_argument('--info', '-i', action='store_true', help='info log level')
    parser.add_argument('--debug', '-d', action='store_true', help='debug log level')
    return parser


def main():
    parser = setup_parser()
    args = parser.parse_args()

    if args.info:
        logging.basicConfig(format='%(levelname)s : %(message)s', level=logging.INFO)
    elif args.debug:
        logging.basicConfig(format='%(levelname)s : %(message)s', level=logging.DEBUG)
    else:
        logging.basicConfig(format='%(levelname)s : %(message)s', level=logging.ERROR)

    logging.info('reading data')
    data = Data() 
    if args.info: 
        data.set_log_level(logging.INFO)
    elif args.debug:
        data.set_log_level(logging.DEBUG)
    else: 
        data.set_log_level(logging.ERROR)
    
    data.read_data(args.datapath)

    parsed_output = Output()
    parsed_output.add_field(data.input_param, 'input_param')
    parsed_output.add_field(data.nodes, 'nodes')
    parsed_output.add_field(data.pipes, 'pipes')
    parsed_output.add_field(data.compressors, 'compressors')
    parsed_output.add_field(data.gnodes, 'gnodes')
    parsed_output.add_field(data.pslack, 'pslack')
    parsed_output.add_field(data.cslack, 'cslack')
    parsed_output.add_field(data.qbar, 'qbar')
    parsed_output.add_field(data.gbar, 'gbar')
    parsed_output.add_field(data.smax, 'smax')
    parsed_output.add_field(data.dmax, 'dmax')
    parsed_output.add_field(data.cs, 'cs')
    parsed_output.add_field(data.cd, 'cd')
    parsed_output.add_field(data.disruption, 'disruption')

    parsed_output.write_to_json_file(args.outputfolder + args.jsonfile)


if __name__ == '__main__':
    main()
