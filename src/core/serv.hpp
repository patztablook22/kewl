class serv {
public:
	serv(), ~serv();
	friend void io::operator>>(std::wstring &);
	void conn(std::string, uint16_t, std::wstring);
	int disconn(bool);
	void snd(std::wstring);

private:
	void operator<<(std::wstring);
	void operator<<(int);
	void operator>>(int &);
	void operator>>(std::wstring &);
	void operator>>(core::io::msg &);
	void handler(std::string, uint16_t, std::wstring);
	int open_conn(const char *, uint16_t);
	SSL_CTX *init_ctx();
	void certz_echo(SSL *);
	SSL_CTX *ctx;
	SSL *ssl;
	std::thread sniffer;
	bool free;
public:
	class status {
	public:
		status();
		friend void serv::handler(std::string, uint16_t, std::wstring);
		friend int serv::disconn(bool);
		void draw(int, int), draw();
		std::wstring gnick(), gname();
		int connno();
		void gotherz(std::vector<std::wstring> &);
		bool gactive();
		void join(std::wstring), leave(std::wstring);
		void chomg(wchar_t, uint8_t);
	private:
		std::wstring nick, name, omg;
		std::vector<std::wstring> otherz;
		bool active;
		unsigned int clientz;
	} status;

	class ignore {
	public:
		void glist(std::vector<std::wstring> &list);
		bool ignored(std::wstring);
		uint8_t add(std::wstring), remove(std::wstring), clear();
	private:
		std::vector<std::wstring> list;
	} ignore;

	class ping {
	public:
		ping();
		uint8_t do_it(uint8_t);
		uint8_t cancel();
		void recv(uint8_t);
	private:
		void snd(), statz();
		uint8_t n, seq;
		timeval tm, sum;
		long long int min, max;
		uint32_t id;
	} ping;
} serv;
