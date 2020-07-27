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
