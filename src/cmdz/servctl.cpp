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
		if (!core::serv.status.gactive()) {
			core::io << L"ERR: no connection alive";
			return 0;
		}

		std::wstring todo;
		if (core::exec.escape(arg, todo) >= 255) {
			core::io << L"ERR: command 2 long";
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
