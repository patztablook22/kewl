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
			core::exec.macroz.mcrz(tmp);
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
			L"macroz are imported from ~/.config/kewl_macroz", \
			L"macro named \"rc\" iz auto-executed at kewl'z startup", \
			L"u can execute macro as a cmd, but prepend itz name with '_'", \
			L"~/.config/kewl_macroz syntax:", \
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
