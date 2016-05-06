#include <iostream>
#include <vector>
#include <string>
#include <limits>

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

using namespace std;

size_t get_memory_size ()
{
	size_t pages = sysconf(_SC_PHYS_PAGES);
	size_t page_size = sysconf(_SC_PAGE_SIZE);
	return pages * page_size;
}

size_t get_buffer_size(const vector<string>& args)
{
	bool first = true;
	for (const string& arg : args) {
		if (first) {
			first = false;
			continue;
		}
		if (arg == "half") {
			return get_memory_size() >> 1;
		} else if (arg == "quarter") {
			return get_memory_size() >> 2;
		} else if (arg == "eighth") {
			return get_memory_size() >> 3;
		} else if (arg[0] != '-') {
			int end = 0;
			while (arg[end] >= '0' && arg[end] <= '9')
				++end;
			string num = arg.substr(0, end);
			size_t size = atoi(num.c_str());
			switch (arg[end]) {
				case 'e':
				case 'E':
					size <<= 10;
				case 'p':
				case 'P':
					size <<= 10;
				case 't':
				case 'T':
					size <<= 10;
				case 'g':
				case 'G':
					size <<= 10;
				case 'm':
				case 'M':
					size <<= 10;
				case 'k':
					size <<= 10;
				case '\0':
					break;
				default:
					cerr << "INVALID POSTIFX: " << arg[end] << endl;
					return 0;
			}
			return size;
		}
	}
	return 0;
}

void pass_through_io (size_t buf_size, FILE *infile, FILE *outfile)
{
	vector<char> buf(buf_size);
	size_t count = 0;
	while ((count = fread(&buf[0], 1, buf.size(), infile)) > 0) {
		fwrite(&buf[0], 1, count, outfile);
	}
}

int main (int argc, char *argv[])
{
	vector<string> args(argc);
	for (int k = 0; k < argc; ++k)
		args[k] = argv[k];
	
	size_t size = get_buffer_size(args);
	//cerr << "size: " << size << endl;
	if (size == 0) {
		cerr << "ERROR: Invalid buffer size" << endl;
		return EXIT_FAILURE;
	} else {
		try {
			pass_through_io(size, stdin, stdout);
		} catch (bad_alloc) {
			cerr << "ERROR: attempted to allocate too much memory." << endl;
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
}
