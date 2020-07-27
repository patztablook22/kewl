class quit: public core::exec::cmd {
public:
	quit()
	{
		core::exec.add(L"quit", L"<null>", L"quit kewl", this);
	}

	int usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 2;
		exit(EXIT_SUCCESS);
	}
} quit;

class help: public core::exec::cmd {
public:
	help()
	{
		core::exec.add(L"help", L"[cmd]", L"get some help", this, L"/");
	}

	int usr(std::vector<std::wstring> arg)
	{
		std::vector<std::wstring> cmdz;
		core::exec.gcmdz(cmdz);
		int max_x, max_y;
		getmaxyx(stdscr, max_y, max_x);
		if (arg.size() == 1) {
			int ipl = (max_x - 8) / 16;
			if (ipl <= 0)
				ipl = 1;
			std::wstring tmp;
			core::io << core::io::msg(L"kewl", L"HELP:" + std::wstring(L"-------------").substr(0, max_x - 13));
			for (int i = 0; i < cmdz.size(); i++) {
				if (i % ipl) {
					tmp += std::wstring(16 - cmdz[i - 1].size(), 32);
				} else if (i != 0) {
					core::io << core::io::msg(L"kewl", tmp);
					tmp.clear();
				}
				tmp += cmdz[i];
			}
			core::io << core::io::msg(L"kewl", tmp);
			core::io << core::io::msg(L"kewl", std::wstring(L"------------------").substr(0, max_x - 8));
		} else if (arg.size() == 2) {
			if (arg[1][0] == '/')
				arg[1].erase(0, 1);
			if (!core::exec.iz_cmd(arg[1])) {
				core::io << core::io::msg(L"kewl", L"ERR: no such command: \"" + arg[1] + L"\"");
				return 1;
			}
			core::io << core::io::msg(L"kewl", L"HELP:" + std::wstring(L"-------------").substr(0, max_x - 13));
			core::io << core::io::msg(L"kewl", L"SYNOPSIS: " + arg[1] + L" " + core::exec.gsyn(arg[1]));
			core::io << core::io::msg(L"kewl", L"DESCRIPTION: " + core::exec.gdesc(arg[1]));
			core::io << core::io::msg(L"kewl", std::wstring(L"------------------").substr(0, max_x - 8));
		} else {
			return 2;
		}
		return 0;
	}
} help;

class cls: public core::exec::cmd {
public:
	cls()
	{
		core::exec.add(L"cls", L"<null>", L"clear screen", this);
	}

	int usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 2;
		core::io.cls();
		core::io << core::io::msg(L"kewl", L"da screen has been cleared");
		return 0;
	}
} cls;

class conn: public core::exec::cmd {
public:
	conn()
	{
		core::exec.add(L"conn", L"<address>:<port> <nick0>,[...],[nick15]", L"connect to kewl serv", this);
	}

	int usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 3)
			return 2;
		int pos = arg[1].find_last_of(':');
		if (pos == std::wstring::npos || pos == 0 || pos == arg[1].size() - 1)
			return 2;
		std::string hostname(arg[1].begin(), arg[1].begin() + pos);
		
		int port = 0;
		try {
			port = std::stoi(arg[1].substr(pos + 1, arg[1].size() - pos - 1));
		} catch (std::invalid_argument) {
			return 2;
		}
		if (arg[1].substr(pos + 1, arg[1].size() - pos - 1) !=  std::to_wstring(port))
			return 2;
		if (port <= 0 || port > 65535)
			return 2;
		std::wstring usrz = arg[2];
		pos = arg[2].find(',');
		if (pos == std::wstring::npos)
			pos = arg[2].size();
		for (int i = 1; arg[2].size() > 0; i++) {
			if (pos > 15 || pos < 3 || !core::io.iz_k(arg[2].substr(0, pos)))
				return 2;
			if (arg[2].substr(0, pos) == L"kewl" || arg[2].substr(0, pos) == L"serv") {
				core::io << core::io::msg(L"kewl", L"ERR: u can not use kewl/serv as ur nick");
				return 1;
			}
			if (arg[2].size() == pos + 1)
				return 2;
			arg[2].erase(0, pos + 1);
			if (i == 16)
				return 2;
			pos = arg[2].find(',');
			if (pos == std::wstring::npos)
				pos = arg[2].size();
		}
		core::serv.conn(hostname, std::to_string(port), usrz);
		return 0;
	}
} conn;

class disconn: public core::exec::cmd {
public:
	disconn()
	{
		core::exec.add(L"disconn", L"<null>", L"disconnect from kewl serv", this);
	}

	int usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 2;
		return core::serv.disconn();
	}

	int serv(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 2;
		core::io << core::io::msg(L"serv", L"imma shut down...");
		core::io.beep();
		return core::serv.disconn();
	}
private:
} disconn;

class usrz: public core::exec::cmd {
public:
	usrz()
	{
		core::exec.add(L"usrz", L"<null>", L"list all usrz on serv", this);
	}

	int usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 2;
		if (core::serv.status.gnick().size() == 0) {
			core::io << core::io::msg(L"kewl", L"ERR: no connection alive");
			return 1;
		}
		int max_x, max_y;
		getmaxyx(stdscr, max_y, max_x);
		int ipl = (max_x - 8) / 16;
		if (ipl <= 0)
			ipl = 1;
		std::vector<std::wstring> usrz;
		core::serv.status.gotherz(usrz);
		usrz.insert(usrz.begin(), core::serv.status.gnick());
		core::io << core::io::msg(L"serv", L"USRZ:" + std::wstring(L"-------------").substr(0, max_x - 13));
		std::wstring tmp;
		if (usrz.size() == 0) {
			core::serv.disconn(false);
			return 2;
		}
		for (int i = 0; i < usrz.size(); i++) {
			if (usrz.size() == 0)
				exit(1);
			if (i % ipl) {
				tmp += std::wstring(16 - usrz[i - 1].size(), 32);
			} else if (i != 0) {
				core::io << core::io::msg(L"serv", tmp);
				tmp.clear();
			}
			tmp += usrz[i];
		}
		core::io << core::io::msg(L"serv", tmp);
		core::io << core::io::msg(L"serv", std::wstring(L"------------------").substr(0, max_x - 8));

		return 0;
	}

	int serv(std::vector<std::wstring> arg)
	{
		if (arg.size() != 3)
			return 2;
		if (arg[1] == L"conn") {
			core::serv.status.join(arg[2]);
			core::io << core::io::msg(L"serv", arg[2] + L" connected");
		} else if (arg[1] == L"disconn") {
			core::serv.status.leave(arg[2]);
			core::io << core::io::msg(L"serv", arg[2] + L" disconnected");
		}
		core::io.beep();
		return 0;
	}
} usrz;

class ping: public core::exec::cmd {
public:
	ping()
	:in_use(false)
	{
		core::exec.add(L"ping", L"<null>", L"get ping on current server", this);
	}

	int usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 2;
		if (!core::serv.status.gactive()) {
			core::io << core::io::msg(L"kewl", L"ERR: no connection alive");
			return 1;
		}
		if (in_use) {
			core::io << core::io::msg(L"kewl", L"ERR: ping request already sent");
			return 1;
		}
		in_use = true;
		core::io << core::io::msg(L"kewl", L"ping request sent");
		gettimeofday(&tm, NULL);
		core::serv << core::io::msg(L"kewl", L"/ping");
		
		return 0;
	}

	int serv(std::vector<std::wstring> arg)
	{
		timeval tmp;
		gettimeofday(&tmp, NULL);
		double ms = 1000000 * (tmp.tv_sec - tm.tv_sec) + tmp.tv_usec - tm.tv_usec;
		ms /= 1000;
		int round0 = (ms - (int)ms) * 100;
		if (round0 % 10 >= 5)
			round0 += 10;
		round0 /= 10;
		core::io << core::io::msg(L"kewl", L"ping-back received; time ~ " + std::to_wstring((int)ms) + L'.' + std::to_wstring(round0) + L"ms");
		in_use = false;
		return 0;
	}
private:
	timeval tm;
	bool in_use;
} ping;

class version: public core::exec::cmd {
public:
	version()
	{
		core::exec.add(L"version", L"<null>", L"print current version", this);
	}

	int usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 2;
		core::io << core::io::msg(L"kewl", L"current version: " + core::io.ver_echo(VERSION));
		return 0;
	}
} version;

class about: public core::exec::cmd {
public:
	about()
	{
		core::exec.add(L"about", L"<null>", L"wtf iz kewl about", this);
	}

	int usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 2;
		int max_x, max_y;
		getmaxyx(stdscr, max_y, max_x);
		core::io << core::io::msg(L"kewl", L"ABOUT:" + std::wstring(L"------------").substr(0, max_x - 14));
		core::io << core::io::msg(L"kewl", L"Konverzace Everybody Will Like client");
		core::io << core::io::msg(L"kewl", L"C++ IRC with SSL encryption");
		core::io << core::io::msg(L"kewl", L"\"lol imma waste my time\" ~ patz, d6022");
		core::io << core::io::msg(L"kewl", std::wstring(L"------------------").substr(0, max_x - 8));
		return 0;
	}
} about;

class mute: public core::exec::cmd {
public:
	mute()
	{
		core::exec.add(L"mute", L"<nick>", L"mute/unmute incoming msgz from \"nick\"", this, L"@");
	}

	int usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 2;

		return 0;
	}

} mute;

class beep: public core::exec::cmd {
public:
	beep()
	{
		core::exec.add(L"beep", L"<{test,on,off,set delay_in_ms}>", L"test / turn on/off / set delay of beep", this);
	}

	int usr(std::vector<std::wstring> arg)
	{
		if (arg.size() < 2)
			return 2;
		if (arg[1] == L"test") {
			if (arg.size() != 2)
				return 2;
			core::io.beep(true);
		} else if (arg[1] == L"on") {
			if (arg.size() > 2)
				return 2;
			if (core::io.beep_gon()) {
				core::io << core::io::msg(L"kewl", L"ERR: beep already iz enabled");
				return 1;
			}
			core::io.beep_son(true);
			core::io << core::io::msg(L"kewl", L"beep enabled");
		} else if (arg[1] == L"off") {
			if (!core::io.beep_gon()) {
				core::io << core::io::msg(L"kewl", L"ERR: beep already iz disabled");
				return 1;
			}
			core::io.beep_son(false);
			core::io << core::io::msg(L"kewl", L"beep disabled");
		} else if (arg[1] == L"set") {
			if (arg.size() != 3)
			return 2;
			unsigned int tmp;
			try {
				tmp = std::stoi(arg[2]);
			} catch (std::invalid_argument) {
				return 2;
			} catch (std::out_of_range) {
				core::io << core::io::msg(L"kewl", L"ERR: value iz 2 high");
				return 1;
			}
			if (arg[2] != std::to_wstring(tmp))
				return 2;
			if (tmp < 0) {
				core::io << core::io::msg(L"kewl", L"ERR: beep delay must be nonnegative");
				return 1;
			}
			if (core::io.beep_gdelay()  == tmp) {
				core::io << core::io::msg(L"kewl", L"ERR: same as current value");
				return 1;
			}
			core::io.beep_sdelay(tmp);
			core::io << core::io::msg(L"kewl", L"beep set to " + arg[2] + L"ms");
		} else {
			return 2;
		}

		return 0;
	}
} beep;
