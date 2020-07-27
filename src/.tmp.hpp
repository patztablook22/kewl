namespace basis {
#include "basis/cursed.cpp"
}
namespace core {
class env {
public:
	env();
	void init();
	void on(std::wstring), off();
	std::wstring gstr(std::wstring), grange(std::wstring);
	void gattrz(std::wstring, std::vector<std::wstring> &);
	int gint(std::wstring);
	bool gtru(std::wstring);
	wint_t gchar(std::wstring);
	uint8_t set(std::wstring, std::vector<std::wstring>), set(std::wstring, std::wstring), set(std::wstring, wint_t), set(std::wstring, int);
	uint8_t def_col(std::wstring, std::vector<unsigned int>);
	uint8_t reset(std::wstring);
	void reset();
	void gcolorz(std::vector<std::wstring> &), gweightz(std::vector<std::wstring> &), gpropertiez(std::vector<std::wstring> &);
	class ATT {
	public:
		ATT(std::wstring, std::vector<std::wstring>);
		void on(), off();
		uint8_t svalz(std::vector<std::wstring>);
		void reset();
		void gvalz(std::vector<unsigned int> &);
	private:
		unsigned int valz[3], defaultz[3];
		unsigned int id;
	};
		class STR {
	public:
		STR(std::wstring, unsigned int, unsigned int, std::wstring);
		std::wstring gval(), grange();
		uint8_t sval(std::wstring);
		void reset();
	private:
		unsigned int range[2];
		std::wstring val, da_default;
	};
		class CHR {
	public:
		CHR(std::wstring, wint_t);
		wint_t gval();
		uint8_t sval(wint_t);
		void reset();
	private:
		wint_t val, da_default;
	};
		class NUM {
	public:
		NUM(std::wstring, int, int, int);
		int gval();
		std::wstring grange();
		uint8_t sval(int);
		void reset();
	private:
		int range[2];
		int val, da_default;
	};
		class TRU {
	public:
		TRU(std::wstring, std::wstring);
		bool gval();
		uint8_t sval(std::wstring);
		void reset();
	private:
		bool val, da_default;
	};
	private:
	bool existz(std::wstring);
	std::map<std::wstring, unsigned int> colorz, weightz;
	std::map<std::wstring, ATT *> ATTz;
	std::map<std::wstring, STR *> STRz;
	std::map<std::wstring, CHR *> CHRz;
	std::map<std::wstring, NUM *> NUMz;
	std::map<std::wstring, TRU *> TRUz;
	std::wstring active;
} env;
class io {
public:
	io(), ~io();

	void init(bool);
	void sascii();
	bool iz_k(wint_t), iz_k(std::wstring), iz_k(std::string);
	std::wstring sha256(std::wstring);
	std::wstring trim(std::wstring);
	std::wstring ver_echo(int);

	class msg {
	public:
		msg(), msg(std::wstring, std::wstring);

		void set(std::wstring, std::wstring);
		void operator=(msg);

		std::wstring grecv(), gfrom(), gbody();
		std::wstring gprop0();
		uint8_t glen();
		bool gvalid();
	private:
		std::wstring recv, from, body;
		std::wstring prop0;
		uint8_t len;
		bool valid;
	};
	
	void ready();
	void operator>>(std::wstring &);
	void operator<<(msg), operator<<(std::wstring);
	void passwd();
	void cls(), da_erase(), mkwin();
	uint8_t beep();

	class stfu {
	public:
		stfu();
		bool gon();
		void toggle();
	private:
		bool on[2];
	} stfu;

	class myside {
	public:
		myside();
		bool operator==(const bool);
	private:
		std::thread::id meesa;
	} myside;

private:
	void _mkwin();
	std::mutex scr_mtx;
	timeval beep_tm;

	class i {
	public:
		friend void io::operator>>(std::wstring &);
		friend void io::passwd();
		void draw();
		void draw(int, int);
		void _draw(int, int);
	private:
		bool use_normal, use_passwd;
		std::wstring buf;
		size_t pos, begin;

		void help();

		class bind {
		public:
			std::wstring ed(uint32_t);
			void set(uint32_t, std::wstring);
			void del(uint32_t);
			void z(std::vector<uint32_t> &);
		private:
			std::map<uint32_t, std::wstring> _bindz;
		} bind;

		class acompl {
		public:
			acompl();
			void reset();
			void get();
			bool move(bool);
			void quarry();
			void popup(int, int);
		private:
			std::vector<std::wstring> possibz;
			int iter, pos, len, size;
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
			hist();
			void prepare();
			void buftohist();
			void histtobuf(bool);
		private:
			std::vector<std::wstring> da_hist;
			int pos, len;
		} hist;
	} i;

	class o {
	public:
		o();
		friend void io::operator<<(msg);
		friend void io::cls(), io::da_erase();
		int glen(), gpos0(), gpos1();
		int glinez(int, int, int);
		std::wstring gtitle();
		void echo(int, int, int, int, int);
		void scrll(bool), jump(bool);
		void scrll_resize(), scrll_chk(int, int);
		bool new_msg;
	private:
		std::vector<msg> msgz;
		std::wstring title;
		int pos0, pos1, pos2;
	} o;

	short int default_contentz[8][3];
	bool ascii, iz_ready;
} io;
class exec {

public:

	size_t interpreter (std::wstring, std::vector<std::wstring> &, size_t = -1);
	size_t escape (std::vector<std::wstring>, std::wstring &, size_t = -1);

	class cmd {

	public:

		virtual uint8_t usr (std::vector<std::wstring>) = 0;
		virtual uint8_t serv (std::vector<std::wstring>);
		virtual void acompl (std::vector<std::wstring>, std::vector<std::wstring> &);

	};

	class cmd_handler {

	public:

		cmd_handler (std::vector<std::vector<std::wstring>>, cmd *);
		cmd *ptr();

		void man (std::vector<std::vector<std::wstring>> &);
		void acompl (std::vector<std::wstring>, std::vector<std::wstring> &);

	private:

		std::vector<std::vector<std::wstring>> _man;
		cmd *_ptr;

	};

	void add (std::wstring, std::vector<std::vector<std::wstring>>, cmd *);
	void operator << (std::wstring);

private:
	void _exec_k(std::wstring), _exec_s(std::wstring);
public:

	void cmdz(std::vector<std::wstring> &);
	void man(std::wstring, std::vector<std::vector<std::wstring>> &);
	void acompl(std::vector<std::wstring>, std::vector<std::wstring> &);
	bool cmd(std::wstring);

	class macroz {
	public:
		macroz();
		void init(), load();
		friend void exec::_exec_k(std::wstring);
		void mcrz(std::vector<std::wstring> &);
	private:
		class macro {
		public:
			macro(std::wstring, std::vector<std::wstring> &);
			void do_it();
		private:
			std::vector<std::wstring> content;
		};
		bool mcr(std::wstring);
		std::string location;
		std::map<std::wstring, macro *> _mcrz;
		std::vector<std::wstring> called;

	} macroz;

private:
	std::map<std::wstring, cmd_handler *> _cmdz;
} exec;
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
class cmdl {
public:
	class opt {
	public:
		virtual void usr(std::wstring) = 0;
	};

	class opt_handler {
	public:
		opt_handler(std::wstring, opt *);
		opt *gptr();
		std::wstring gdesc();
	private:
		std::wstring desc;
		opt *ptr;		
	};
	void add(std::wstring, std::wstring, opt *);
	void goptz(std::vector<std::wstring> &);
	std::wstring gdesc(std::wstring);
	bool iz_opt(std::wstring);
	void operator<<(std::wstring);
private:
	std::map<std::wstring, opt_handler *> optz;
} cmdl;
#include "core/env/var_tru.cpp"
#include "core/env/var_str.cpp"
#include "core/env/var_num.cpp"
#include "core/env/var_chr.cpp"
#include "core/env/var_att.cpp"
#include "core/env/initz.cpp"
#include "core/env/env.cpp"
#include "core/io/stfu.cpp"
#include "core/io/operator_ostream.cpp"
#include "core/io/operator_istream.cpp"
#include "core/io/o.cpp"
#include "core/io/myside.cpp"
#include "core/io/msg.cpp"
#include "core/io/mkwin.cpp"
#include "core/io/io.cpp"
#include "core/io/i.cpp"
#include "core/io/erase.cpp"
#include "core/io/bind.cpp"
#include "core/io/beep.cpp"
#include "core/exec/operator_istream.cpp"
#include "core/exec/macroz.cpp"
#include "core/exec/interpreter.cpp"
#include "core/exec/exec.cpp"
#include "core/exec/cmd_handler.cpp"
#include "core/serv/serv.cpp"
#include "core/cmdl/opt_handler.cpp"
#include "core/cmdl/cmdl.cpp"
}
namespace cmdz {
#include "cmdz/version.cpp"
#include "cmdz/usrz.cpp"
#include "cmdz/tree.cpp"
#include "cmdz/servctl.cpp"
#include "cmdz/say.cpp"
#include "cmdz/reset.cpp"
#include "cmdz/redraw.cpp"
#include "cmdz/quit.cpp"
#include "cmdz/ping.cpp"
#include "cmdz/macroz.cpp"
#include "cmdz/hr.cpp"
#include "cmdz/help.cpp"
#include "cmdz/erase.cpp"
#include "cmdz/env.cpp"
#include "cmdz/disconn.cpp"
#include "cmdz/conn.cpp"
#include "cmdz/cls.cpp"
#include "cmdz/bind.cpp"
#include "cmdz/beep.cpp"
#include "cmdz/about.cpp"
}
namespace optz {
#include "optz/version.cpp"
#include "optz/help.cpp"
#include "optz/debug.cpp"
#include "optz/ascii.cpp"
#include "optz/about.cpp"
}
