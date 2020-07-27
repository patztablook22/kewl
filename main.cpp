/*
 * Konverzace Everybody Will Like client
 * C++ IRC
 * "lol imma waste my time" ~ patz, d6022
 */

#define VERSION 85	// editing diz may cause unexpected behaviour

// needed libz:
// #include "libz/ur_package_manager.hpp"

// extern scriptz:
#include "core.hpp"		// io, exec, serv, cmdz


int main(int argc, const char *argv[])
{
	std::string arg;
	for (int i = 1; i < argc; i++) {
		if (i != 1)
			arg += " ";
		arg += argv[i];
	}
	core::exec.usr << std::wstring(arg.begin(), arg.end());

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
