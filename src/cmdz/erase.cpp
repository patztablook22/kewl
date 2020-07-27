class erase: public core::exec::cmd {
public:
	erase()
	{
		core::exec.add(L"erase", {{L"", L"erase all messages and print kewl logo etc."}}, this);
	}
	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 1;
		core::io.da_erase();
		core::io.mkwin();
		return 0;
	}
} erase;
