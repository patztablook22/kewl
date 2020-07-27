class quit: public core::exec::cmd {
public:
	quit()
	{
		core::exec.add(L"quit", {{L"", L"quit kewl"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
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
		core::exec.add(L"help", {{L"", L"get list of cmdz"}, {L"cmd", L"get help 4 da cmd"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
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
				return 2;
			if (arg[1].size() > 15) {
				core::io << core::io::msg(L"kewl", L"ERR: cmdz hav limited size to 15 charz");
				return 1;
			}
			if (!core::exec.iz_cmd(arg[1])) {
				core::io << core::io::msg(L"kewl", L"ERR: no such command: \"" + arg[1] + L"\"");
				return 1;
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
				core::io << core::io::msg(L"kewl", L"no manual for \"" + arg[1] + L"\" given :c");
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
			return 2;
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
			return 2;
		core::io.da_erase();
		return 0;
	}
} erase;

class reset: public core::exec::cmd {
public:
	reset()
	{
		core::exec.add(L"reset", {{L"", L"reset IO screen"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 2;
		endwin();
		core::io.init(false);
		return 0;
	}
} reset;

class conn: public core::exec::cmd {
public:
	conn()
	{
		core::exec.add(L"conn", {{L"address:port nick", L"connect to kewl serv"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
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
		core::serv.conn(hostname, std::to_string(port), arg[2]);
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
			return 2;
		try {
			core::serv << core::io::msg(L"kewl", L"/disconn");
		} catch (...) {
			core::io << core::io::msg(L"kewl", L"ERR: no connection alive");
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
public:
	usrz()
	{
		core::exec.add(L"usrz", {{L"list", L"print all usrz on da server (default option)"}, {L"register", L"register ur actual nick on da server"}, {L"chpasswd", L"change ur passwd (if ur registered) on da server"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (core::serv.status.gnick().size() == 0) {
			core::io << core::io::msg(L"kewl", L"ERR: no connection alive");
			return 1;
		}
		if (arg.size() == 1 || arg[1] == L"list") {
			int max_x, max_y;
			getmaxyx(stdscr, max_y, max_x);
			int ipl = (max_x - 8) / 16;
			if (ipl <= 0)
				ipl = 1;
			std::vector<std::wstring> usrz;
			core::serv.status.gotherz(usrz);
			usrz.insert(usrz.begin(), core::serv.status.gnick());
			core::io << core::io::msg(L"hr", L"");
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
			core::io << core::io::msg(L"hr", L"");
		} else if (arg[1] == L"register") {
			try {
				core::serv << core::io::msg(L"kewl", L"/register");
				core::io << core::io::msg(L"kewl", L"registration request sent");
			} catch (...) {}
		} else if (arg[1] == L"chpasswd") {
			try {
				core::serv << core::io::msg(L"kewl", L"/chpasswd");
				core::io << core::io::msg(L"kewl", L"passwd change request sent");
			} catch (...) {}
		} else {
			return 2;
		}

		return 0;
	}

	void acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
	{
		switch (arg.size()) {
		case 1:
			trg = {L"chpasswd", L"list", L"register"};
			break;
		}
	}

	uint8_t serv(std::vector<std::wstring> arg)
	{
		if (arg.size() != 3)
			return 2;
		if (arg[1] == L"conn") {
			core::serv.status.join(arg[2]);
			core::io << core::io::msg(L"serv", arg[2] + L" connected");
		} else if (arg[1] == L"disconn") {
			core::serv.status.leave(arg[2]);
			core::io << core::io::msg(L"serv", arg[2] + L" disconnected");
		} else if (arg[1] == L"lost") {
			core::serv.status.leave(arg[2]);
			core::io << core::io::msg(L"serv", L"connection with " + arg[2] + L" lost");
		} else if (arg[1] == L"kick") {
			core::serv.status.leave(arg[2]);
			core::io << core::io::msg(L"serv", arg[2] + L" was kicked out");
		} else if (arg[1] == L"register") {
			core::io << core::io::msg(L"kewl", (arg[2] == L"0") ? L"serv requests new usr passwd" : L"serv requests control passwd");
			core::io.send_passwd();
		} else if (arg[1] == L"chpasswd") {
			if (arg[2].size() == 0)
				return 2;
			switch (arg[2][0]) {
			case L'0':
				core::io << core::io::msg(L"kewl", L"serv requests auth passwd");
				break;
			case L'1':
				core::io << core::io::msg(L"kewl", L"serv requests new usr passwd");
				break;
			case L'2':
				core::io << core::io::msg(L"kewl", L"serv requests control passwd");
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
	:in_use(false)
	{
		core::exec.add(L"ping", {{L"", L"get ping on current server"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
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

	uint8_t serv(std::vector<std::wstring> arg)
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
		core::exec.add(L"version", {{L"", L"print current version"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
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
		core::exec.add(L"about", {{L"", L"wtf iz kewl about"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 2;
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
			return 2;
		if (arg[1] == L"test") {
			if (arg.size() != 2)
				return 2;
			core::io.beep(true);
		} else if (arg[1] == L"on") {
			if (arg.size() > 2)
				return 2;
			if (core::io.beep_gon()) {
				core::io << core::io::msg(L"kewl", L"WARN: beep already iz enabled");
				return 1;
			}
			core::io.beep_son(true);
			core::io << core::io::msg(L"kewl", L"beep enabled");
		} else if (arg[1] == L"off") {
			if (!core::io.beep_gon()) {
				core::io << core::io::msg(L"kewl", L"WARN: beep already iz disabled");
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
			if (core::io.beep_gdelay()  == tmp) {
				core::io << core::io::msg(L"kewl", L"WARN: same as current value");
				return 1;
			}
			core::io.beep_sdelay(tmp);
			core::io << core::io::msg(L"kewl", L"beep delay set to " + arg[2] + L"ms");
		} else {
			return 2;
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
		core::exec.add(L"hr", {{L"", L"print horizontal line (only if da last msg iz not hr too)"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 2;
		core::io << core::io::msg(L"hr", L"");
		return 0;
	}
} hr;

class ui: public core::exec::cmd {
private:
	uint8_t set(std::vector<std::wstring> wut)
	{
		if (wut.size() == 0)
			return 2;
		std::wstring type = wut[0].substr(0, wut[0].find(L'_') + 1);
		if (type == L"attr_") {
			if (wut.size() != 4)
				return 2;
			switch (core::io.ui.set(wut[0], {wut[1], wut[2], wut[3]})) {
			case 0:
				core::io << core::io::msg(L"kewl", L"property set successful");
				break;
			case 255:
				core::io << core::io::msg(L"kewl", L"WARN: same as current value");
				break;
			case 1:
				core::io << core::io::msg(L"kewl", L"ERR: unknown property: \"" + wut[0] + L'"');
				return 1;
			case 2:
				return 2;
			case 3:
				core::io << core::io::msg(L"kewl", L"ERR: unknown color: \"" + wut[1]);
				return 1;
			case 4:
				core::io << core::io::msg(L"kewl", L"ERR: unknown color: \"" + wut[2]);
				return 1;
			case 5:
				core::io << core::io::msg(L"kewl", L"ERR: unknown weight: \"" + wut[3]);
				return 1;
			}
		} else if (type == L"txt_") {
			if (wut.size() != 2)
				return 2;;
			switch (core::io.ui.set(wut[0], wut[1])) {
			case 0:
				core::io << core::io::msg(L"kewl", L"property set successful");
				break;
			case 255:
				core::io << core::io::msg(L"kewl", L"WARN: same as current value");
				break;
			case 1:
				core::io << core::io::msg(L"kewl", L"ERR: property not found: \"" + wut[0] + L'"');
				return 1;
			case 2:
				return 2;
			case 3:
				core::io << core::io::msg(L"kewl", L"ERR: not in length range (" + core::io.ui.grange(wut[0]) + L')');
				return 1;
			}
		} else if (type == L"char_") {
			if (wut.size() != 2)
				return 2;
			if (wut[1].size() != 1) {
				core::io << core::io::msg(L"kewl", L"ERR: not a char: \"" + wut[1] + L'"');
				return 1;
			}
			core::io.ui.set(wut[0], wut[1][0]);
		} else {
			return 2;
		}
		return 0;
	}

	uint8_t define(std::vector<std::wstring> wut)
	{
		if (wut.size() != 2)
			return 2;
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
						return 2;
					std::wstring tmp(wut[1].begin(), wut[1].begin() + pos);
					wut[1].erase(0, pos + 1);
					c[i] = std::stoi(tmp);
					if (std::to_wstring(c[i]) != tmp)
						return 2;
				}
				c[2] = std::stoi(wut[1]);
				if (std::to_wstring(c[2]) != wut[1])
					return 2;				
			}
		} catch (std::invalid_argument) {
			return 2;
		}

		switch (core::io.ui.def_col(wut[0], {c[0] * 1000 / 256, c[1] * 1000 / 256, c[2] * 1000 / 256})) {
		case 0:
			core::io << core::io::msg(L"kewl", L"color (re)definition successful");
			return 0;
		default:
		case 1:
			core::io << core::io::msg(L"kewl", L"ERR: max of colorz reached for current terminal");
			return 1;
		case 2:
			return 2;
		case 255:
			core::io << core::io::msg(L"kewl", L"ERR: ur terminal cannot redefine colorz");
			return 1;
		case 3:
			core::io << core::io::msg(L"kewl", L"ERR: color name length must be between 3 and 15 charz");
			return 1;
		case 4:
			core::io << core::io::msg(L"kewl", L"ERR: red component not in range");
			return 1;
		case 5:
			core::io << core::io::msg(L"kewl", L"ERR: blu component not in range");
			return 1;
		case 6:
			core::io << core::io::msg(L"kewl", L"ERR: green component not in range");
			return 1;
		}
	}
public:
	ui()
	{
		core::exec.add(L"ui", {{L"define col_name #ffffff"}, { L"define col_name 255 255 255", L"define new / redefine existing color"}, {L"set attr_property fg_col bg_col txt_weight"}, {L"set txt_property \"blah blah blah string\""}, {L"set char_property \"#\"", L"set property valuez"}, {L"reset", L"reset all propertiez to their initial statez"}, {L"reset some_property", L"reset some_property to itz initial state"}, {L"list propertiez/colorz/weightz", L"get list of all propertiez/..."}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() == 1)
			return 2;
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
					return 1;
				}
				core:: io << core::io::msg(L"kewl", L"property reset successful");
			} else {
				return 2;
			}
		} else if (arg[1] == L"set") {
			arg.erase(arg.begin(), arg.begin() + 2);
			return set(arg);
		} else if (arg[1] == L"list") {
			if (arg.size() != 3)
				return 2;
			std::vector<std::wstring> tmp;
			if (arg[2] == L"propertiez") {
				core::io.ui.gpropertiez(tmp);
			} else if (arg[2] == L"colorz") {
				core::io.ui.gcolorz(tmp);
			} else if (arg[2] == L"weightz") {
				core::io.ui.gweightz(tmp);
			} else {
				core::io << core::io::msg(L"kewl", L"ERR: unknown list request: \"" + arg[2] + L'"');
				return 1;
			}
			core::io << core::io::msg(L"hr", L"");
			for (int i = 0; i < tmp.size(); i++)
				core::io << core::io::msg(L"kewl", tmp[i]);
			core::io << core::io::msg(L"hr", L"");
		} else {
			return 2;
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
		core::exec.add(L"macroz", {{L"howto", L"how to work with macroz"}, {L"list", L"list all macroz"}, {L"reload", L"reload macroz file"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 2)
			return 2;
		if (arg[1] == L"list") {
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
			core::exec.macroz.load();
		} else if (arg[1] == L"howto") {
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
			return 2;
		}
		return 0;
	}

	void acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
	{
		if (arg.size() != 1)
			return;
		trg = {L"howto", L"list", L"reload"};
	}
private:
} macroz;

class servctl: public core::exec::cmd {
public:
	servctl()
	{
		core::exec.add(L"servctl", {{L"control current server (if u hav permissionz)"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() == 1)
			return 2;
		if (!core::serv.status.gactive()) {
			core::io << core::io::msg(L"kewl", L"ERR: no connection alive");
			return 1;
		}

		std::wstring todo(L"/servctl");		
		for (int i = 1; i < arg.size(); i++) {
			std::wstring tmp;
			bool parz = false;
			for (int j = 0; j < arg[i].size(); j++) {
				wint_t ch = arg[i][j];
				switch (ch) {
				case L'\\':
					tmp += L"\\\\";
					break;
				case L'"':
					tmp += L"\\\"";
					break;
				case L' ':
					tmp += L' ';
					parz = true;
					break;
				default:
					tmp += ch;
					break;
				}
			}
			if (parz)
				todo += L" \"" + tmp + L'"';
			else
				todo += L' ' + tmp;
			tmp.clear();
		}
		if (todo.size() > 255) {
			core::io << core::io::msg(L"kewl", L"ERR: command too long");
			return 1;
		}
		core::serv << core::io::msg(L"kewl", todo);
		return 0;
	}

	void acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
	{
		switch (arg.size()) {
		case 1:
			trg = {L"kick", L"omg", L"shutdown"};
			break;
		case 2:
			if (arg[1] == L"kick")
				core::serv.status.gotherz(trg);
			break;
		}
	}

} servctl;
