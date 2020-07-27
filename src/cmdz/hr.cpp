class hr: public core::exec::cmd {
public:
	hr()
	{
		core::exec.add(L"hr", {{L"", L"print horizontal line (only if da last msg iz not hr 2)"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 1;
		core::io << core::io::msg(L"hr", L"");
		return 0;
	}
} hr;
