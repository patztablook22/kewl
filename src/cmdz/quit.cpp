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
