class say: public core::exec::cmd {
public:
	say()
	{
		core::exec.add(L"say", {{L"\"blah blah blah\"", L"send message via server"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 2)
			return 2;
		if (arg[1].size() == 0)
			return 0;
		if (arg[1][0] == L'/')
			return 0;
		if (!core::serv.status.gactive()) {
			core::io << L"ERR: no server buffer active";
			return 0;
		}
		try {
			core::serv.snd(arg[1]);
		} catch (int) {
			core::serv.disconn(false);
		}
		return 0;
	}
} say;
