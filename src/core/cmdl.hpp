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
