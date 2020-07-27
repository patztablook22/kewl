serv::serv()
:ssl(NULL)
{
	SSL_library_init();
	ctx = init_ctx();
	free = true;
}

serv::~serv()
{
	core::io.beep_son(false);
	if (!status.gactive())
		return;
	try {
		snd(L"/disconn");
		disconn(true);
	} catch (...) {}
}

void serv::snd(std::wstring msg)
{
	if (msg.size() == 0 || msg.size() > 255 || !core::io.iz_k(msg))
		return;
	if (!core::serv.status.gactive())
		throw 1;
	*this << msg;
}

void serv::operator<<(std::wstring todo)
{
	if (ssl == NULL || !core::io.iz_k(todo))
		throw 1;
	if (SSL_write(ssl, todo.c_str(), 4 * todo.size()) <= 0) {
		throw 1;
	}
}

void serv::operator<<(int todo)
{
	if (ssl == NULL)
		throw 1;
	int32_t buf = todo;
	if (SSL_write(ssl, &buf, sizeof(buf)) <= 0)
		throw 1;
}

void serv::conn(std::string hostname, uint16_t port, std::wstring usr)
{
	if (port == 0)
		port = 12204;
	if (usr.size() == 0) {
		std::string tmp(getlogin());
		usr = std::wstring(tmp.begin(), tmp.end());
		core::io << core::io::msg(L"kewl", L"found ur niccname (\\3" + usr + L"\\0)");
	}
	if (port < 1 || port > 65535)
		return;
	if (!core::io.iz_k(usr))
		return;
	if (usr.size() < 3 || usr.size() > 15) {
		core::io << core::io::msg(L"kewl", L"ERR: nick length must be between 3 and 15 charz");
		return;
	}

	if (usr == L"kewl") {
		core::io << core::io::msg(L"kewl", L"ERR: ur not kewl enough to use diz nick");
		return;
	}
	if (usr == L"serv") {
		core::io << core::io::msg(L"kewl", L"ERR: 01101110 01101111 01110000 01100101");
		return;
	}

	if (usr.find_first_of(L" /\\") != std::wstring::npos) {
		core::io << core::io::msg(L"kewl", L"ERR: nick containz forbidden symbol/z (\"/\", \"\\\\\", \" \")");
		return;
	}
	
	if (!free) {
		core::io << core::io::msg(L"kewl", L"ERR: disconn first");
		return;
	}
	free = false;

	sniffer = std::thread(&serv::handler, this, hostname, port, usr);
	sniffer.detach();
}

int serv::disconn(bool e = true)
{
	if (free && e) {
		core::io << core::io::msg(L"kewl", L"ERR: no connection alive");
		return 1;
	}
	if (ssl == NULL)
		return 1;
	ping.cancel();
	SSL *tmp = ssl;
	ssl = NULL;
	//
	int sockfd = SSL_get_fd(tmp);
	SSL_shutdown(tmp);
	if (SSL_get_shutdown(tmp) == 0)
		SSL_shutdown(tmp);
	close(sockfd);
	std::wstring tmp_nick = core::serv.status.nick;
	core::serv.status.clientz = 0;
	core::serv.status.active = false;
	core::serv.status.name.clear();
	core::serv.status.nick.clear();
	core::serv.status.omg.clear();
	core::serv.status.otherz.clear();
	if (e) {
		core::io << core::io::msg(L"kewl", L"connection closed");
	} else {
		core::io << core::io::msg(L"kewl", L"ERR: connection lost");
		core::io.beep();
	}
	free = true;
	return 0;
}

void serv::operator>>(core::io::msg &trg)
{
	std::wstring buf;
	*this >> buf;
	int pos = buf.find(32);
	if (pos == std::wstring::npos || pos == 0)
		throw 1;
	trg = core::io::msg(buf.substr(0, pos), buf.substr(pos + 1, buf.size() - pos - 1));
}

void serv::operator>>(std::wstring &trg)
{
	trg.clear();
	if (ssl == NULL)
		throw 1;
	wchar_t buf[1024];
	int bytez, da_errno;
	do {
		bytez = SSL_read(ssl, &buf, sizeof(buf));
		da_errno = errno;
		if (status.gactive())
			*this << L"/";
	} while (bytez <= 0 && status.gactive() && da_errno == EAGAIN);
	if (bytez <= 0)
		throw 1;
	buf[bytez / 4] = 0;
	trg = buf;
}

void serv::operator>>(int &trg)
{
	if (ssl == NULL)
		throw 1;
	int32_t buf;
	if (SSL_read(ssl, &buf, sizeof(buf)) <= 0)
		throw 1;
	trg = buf;
}

void serv::handler(std::string hostname, uint16_t port, std::wstring usr)
{
	try {
		int bytez, sockfd = open_conn(hostname.c_str(), port);
		if (sockfd <= 0) {
			sockfd = 0;
			free = true;
			return;
		}
	
		int32_t buf0;
		std::wstring buf1;
		std::string tmp;
		ssl = SSL_new(ctx);
		SSL_set_fd(ssl, sockfd);
		if (SSL_connect(ssl) == -1) {
			core::io << core::io::msg(L"kewl", L"ERR: SSL perform failed");
			disconn();
			return;
		}
		core::io << core::io::msg(L"kewl", L"serv found! synchronizing...");
		*this << VERSION;
		*this >> buf1;
		if (buf1[0] == 'v') {
			core::io << core::io::msg(L"serv", L"ERR: " + buf1 + L" required");
			disconn();
			return;
		}
		tmp = SSL_get_cipher(ssl);
		core::io << core::io::msg(L"serv", L"connection established");
		core::io << core::io::msg(L"serv", L"\\1" + std::wstring(tmp.begin(), tmp.end()) + L"\\0 encrypted");
		*this << usr;
		*this >> buf1;
		if (buf1[0] == L'p') {
			core::io << core::io::msg(L"kewl", L"serv requests \\1" + std::wstring(buf1 == L"passwd serv" ? L"serv" : L"usr") + L"\\0 passwd");
			for (;;) {
				core::io.send_passwd();
				*this >> buf1;
				
				if (buf1[0] == L'p' || buf1[0] == L'a') {
					core::io << core::io::msg(L"serv", L"WARN: passwd incorrect");
					if (buf1[0] == L'a') {
						core::io << core::io::msg(L"serv", L"ERR: max of attemptz reached");
						disconn();
					return;
					} else {
						continue;
					}
				}
				break;	
			}
		}
		switch (buf1[0]) {
		case L's':
			break;
		case L'r':
			core::io << core::io::msg(L"kewl", L"ERR: diz nick iz registered");
		default:
			disconn();
			return;
		}
		*this << L"kk";
		*this >> buf1;
		switch (buf1[0]) {
		case L'k':
			if (buf1.size() > 19 || buf1.size() < 6 || !core::io.iz_k(buf1)) {
				disconn();
				return;
			}
			core::serv.status.omg = buf1.substr(1, 3);
			core::serv.status.nick = buf1.substr(4, buf1.size() - 4);
			break;
		case L'f':
			core::io << core::io::msg(L"serv", L"ERR: chatroom iz full");
			disconn();
			return;
		case L'n':
			core::io << core::io::msg(L"serv", L"ERR: entered nick iz already in use");
			disconn();
			return;
		case L'r':
			core::io << core::io::msg(L"serv", L"ERR: diz server iz only 4 already registered usrz");
			disconn();
			return;
		}
		*this << L"ready";

		core::serv.status.otherz.clear();
		for (bool i = true; i;) {
			*this >> buf1;
			*this << L"kk";
			if (buf1[buf1.size() - 1] == L' ') {
				i = false;
				buf1.erase(buf1.size() - 1);
			}
			while (buf1.size() != 0) {
				int pos = buf1.find(32);
				if (pos == std::wstring::npos)
					pos = buf1.size();
				core::serv.status.otherz.push_back(buf1.substr(0, pos));
				buf1.erase(0, pos + 1);				
			}
			
		}

		*this >> buf1;
		buf0 = buf1.find(L'|');
		core::serv.status.clientz = std::stoi(buf1.substr(0, buf0));
		buf1.erase(0, buf0 + 1);
		if (buf1.size() > 15 || buf1.size() < 3 || !core::io.iz_k(buf1)) {
			disconn();
			return;
		}
		core::serv.status.name = buf1;
		core::io.mkwin();

		*this << L"sniffing";
		for (int i = 0;; i++) {
			*this >> buf1;
			if (buf1[0] != L'|') {
				if (i != 0)
					core::io << core::io::msg(L"kewl", L"\\2::\\1 EOF");
				break;
			}
			if (i == 0)
				core::io << core::io::msg(L"kewl", L"\\2::\\1 DOC");
			core::io << core::io::msg(L"serv", buf1.substr(1, buf1.size() - 1));
		}

		{
			int tmp = std::stoi(buf1.substr(1, buf1.size() - 1));
			timeval timeout;
			if (tmp > 1) {
				timeout.tv_sec = tmp * 3 / 4;
				timeout.tv_usec = 0;
			} else {
				timeout.tv_sec = 0;
				timeout.tv_usec = 500000;
			}
			if (setsockopt(SSL_get_fd(ssl), SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
				disconn(false);
				return;
			}
		}

		status.active = true;
		core::io::msg buf2;
		for (;;) {
			*this >> buf2;
			if (!buf2.gvalid())
				continue;
			if (buf2.gbody()[0] == L'/' && buf2.gfrom() == L"serv") {
				core::exec.serv << buf2.gbody().substr(1, buf2.gbody().size() - 1);
			} else if (!core::serv.ignore.ignored(buf2.gfrom())) {
				if (buf2.gfrom() != core::serv.status.gnick())
					core::io.beep();
				core::io << buf2;
			}
		}
	} catch (...) {
		disconn(false);
	}
}
	
int serv::open_conn(const char *hostname, uint16_t port)
{
	int sd;
	struct addrinfo hints, *ai, *tmp;
	struct sockaddr_in addr;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;	// auto IPv4/IPV6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	{
		std::string tmp(hostname);
		core::io << core::io::msg(L"kewl", L"resolving \\1" + std::wstring(tmp.begin(), tmp.end()) + L"\\0 port \\1" + std::to_wstring(port) + L"\\0...");
	}
	if (getaddrinfo(hostname, std::to_string(port).c_str(), &hints, &ai) != 0) {
		core::io << core::io::msg(L"kewl", L"ERR: resolving failed");
		return 0;
	}

	sd = socket(AF_INET, SOCK_STREAM, 0);

	timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	if (setsockopt(sd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) != 0) {
		core::io << core::io::msg(L"kewl", L"ERR: setsockopt failed");
		return 0;
	}

	core::io << core::io::msg(L"kewl", L"trying 2 connect...");
	
	for (tmp = ai; tmp != NULL; tmp = tmp->ai_next) {
		if (connect(sd, tmp->ai_addr, tmp->ai_addrlen) != -1)
			break;
	}
	if (tmp == NULL) {
		core::io << core::io::msg(L"kewl", L"ERR: connect failed");
		return 0;
	}

	return sd;
}

SSL_CTX *serv::init_ctx()
{
	const SSL_METHOD *mth;
	SSL_CTX *tmp;
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	mth = SSLv23_method();
	tmp = SSL_CTX_new(mth);
	if (tmp == NULL) {
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
	return tmp;
}

void serv::certz_echo(SSL *ssl)
{
	X509 *cert;
	char *line;
	cert = SSL_get_peer_certificate(ssl);
	if (cert != NULL) {
	} else {
	}
}

serv::status::status()
{
	nick.clear();
	omg.clear();
	name.clear();
	otherz.clear();
	clientz = 0;
	active = false;
}

bool serv::status::gactive()
{
	return active;
}

void serv::status::draw(int max_x, int max_y)
{
	std::wstring tmp;
	core::io.ui.on(L"attr_statusbar_placeholder");
	mvprintw(max_y - 2, 0, "%*s", max_x, "");
	core::io.ui.off();
	tmp = gactive() ? nick : L"---";
	max_x -= 11 + tmp.size();
	if (max_x < 0)
		return;
	move(max_y - 2, 1);
	core::io.ui.on(L"attr_statusbar_hl");
	printw("[ ");
	core::io.ui.off();
	core::io.ui.on(L"attr_statusbar_base");
	addwstr(tmp.c_str());
	if (gactive()) {
		max_x -= 6;
		if (max_x < 0) {
			core::io.ui.off();
			core::io.ui.on(L"attr_statusbar_hl");
			printw(" ]");
			core::io.ui.off();
			return;
		}
		printw(" (");
		addwstr(omg.c_str());
		printw(")");
	}
	core::io.ui.on(L"attr_statusbar_hl");
	printw(" ]");
	tmp = gactive() ? name : L"---";
	max_x -= 5 + tmp.size();
	if (max_x < 0) {
		core::io.ui.off();
		return;
	}
	core::io.ui.off();
	core::io.ui.on(L"attr_statusbar_placeholder");
	printw(" ");
	core::io.ui.off();
	core::io.ui.on(L"attr_statusbar_hl");
	printw("[ ");
	core::io.ui.off();
	core::io.ui.on(L"attr_statusbar_base");
	addwstr(tmp.c_str());
	core::io.ui.off();
	core::io.ui.on(L"attr_statusbar_hl");
	max_x -= 4 + (gactive() ? std::to_wstring(connno()).size() + std::to_wstring(clientz).size() : 2);
	if (max_x < 0) {
		printw(" ]");
		core::io.ui.off();
		return;
	}
	printw(" | ");
	core::io.ui.off();
	core::io.ui.on(L"attr_statusbar_base");
	gactive() ? printw("%d", connno()) : printw("?");
	printw("/");
	gactive() ? printw("%d", clientz) : printw("?");
	core::io.ui.off();
	core::io.ui.on(L"attr_statusbar_hl");
	printw(" ]");
	core::io.ui.off();
}

void serv::status::draw()
{
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	draw(max_x, max_y);
}

std::wstring serv::status::gnick()
{
	return nick;
}

std::wstring serv::status::gname()
{
	return name;
}

int serv::status::connno()
{
	return otherz.size() + (nick.size() ? 1 : 0);
}

void serv::status::gotherz(std::vector<std::wstring> &trg)
{
	trg = otherz;
}

void serv::status::join(std::wstring da_usr)
{
	if (std::find(otherz.begin(), otherz.end(), da_usr) != otherz.end())
		return;
	if (da_usr != nick)
		otherz.push_back(da_usr);
}

void serv::status::leave(std::wstring da_usr)
{
	if (connno() <= 0)
		return;
	int pos = std::find(otherz.begin(), otherz.end(), da_usr) - otherz.begin();
	if (pos == otherz.size())
		return;
	if (da_usr != nick)
		otherz.erase(otherz.begin() + pos);
}

void serv::status::chomg(wchar_t perm, uint8_t val)
{
	uint8_t pos;
	switch (perm) {
	case L'o':
		pos = 0;
		break;
	case L'm':
		pos = 1;
		break;
	case L'g':
		pos = 2;
		break;
	default:
		return;
	}
	wchar_t da_val = L'-';
	if (val == 1) {
		da_val = perm;
	} else if (val == 2) {
		switch (perm) {
		case L'o':
			da_val = L'O';
			break;
		case L'm':
			da_val = L'M';
			break;
		case L'g':
			da_val = L'G';
			break;
		}
	}
	omg[pos] = da_val;
	core::io.mkwin();
}

void serv::ignore::glist(std::vector<std::wstring> &trg)
{
	trg = list;
}

bool serv::ignore::ignored(std::wstring nicc)
{
	if (nicc != core::serv.status.gnick() && std::find(list.begin(), list.end(), nicc) != list.end())
		return true;
	return false;
}

uint8_t serv::ignore::add(std::wstring nicc)
{
	if (nicc.size() < 3 || nicc.size() > 15)
		return 2;
	if (nicc == L"kewl" || nicc == L"serv")
		return 3;
	if (ignored(nicc))
		return 1;
	list.push_back(nicc);
	return 0;
}

uint8_t serv::ignore::remove(std::wstring nicc)
{
	int pos = std::find(list.begin(), list.end(), nicc) - list.begin();
	if (pos == list.size())
		return 1;
	list.erase(list.begin() + pos, list.begin() + pos + 1);
	return 0;
}

uint8_t serv::ignore::clear()
{
	if (list.size() == 0)
		return 1;
	list.clear();
	return 0;
}

serv::ping::ping()
:id(0)
{
}

uint8_t serv::ping::do_it(uint8_t da_n)
{
	if (da_n < 1 || da_n > 64)
		return 2;
	if (!core::serv.status.gactive())
		return 1;
	if (n != 0)
		return 3;
	id = (id + 1) % 100;
	n = da_n;
	sum.tv_sec = 0;
	sum.tv_usec = 0;
	seq = 0;
	core::io << core::io::msg(L"kewl", L"PING \"" + core::serv.status.gname() + L"\" 44 bytez packet/z");
	snd();
	return 0;
}

void serv::ping::snd()
{
	std::wstring da_id(std::to_wstring(id));
	da_id.insert(0, std::wstring(3 - da_id.size(), L'x') + L' ');
	seq++;
	gettimeofday(&tm, 0);
	try {
		core::serv.snd(L"/ping " + da_id);
	} catch (...) {}
}

void serv::ping::recv(uint8_t da_id)
{
	if (n == 0 || da_id != id)
		return;
	timeval tmp;
	gettimeofday(&tmp, 0);
	tmp.tv_sec -= tm.tv_sec;
	tmp.tv_usec -= tm.tv_usec;
	if (tmp.tv_usec < 0) {
		tmp.tv_sec--;
		tmp.tv_usec += 1000000;
	}
	sum.tv_sec += tmp.tv_sec;
	sum.tv_usec += tmp.tv_usec;
	if (sum.tv_usec > 1000000) {
		sum.tv_sec++;
		sum.tv_usec -= 1000000;
	}
	unsigned long long int ms = tmp.tv_sec;
	ms *= (unsigned long long int)10000;
	ms += (unsigned long long int)tmp.tv_usec / (unsigned long long int)100;
	if (ms < min || seq == 1)
		min = ms;
	if (ms > max || seq == 1)
		max = ms;
	core::io << core::io::msg(L"kewl", L"64 bytez from \"" + core::serv.status.gname() + L"\"; seq=\\1" + std::to_wstring(seq) + L"\\0 time=\\1" + std::to_wstring(ms / 10) + L'.' + std::to_wstring(ms % 10) + L"\\0ms");
	if (seq >= n) {
		statz();
		return;
	}
	usleep(CLOCKS_PER_SEC);
	if (n == 0 || da_id != id)
		return;
	snd();
}

void serv::ping::statz()
{
	if (n == 0)
		return;
	core::io << core::io::msg(L"kewl", L'"' + core::serv.status.gname() + L"\"'z ping statz:");
	unsigned long long int ms = sum.tv_sec;
	ms *= (unsigned long long int)10000;
	ms += (unsigned long long int)sum.tv_usec / (unsigned long long int)100;
	ms /= n;
	core::io << core::io::msg(L"kewl", L"\\1" + std::to_wstring(n) + L"\\0 packet/z, min=\\1" + std::to_wstring(min / 10) + L'.' + std::to_wstring(min % 10) + L"\\0, avg=\\1" + std::to_wstring(ms / 10) + L'.' + std::to_wstring(ms % 10) + L"\\0ms, max=\\1" + std::to_wstring(max / 10) + L'.' + std::to_wstring(max % 10) + L"\\0ms");
	n = 0;
}

uint8_t serv::ping::cancel()
{
	if (n == 0)
		return 1;
	n = 0;
	core::io << core::io::msg(L"kewl", L"WARN: ping process interrupted");
	return 0;
}
