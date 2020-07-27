class beep: public core::exec::cmd {

  public:

    beep()
    {
      core::exec.add(L"beep", {{L"", L"test da beep"}}, this);
    }

    uint8_t usr (std::vector<std::wstring> arg)
    {
      if (arg.size() != 1)
        return 1;

      switch (core::io.beep()) {

        case 1:
          core::io << L"*beep is off*";
          break;
        case 2:
          core::io << L"*delay not elapsed*";
          break;
        case 0:
          core::io << L"*beep*";
          break;

      }

      return 0;

    }

} beep;
