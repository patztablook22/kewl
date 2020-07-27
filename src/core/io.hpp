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
