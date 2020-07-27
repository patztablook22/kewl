class conn: public core::exec::cmd {

  public:

    conn()
    {
      core::exec.add(L"conn", {{L"address:port nick", L"connect 2 kewl serv", L"using port 12204, if not specified", L"using ur usrname, if not specified"}}, this);
    }

    uint8_t usr (std::vector<std::wstring> arg)
    {
      if (arg.size() < 2)
        return 1;

      else if (arg.size() > 3)
        return 3;

      int pos = arg[1].find_last_of(':');
      if (pos == 0 || pos == arg[1].size() - 1)
        return 1;

      uint16_t port = 0;

      if (pos != std::wstring::npos) {

        try {
          port = std::stoi(arg[1].substr(pos + 1, arg[1].size() - pos - 1));
        } catch (std::invalid_argument) {
          return 1;
        }

        if (std::to_wstring(port) != arg[1].substr(pos + 1, arg[1].size() - pos - 1))
          return 1;

        if (port < 1 || port > 65535)
          return 1;

      } else {

        pos = arg[1].size();

      }

      std::wstring da_hostname = arg[1].substr(0, pos);
      std::string hostname(da_hostname.begin(), da_hostname.end());
      std::wstring usr;

      if (arg.size() == 3)
        usr = arg[2];

      core::serv.conn(hostname, port, usr);

      return 0;

    }

} conn;
