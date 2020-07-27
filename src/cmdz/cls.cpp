class cls: public core::exec::cmd {

  public:

    cls()
    {
      core::exec.add(L"cls", {{L"", L"clear screen"}}, this);
    }

    uint8_t usr (std::vector<std::wstring> arg)
    {
      if (arg.size() != 1)
        return 1;

      core::io.cls();
      return 0;
    }

} cls;
