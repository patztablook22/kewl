class quit: public core::exec::cmd {
public:
	quit()
	{
		core::exec.add(L"quit", {{L"", L"quit kewl"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 1;
		exit(EXIT_SUCCESS);
	}
} quit;

class help: public core::exec::cmd {
public:
	help()
	{
		core::exec.add(L"help", {{L"", L"get list of cmdz"}, {L"cmd", L"get help 4 da cmd"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		std::vector<std::wstring> cmdz;
		core::exec.gcmdz(cmdz);
		int max_x, max_y;
		getmaxyx(stdscr, max_y, max_x);
		if (arg.size() == 1) {
			int ipl = max_x - 1;
			if (core::io.ui.gint(L"int_usr_max_padding") > 4)
				ipl -= core::io.ui.gint(L"int_usr_max_padding");
			else
				ipl -= 4;
			ipl -= core::io.ui.gstr(L"txt_usr_before").size() + core::io.ui.gstr(L"txt_usr_after").size() + 5 * core::io.ui.gtru(L"bool_show_time");
			ipl /= 16;
			if (ipl <= 0)
				ipl = 1;
			std::wstring tmp;
			core::io << core::io::msg(L"hr", L"");
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
			core::io << core::io::msg(L"hr", L"");
		} else if (arg.size() == 2) {
			if (arg[1].size() == 0)
				return 1;
			if (arg[1].size() > 15) {
				core::io << core::io::msg(L"kewl", L"ERR: cmdz hav limited size to 15 charz");
				return -1;
			}
			if (!core::exec.iz_cmd(arg[1])) {
				core::io << core::io::msg(L"kewl", L"ERR: no such command: \"" + arg[1] + L"\"");
				return -1;
			}
			std::vector<std::vector<std::wstring>> man;
			core::exec.gman(arg[1], man);
			if (man.size() != 0) {
				core::io << core::io::msg(L"hr", L"");
				for (int i = 0; i < man.size(); i++) {
					for (int j = 0; j < man[i].size(); j++) {
						core::io << core::io::msg(L"kewl", (j == 0 ?  arg[1] : L"->") + L' ' + man[i][j]);
					}
				}
				core::io << core::io::msg(L"hr", L"");
			} else {
				core::io << core::io::msg(L"kewl", L"no manual 4 \"" + arg[1] + L"\" given :c");
			}
		} else {
			return 2;
		}
		return 0;
	}

	void acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
	{
		if (arg.size() != 1)
			return;
		core::exec.gcmdz(trg);
	}
} help;

class cls: public core::exec::cmd {
public:
	cls()
	{
		core::exec.add(L"cls", {{L"", L"clear screen"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 1;
		core::io.cls();
		return 0;
	}
} cls;

class erase: public core::exec::cmd {
public:
	erase()
	{
		core::exec.add(L"erase", {{L"", L"erase all messages and print kewl logo etc."}}, this);
	}
	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 1;
		core::io.da_erase();
		core::io.mkwin();
		return 0;
	}
} erase;

class reset: public core::exec::cmd {
public:
	reset()
	{
		core::exec.add(L"reset", {{L"", L"reset ui"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 1;
		endwin();
		core::io.init(false);
		return 0;
	}
} reset;

class conn: public core::exec::cmd {
public:
	conn()
	{
		core::exec.add(L"conn", {{L"address:port nick", L"connect 2 kewl serv", L"using port 12204, if not specified", L"using ur usrname, if not specified"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() < 2)
			return 1;
		else if (arg.size() > 3)
			return 3;
		int pos = arg[1].find_last_of(':');
		if (pos == 0 || pos == arg[1].size() - 1)
			return 1;
		uint16_t port = 0;
		if (pos != std::wstring::npos) {
			try {
				port = std::stoi(arg[1].substr(pos + 1, arg[1].size() - pos - 1));
			} catch (std::invalid_argument) {
				return 1;
			}
			if (std::to_wstring(port) != arg[1].substr(pos + 1, arg[1].size() - pos - 1))
				return 1;
			if (port < 1 || port > 65535)
				return 1;
		} else {
			pos = arg[1].size();
		}
		std::wstring da_hostname = arg[1].substr(0, pos);
		std::string hostname(da_hostname.begin(), da_hostname.end());
		std::wstring usr;
		if (arg.size() == 3) {
			usr = arg[2];
		}
		core::serv.conn(hostname, port, usr);
		return 0;
	}
} conn;

class disconn: public core::exec::cmd {
public:
	disconn()
	{
		core::exec.add(L"disconn", {{L"", L"disconnect from kewl serv"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 1;
		try {
			core::serv.snd(L"/disconn");
		} catch (...) {
			core::serv.disconn();
		}
		return 0;
	}

	uint8_t serv(std::vector<std::wstring> arg)
	{
		if (arg.size() == 1)
			return core::serv.disconn(false);
		if (arg[1] == L"s")
			core::io << core::io::msg(L"serv", L"imma shutdown");
		else if (arg[1] == L"k")
			core::io << core::io::msg(L"serv", L"u were kicked out :'(");
		if (arg.size() == 3)
			core::io << core::io::msg(L"serv", L"@" + core::serv.status.gnick() + L" " + arg[2]);
		core::io.beep();
		return core::serv.disconn();
	}
private:
} disconn;

class usrz: public core::exec::cmd {
private:
	void usrz_list(std::wstring nicc, std::vector<std::wstring> &list)
	{
		if (list.size() == 0) {
			core::io << core::io::msg(L"kewl", L"*list iz empty*");
			return;
		}
		int max_x, max_y;
		getmaxyx(stdscr, max_y, max_x);
		int ipl = max_x - 1;
		if (core::io.ui.gint(L"int_usr_max_padding") > 4)
			ipl -= core::io.ui.gint(L"int_usr_max_padding");
		else
			ipl -= 4;
		ipl -= core::io.ui.gstr(L"txt_usr_before").size() + core::io.ui.gstr(L"txt_usr_after").size() + 5 * core::io.ui.gtru(L"bool_show_time");
		ipl /= 18;
		if (ipl <= 0)
			ipl = 1;
		core::io << core::io::msg(L"hr", L"");
		std::wstring tmp;
		for (int i = 0; i < list.size(); i++) {
			if (i % ipl) {
				tmp += L' ';
			} else if (i != 0) {
				core::io << core::io::msg(nicc, tmp);
				tmp.clear();
			}
			tmp += L"\\4[\\0" + list[i] + std::wstring(15 - list[i].size(), 32) + L"\\4]\\0";
		}
		core::io << core::io::msg(nicc, tmp);
		core::io << core::io::msg(L"hr", L"");
	};
public:
	usrz()
	{
		core::exec.add(L"usrz", {{L""}, {L"list", L"print all usrz on da server (default option)"}, {L"whoiz nicc", L"get info about usr \"nicc\""}, {L"ignore add/remove nicc", L"ignore/unignore \"nicc\""}, {L"clear", L"clear da ignore list"}, {L"ignore list", L"list all ignored usrz"}, {L"register", L"register ur actual nick on da server"}, {L"chpasswd", L"change ur passwd (if ur registered) on da server"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() == 1 || arg[1] == L"list") {
			if (arg.size() > 2)
				return 2;
			if (core::serv.status.gnick().size() == 0) {
				core::io << core::io::msg(L"kewl", L"ERR: no connection alive");
				return -1;
			}
			std::vector<std::wstring> usrz;
			core::serv.status.gotherz(usrz);
			usrz.insert(usrz.begin(), core::serv.status.gnick());
			usrz_list(L"serv", usrz);
		} else if (arg[1] == L"whoiz") {
			if (arg.size() < 3)
				return 2;
			else if (arg.size() > 3)
				return 3;
			if (arg[2].size() < 3 || arg[2].size() > 15) {
				core::io << core::io::msg(L"kewl", L"ERR: nicc size must be between 3 and 15 charz");
				return -1;
			}
			if (!core::serv.status.gactive()) {
				core::io << core::io::msg(L"kewl", L"ERR: no connection alive");
				return -1;
			}
			core::serv.snd(L"/whoiz \"" + arg[2] + L'"');
		} else if (arg[1] == L"ignore") {
			if (arg.size() < 3)
				return 2;
			if (arg[2] == L"add") {
				if (arg.size() < 4)
					return 3;
				else if (arg.size() > 4)
					return 4;
				if (arg[3].size() == 0)
					return 3;
				if (arg[3] == core::serv.status.gnick())
					core::io << core::io::msg(L"kewl", L"WARN: ignoring urself haz no effect");
				switch (core::serv.ignore.add(arg[3])) {
				case 0:
					core::io << core::io::msg(L"kewl", L"ignoring msgz from \"" + arg[3] + L'"');
					break;
				case 1:
					core::io << core::io::msg(L"kewl", L"WARN: ur already ignoring diz usr");
					return -1;
				case 2:
					core::io << core::io::msg(L"kewl", L"ERR: nicc size must be between 3 and 15 charz");
					return -1;
				case 3:
					core::io << core::io::msg(L"kewl", L"ERR: it refused!");
					return -1;
				}
			} else if (arg[2] == L"remove") {
				if (arg.size() < 4)
					return 3;
				else if (arg.size() > 4)
					return 4;
				if (arg[3].size() == 0)
					return 3;
				if (core::serv.ignore.remove(arg[3])) {
					core::io << core::io::msg(L"kewl", L"WARN: ur not ignoring diz usr");
					return -1;
				}
				core::io << core::io::msg(L"kewl", L"stopped ignoring msgz from \"" + arg[3] + L'"');
			} else if (arg[2] == L"clear") {
				if (arg.size() > 3)
					return 3;
				if (core::serv.ignore.clear()) {
					core::io << core::io::msg(L"kewl", L"WARN: ignore list iz empty");
					return -1;
				}
				core::io << core::io::msg(L"kewl", L"ignore list cleared");
			} else if (arg[2] == L"list") {
				if (arg.size() > 3)
					return 3;
				std::vector<std::wstring> ignored;
				core::serv.ignore.glist(ignored);
				usrz_list(L"kewl", ignored);
			} else {
				return 2;
			}
		} else if (arg[1] == L"register") {
			if (arg.size() > 2)
				return 2;
			if (core::serv.status.gnick().size() == 0) {
				core::io << core::io::msg(L"kewl", L"ERR: no connection alive");
				return -1;
			}
			try {
				core::serv.snd(L"/register");
				core::io << core::io::msg(L"kewl", L"registration request sent");
			} catch (...) {}
		} else if (arg[1] == L"chpasswd") {
			if (arg.size() > 2)
				return 2;
			if (core::serv.status.gnick().size() == 0) {
				core::io << core::io::msg(L"kewl", L"ERR: no connection alive");
				return -1;
			}
			try {
				core::serv.snd( L"/chpasswd");
				core::io << core::io::msg(L"kewl", L"passwd change request sent");
			} catch (...) {}
		} else {
			return 1;
		}

		return 0;
	}

	void acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
	{
		switch (arg.size()) {
		case 1:
			trg = {L"chpasswd", L"ignore", L"list", L"register", L"whoiz"};
			break;
		case 2:
			if (arg[1] == L"ignore")
				trg = {L"add", L"clear", L"list", L"remove"};
			else if (arg[1] == L"whoiz")
				core::serv.status.gotherz(trg);
			break;
		case 3:
			if (arg[1] != L"ignore")
				return;
			if (arg[2] == L"add") {
				core::serv.status.gotherz(trg);
				for (int i = 0; i < trg.size(); i++)
					if (core::serv.ignore.ignored(trg[i]))
						trg.erase(trg.begin() + i, trg.begin() + i + 1);
			} else if (arg[2] == L"remove") {
				core::serv.ignore.glist(trg);
			}
			break;
		}
	}

	uint8_t serv(std::vector<std::wstring> arg)
	{
		if (arg.size() < 2)
			return 2;
		if (arg[1] == L"conn") {
			if (arg.size() != 3)
				return 2;
			core::serv.status.join(arg[2]);
			core::io << core::io::msg(L"serv", L"\\3" + arg[2] + L"\\0 connected");
		} else if (arg[1] == L"disconn") {
			if (arg.size() != 3)
				return 2;
			core::serv.status.leave(arg[2]);
			core::io << core::io::msg(L"serv", L"\\3" + arg[2] + L"\\0 disconnected");
		} else if (arg[1] == L"lost") {
			if (arg.size() != 3)
				return 2;
			core::serv.status.leave(arg[2]);
			core::io << core::io::msg(L"serv", L"connection with \\3" + arg[2] + L"\\0 lost");
		} else if (arg[1] == L"timeo") {
			if (arg.size() != 3)
				return 2;
			core::serv.status.leave(arg[2]);
			core::io << core::io::msg(L"serv", L"\\3" + arg[2] + L"\\0 timed out");
		} else if (arg[1] == L"kick") {
			if (arg.size() != 3)
				return 2;
			core::serv.status.leave(arg[2]);
			core::io << core::io::msg(L"serv", L"\\3" + arg[2] + L"\\0 was kicked out");
		} else if (arg[1] == L"omg") {
			if (arg.size() != 5)
				return 2;
			if (arg[2].size() == 0 || arg[3].size() != 1)
				return 2;
			core::io << core::io::msg(L"serv", L"\\3" + arg[2] + L"\\0'z omg \"\\1" + arg[3] + L"\\0\" haz now value of \\1" + arg[4]);
			if (arg[2] == core::serv.status.gnick()) {
				try {
					int tmp = std::stoi(arg[4]);
					if (tmp < 0 || tmp > 2)
						return 2;
					core::serv.snd(L"/omg_f5");
					core::serv.status.chomg(arg[3][0], tmp);
				} catch (...) {
					return 1;
				}
			}
		} else if (arg[1] == L"register") {
			if (arg.size() != 3)
				return 2;
			core::io << core::io::msg(L"kewl", (arg[2] == L"0") ? L"serv requests \\1new usr\\0 passwd" : L"serv requests \\1control\\0 passwd");
			core::io.send_passwd();
		} else if (arg[1] == L"chpasswd") {
			if (arg.size() != 3 || arg[2].size() == 0)
				return 2;
			switch (arg[2][0]) {
			case L'0':
				core::io << core::io::msg(L"kewl", L"serv requests \\1auth\\0 passwd");
				break;
			case L'1':
				core::io << core::io::msg(L"kewl", L"serv requests \\1new usr\\0 passwd");
				break;
			case L'2':
				core::io << core::io::msg(L"kewl", L"serv requests \\1control\\0 passwd");
				break;
			}
			core::io.send_passwd();
		} else {
			return 2;
		}

		core::io.beep();
		return 0;
	}
} usrz;

class ping: public core::exec::cmd {
public:
	ping()
	{
		core::exec.add(L"ping", {{L"no_of_triez", L"4 by default, between 1 and 64", L"get ping on current server"}, {L"cancel", L"cancel pinging process"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() > 2)
			return 2;
		uint8_t n = 4;
		if (arg.size() == 2) {
			if (arg[1] == L"cancel") {
				if (core::serv.ping.cancel() != 0) {
					core::io << core::io::msg(L"kewl", L"ERR: not pinging");
					return -1;
				}
				return 0;				
			}
			try {
				n = std::stoi(arg[1]);
			} catch (std::invalid_argument) {
				return 1;
			}
			if (std::to_wstring(n) != arg[1])
				return 1;
		}
		switch (core::serv.ping.do_it(n)) {
		case 0:
			break;
		case 1:
			core::io << core::io::msg(L"kewl", L"ERR: no connection alive");
			return -1;
		case 2:
			core::io << core::io::msg(L"kewl", L"ERR: must be between 1 and 64");
			return -1;
		case 3:
			core::io << core::io::msg(L"kewl", L"WARN: already pinging");
			return -1;
		}
		return 0;
	}

	uint8_t serv(std::vector<std::wstring> arg)
	{
		if (arg.size() != 3)
			return 2;
		uint8_t tmp;
		try {
			tmp = std::stoi(arg[2]);
		} catch (std::invalid_argument) {
			return 2;
		}
		if (std::to_wstring(tmp) != arg[2])
			return 2;
		core::serv.ping.recv(tmp);
		return 0;
	}

	void acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
	{
		if (arg.size() != 1)
			return;
		trg = {L"4", L"cancel"};
	}
} ping;

class version: public core::exec::cmd {
public:
	version()
	{
		core::exec.add(L"version", {{L"", L"print current version"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 1;
		core::io << core::io::msg(L"kewl", L"current version | \\1" + core::io.ver_echo(VERSION));
		std::string tmp(__DATE__);
		tmp += ' ';
		tmp += __TIME__;
		core::io << core::io::msg(L"kewl", L"    compiled on | \\1" + std::wstring(tmp.begin(), tmp.end()));
		
		return 0;
	}
} version;

class about: public core::exec::cmd {
public:
	about()
	{
		core::exec.add(L"about", {{L"", L"wtf iz kewl about"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 1;
		int max_x, max_y;
		getmaxyx(stdscr, max_y, max_x);
		core::io << core::io::msg(L"hr", L"");
		core::io << core::io::msg(L"kewl", L"Konverzace Everybody Will Like client");
		core::io << core::io::msg(L"kewl", L"C++ IRC with SSL encryption");
		core::io << core::io::msg(L"kewl", L"\"lol imma waste my time\" ~ patz, d6022");
		core::io << core::io::msg(L"hr", L"");
		return 0;
	}
} about;

class beep: public core::exec::cmd {
public:
	beep()
	{
		core::exec.add(L"beep", {{L"test", L"test da beep"}, {L"on/off", L"turn da beep on/off"}, {L"set delay_in_ms", L"set minimal delay between 2 beepz"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() < 2)
			return 1;
		if (arg[1] == L"test") {
			if (arg.size() != 2)
				return 2;
			core::io.beep(true);
		} else if (arg[1] == L"on") {
			if (arg.size() != 2)
				return 2;
			if (core::io.beep_gon()) {
				core::io << core::io::msg(L"kewl", L"WARN: beep already iz enabled");
				return -1;
			}
			core::io.beep_son(true);
			core::io << core::io::msg(L"kewl", L"beep enabled");
		} else if (arg[1] == L"off") {
			if (!core::io.beep_gon()) {
				core::io << core::io::msg(L"kewl", L"WARN: beep already iz disabled");
				return -1;
			}
			core::io.beep_son(false);
			core::io << core::io::msg(L"kewl", L"beep disabled");
		} else if (arg[1] == L"set") {
			if (arg.size() < 3)
				return 2;
			else if (arg.size() > 3)
				return 3;
			unsigned int tmp;
			try {
				tmp = std::stoi(arg[2]);
			} catch (std::invalid_argument) {
				return 2;
			} catch (std::out_of_range) {
				core::io << core::io::msg(L"kewl", L"ERR: value iz 2 high");
				return -1;
			}
			if (arg[2] != std::to_wstring(tmp))
				return 2;
			if (core::io.beep_gdelay()  == tmp) {
				core::io << core::io::msg(L"kewl", L"WARN: same as current value");
				return -1;
			}
			core::io.beep_sdelay(tmp);
			core::io << core::io::msg(L"kewl", L"beep delay set to \\1" + arg[2] + L"ms");
		} else {
			return 1;
		}

		return 0;
	}

	void acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
	{
		if (arg.size() != 1)
			return;
		trg = {L"on", L"off", L"set", L"test"};
	}
} beep;

class hr: public core::exec::cmd {
public:
	hr()
	{
		core::exec.add(L"hr", {{L"", L"print horizontal line (only if da last msg iz not hr 2)"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 1;
		core::io << core::io::msg(L"hr", L"");
		return 0;
	}
} hr;

class ui: public core::exec::cmd {
private:
	std::wstring property_query(std::wstring name)
	{
		int pos = name.find(L'_');
		if (pos == std::wstring::npos || pos == 0 || pos == name.size() - 1)
			return L"";
		std::wstring type = name.substr(0, pos);
		if (type == L"attr") {
			std::vector<std::wstring> res;
			core::io.ui.gattrz(name, res);
			if (res.size() != 3)
				return L"???";
			return L'"' + res[0] + L"\", \"" + res[1] + L"\", \"" + res[2] + L'"';
		} else if (type == L"txt") {
			return L'"' + core::io.ui.gstr(name) + L'"';
		} else if (type == L"char") {
			return L'"' + std::wstring(1, core::io.ui.gchar(name)) + L'"';
		} else if (type == L"int") {
			return std::to_wstring(core::io.ui.gint(name));
		} else if (type == L"bool") {
			return core::io.ui.gtru(name) ? L"TRU" : L"FALZ";
		}
		return L"";
	}

	uint8_t set(std::vector<std::wstring> wut)
	{
		if (wut.size() == 0)
			return 2;
		std::wstring type = wut[0].substr(0, wut[0].find(L'_') + 1);
		if (type == L"attr_") {
			if (wut.size() < 4)
				return 5;
			else if (wut.size() > 4)
				return 6;
			switch (core::io.ui.set(wut[0], {wut[1], wut[2], wut[3]})) {
			case 0:
				core::io << core::io::msg(L"kewl", L"property set successful");
				break;
			case 255:
				core::io << core::io::msg(L"kewl", L"WARN: same as current value");
				break;
			case 1:
				core::io << core::io::msg(L"kewl", L"ERR: unknown property: \"" + wut[0] + L'"');
				return -1;
			case 2:
				return -1;
			case 3:
				core::io << core::io::msg(L"kewl", L"ERR: unknown color: \"" + wut[1]);
				return -1;
			case 4:
				core::io << core::io::msg(L"kewl", L"ERR: unknown color: \"" + wut[2]);
				return -1;
			case 5:
				core::io << core::io::msg(L"kewl", L"ERR: unknown weight: \"" + wut[3]);
				return -1;
			}
		} else if (type == L"txt_") {
			if (wut.size() < 2)
				return 3;
			else if (wut.size() > 2)
				return 4;
			switch (core::io.ui.set(wut[0], wut[1])) {
			case 0:
				core::io << core::io::msg(L"kewl", L"property set successful");
				break;
			case 255:
				core::io << core::io::msg(L"kewl", L"WARN: same as current value");
				break;
			case 1:
				core::io << core::io::msg(L"kewl", L"ERR: property not found: \"" + wut[0] + L'"');
				return -1;
			case 2:
				return -1;
			case 3:
				core::io << core::io::msg(L"kewl", L"ERR: not in length range (" + core::io.ui.grange(wut[0]) + L')');
				return -1;
			}
		} else if (type == L"char_") {
			if (wut.size() < 2)
				return 3;
			else if (wut.size() > 2)
				return 4;
			if (wut[1].size() != 1) {
				core::io << core::io::msg(L"kewl", L"ERR: not a char: \"" + wut[1] + L'"');
				return -1;
			}
			switch (core::io.ui.set(wut[0], (wint_t)wut[1][0])) {
			case 0:
				core::io << core::io::msg(L"kewl", L"property set successful");
				break;
			case 255:
				core::io << core::io::msg(L"kewl", L"WARN: same as current value");
				break;
			case 1:
				core::io << core::io::msg(L"kewl", L"ERR: property not found: \"" + wut[0] + L'"');
				return -1;
			}
		} else if (type == L"int_") {
			if (wut.size() < 2)
				return 3;
			else if (wut.size() > 2)
				return 4;
			int tmp;
			try {
				tmp = std::stoi(wut[1]);
			} catch (std::invalid_argument) {
				core::io << core::io::msg(L"kewl", L"ERR: entered value iz not a number");
				return -1;
			}
			if (wut[1] != std::to_wstring(tmp)) {
				core::io << core::io::msg(L"kewl", L"ERR: entered value iz not a number");
				return -1;
			}
			switch (core::io.ui.set(wut[0], tmp)) {
			case 0:
				core::io << core::io::msg(L"kewl", L"property set successful");
				break;
			case 255:
				core::io << core::io::msg(L"kewl", L"WARN: same as current value");
				return -1;
			case 1:
				core::io << core::io::msg(L"kewl", L"ERR: property not found: \"" + wut[0] + L'"');
				return -1;
			case 2:
				return -1;
			case 3:
				core::io << core::io::msg(L"kewl", L"ERR: not in range (" + core::io.ui.grange(wut[0]) + L')');
				return -1;
			}
		} else if (type == L"bool_") {
			if (wut.size() < 2)
				return 3;
			else if (wut.size() > 2)
				return 4;
			switch (core::io.ui.set(wut[0], wut[1])) {
			case 0:
				core::io << core::io::msg(L"kewl", L"property set successful");
				break;
			case 255:
				core::io << core::io::msg(L"kewl", L"WARN: same as current value");
				return -1;
			case 1:
				core::io << core::io::msg(L"kewl", L"ERR: property not found \"" + wut[0] + L'"');
				return -1;
			case 3:
				core::io << core::io::msg(L"kewl", L"ERR: must be TRU/FALZ");
				return -1;
			}
		} else {
			return 2;
		}
		return 0;
	}

	uint8_t define(std::vector<std::wstring> wut)
	{
		if (wut.size() < 2)
			return 3;
		else if (wut.size() > 2)
			return 4;
		unsigned int c[3];
		try {
			if (wut[1].size() == 7 && wut[1][0] == L'#') {
				c[0] = std::stoi(std::wstring(wut[1].begin() + 1, wut[1].begin() + 3), NULL, 16);
				c[1] = std::stoi(std::wstring(wut[1].begin() + 3, wut[1].begin() + 5), NULL, 16);
				c[2] = std::stoi(std::wstring(wut[1].begin() + 5, wut[1].begin() + 7), NULL, 16);
			} else {
				for (int i = 0; i < 2; i++) {
					int pos = wut[1].find(L',');
					if (pos == std::wstring::npos)
						return 3;
					std::wstring tmp(wut[1].begin(), wut[1].begin() + pos);
					wut[1].erase(0, pos + 1);
					c[i] = std::stoi(tmp);
					if (std::to_wstring(c[i]) != tmp)
						return 3;
				}
				c[2] = std::stoi(wut[1]);
				if (std::to_wstring(c[2]) != wut[1])
					return 3;				
			}
		} catch (std::invalid_argument) {
			return 3;
		}

		switch (core::io.ui.def_col(wut[0], {c[0] * 1000 / 256, c[1] * 1000 / 256, c[2] * 1000 / 256})) {
		case 0:
			core::io << core::io::msg(L"kewl", L"color (re)definition successful");
			return 0;
		default:
		case 1:
			core::io << core::io::msg(L"kewl", L"ERR: max of colorz reached 4 current terminal");
			return -1;
		case 2:
			return 3;
		case 255:
			core::io << core::io::msg(L"kewl", L"ERR: unable 2 (re)define diz color");
			return -1;
		case 3:
			core::io << core::io::msg(L"kewl", L"ERR: color name length must be between 3 and 15 charz");
			return -1;
		case 4:
			core::io << core::io::msg(L"kewl", L"ERR: red component not in range");
			return -1;
		case 5:
			core::io << core::io::msg(L"kewl", L"ERR: blu component not in range");
			return -1;
		case 6:
			core::io << core::io::msg(L"kewl", L"ERR: green component not in range");
			return -1;
		}
	}
public:
	ui()
	{
		core::exec.add(L"ui", {{L"define col_name #ffffff"}, { L"define col_name 255 255 255", L"define new / redefine existing color"}, {L"set attr_property fg_col bg_col txt_weight"}, {L"set txt_property \"blah blah blah string\""}, {L"set char_property \"#\""}, {L"set int_property 1234"}, {L"set bool_property TRU/FALZ", L"set property valuez"}, {L"reset", L"reset all propertiez 2 their initial statez"}, {L"reset some_property", L"reset some_property 2 itz initial state"}, {L"list propertiez/colorz/weightz", L"get list of all propertiez/..."}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() == 1)
			return 1;
		if (arg[1] == L"define") {
			arg.erase(arg.begin(), arg.begin() + 2);
			return define(arg);
		} else if (arg[1] == L"reset") {
			if (arg.size() == 2) {
				core::io.ui.reset();
				core::io << core::io::msg(L"kewl", L"propertiez reset successful");
			} else if (arg.size() == 3) {
				if (core::io.ui.reset(arg[2]) == 1) {
					core::io << core::io::msg(L"kewl", L"ERR: unknown property: \"" + arg[2] + L'"');
					return -1;
				}
				core:: io << core::io::msg(L"kewl", L"property reset successful");
			} else {
				return 3;
			}
		} else if (arg[1] == L"set") {
			arg.erase(arg.begin(), arg.begin() + 2);
			return set(arg);
		} else if (arg[1] == L"list") {
			if (arg.size() < 3)
				return 2;
			if (arg.size() > 3)
				return 3;
			std::vector<std::wstring> tmp;
			if (arg[2] == L"propertiez") {
				int max_x, max_y;
				getmaxyx(stdscr, max_y, max_x);
				max_y = core::io.ui.gint(L"int_usr_max_padding");
				max_x -= (max_y > 4 ? max_y : 4);
				max_x -= 1 + (core::io.ui.gtru(L"bool_show_time") ? 5 + core::io.ui.gstr(L"txt_time_before").size() + core::io.ui.gstr(L"txt_time_after").size() : 0) + core::io.ui.gstr(L"txt_usr_before").size() + core::io.ui.gstr(L"txt_usr_after").size();
				core::io.ui.gpropertiez(tmp);
				if (max_x > 58) {
					int longest = 0;
					for (int i = 0; i < tmp.size(); i++) {
						if (tmp[i].size() > longest)
							longest = tmp[i].size();
					}
					longest += 3;
					for (int i = 0; i < tmp.size(); i++)
						tmp[i] += (std::wstring(longest - tmp[i].size(), 32) + L"\\5" + property_query(tmp[i]));
				}
			} else if (arg[2] == L"colorz") {
				core::io.ui.gcolorz(tmp);
			} else if (arg[2] == L"weightz") {
				core::io.ui.gweightz(tmp);
			} else {
				core::io << core::io::msg(L"kewl", L"ERR: unknown list request: \"" + arg[2] + L'"');
				return -1;
			}
			core::io << core::io::msg(L"hr", L"");
			for (int i = 0; i < tmp.size(); i++)
				core::io << core::io::msg(L"kewl", tmp[i]);
			core::io << core::io::msg(L"hr", L"");
		} else {
			return 1;
		}
		return 0;
	}

	void acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
	{
		switch (arg.size()) {
		case 1:
			trg = {L"define", L"list", L"reset", L"set"};
			break;
		case 2:
			if (arg[1] == L"define")
				core::io.ui.gcolorz(trg);
			else if (arg[1] == L"set" || arg[1] == L"reset")
				core::io.ui.gpropertiez(trg);
			else if (arg[1] == L"list")
				trg = {L"colorz", L"propertiez", L"weightz"};
			break;
		case 3:
			
			if (arg[1] == L"define")
				trg = {L"#ffffff", L"255,255,255"};
			else if (arg[1] == L"set" && arg[2].substr(0, 5) == L"attr_")
				core::io.ui.gcolorz(trg);
			else if (arg[1] == L"set" && arg[2].substr(0, 5) == L"bool_")
				trg = {L"FALZ", L"TRU"};
			break;
		case 4:
			if (arg[1] == L"set" && arg[2].substr(0, 5) == L"attr_")
				core::io.ui.gcolorz(trg);
			break;
		case 5:
			if (arg[1] == L"set" && arg[2].substr(0, 5) == L"attr_")
				core::io.ui.gweightz(trg);
			break;
		}
	}
} ui;

class macroz: public core::exec::cmd {
public:
	macroz()
	{
		core::exec.add(L"macroz", {{L"how2", L"how 2 work with macroz"}, {L"list", L"list all macroz"}, {L"reload", L"reload macroz file"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() < 2)
			return 2;
		if (arg[1] == L"list") {
			if (arg.size() > 2)
				return 2;
			std::vector<std::wstring> tmp;
			core::exec.macroz.gmacroz(tmp);
			if (tmp.size() != 0) {
				core::io << core::io::msg(L"hr", L"");
				for (int i = 0; i < tmp.size(); i++)
					core::io << core::io::msg(L"kewl", tmp[i]);
				core::io << core::io::msg(L"hr", L"");
			} else {
				core::io << core::io::msg(L"kewl", L"no macroz defined");
			}
		} else if (arg[1] == L"reload") {
			if (arg.size() > 2)
				return 2;
			core::exec.macroz.load();
		} else if (arg[1] == L"how2") {
			if (arg.size() > 2)
				return 2;
			std::vector<std::wstring> tmp({ \
			L"macroz are imported from ~/.kewl_macroz", \
			L"macro named \"rc\" iz auto-executed at kewl'z startup", \
			L"u can execute macro as a cmd, but prepend itz name with '_'", \
			L"~/.kewl_macroz syntax:", \
			L"macro_name: (1-15 charz, ':' after it!)", \
			L"[TAB] # diz iz sample macro", \
			L"[TAB] some_cmd", \
			L"[TAB] another_etc", \
			});
			core::io << core::io::msg(L"hr", L"");
			for (int i = 0; i < tmp.size(); i++)
				core::io << core::io::msg(L"kewl", tmp[i]);
			core::io << core::io::msg(L"hr", L"");
		} else {
			return 1;
		}
		return 0;
	}

	void acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
	{
		if (arg.size() != 1)
			return;
		trg = {L"how2", L"list", L"reload"};
	}
private:
} macroz;

class servctl: public core::exec::cmd {
public:
	servctl()
	{
		core::exec.add(L"servctl", {{L"omg", L"get ur permissionz on active server"}, {L"shutdown", L"shutdown da server (o+ needed)"}, {L"kick", L"kick usr from da server (m greatest than targetz highest omg needed)"}, {L"say \"blah blah blah\"", L"enforce serv to say passed msg (any non-zero omg needed)"}, {L"chomg usr o/m/g 0-2", L"set \"usr\"'z o/m/g value"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() == 1)
			return 2;
		if (!core::serv.status.gactive() && false) {
			core::io << core::io::msg(L"kewl", L"ERR: no connection alive");
			return 1;
		}

		std::wstring todo;
		if (core::exec.escape(arg, todo) >= 255) {
			core::io << core::io::msg(L"kewl", L"ERR: command 2 long");
			return 1;
		}

		try {
			core::serv.snd(L'/' + todo);
		} catch (...) {}
		return 0;
	}

	void acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
	{
		switch (arg.size()) {
		case 1:
			trg = {L"chomg", L"kick", L"omg", L"say", L"shutdown"};
			break;
		case 2:
			if (arg[1] == L"kick" || arg[1] == L"chomg")
				core::serv.status.gotherz(trg);
			break;
		case 3:
			if (arg[1] == L"chomg")
				trg = {L"o", L"m", L"g"};
			break;
		case 4:
			if (arg[1] == L"chomg")
				trg = {L"0", L"1", L"2"};
			break;
		}
	}

} servctl;

class redraw: public core::exec::cmd {
public:
	redraw()
	{
		core::exec.add(L"redraw", {{L"", L"redraw ui"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() > 1)
			return 1;
		core::io.mkwin();
		return 0;
	}
} redraw;
