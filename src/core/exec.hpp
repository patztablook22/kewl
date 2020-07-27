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
