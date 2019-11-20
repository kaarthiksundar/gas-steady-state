import logging
import argparse 


def setup_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument('--datapath', '-p', type=str,
                        default='./model6ss_test_0/', help='path to folder containing csv files')
    parser.add_argument('--outputfolder', '-o', type=str, 
                        default='./', help='folder where json file has to be saved')
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
    
    data.read_data(slack_junctions_file = args.datapath + args.slacks, 
                junctions_file = args.datapath + args.junctions, 
                pipes_file = args.datapath + args.pipes, 
                pipe_dissolve_file = args.datapath + args.dissolve, 
                facilities_file = args.datapath + args.facilities, 
                consumers_file = args.datapath + args.consumers, 
                producers_file = args.datapath + args.producers)

    updated_data = UpdatedData(data)
    if args.info: 
        updated_data.set_log_level(logging.INFO)
    elif args.debug:
        updated_data.set_log_level(logging.DEBUG)
    else: 
        updated_data.set_log_level(logging.ERROR)
    updated_data.populate_compressors()

    parsed_output = Output()
    parsed_output.add_metadata()
    parsed_output.add_field(updated_data.junctions, 'junction')
    parsed_output.add_field(updated_data.pipes, 'pipe')
    parsed_output.add_field(updated_data.compressors, 'compressor')
    parsed_output.add_field(data.consumers, 'consumer')
    parsed_output.add_field(data.producers, 'producer')

    parsed_output.write_to_json_file('rbl.json')


if __name__ == '__main__':
    main()
