class disconn: public core::exec::cmd {

  public:

    disconn()
    {
      core::exec.add(L"disconn", {{L"", L"disconnect from kewl serv"}}, this);
    }

    uint8_t usr (std::vector<std::wstring> arg)
    {
      if (arg.size() != 1)
        return 1;

      try {
        core::serv.snd(L"/disconn");
      } catch (...) {
        core::serv.disconn();
      }

      return 0;
    }

    uint8_t serv (std::vector<std::wstring> arg)
    {
      if (arg.size() == 1)
        return core::serv.disconn(false);

      if (arg[1] == L"s")
        core::io << core::io::msg(L"serv", L"imma shutdown");

      else if (arg[1] == L"k")
        core::io << core::io::msg(L"serv", L"u were kicked out :'(");

      if (arg.size() == 3)
        core::io << core::io::msg(L"serv", L"@" + core::serv.status.gnick() + L" " + arg[2]);

      core::io.beep();
      return core::serv.disconn();
    }

} disconn;
