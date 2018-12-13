#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <string>
#include <cstdio>

namespace Debug {
	template<typename... Args>
	static void info(std::string fmt, Args... args) {
		std::fprintf(stdout, (fmt + "\n").c_str(), args...);
		fflush(stdout);
	}

	template<typename... Args>
	static void warning(std::string fmt, Args... args) {
		std::fprintf(stdout, ("Warning: " + fmt + "\n").c_str(), args...);
		fflush(stdout);
	}

	template<typename... Args>
	static void error(std::string fmt, Args... args) {
		fprintf(stderr, ("Error: " + fmt + "\n").c_str(), args...);
		fflush(stderr);
	}

	template<typename... Args>
	static void fatalError(std::string fmt, Args... args) {
		fprintf(stderr, ("Fatal Error: " + fmt + "\n").c_str(), args...);
		fflush(stderr);
		exit(EXIT_FAILURE);
	}
};

#endif