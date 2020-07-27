/*
 * Konverzace Everybody Will Like client
 * C++ IRC with SSL encryption
 * "lol imma waste my time" ~ patz, d6022
 */

#define VERSION 92	// editing diz may cause unexpected behaviour

// needed libz:
// #include "libz/ur_package_manager.hpp"

// extern scriptz:
#include "core.hpp"		// io, exec, cmdl, serv, cmdz, funcz


int main(int argc, const char *argv[])
{
	std::wstring input;
	/* work with cmd line argumentz */
	{
		int tmp;
		for (int i = 0; i < argc; i++) {
			int j;
			input += L'"';
			for (j = 0; j < std::string(argv[i]).size(); j++) {
				switch (argv[i][j]) {
				case L'\\':
					input += L"\\\\";
					break;
				case L'"':
					input += L"\\\"";
					break;
				default:
					input += argv[i][j];
					break;
				}
			}
			input += L"\" ";
			if (i == 0)
				tmp = j + 3;

		}

		if (input.size() > tmp && input.substr(tmp, 3) == L"\"--") {
			input.erase(tmp + 1, 2);
			core::cmdl << input;
			input.clear();
		}
		input.erase(0, tmp);
		core::io.init();
		core::exec.macroz.init();
		core::exec.usr << input;
	}

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
