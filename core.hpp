namespace core {

/*********************************************************************************************/
class io {
public:
	io(), ~io();

	class ui {
	public:
		ui();
		void init();
		void on(std::wstring), off(std::wstring);
		std::wstring gstr(std::wstring), grange(std::wstring);
		void echo(std::wstring, unsigned int);
		uint8_t set(std::wstring, std::vector<std::wstring>), set(std::wstring, std::wstring), set (std::wstring, wint_t);
		uint8_t reset(std::wstring);
		void reset();

		void gcolorz(std::vector<std::wstring> &), gweightz(std::vector<std::wstring> &), gpropertiez(std::vector<std::wstring> &);

		class property0 {
		public:
			property0(std::wstring, std::vector<std::wstring>);
			void on(), off();
			uint8_t svaluez(std::vector<std::wstring>);
			void reset();
		private:
			unsigned int valuez[3], defaultz[3];
			unsigned int id;
		};

		class property1 {
		public:
			property1(std::wstring, unsigned int, unsigned int, std::wstring);
			std::wstring gstr(), grange();
			uint8_t svalue(std::wstring);
			void reset();
		private:
			unsigned int range[2];
			std::wstring value, da_default;
		};

		class property2 {
		public:
			property2(std::wstring, wint_t);
			void echo(unsigned int);
			uint8_t svalue(wint_t);
			void reset();
		private:
			wint_t value, da_default;
		};

	private:
		bool existz(std::wstring);
		std::map<std::wstring, unsigned int> colorz, weightz;
		std::map<std::wstring, property0 *> propertiez0;
		std::map<std::wstring, property1 *> propertiez1;
		std::map<std::wstring, property2 *> propertiez2;
	} ui;

	void init(bool = true);

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
		std::wstring gprop0(), gprop1();
		int gw0();
		bool gvalid();
	private:
		std::wstring from, body;
		std::wstring prop0, prop1;
		int w0;
		bool valid;
	};
	
	void operator>>(std::wstring &);
	void operator<<(msg);
	void send_passwd();
	void cls(), da_erase(), reset();
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
		friend void io::operator>>(std::wstring &);
		friend void io::send_passwd();
		void draw();
		void draw(int, int);
		void _draw(int, int);
	private:
		bool use_normal, use_passwd;
		std::wstring buf;
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
		friend void io::cls(), io::da_erase(), io::reset();
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
		virtual uint8_t usr(std::vector<std::wstring>) = 0;
		virtual void acompl(std::vector<std::wstring>, std::vector<std::wstring> &);
		virtual uint8_t serv(std::vector<std::wstring>);
	};

	class cmd_handler {
	public:
		cmd_handler(std::vector<std::vector<std::wstring>>, cmd *);
		void gman(std::vector<std::vector<std::wstring>> &);
		void gacompl(std::vector<std::wstring>, std::vector<std::wstring> &);
		cmd *gptr();
	private:
		std::vector<std::vector<std::wstring>> man;
		cmd *ptr;
	};

	void add(std::wstring, std::vector<std::vector<std::wstring>>, cmd*);
	
	class usr {
	public:
		void operator<<(std::wstring);
	} usr;

	class serv {
	public:
		void operator<<(std::wstring);
	} serv;

	void gcmdz(std::vector<std::wstring> &);
	void gman(std::wstring, std::vector<std::vector<std::wstring>> &);
	void cmd_gacompl(std::wstring, std::vector<std::wstring> &);
	bool iz_cmd(std::wstring);

	class macroz {
	public:
		macroz();
		void init(), load();
		friend void exec::usr::operator<<(std::wstring);
		void gmacroz(std::vector<std::wstring> &);
	private:
		class macro {
		public:
			macro(std::wstring, std::vector<std::wstring> &);
			void do_it();
		private:
			std::vector<std::wstring> content;
		};
		bool existz(std::wstring);
		std::string location;
		std::map<std::wstring, macro *> da_macroz;
		std::vector<std::wstring> done;

	} macroz;

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
