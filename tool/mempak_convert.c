#include <stdio.h>
#include <getopt.h>
#include "mempak.h"

#define DEFAULT_FORMAT_STR	"n64"

static void print_usage(void)
{
	printf("Usage: ./mempak_convert in_file out_file <options>\n");
	printf("\n");
	printf("Options:\n");
	printf("   -h                Display help\n");
	printf("   -f format         Write file in specified format (default: %s)\n", DEFAULT_FORMAT_STR);
	printf("\n");
	printf("Formats:\n");
	printf("   mpk               Standard 32kB .mpk file format\n");
	printf("   mpk4              128kB .mpk file (4 copies or the 32kB block)\n");
	printf("   n64               .N64 file format\n");
}

int main(int argc, char **argv)
{
	mempak_structure_t *mpk;
	const char *infile;
	const char *outfile;
	unsigned char type;
	struct option long_options[] = {
		{ "format", required_argument, 0, 'f' },
		{ "help", no_argument, 0, 'h' },
		{ }, // terminator
	};
	const char *format = DEFAULT_FORMAT_STR;

	if (argc < 2) {
		print_usage();
		return 1;
	}

	while(1) {
		int c;

		c = getopt_long(argc, argv, "f:h", long_options, NULL);
		if (c==-1)
			break;

		switch(c)
		{
			case 'h':
				print_usage();
				return 0;

			case 'f':
				format = optarg;
				break;

			case '?':
				fprintf(stderr, "Unknown argument. Try -h\n");
				return -1;
		}
	}

	type = mempak_string2format(format);
	if (type == MPK_FORMAT_INVALID) {
		fprintf(stderr, "Unknown format specified\n");
		return -1;
	}

	infile = argv[optind];
	outfile = argv[optind+1];

	mpk = mempak_loadFromFile(infile);
	if (!mpk) {
		fprintf(stderr, "Could not load mempak file '%s'\n", infile);
		return 1;
	}
	printf("Loaded file '%s' (%s format)\n", infile, mempak_format2string(mpk->file_format));

	mempak_saveToFile(mpk, outfile, type);
	mempak_free(mpk);

	printf("Wrote file '%s' in %s format\n", outfile, mempak_format2string(type));

	return 0;
}