#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>     /* for printf */
#include <getopt.h>


int test_getopt(int argc, char *argv[], char *optstring)
{
	int opt;
	int i;
	printf("%-20s:%d\n", "argc", argc);
	printf("%-20s:", "argv");
	for(i = 0; i < argc; i++) {
		printf("%s ", argv[i]);
	}
	printf("\n");
	printf("%-20s:%s\n", "optstring", optstring);
	optind = 1;

	while ((opt = getopt(argc, argv, optstring)) != -1) {
		printf("option is %c(%x), option argument is %s; optind=%d\n", opt, opt, optarg, optind);
	}

	if (optind < argc) {
		printf("name argument =");
		for(i = optind; i < argc; i++) {
			printf(" %s", argv[i]);
		}
		printf("\n");
	}

	return 0;
}

int test_getopt_long(int argc, char **argv, char *optstring, struct option long_options[]) {
	int opt;
	int option_index = 0;
	int i;

	printf("%-20s:%d\n", "argc", argc);
	printf("%-20s:", "argv");
	for(i = 0; i < argc; i++) {
		printf("%s ", argv[i]);
	}
	printf("\n");
	printf("%-20s:%s\n", "optstring", optstring);
	printf("%-20s:\n", "long_options");
	for(i = 0; long_options[i].name != 0; i++) {
		printf("%-20s:%20s%20s%20s%5c\n",
			"", 
			long_options[i].name, 
			(long_options[i].has_arg == no_argument) ? "no_argument" : (long_options[i].has_arg == required_argument) ? "required_argument" : "optional_argument", 
			long_options[i].flag ? "use value" : "not use value", 
			long_options[i].val
			);
	}
	optind = 1;

	while((opt = getopt_long(argc, argv, optstring, long_options, &option_index)) != -1) {
		printf("option is %c(%x), long option is %s, long option arg is %s, optind=%d\n",
			opt,
			opt,
			opt != '?' ? long_options[option_index].name : 0,
			optarg,
			optind
			);
	}

	if (optind < argc) {
		printf("name argument =");
		for(i = optind; i < argc; i++) {
			printf(" %s", argv[i]);
		}
		printf("\n");
	}
}

int get_index() {
	int index;
	int n = scanf("%d", &index);
	if(n != 1) {
		index = -1;
	}

	return index;
}

int main(int argc, char **argv) {
	char *cmd_argv[] = {
		argv[0],
		"-n",
		"-t",
		"t",
		"-y",
		"parameter",
		"--t",
	};

	char *cmd_argv_long[] = {
		argv[0],
		"-n",
		"-t",
		"t",
		"--add=add",
		"--delete=delete",
		"--create",
		"create",
		"-c",
		"create",
		"--append",
		"-tt",
		"-yy",
		"parameter",
		"-t",
		"--t",
		"--t",
		"--t",
	};

	int d = 0;

	static struct option long_options[] = {
		{"add",     required_argument, 0,  0 },
		{"append",  no_argument,       0,  0 },
		{"delete",  required_argument, &d,  'd' },
		{"verbose", no_argument,       0,  0 },
		{"create",  required_argument, 0, 'c'},
		{"file",    required_argument, 0,  0 },
		{0,         0,                 0,  0 }
	};

	char *menu[] = {
		"getopt:nonal mode(parameter is left for name argument)",
		"getopt:optstring start with -(parameter is left for 1)",
		"getopt:optstring start with +(stop at first error,left is parameter for name argument)",
		"getopt_long:normal",
	};

	int i;

	for(i = 0; i < sizeof(menu)/sizeof(char *); i++) {
		printf("%-8d:\t%s\n", i, menu[i]);
	}
	printf("which do you select:");
	switch(get_index()) {
		case 0:
			test_getopt(sizeof(cmd_argv)/sizeof(char *), cmd_argv, "nt:");
			break;
		case 1:
			test_getopt(sizeof(cmd_argv)/sizeof(char *), cmd_argv, "-nt:");
			break;
		case 2:
			test_getopt(sizeof(cmd_argv)/sizeof(char *), cmd_argv, "+nt:");
			break;
		case 3:
			test_getopt_long(sizeof(cmd_argv_long)/sizeof(char *), cmd_argv_long, "nt:", long_options);
			printf("delete is %c\n", d);
			break;
		default:
			break;
	}
}
