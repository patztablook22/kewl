cmdl::opt_handler::opt_handler(std::wstring da_desc, opt *da_ptr)
{
	desc = da_desc;
	ptr = da_ptr;
}

cmdl::opt *cmdl::opt_handler::gptr()
{
	return ptr;
}

std::wstring cmdl::opt_handler::gdesc()
{
	return desc;
}

void cmdl::add(std::wstring name, std::wstring da_desc, opt *ptr)
{
	if (optz.find(name) != optz.end() || name.size() < 3 || name.size() > 10 || !core::io.iz_k(name))
		return;
	try {
		optz[name] = new opt_handler(da_desc, ptr);
	} catch (std::bad_alloc) {
		exit(EXIT_FAILURE);
	};
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
	if (core::exec.interpreter(input, arg) != input.size()) {
		std::wcerr << L"ERR: interpretation failed\n";
		std::wcerr << L"maybe wanna use " << arg[0] << L" --help\n";
		exit(EXIT_FAILURE);
	}

	if (optz.find(arg[1]) == optz.end()) {
		std::wcerr << L"ERR: opttion not found: \"" + arg[1] + L"\"\n";
		std::wcerr << L"maybe wanna use " << arg[0] << L" --help\n";
		exit(EXIT_FAILURE);
	}

	switch (optz[arg[1]]->gptr()->usr(arg)) {
	case 0:
		exit(EXIT_SUCCESS);
		break;
	case 2:
		std::wcerr << L"ERR: invalid input\n";
		std::wcerr << L"maybe wanna use " << arg[0] << L" --help\n";
		exit(EXIT_FAILURE);	
		break;
	default:
		exit(EXIT_FAILURE);
		break;
	}
}
