namespace core {

/*********************************************************************************************/

class io {
public:
	io(), ~io();

	bool iz_k(wint_t);
	bool iz_k(std::wstring); // check if itz allowed to use => only aZ, 09, аЯ, áŽ, ...
	std::wstring trim(std::wstring);
	std::wstring ver_echo(int);

	class msg {
	public:
		msg(), msg(std::wstring, std::wstring);

		void set(std::wstring, std::wstring);
		void operator=(msg);

		std::wstring gfrom(), gbody();
		int gcol0(), gcol1(), gw0();
		bool gvalid();
	private:
		std::wstring from, body;
		int col0, col1, w0;
		bool valid;
	};
	
	void operator>>(std::wstring &);
	void operator<<(msg);
	void send_passwd();
	void cls(), mkwin();
private:
	void _mkwin();
	std::mutex scr_mtx;

	class i {
	public:
		i();
		friend void io::operator>>(std::wstring &);
		friend void io::send_passwd();
		void draw();
		void draw(int, int);
		void _draw(int, int);
	private:
		bool use_normal, use_passwd;
		std::wstring buf, prompt_normal, prompt_passwd;
		size_t pos, begin;

		class acompl {
		public:
			acompl();
			void reset();
			void get();
		private:
			std::vector<std::wstring> possibz;
			int iter;
			std::wstring prez;
			bool second;
		} acompl;

		class clpbrd {
		public:
			void cut(bool);
			void paste();
		private:
			std::wstring buf;
		} clpbrd;

		class hist {
		public:
			void buftohist();
			void histtobuf(bool);
		private:
			std::wstring body[32];
			int pos, len = 1;
		} hist;
	} i;

	class o {
	public:
		o();
		friend void io::operator<<(msg);
		friend void io::cls();
		int glen(), gpos0(), gpos1();
		int glinez(int, int, int);
		std::wstring gtitle();
		void echo(int, int, int, int, int);
		void scrll(bool);
		void jump(bool);
		void scrll_resize();
		bool new_msg = false;
	private:
		msg msgz[320];
		std::wstring title;
		int len, pos0, pos1;
	} o;

} io;

/*********************************************************************************************/

class exec {
public:
	class cmd {
	public:
		virtual int usr(std::vector<std::wstring>) = 0;
		virtual int serv(std::vector<std::wstring>);
	};

	class cmd_handler {
	public:
		cmd_handler(std::wstring, std::wstring, cmd *, std::wstring);
		std::wstring gsyn(), gdesc();
		wint_t gacompl(int);
		cmd *gptr();
	private:
		std::wstring syn, desc;
		cmd *ptr;
		std::wstring acompl;
	};

	void add(std::wstring, std::wstring, std::wstring, cmd*);
	void add(std::wstring, std::wstring, std::wstring, cmd *, std::wstring);
	
	class usr {
	public:
		void operator<<(std::wstring);
	} usr;

	class serv {
	public:
		void operator<<(std::wstring);
	} serv;

	void gcmdz(std::vector<std::wstring> &);
	std::wstring gsyn(std::wstring), gdesc(std::wstring);
	wint_t gacompl(std::wstring, int);
	bool iz_cmd(std::wstring);
private:
	std::vector<std::wstring> interpreter(std::wstring);
	std::map<std::wstring, cmd_handler *> cmdz;
} exec;

/*********************************************************************************************/

class serv {
public:
	serv();
	friend void io::operator>>(std::wstring &);
	void conn(std::string, std::string, std::wstring);
	int disconn(bool);
	void operator<<(core::io::msg);

private:
	void operator<<(std::wstring);
	void operator<<(int);
	void operator>>(int &);
	void operator>>(std::wstring &);
	void operator>>(core::io::msg &);
	void handler(std::string, std::string, std::wstring);
	int open_conn(const char *, const char *);
	SSL_CTX *init_ctx();
	void certz_echo(SSL *);
	SSL_CTX *ctx;
	SSL *ssl;
	std::wstring hoi_msg, boi_msg;
	std::thread sniffer;
	bool free;
public:
	class status {
	public:
		status();
		friend void serv::handler(std::string, std::string, std::wstring);
		friend int serv::disconn(bool);
		void draw(int, int), draw();
		std::wstring gnick();
		int connno();
		void gotherz(std::vector<std::wstring> &);
		bool gactive();
		void join(std::wstring), leave(std::wstring);
	private:
		std::wstring nick, name;
		std::vector<std::wstring> otherz;
		unsigned int clientz;
	} status;
} serv;

/*********************************************************************************************/


#include "core/io.cpp"
#include "core/exec.cpp"
#include "core/serv.cpp"
#include "core/cmdz.cpp"
}
