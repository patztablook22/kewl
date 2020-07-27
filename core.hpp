namespace core {

/*********************************************************************************************/

class io {
public:
	io(), ~io();
	void init();

	bool iz_k(wint_t);
	bool iz_k(std::wstring);
	bool iz_k(std::string); // check if itz allowed to use => only aZ, 09, аЯ, áŽ, ...
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
	void cls(), reset();
	void beep(bool), beep(), beep_sdelay(unsigned int), beep_son(bool), mkwin();
	bool beep_gon();
	int beep_gdelay();
private:
	void _mkwin();
	std::mutex scr_mtx;
	bool beep_on;
	int beep_delay;
	timeval beep_tm;

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
		friend void io::cls(), io::reset();
		int glen(), gpos0(), gpos1();
		int glinez(int, int, int);
		std::wstring gtitle();
		void echo(int, int, int, int, int);
		void scrll(bool), jump(bool);
		void scrll_resize(), scrll_chk(int, int);
		bool new_msg = false;
	private:
		msg msgz[320];
		std::wstring title;
		int len, pos0, pos1, pos2;
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

class perform {
public:
	class func {
	public:
		virtual int usr(std::vector<std::wstring>) = 0;
	};

	class func_handler {
	public:
		func_handler(std::wstring, func *);
		func *gptr();
		std::wstring gdesc();
	private:
		std::wstring desc;
		func *ptr;		
	};
	void add(std::wstring, std::wstring, func *);
	void gfuncz(std::vector<std::wstring> &);
	std::wstring gdesc(std::wstring);
	bool iz_func(std::wstring);
	void operator<<(std::wstring);
private:
	std::map<std::wstring, func_handler *> funcz;
} perform;

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
#include "core/perform.cpp"
#include "core/serv.cpp"

namespace cmdz {
	#include "core/cmdz.cpp"
}
namespace funcz {
	#include "core/funcz.cpp"
}

}
