class redraw: public core::exec::cmd {
public:
	redraw()
	{
		core::exec.add(L"redraw", {{L"", L"redraw ui"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() > 1)
			return 1;
		core::io.mkwin();
		return 0;
	}
} redraw;
