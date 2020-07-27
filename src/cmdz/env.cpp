class env: public core::exec::cmd {

  private:

    std::wstring property_query (std::wstring name)
    {
      int pos = name.find(L'_');
      if (pos == std::wstring::npos || pos == 0 || pos == name.size() - 1)
        return L"";

      std::wstring type = name.substr(0, pos);

      if (type == L"attr") {

        std::vector<std::wstring> res;
        core::env.gattrz(name, res);

        if (res.size() != 3)
          return L"???";

        return L'"' + res[0] + L"\", \"" + res[1] + L"\", \"" + res[2] + L'"';

      } else if (type == L"txt") {
        return L'"' + core::env.gstr(name) + L'"';
      } else if (type == L"char") {
        return L'"' + std::wstring(1, core::env.gchar(name)) + L'"';
      } else if (type == L"int") {
        return std::to_wstring(core::env.gint(name));
      } else if (type == L"bool") {
        return core::env.gtru(name) ? L"TRU" : L"FALZ";
      }

      return L"";

    }

    uint8_t set (std::vector<std::wstring> wut)
    {
      if (wut.size() == 0)
        return 2;

      std::wstring type = wut[0].substr(0, wut[0].find(L'_') + 1);

      if (type == L"attr_") {

        if (wut.size() < 4)
          return 5;

        else if (wut.size() > 4)
          return 6;

        switch (core::env.set(wut[0], {wut[1], wut[2], wut[3]})) {

          case 0:

            core::io << L"property set successful";
            break;

          case 255:

            core::io << L"WARN: same as current value";
            break;

          case 1:

            core::io << L"ERR: unknown property: \"" + wut[0] + L'"';
            return -1;

          case 2:

            return -1;

          case 3:

            core::io << L"ERR: unknown color: \"" + wut[1];
            return -1;

          case 4:

            core::io << L"ERR: unknown color: \"" + wut[2];
            return -1;

          case 5:

            core::io << L"ERR: unknown weight: \"" + wut[3];
            return -1;

        }

      } else if (type == L"txt_") {

        if (wut.size() < 2)
          return 3;

        else if (wut.size() > 2)
          return 4;

        switch (core::env.set(wut[0], wut[1])) {

          case 0:

            core::io << L"property set successful";
            break;

          case 255:

            core::io << L"WARN: same as current value";
            break;

          case 1:

            core::io << L"ERR: property not found: \"" + wut[0] + L'"';
            return -1;

          case 2:

            return -1;

          case 3:

            core::io << L"ERR: not in length range (" + core::env.grange(wut[0]) + L')';
            return -1;

        }

      } else if (type == L"char_") {

        if (wut.size() < 2)
          return 3;

        else if (wut.size() > 2)
          return 4;

        if (wut[1].size() != 1) {
          core::io << L"ERR: not a char: \"" + wut[1] + L'"';
          return -1;
        }

        switch (core::env.set(wut[0], (wint_t)wut[1][0])) {

          case 0:

            core::io << L"property set successful";
            break;

          case 255:

            core::io << L"WARN: same as current value";
            break;

          case 1:

            core::io << L"ERR: property not found: \"" + wut[0] + L'"';
            return -1;

        }

      } else if (type == L"int_") {

        if (wut.size() < 2)
          return 3;

        else if (wut.size() > 2)
          return 4;

        int tmp;

        try {
          tmp = std::stoi(wut[1]);
        } catch (std::invalid_argument) {
          core::io << L"ERR: entered value iz not a number";
          return -1;
        }

        if (wut[1] != std::to_wstring(tmp)) {
          core::io << L"ERR: entered value iz not a number";
          return -1;
        }

        switch (core::env.set(wut[0], tmp)) {

          case 0:
            core::io << L"property set successful";
            break;
          case 255:
            core::io << L"WARN: same as current value";
            return -1;
          case 1:
            core::io << L"ERR: property not found: \"" + wut[0] + L'"';
            return -1;
          case 2:
            return -1;
          case 3:
            core::io << L"ERR: not in range (" + core::env.grange(wut[0]) + L')';
            return -1;
        }
      } else if (type == L"bool_") {
        if (wut.size() < 2)
          return 3;
        else if (wut.size() > 2)
          return 4;
        switch (core::env.set(wut[0], wut[1])) {
          case 0:
            core::io << L"property set successful";
            break;
          case 255:
            core::io << L"WARN: same as current value";
            return -1;
          case 1:
            core::io << L"ERR: property not found \"" + wut[0] + L'"';
            return -1;
          case 3:
            core::io << L"ERR: must be TRU/FALZ";
            return -1;
        }
      } else {
        return 2;
      }
      return 0;
    }

    uint8_t define(std::vector<std::wstring> wut)
    {
      if (wut.size() < 2)
        return 3;
      else if (wut.size() > 2)
        return 4;
      unsigned int c[3];
      try {
        if (wut[1].size() == 7 && wut[1][0] == L'#') {
          c[0] = std::stoi(std::wstring(wut[1].begin() + 1, wut[1].begin() + 3), NULL, 16);
          c[1] = std::stoi(std::wstring(wut[1].begin() + 3, wut[1].begin() + 5), NULL, 16);
          c[2] = std::stoi(std::wstring(wut[1].begin() + 5, wut[1].begin() + 7), NULL, 16);
        } else {
          for (int i = 0; i < 2; i++) {
            int pos = wut[1].find(L',');
            if (pos == std::wstring::npos)
              return 3;
            std::wstring tmp(wut[1].begin(), wut[1].begin() + pos);
            wut[1].erase(0, pos + 1);
            c[i] = std::stoi(tmp);
            if (std::to_wstring(c[i]) != tmp)
              return 3;
          }
          c[2] = std::stoi(wut[1]);
          if (std::to_wstring(c[2]) != wut[1])
            return 3;				
        }
      } catch (std::invalid_argument) {
        return 3;
      }

      switch (core::env.def_col(wut[0], {c[0] * 1000 / 256, c[1] * 1000 / 256, c[2] * 1000 / 256})) {
        case 0:
          core::io << L"color (re)definition successful";
          return 0;
        default:
        case 1:
          core::io << L"ERR: max of colorz reached 4 current terminal";
          return -1;
        case 2:
          return 3;
        case 255:
          core::io << L"ERR: unable 2 (re)define diz color";
          return -1;
        case 3:
          core::io << L"ERR: color name length must be between 3 and 15 charz";
          return -1;
        case 4:
          core::io << L"ERR: red component not in range";
          return -1;
        case 5:
          core::io << L"ERR: blu component not in range";
          return -1;
        case 6:
          core::io << L"ERR: green component not in range";
          return -1;
      }
    }
  public:
    env()
    {
      core::exec.add(L"env", {{L"define col_name #ffffff"}, { L"define col_name 255 255 255", L"define new / redefine existing color"}, {L"set attr_property fg_col bg_col txt_weight"}, {L"set txt_property \"blah blah blah string\""}, {L"set char_property \"#\""}, {L"set int_property 1234"}, {L"set bool_property TRU/FALZ", L"set property valuez"}, {L"reset", L"reset all propertiez 2 their initial statez"}, {L"reset some_property", L"reset some_property 2 itz initial state"}, {L"list propertiez/colorz/weightz", L"get list of all propertiez/..."}}, this);
    }

    uint8_t usr(std::vector<std::wstring> arg)
    {
      if (arg.size() == 1)
        return 1;
      if (arg[1] == L"define") {
        arg.erase(arg.begin(), arg.begin() + 2);
        return define(arg);
      } else if (arg[1] == L"reset") {
        if (arg.size() == 2) {
          core::env.reset();
          core::io << L"propertiez reset successful";
        } else if (arg.size() == 3) {
          if (core::env.reset(arg[2]) == 1) {
            core::io << L"ERR: unknown property: \"" + arg[2] + L'"';
            return -1;
          }
          core:: io << L"property reset successful";
        } else {
          return 3;
        }
      } else if (arg[1] == L"set") {
        arg.erase(arg.begin(), arg.begin() + 2);
        return set(arg);
      } else if (arg[1] == L"list") {
        if (arg.size() < 3)
          return 2;
        if (arg.size() > 3)
          return 3;
        std::vector<std::wstring> tmp;
        if (arg[2] == L"propertiez") {
          int max_x, max_y;
          basis::termxy(max_x, max_y);
          max_y = core::env.gint(L"int_usr_max_padding");
          max_x -= (max_y > 4 ? max_y : 4);
          max_x -= 1 + (core::env.gtru(L"bool_show_time") ? 5 + core::env.gstr(L"txt_time_before").size() + core::env.gstr(L"txt_time_after").size() : 0) + core::env.gstr(L"txt_usr_before").size() + core::env.gstr(L"txt_usr_after").size();
          core::env.gpropertiez(tmp);
          if (max_x > 58) {
            int longest = 0;
            for (int i = 0; i < tmp.size(); i++) {
              if (tmp[i].size() > longest)
                longest = tmp[i].size();
            }
            longest += 3;
            for (int i = 0; i < tmp.size(); i++)
              tmp[i] += (std::wstring(longest - tmp[i].size(), 32) + L"\\5" + property_query(tmp[i]));
          }
        } else if (arg[2] == L"colorz") {
          core::env.gcolorz(tmp);
        } else if (arg[2] == L"weightz") {
          core::env.gweightz(tmp);
        } else {
          core::io << L"ERR: unknown list request: \"" + arg[2] + L'"';
          return -1;
        }
        core::io << core::io::msg(L"hr", L"");
        for (int i = 0; i < tmp.size(); i++)
          core::io << tmp[i];
        core::io << core::io::msg(L"hr", L"");
      } else {
        return 1;
      }
      return 0;
    }

    void acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
    {
      switch (arg.size()) {
        case 1:
          trg = {L"define", L"list", L"reset", L"set"};
          break;
        case 2:
          if (arg[1] == L"define")
            core::env.gcolorz(trg);
          else if (arg[1] == L"set" || arg[1] == L"reset")
            core::env.gpropertiez(trg);
          else if (arg[1] == L"list")
            trg = {L"colorz", L"propertiez", L"weightz"};
          break;
        case 3:

          if (arg[1] == L"define")
            trg = {L"#ffffff", L"255,255,255"};
          else if (arg[1] == L"set" && arg[2].substr(0, 5) == L"attr_")
            core::env.gcolorz(trg);
          else if (arg[1] == L"set" && arg[2].substr(0, 5) == L"bool_")
            trg = {L"FALZ", L"TRU"};
          break;
        case 4:
          if (arg[1] == L"set" && arg[2].substr(0, 5) == L"attr_")
            core::env.gcolorz(trg);
          break;
        case 5:
          if (arg[1] == L"set" && arg[2].substr(0, 5) == L"attr_")
            core::env.gweightz(trg);
          break;
      }
    }
} env;
