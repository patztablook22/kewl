class about: public core::cmdl::opt {
public:
	about()
	{
		core::cmdl.add(L"about", L"wtf iz kewl about", this);
	}

	void usr(std::wstring self)
	{
		std::wcout << L"Konverzace Everybody Will Like client" << std::endl;
		std::wcout << L"C++ IRC with SSL encryption" << std::endl;
		std::wcout << L"\"lol imma waste my time\" ~ patz, d6022" << std::endl;
		exit(EXIT_SUCCESS);
	}
} about;
