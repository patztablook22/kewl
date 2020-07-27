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
