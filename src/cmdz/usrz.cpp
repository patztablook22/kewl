class usrz: public core::exec::cmd {
private:
	void usrz_list(std::wstring nicc, std::vector<std::wstring> &list)
	{
		if (list.size() == 0) {
			core::io << L"*list iz empty*";
			return;
		}
		int max_x, max_y;
		basis::termxy(max_x, max_y);
		int ipl = max_x - 1;
		if (core::env.gint(L"int_usr_max_padding") > 4)
			ipl -= core::env.gint(L"int_usr_max_padding");
		else
			ipl -= 4;
		ipl -= core::env.gstr(L"txt_usr_before").size() + core::env.gstr(L"txt_usr_after").size() + 5 * core::env.gtru(L"bool_show_time");
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
				core::io << L"ERR: no connection alive";
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
				core::io << L"ERR: nicc size must be between 3 and 15 charz";
				return -1;
			}
			if (!core::serv.status.gactive()) {
				core::io << L"ERR: no connection alive";
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
					core::io << L"WARN: ignoring urself haz no effect";
				switch (core::serv.ignore.add(arg[3])) {
				case 0:
					core::io << L"ignoring msgz from \"" + arg[3] + L'"';
					break;
				case 1:
					core::io << L"WARN: ur already ignoring diz usr";
					return -1;
				case 2:
					core::io << L"ERR: nicc size must be between 3 and 15 charz";
					return -1;
				case 3:
					core::io << L"ERR: it refused!";
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
					core::io << L"WARN: ur not ignoring diz usr";
					return -1;
				}
				core::io << L"stopped ignoring msgz from \"" + arg[3] + L'"';
			} else if (arg[2] == L"clear") {
				if (arg.size() > 3)
					return 3;
				if (core::serv.ignore.clear()) {
					core::io << L"WARN: ignore list iz empty";
					return -1;
				}
				core::io << L"ignore list cleared";
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
				core::io << L"ERR: no connection alive";
				return -1;
			}
			try {
				core::serv.snd(L"/register");
				core::io << L"registration request sent";
			} catch (...) {}
		} else if (arg[1] == L"chpasswd") {
			if (arg.size() > 2)
				return 2;
			if (core::serv.status.gnick().size() == 0) {
				core::io << L"ERR: no connection alive";
				return -1;
			}
			try {
				core::serv.snd( L"/chpasswd");
				core::io << L"passwd change request sent";
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
			core::io << (arg[2] == L"0" ? L"serv requests \\1new usr\\0 passwd" : L"serv requests \\1control\\0 passwd");
			core::io.passwd();
		} else if (arg[1] == L"chpasswd") {
			if (arg.size() != 3 || arg[2].size() == 0)
				return 2;
			switch (arg[2][0]) {
			case L'0':
				core::io << L"serv requests \\1auth\\0 passwd";
				break;
			case L'1':
				core::io << L"serv requests \\1new usr\\0 passwd";
				break;
			case L'2':
				core::io << L"serv requests \\1control\\0 passwd";
				break;
			}
			core::io.passwd();
		} else {
			return 2;
		}

		core::io.beep();
		return 0;
	}
} usrz;
