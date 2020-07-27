class help: public core::exec::cmd {
public:
	help()
	{
		core::exec.add(L"help", {{L"", L"get list of cmdz"}, {L"*cmd*", L"get help 4 da cmd"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		std::vector<std::wstring> cmdz;
		core::exec.cmdz(cmdz);
		int max_x, max_y;
		basis::termxy(max_x, max_y);
		if (arg.size() == 1) {
			int ipl = max_x - 1;
			if (core::env.gint(L"int_usr_max_padding") > 4)
				ipl -= core::env.gint(L"int_usr_max_padding");
			else
				ipl -= 4;
			ipl -= core::env.gstr(L"txt_usr_before").size() + core::env.gstr(L"txt_usr_after").size() + 5 * core::env.gtru(L"bool_show_time");
			ipl /= 16;
			if (ipl <= 0)
				ipl = 1;
			std::wstring tmp;
			core::io << core::io::msg(L"hr", L"");
			for (int i = 0; i < cmdz.size(); i++) {
				if (i % ipl) {
					tmp += std::wstring(16 - cmdz[i - 1].size(), 32);
				} else if (i != 0) {
					core::io << tmp;
					tmp.clear();
				}
				tmp += cmdz[i];
			}
			core::io << tmp;
			core::io << core::io::msg(L"hr", L"");
		} else if (arg.size() == 2) {
			if (arg[1].size() == 0)
				return 1;
			if (arg[1].size() > 15) {
				core::io << L"ERR: cmdz hav limited size to 15 charz";
				return -1;
			}
			if (!core::exec.cmd(arg[1])) {
				core::io << L"ERR: no such command: \"" + arg[1] + L"\"";
				return -1;
			}
			std::vector<std::vector<std::wstring>> man;
			core::exec.man(arg[1], man);
			if (man.size() != 0) {
				core::io << core::io::msg(L"hr", L"");
				for (int i = 0; i < man.size(); i++) {
					for (int j = 0; j < man[i].size(); j++) {
						core::io << (j == 0 ?  arg[1] : L"->") + L' ' + man[i][j];
					}
				}
				core::io << core::io::msg(L"hr", L"");
			} else {
				core::io << L"no manual 4 \"" + arg[1] + L"\" given :c";
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
		core::exec.cmdz(trg);
	}
} help;
