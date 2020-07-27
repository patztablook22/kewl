perform::func_handler::func_handler(std::wstring da_desc, func *da_ptr)
{
	desc = da_desc;
	ptr = da_ptr;
}

perform::func *perform::func_handler::gptr()
{
	return ptr;
}

std::wstring perform::func_handler::gdesc()
{
	return desc;
}

void perform::add(std::wstring name, std::wstring da_desc, func *ptr)
{
	if (funcz.find(name) != funcz.end() || name.size() < 3 || name.size() > 10 || !core::io.iz_k(name))
		return;
	try {
		funcz[name] = new func_handler(da_desc, ptr);
	} catch (std::bad_alloc) {
		exit(EXIT_FAILURE);
	};
}

void perform::gfuncz(std::vector<std::wstring> &output)
{
	output.clear();
	for (const std::pair<std::wstring, func_handler *> &handler: funcz)
		output.push_back(handler.first);
}

std::wstring perform::gdesc(std::wstring da_cmd)
{
	if (funcz.find(da_cmd) == funcz.end())
		return NULL;
	return funcz[da_cmd]->gdesc();
}

bool perform::iz_func(std::wstring input)
{
	return funcz.find(input) == funcz.end() ? false : true;
}

void perform::operator<<(std::wstring input)
{
	std::vector<std::wstring> arg;
	for (int i = 0; input.size() != 0; i++) {
		int pos = input.find(32);
		if (pos == std::wstring::npos)
			pos = input.size();
		if (i == 1) {
			if (input.substr(0, 2) == L"--" && pos > 2) {
				arg.push_back(input.substr(2, pos - 2));
			} else {
				std::wcerr << L"ERR: wut?!\n";
				std::wcerr << L"maybe wanna use " << arg[0] << L" --help\n";
				exit(EXIT_FAILURE);
			}				
		} else {
			arg.push_back(input.substr(0, pos));
		}
		input.erase(0, pos + 1);
	}

	if (funcz.find(arg[1]) == funcz.end()) {
		std::wcerr << L"ERR: function not found: \"" + arg[1] + L"\"\n";
		std::wcerr << L"maybe wanna use " << arg[0] << L" --help\n";
		exit(EXIT_FAILURE);
	}

	switch (funcz[arg[1]]->gptr()->usr(arg)) {
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
