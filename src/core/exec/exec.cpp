void exec::add(std::wstring name, std::vector<std::vector<std::wstring>> da_man, class cmd *da_ptr)
{
  if (_cmdz.find(name) != _cmdz.end() || name.size() < 2 || name.size() > 10 || !core::io.iz_k(name))
    return;
  if (name.find('/') != std::wstring::npos || name.find(L'_') == 0)
    return;
  try {
    _cmdz[name] = new cmd_handler(da_man, da_ptr);
  } catch (std::bad_alloc) {
    exit(EXIT_FAILURE);
  }
}

void exec::cmdz(std::vector<std::wstring> &output)
{
  output.clear();
  for (const std::pair<std::wstring, cmd_handler *> &handler: _cmdz)
    output.push_back(handler.first);
}

void exec::man(std::wstring da_cmd, std::vector<std::vector<std::wstring>> &trg)
{
  _cmdz[da_cmd]->man(trg);
}

void exec::acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
{
  trg.clear();
  if (arg.size() == 0)
    return;
  arg[0].erase(0, 1);
  if (_cmdz.find(arg[0]) == _cmdz.end())
    return;

  _cmdz[arg[0]]->acompl(arg, trg);
}

bool exec::cmd(std::wstring input)
{
  return _cmdz.find(input) != _cmdz.end();
}
