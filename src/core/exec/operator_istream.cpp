void exec::operator<<(std::wstring input)
{
	if (core::io.myside == true)
		_exec_k(input);
	else
		_exec_s(input);
}

void exec::_exec_k(std::wstring input)
{
	std::vector<std::wstring> arg;
	if (core::exec.interpreter(input, arg) != input.size()) {
		core::io << L"ERR: interpretation failed";
		return;
	}
	if (arg.size() == 0 || input.size() == 0 || input[0] == L' ')
		return;
	bool stfu = (arg[arg.size() - 1] == L"**stfu" && input[input.size() - 1] != L'"' && input[input.size() - 2] != L'"');
	if (stfu)
		arg.erase(arg.end() - 1);
	switch (arg[0][0]) {
	case L'_':
		if (arg.size() != 1) {
			core::io << L"ERR: macroz do not take any argument";
			return;
		}
		if (stfu)
			core::io << L"WARN: u cannot use stfu mode on whole macroz";

		arg[0].erase(0, 1);
		if (arg[0].size() == 0)
			return;
		if (arg[0].size() > 15) {
			core::io << L"ERR: macroz hav limited length to 15 charz";
			return;
		}
		if (!core::exec.macroz.mcr(arg[0])) {
			core::io << L"ERR: macro not found: \"" + arg[0] + L'"';
			return;
		}
		if (std::find(core::exec.macroz.called.begin(), core::exec.macroz.called.end(), arg[0]) == core::exec.macroz.called.end()) {
			core::exec.macroz.called.push_back(arg[0]);
			bool first = core::exec.macroz.called.size() == 1;
			core::exec.macroz._mcrz[arg[0]]->do_it();
			if (first)
				core::exec.macroz.called.clear();
		} else {
			core::io << L"WARN: skipping recursive macro call";
			return;
		}
		break;
	default:
		if (arg[0].size() == 0 && arg.size() == 1)
			return;
		if (arg[0].size() > 15) {
			core::io << L"ERR: cmdz hav limited length to 15 charz";
			return;
		}
		if (core::exec._cmdz.find(arg[0]) == core::exec._cmdz.end()) {
			core::io << L"ERR: command not found: \"" + arg[0] + L'"';
			return;
		}

		if (arg[0] == L"macroz" && core::exec.macroz.called.size() != 0) {
			core::io << L"ERR: macroz cannot manage themselvez";
			return;
		}

		if (stfu)
			core::io.stfu.toggle();

		int rtn = core::exec._cmdz[arg[0]]->ptr()->usr(arg);
		if (rtn == 0 || rtn == 255) {
		} else if (rtn >= arg.size()) {
			core::io << L"ERR: not enough arguments given!";
		} else {
			core::io << L"ERR: invalid input!";
			std::wstring err;
			bool crop = false;
			int tmp0 = core::exec.escape(arg, input, rtn - 1);
			if (tmp0 >= 9) {
				input.erase(0, input.size() - 7);
				input.insert(0, L"...");
				tmp0 = input.size();
			}
			core::exec.escape({input}, input);
			int tmp1 = core::exec.escape({arg[rtn]}, err);
			core::exec.escape({err}, err);
			if (input[0] == L'"')
				input = input.substr(1, input.size() - 2);
			if (err[0] == L'"')
				err = err.substr(1, err.size() - 2);
			core::io << input + L' ' + err + L"\\7 <-[HERE]";
			core::io << std::wstring(tmp0 + 1, 32) + L"\\7" + std::wstring(tmp1, L'~');
		}
		if (stfu)
			core::io.stfu.toggle();
	}
}

void exec::_exec_s(std::wstring input)
{
	std::vector<std::wstring> arg;
	core::exec.interpreter(input, arg);
	if (arg.size() == 0 || arg[0].size() == 0 || input[0] == 32)
		return;
	if (core::exec._cmdz.find(arg[0]) == core::exec._cmdz.end())
		return;
	core::exec._cmdz[arg[0]]->ptr()->serv(arg);
}
