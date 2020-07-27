serv::serv()
:ssl(NULL)
{
	SSL_library_init();
	ctx = init_ctx();
	free = true;
}

void serv::operator<<(core::io::msg todo)
{
	if (!todo.gvalid())
		return;
	*this << todo.gfrom() + L' ' + todo.gbody();
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

void serv::conn(std::string hostname, std::string port, std::wstring usrz)
{
	if (!free) {
		core::io << core::io::msg(L"kewl", L"ERR: disconn first");
		return;
	}
	free = false;

	sniffer = std::thread(&serv::handler, this, hostname, port, usrz);
	sniffer.detach();
}

int serv::disconn(bool e = true)
{
	if (free && e) {
		core::io << core::io::msg(L"kewl", L"ERR: no connection alive");
		return 1;
	}
	if (ssl == NULL) {
		if (!core::serv.status.gactive() && e)
			core::io << core::io::msg(L"kewl", L"connecting process interrupted");
		free = true;
		return 1;
	}
	SSL *tmp = ssl;
	ssl = NULL;
	int sockfd = SSL_get_fd(tmp);
	SSL_shutdown(tmp);
	SSL_shutdown(tmp);
	close(sockfd);
	std::wstring tmp_nick = core::serv.status.nick;
	core::serv.status.clientz = 0;
	core::serv.status.name.clear();
	core::serv.status.nick.clear();
	if (!e) {
		core::io << core::io::msg(L"kewl", L"ERR: connection lost");
	} else {
		if (boi_msg.size() != 0)
			core::io << core::io::msg(L"serv", L'@' + tmp_nick + L' ' + boi_msg);
		core::io << core::io::msg(L"kewl", L"connection closed");
	}
	hoi_msg.clear();
	boi_msg.clear();
	free = true;
	return 0;
}

void serv::operator>>(core::io::msg &trg)
{
	std::wstring buf;
	*this >> buf;
	int pos = buf.find(32);
	if (pos == std::wstring::npos || pos == 0 || pos == buf.size())
		throw 1;
	trg = core::io::msg(buf.substr(0, pos), buf.substr(pos + 1, buf.size() - pos - 1));
}

void serv::operator>>(std::wstring &trg)
{
	if (ssl == NULL)
		throw 1;
	wchar_t buf[1024];
	int bytez = SSL_read(ssl, &buf, sizeof(buf));
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

void serv::handler(std::string hostname, std::string port, std::wstring usrz)
{
	try {
		core::io << core::io::msg(L"kewl", L"resolving " + std::wstring(hostname.begin(), hostname.end()) + L':' + std::wstring(port.begin(), port.end()) + L"...");
	
		int bytez, sockfd = open_conn(hostname.c_str(), port.c_str());
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
		core::io << core::io::msg(L"serv", std::wstring(tmp.begin(), tmp.end()) + L" encrypted");
		if (buf1[0] == 'p') {
			core::io << core::io::msg(L"kewl", L"server requests passwd");
			for (;;) {
				core::io.send_passwd();
				*this >> buf1;
				
				if (buf1[0] == 'p' || buf1[0] == 'a') {
					core::io << core::io::msg(L"serv", L"WARN: passwd incorrect");
					if (buf1[0] == 'a') {
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
		if (buf1[0] != 's') {
			disconn();
			return;
		}
		*this << usrz;
		*this >> buf1;
		switch (buf1[0]) {
		case 'k':
			if (buf1.size() > 16 || buf1.size() < 3 || !core::io.iz_k(buf1)) {
				disconn();
				return;
			}
			core::serv.status.nick = buf1.substr(1, buf1.size() - 1);
			break;
		case 'f':
			core::io << core::io::msg(L"serv", L"ERR: chatroom iz full");
			disconn();
			return;
		case 'n':
			core::io << core::io::msg(L"serv", L"ERR: all of da entered nickz are already in use");
			disconn();
			return;
		}
		*this << L"ready";

		core::serv.status.otherz.clear();
		for (bool i = true; i;) {
			*this >> buf1;
			*this << L"kk";
			if (buf1[buf1.size() - 1] == L',') {
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

		*this << L"kk";
		*this >> buf1;
		hoi_msg = buf1.erase(0, 1);
		*this << L"kk";
		*this >> buf1;
		boi_msg = buf1.erase(0, 1);
		if (hoi_msg.size() != 0)
			core::io << core::io::msg(L"serv", L'@' + core::serv.status.nick + L' ' + hoi_msg);

		*this << L"sniffing";
		core::io::msg buf2;
		for (;;) {
			*this >> buf2;
			if (buf2.gfrom() != core::serv.status.nick && buf2.gfrom() != L"kewl" && buf2.gbody()[0] != L'/')
				core::io.beep();
			if (buf2.gbody()[0] == L'/' && buf2.gfrom() == L"serv")
				core::exec.serv << buf2.gbody().substr(1, buf2.gbody().size() - 1);
			else
				core::io << buf2;
		}
	} catch (...) {
		disconn(false);
	}
}
	
int serv::open_conn(const char *hostname, const char *port)
{
	int sd;
	struct addrinfo hints, *ai, *tmp;
	struct sockaddr_in addr;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;	// auto IPv4/IPV6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	
	if (getaddrinfo(hostname, port, &hints, &ai) != 0) {
		core::io << core::io::msg(L"kewl", L"ERR: resolving failed");
		return 0;
	}

	sd = socket(AF_INET, SOCK_STREAM, 0);
	
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
	name.clear();
	otherz.clear();
	clientz = 0;
}

bool serv::status::gactive()
{
	return (clientz > 0);
}

void serv::status::draw(int max_x, int max_y)
{
	std::wstring tmp;
	core::io.ui.on(L"attr_frame_base");
	mvprintw(max_y - 2, 0, "%*s", max_x, "");
	core::io.ui.off(L"attr_frame_base");
	tmp = gactive() ? nick : L"---";
	max_x -= 11 + tmp.size();
	if (max_x < 0)
		return;
	move(max_y - 2, 1);
	core::io.ui.on(L"attr_frame_hl");
	printw("[ ");
	core::io.ui.off(L"attr_frame_hl");
	core::io.ui.on(L"attr_frame_base");
	addwstr(tmp.c_str());
	core::io.ui.off(L"attr_frame_base");
	core::io.ui.on(L"attr_frame_hl");
	printw(" ]");
	tmp = gactive() ? name : L"---";
	max_x -= 5 + tmp.size();
	if (max_x < 0) {
		core::io.ui.off(L"attr_frame_hl");
		return;
	}
	printw(" [ ");
	core::io.ui.off(L"attr_frame_hl");
	core::io.ui.on(L"attr_frame_base");
	addwstr(tmp.c_str());
	core::io.ui.off(L"attr_frame_base");
	core::io.ui.on(L"attr_frame_hl");
	max_x -= 4 + (gactive() ? std::to_wstring(connno()).size() + std::to_wstring(clientz).size() : 2);
	if (max_x < 0) {
		printw(" ]");
		core::io.ui.off(L"attr_frame_hl");
		return;
	}
	printw(" | ");
	core::io.ui.off(L"attr_frame_hl");
	core::io.ui.on(L"attr_frame_base");
	gactive() ? printw("%d", connno()) : printw("?");
	core::io.ui.off(L"attr_frame_base");
	core::io.ui.on(L"attr_frame_hl");
	printw("/");
	core::io.ui.off(L"attr_frame_hl");
	core::io.ui.on(L"attr_frame_base");
	gactive() ? printw("%d", clientz) : printw("?");
	core::io.ui.off(L"attr_frame_base");
	core::io.ui.on(L"attr_frame_hl");
	printw(" ]");
	core::io.ui.off(L"attr_frame_hl");
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
	if (connno() >= clientz)
		return;
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
