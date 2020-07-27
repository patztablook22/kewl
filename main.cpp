/*
 * Konverzace Everybody Will Like client
 * C++ IRC with SSL encryption
 * "lol imma waste my time" ~ patz, d6022
 */

#define VERSION 90	// editing diz may cause unexpected behaviour

// needed libz:
// #include "libz/ur_package_manager.hpp"

// extern scriptz:
#include "core.hpp"		// io, exec, perform, serv, cmdz, funcz


int main(int argc, const char *argv[])
{
	std::wstring arg;
	for (int i = 1; i < argc; i++) {
		if (i != 1)
			arg += L' ';
		std::string tmp(argv[i]);
		arg += std::wstring(tmp.begin(), tmp.end());
	}
	if (arg.size() > 0) {
		std::string tmp;
		switch (arg[0]) {
		case L'-':
			tmp = argv[0];
			core::perform << std::wstring(tmp.begin(), tmp.end()) + L' ' + arg;
			arg.clear();
			break;
		}
	}
	core::io.init();
	core::exec.macroz.init();
	core::exec.usr << arg;

	std::wstring input;
	for (;;) {
		core::io >> input;
		if (core::io.trim(input)[0] == '/')
			core::exec.usr << input.substr(1, input.size() - 1);
		else
			try {
				core::serv << core::io::msg(core::serv.status.gnick(), input);
			} catch (int) {
				core::serv.disconn(false);
			}
	}
	return 0;
}
