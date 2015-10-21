import traceback
import argparse

def main():

	argument_parser = argparse.ArgumentParser()
	argument_parser.add_argument('--log-file')
	argument_parser.add_argument('--log-level', choices=['INFO', 'DEBUG'], default='INFO')

	sub_parsers = argument_parser.add_subparsers()

	sub_parser = sub_parsers.add_parser('sub_parse_0', help='sub parse 0')
	#sub_parser.add_argument('parameter-0', default='')
	sub_parser.add_argument('parameter-0', metavar='parameter-0', type=int, nargs='+', help='parameter 0')
	sub_parser.add_argument('--arg-append', help='argument append', default=[], action='append')
	sub_parser.add_argument('--arg-bool', help='argument bool', action='store_true')
	sub_parser.add_argument( '--arg-choice', help='argument choice', default='choice 0', choices=['choice 0', 'choice 1'])
	sub_parser.add_argument('--arg-float', help='argument float', default=1, type=float)
	sub_parser.add_argument('--arg-str', help='argument string', default='argumnet string content')
	sub_parser.set_defaults(handler=sub_parser_handler)

	args = argument_parser.parse_args()

def sub_parser_handler():
	print('sub_parser_handler')

if '__main__' == __name__:
	main()

