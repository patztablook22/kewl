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

class about: public core::cmdl::opt {
public:
	about()
	{
		core::cmdl.add(L"about", L"wtf iz kewl about", this);
	}

	void usr(std::wstring self)
	{
		std::wcout << L"Konverzace Everybody Will Like client" << std::endl;
		std::wcout << L"C++ IRC with SSL encryption" << std::endl;
		std::wcout << L"\"lol imma waste my time\" ~ patz, d6022" << std::endl;
		exit(EXIT_SUCCESS);
	}
} about;

class debug: public core::cmdl::opt {
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
		if (!isatty(STDIN_FILENO)) {
			std::wcerr << L"ERR: failed to read stdin" << std::endl;
			exit(EXIT_FAILURE);
		}

		initscr();
		noecho();
		keypad(stdscr, true);
		raw();
		title();
	}

public:
	debug()
	{
		core::cmdl.add(L"debug", L"run kewl debug mode (key-capturing test)", this);
	}

	void usr(std::wstring self)
	{
		init();
		wint_t ch;
		do {
			get_wch(&ch);
			title();
			if (ch == 0 || ch == -1)
				addwstr(L"???");
			else
				printw("%d", ch);
			refresh();
		} while (ch != 3);
		exit(EXIT_SUCCESS);
	}
} debug;

class ascii: public core::cmdl::opt {
public:
	ascii()
	{
		core::cmdl.add(L"ascii", L"use only base-ascii charz 4 environment", this);
	}

	void usr(std::wstring self)
	{
		core::io.sascii();
		return;
	}
} ascii;
