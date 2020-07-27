class version: public core::cmdl::opt {
public:
	version()
	{
		core::cmdl.add(L"version", L"print current version", this);
	}

	void usr(std::wstring self)
	{
		std::wcout << L"current version: " << core::io.ver_echo(VERSION) << std::endl;
		std::string tmp(__DATE__);
		tmp += ' ';
		tmp += __TIME__;
		std::wcout << L"compiled on: " + std::wstring(tmp.begin(), tmp.end());
		exit(EXIT_SUCCESS);
	}
} version;
