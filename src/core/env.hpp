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
