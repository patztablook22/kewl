class help: public core::cmdl::opt {
public:
	help()
	{
		core::cmdl.add(L"help", L"print diz help (surprisingly lol)", this);
	}

	void usr(std::wstring self)
	{
		std::wcout << L"SYNOPSIS:" << std::endl;
		std::wcout << L'\t' << self << L" --opt --etc some_kewl_cmd cmdz_arg --cmdz_arg_2!" << std::endl;
		std::wcout << L"\t\tlaunch kewl with command line optionz \"opt\", \"etc\"" << std::endl;
		std::wcout << L"\t\tand execute \"some_kewl_cmd\" with argumentz \"cmdz_arg\", \"--cmd_arg_2!\"" << std::endl;
		std::wcout << L"OPTZ:" << std::endl;
		std::vector<std::wstring> tmp;
		core::cmdl.goptz(tmp);
		for (int i = 0; i < tmp.size(); i++) {
			std::wcout << '\t' << self << L" --" + tmp[i] << std::endl;
			std::wcout << L"\t\t" << core::cmdl.gdesc(tmp[i]) << std::endl;
		}

		std::wcout << L"CMDZ:" << std::endl;
		std::wcout << '\t' << self << L" help " << std::endl;
		std::wcout << L"\t\tget list of all in-kewl cmdz" << std::endl;
		std::wcout << '\t' << self << L" help cmd" << std::endl;
		std::wcout << L"\t\tget help for da in-kewl cmd" << std::endl;

		exit(EXIT_SUCCESS);
	}

} help;
