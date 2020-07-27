/*
 * Konverzace Everybody Will Like client
 * C++ IRC with SSL encryption
 * "lol imma waste my time" ~ patz, d6022
 */

#define VERSION 98	// editing diz may cause unexpected behaviour

// needed libz:
// #include "libz/ur_package_manager.hpp"

// extern scriptz:
#include "core.hpp"		// io, exec, cmdl, serv, cmdz, optz


int main(int argc, const char *argv[])
{
	std::wstring input;
	/* work with cmd line argumentz */
	for (int i = 1; i < argc; i++) {
		bool opt = false;
		std::string tmp(argv[i]);
		std::wstring res(tmp.begin(), tmp.end());
		if (res.size() == 0)
			continue;
		if (res.size() > 2 && res.substr(0, 2) == L"--" && !input.size()) {
			res.erase(0, 2);
			opt = true;
		}
		core::exec.escape({res}, res);
		if (opt) {
			tmp = argv[0];
			std::wstring self(tmp.begin(), tmp.end());
			core::exec.escape({self}, self);
			core::cmdl << res + L" " + self;
		} else {
			if (input.size() != 0)
				input+= L' ';
			input += res;
		}
	}

	signal(SIGABRT, exit);
	signal(SIGQUIT, exit);
	signal(SIGINT, exit);
	signal(SIGHUP, exit);
	signal(SIGTERM, exit);
	signal(SIGPIPE, SIG_IGN);
	
	core::io.init();
	core::exec.macroz.init();
	core::exec.usr << input;

	/* prompt for input */
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
