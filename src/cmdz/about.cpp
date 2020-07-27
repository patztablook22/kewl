class about: public core::exec::cmd {

  public:

    about()
    {
      core::exec.add(L"about", {{L"", L"wtf iz kewl about"}}, this);
    }

    uint8_t usr (std::vector<std::wstring> arg)
    {
      core::io << core::io::msg(L"hr", L"");
      core::io << L"Konverzace Everybody Will Like client";
      core::io << L"C++ IRC with SSL encryption";
      core::io << L"kewl", L"\"lol imma waste my time\" ~ patz, d6022";
      core::io << core::io::msg(L"hr", L"");

      return 0;
    }

} about;
