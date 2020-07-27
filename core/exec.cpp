void exec::cmd::acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
{
}

uint8_t exec::cmd::serv(std::vector<std::wstring>)
{
	return 0;
}

std::vector<std::wstring> exec::interpreter(std::wstring input)
{
	std::vector<std::wstring> result;
	input = core::io.trim(input);
	int pos;
	while ((pos = input.find(32)) != std::wstring::npos) {
		if (pos != 0)
			result.push_back(input.substr(0, pos));
		input.erase(0, pos + 1);
	}
	result.push_back(input);
	return result;
}

exec::cmd_handler::cmd_handler(std::vector<std::vector<std::wstring>> da_man, cmd *da_ptr)
{
	man = da_man;
	ptr = da_ptr;
}

void exec::cmd_handler::gman(std::vector<std::vector<std::wstring>> &trg)
{
	trg = man;
}

void exec::cmd_handler::gacompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
{
	ptr->acompl(arg, trg);
}

exec::cmd *exec::cmd_handler::gptr()
{
	return ptr;
}

void exec::add(std::wstring name, std::vector<std::vector<std::wstring>> da_man, cmd *da_ptr)
{
	if (cmdz.find(name) != cmdz.end() || name.size() < 2 || name.size() > 10 || !core::io.iz_k(name))
		return;
	if (name.find('/') != std::wstring::npos)
		return;
	try {
			cmdz[name] = new cmd_handler(da_man, da_ptr);
	} catch (std::bad_alloc) {
		exit(EXIT_FAILURE);
	}
}

void exec::gcmdz(std::vector<std::wstring> &output)
{
	output.clear();
	for (const std::pair<std::wstring, cmd_handler *> &handler: cmdz)
		output.push_back(handler.first);
}

void exec::gman(std::wstring da_cmd, std::vector<std::vector<std::wstring>> &trg)
{
	cmdz[da_cmd]->gman(trg);
}

void exec::cmd_gacompl(std::wstring input, std::vector<std::wstring> &trg)
{
	trg.clear();
	std::vector<std::wstring> arg = interpreter(input);
	if (input[input.size() - 1] != 32)
		arg.erase(arg.end() - 1);
	arg[0].erase(0, 1);
	if (cmdz.find(arg[0]) == cmdz.end())
		return;
	
	cmdz[arg[0]]->gacompl(arg, trg);
}

bool exec::iz_cmd(std::wstring input)
{
	return cmdz.find(input) == cmdz.end() ? false : true;
}

void exec::usr::operator<<(std::wstring input)
{
	std::vector<std::wstring> arg = core::exec.interpreter(input);
	if (arg.size() == 0 || arg[0].size() == 0 || input[0] == 32)
		return;
	if (arg[0].size() > 15) {
		core::io << core::io::msg(L"kewl", L"ERR: cmdz hav limited size to 15 charz");
		return;
	}
	if (core::exec.cmdz.find(arg[0]) == core::exec.cmdz.end()) {
		core::io << core::io::msg(L"kewl", L"ERR: command not found: \"" + arg[0] + L"\"");
		return;
	}
	switch (core::exec.cmdz[arg[0]]->gptr()->usr(arg)) {
	case 0:
		break;
	case 2:
		core::io << core::io::msg(L"kewl", L"ERR: invalid input");
		break;
	default:
		break;
	}
}

void exec::serv::operator<<(std::wstring input)
{
	std::vector<std::wstring> arg = core::exec.interpreter(input);
	if (arg.size() == 0 || arg[0].size() == 0 || input[0] == 32)
		return;
	if (core::exec.cmdz.find(arg[0]) == core::exec.cmdz.end())
		return;
	core::exec.cmdz[arg[0]]->gptr()->serv(arg);
}
