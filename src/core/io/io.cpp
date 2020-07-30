io::io()
:ascii(false), iz_ready(false)
{
  std::setlocale(LC_ALL, std::setlocale(LC_CTYPE, getenv("LANG")) );
}

void io::init(bool init_env = true)
{
	if (!isatty(STDIN_FILENO)) {
		std::wcerr << L"ERR: failed to read stdin" << std::endl;
		exit(EXIT_FAILURE);
	}

	initscr();
	noecho();
	keypad(stdscr, true);
	raw();
	start_color();
	use_default_colors();

	if (init_env) {
		core::env.init();
		for (int i = 0; i < 8; i++)
			color_content(i, &default_contentz[i][0], &default_contentz[i][1], &default_contentz[i][2]);
		init_color(COLOR_WHITE, 742, 742, 742);
	}

	beep_tm.tv_sec = 0;
	beep_tm.tv_usec = 0;

	if (init_env)
		da_erase();
	if (!init_env)
		mkwin();
}

void io::sascii()
{
	ascii = true;
}

bool io::gascii()
{
  return ascii;
}

io::~io()
{
	for (int i = 0; i < 8; i++)
		init_color(i, default_contentz[i][0], default_contentz[i][1], default_contentz[i][2]);
	standend();
	endwin();
}

bool io::iz_k(wint_t chk)
{
	if ( \
		(chk >= 32 && chk <= 591) /* ASCII etc. */ || \
		(chk >= 1025 && chk <= 1105) /* cука блять etc. xd */ || \
		(chk >= 9472 && chk <= 9679) /* box-drawing charz etc. */ \
		)
		return true;
	return false;
}

bool io::iz_k(std::wstring chk)
{
	for (int i = 0; i < chk.size(); i++)
		if(!iz_k(chk[i]))
			return false;
	return true;
}

bool io::iz_k(std::string chk)
{
	return iz_k(std::wstring(chk.begin(), chk.end()));
}

std::wstring io::sha256(std::wstring input)
{
	std::string tmp(input.begin(), input.end());
	const char *string = tmp.c_str();
	char outputBuffer[65];
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, string, strlen(string));
	SHA256_Final(hash, &sha256);
	for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
	}
	outputBuffer[64] = 0;
	tmp = outputBuffer;
	return std::wstring(tmp.begin(), tmp.end());
}

std::wstring io::trim(std::wstring s)
{
	size_t i, done = 0;
	std::replace(s.begin(), s.end(), 9, 32);
	while ((i = s.find(32, done)) != std::wstring::npos) {
		if (i == 0 || i == s.size() - 1 || s[i - 1] == 32 || s[i + 1] == 32)
			s.erase(i, 1);
		else
			done++;
	}
	return s;
}

std::wstring io::ver_echo(int v)
{
	std::wstring w[3];
	for (int j = 2; j > 0; j--) {
		w[j] = std::to_wstring(v % 10);
		v /= 10;
	}
	w[0] = std::to_wstring(v);
	return std::wstring(L"v") + w[0] + std::wstring(L".") + w[1] + std::wstring(L".") + w[2];
}

void io::passwd()
{
	if (i.use_passwd)
		return;
		i.use_passwd = true;
	i.buf.erase();
	i.pos = 0;
	i.draw();
}

void io::ready()
{
	iz_ready = true;
}

void io::cls()
{
	o.pos0 = o.glen();
	o.pos1 = 0;
	o.pos2 = o.glen();
	mkwin();
}
