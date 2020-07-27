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
