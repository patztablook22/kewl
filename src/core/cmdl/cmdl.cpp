void cmdl::add(std::wstring name, std::wstring da_desc, opt *ptr)
{
  if (optz.find(name) != optz.end() || name.size() < 3 || name.size() > 15 || !core::io.iz_k(name))
    return;

  try {
    optz[name] = new opt_handler(da_desc, ptr);
  } catch (std::bad_alloc) {
    exit(EXIT_FAILURE);
  }
}

void cmdl::goptz(std::vector<std::wstring> &output)
{
  output.clear();
  for (const std::pair<std::wstring, opt_handler *> &handler: optz)
    output.push_back(handler.first);
}

std::wstring cmdl::gdesc(std::wstring da_cmd)
{
  if (optz.find(da_cmd) == optz.end())
    return NULL;

  return optz[da_cmd]->gdesc();
}

bool cmdl::iz_opt(std::wstring input)
{
  return optz.find(input) == optz.end() ? false : true;
}

void cmdl::operator<<(std::wstring input)
{
  std::vector<std::wstring> arg;

  if (core::exec.interpreter(input, arg) != input.size() || arg.size() != 2) {

    std::wcerr << L"ERR: interpretation failed\n";
    std::wcerr << L"maybe wanna use " << arg[0] << L" --help\n";
    exit(EXIT_FAILURE);

  }

  if (optz.find(arg[0]) == optz.end()) {

    std::wcerr << L"ERR: option not found: \"" + arg[0] + L"\"\n";
    std::wcerr << L"maybe wanna use " << arg[1] << L" --help\n";
    exit(EXIT_FAILURE);

  }

  optz[arg[0]]->gptr()->usr(arg[1]);

}
