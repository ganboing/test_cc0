#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <iostream>
#include <exception>
#include <stdexcept>

int main(int argc, char** argv) {
	bool cc0_debug = false;
	const char* cc0_input = NULL;
	const char* cc0_output = NULL;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
			if (argv[i + 1] != NULL && *argv[i + 1] != '-') {
				cc0_output = argv[++i];
			} else {
				cc0_output = "a.bin";
			}
		} else if ((strcmp(argv[i], "--debug") == 0)
				|| (strcmp(argv[i], "-g") == 0)) {
			cc0_debug = true;
		} else if ((strcmp(argv[i], "-c") == 0)) {
			//cc0_compile_only = true;
			::std::cerr << "should not be compiling!!\n";
			return -1;
		} else if ((strcmp(argv[i], "--help") == 0)
				|| strcmp(argv[i], "-h") == 0) {
			::std::cerr << "should not be printing help!!\n";
			return -1;
		} else {
			if (cc0_input != NULL) {
				::std::cerr << "multiple input files applied!!\n";
				return -1;
			}
			cc0_input = argv[i];
		}
	}

	if (cc0_input == NULL) {
		::std::cerr << "invalid argument!!\n";
		return -1;
	} else {
		struct timespec current_time;
		clock_gettime(CLOCK_REALTIME, &current_time);
		::std::stringstream tmp_file_name_impl;
		tmp_file_name_impl << "/tmp/cc0_test_tmp_" << getpid() << "_"
				<< current_time.tv_nsec;
		::std::string tmp_file = tmp_file_name_impl.str();
		::std::stringstream il_gen_arg_impl;
		::std::stringstream il_cc_arg_impl;
		il_gen_arg_impl << "il_gen -o " << tmp_file << " " << cc0_input;
		if (cc0_output == NULL) {
			il_cc_arg_impl << "il_cc " << tmp_file;
		} else {
			il_cc_arg_impl << "il_cc -o " << cc0_output << " " << tmp_file;
		}
		if (cc0_debug) {
			il_gen_arg_impl << " -g";
			il_cc_arg_impl << " -g";
		}
		::std::string il_gen_arg = il_gen_arg_impl.str();
		::std::string il_cc_arg = il_cc_arg_impl.str();
		::std::cout << "invoking:\n" << il_gen_arg << "\n" << il_cc_arg << "\n";
		int il_gen_status = system(il_gen_arg.c_str());
		if (il_gen_status) {
			::std::cerr << "il_gen failed\n";
			return il_gen_status;
		}
		int il_cc_status = system(il_cc_arg.c_str());
		if (il_cc_status) {
			::std::cerr << "il_cc failed\n";
			return il_cc_status;
		}
		return 0;
	}
}
