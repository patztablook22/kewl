class version: public core::exec::cmd {
public:
	version()
	{
		core::exec.add(L"version", {{L"", L"print version info"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 1;
		core::io << L" kewl-client ─┬─ release \\1" + core::io.ver_echo(VERSION);
		std::string tmp(__DATE__);
		tmp += ' ';
		tmp += __TIME__;
		core::io << L"              └─ compile \\1" + std::wstring(tmp.begin(), tmp.end());
		
		return 0;
	}
} version;
