class help: public core::perform::func {
public:
	help()
	{
		core::perform.add(L"help", L"print diz help (surprisingly lol)", this);
	}

	int usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 2)
			return 2;

		std::wcout << L"GENERAL:" << std::endl;
		std::wcout << L'\t' << arg[0] << std::endl;
		std::wcout << L"\t\tlaunch kewl" << std::endl;
		std::wcout << L'\t' << arg[0] << L" --func" << std::endl;
		std::wcout << L"\t\tout-of-da-box function, read \"FUNCZ\" below" << std::endl;
		std::wcout << L'\t' << arg[0] << L" cmd (or " << arg[0] << L" /cmd)" << std::endl;
		std::wcout << L"\t\tstart kewl and execute da cmd, read \"CMDZ\" below" << std::endl;
		std::wcout << L"FUNCZ:" << std::endl;
		std::vector<std::wstring> tmp;
		core::perform.gfuncz(tmp);
		for (int i = 0; i < tmp.size(); i++) {
			std::wcout << '\t' << arg[0] << L" --" + tmp[i] << std::endl;
			std::wcout << L"\t\t" << core::perform.gdesc(tmp[i]) << std::endl;
		}

		std::wcout << L"CMDZ:" << std::endl;
		std::wcout << '\t' << arg[0] << L" help " << std::endl;
		std::wcout << L"\t\tget list of all in-kewl cmdz" << std::endl;
		std::wcout << '\t' << arg[0] << L" help cmd" << std::endl;
		std::wcout << L"\t\tget help for da in-kewl cmd" << std::endl;

		return 0;
	}

} help;

class version: public core::perform::func {
public:
	version()
	{
		core::perform.add(L"version", L"print current version", this);
	}

	int usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 2)
			return 2;

		std::wstring tmp(core::io.ver_echo(VERSION));
		std::wcout << L"current version: " << std::wstring(tmp.begin(), tmp.end()) << std::endl;
		return 0;
	}
} version;

class about: public core::perform::func {
public:
	about()
	{
		core::perform.add(L"about", L"wtf iz kewl about", this);
	}

	int usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 2)
			return 2;

		std::wcout << L"Konverzace Everybody Will Like client" << std::endl;
		std::wcout << L"C++ IRC with SSL encryption" << std::endl;
		std::wcout << L"\"lol imma waste my time\" ~ patz, d6022" << std::endl;

		return 0;
	}
} about;

class debug: public core::perform::func {
private:
	void title()
	{
		erase();
		printw("hOI! Welcom to da kewl debug mode...\n");
		printw("press a key to get itz in-kewl UTF value\n");
		printw("u can quit via CTRL+C\n");
		printw("----------------------------------------\n");
	}

	void init()
	{
		initscr();
		noecho();
		keypad(stdscr, true);
		raw();
		title();
	}

public:
	debug()
	{
		core::perform.add(L"debug", L"run kewl debug mode (key-capturing test)", this);
	}

	int usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 2)
			return 2;

		init();
		wint_t ch;

		do {
			get_wch(&ch);
			if (ch == 0 || ch == -1) {
				exit(EXIT_FAILURE);
				std::wcerr << L"ERR: failed to capture keypress\n";
			}
			title();
			printw("%d", ch);
			refresh();
		} while (ch != 3);

		return 0;
	}
} debug;
