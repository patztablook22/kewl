void exec::cmd::acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
{
}

uint8_t exec::cmd::serv(std::vector<std::wstring>)
{
	return 0;
}

size_t exec::interpreter(std::wstring input, std::vector<std::wstring> &trg, size_t end)
{
	bool in = false, par = false;
	int iter = 0;
	std::wstring tmp;
	while (iter < input.size()) {
		wint_t ch = input[iter++];
		switch (ch) {
		case L'\\':
			if (iter == input.size())
				break;
			switch (input[iter++]) {
			case L'\\':
				tmp += L'\\';
				break;
			case L'"':
				tmp += L'"';
				break;
			case L'\'':
				tmp+= L'\'';
				break;
			default:
				break;
			}
			break;
		case L'"':
			switch (in) {
			case true:
				if (iter < input.size() && input[iter++] != 32)
					return (end == -1 ? iter - 1 : -1);
				trg.push_back(tmp);
				if (iter > end)
					return iter;
				tmp.clear();
				in = false;
				break;
			case false:
				if (iter > 1 && input[iter - 2] != 32)
					return (end == -1 ? iter - 1 : -1);
				in = true;
			}	
			break;
		case L' ':
			switch (in) {
			case true:
				if (tmp.size() == 0 || tmp[tmp.size() - 1] != L' ')
					tmp += L' ';
				break;
			case false:
				trg.push_back(tmp);
				if (iter > end)
					return iter;
				tmp.clear();
				break;
			}
			break;
		default:
			tmp += ch;
		}
	}
	if (tmp.size() != 0 || in)
		trg.push_back(tmp);
	if (in)
		return iter + 1;
	return iter;
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
	if (name.find('/') != std::wstring::npos || name.find(L'_') == 0)
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

void exec::cmd_gacompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
{
	trg.clear();
	if (arg.size() == 0)
		return;
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
	std::vector<std::wstring> arg;
	if (core::exec.interpreter(input, arg) != input.size()) {
		core::io << core::io::msg(L"kewl", L"ERR: interpretation failed");
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
			core::io << core::io::msg(L"kewl", L"ERR: macroz do not take any argument");
			return;
		}
		if (stfu)
			core::io << core::io::msg(L"kewl", L"WARN: u cannot use stfu mode on whole macroz");

		arg[0].erase(0, 1);
		if (arg[0].size() == 0)
			return;
		if (arg[0].size() > 15) {
			core::io << core::io::msg(L"kewl", L"ERR: macroz hav limited length to 15 charz");
			return;
		}
		if (!core::exec.macroz.existz(arg[0])) {
			core::io << core::io::msg(L"kewl", L"ERR: macro not found: \"" + arg[0] + L'"');
			return;
		}
		if (std::find(core::exec.macroz.called.begin(), core::exec.macroz.called.end(), arg[0]) == core::exec.macroz.called.end()) {
			core::exec.macroz.called.push_back(arg[0]);
			bool first = core::exec.macroz.called.size() == 1;
			core::exec.macroz.da_macroz[arg[0]]->do_it();
			if (first)
				core::exec.macroz.called.clear();
		} else {
			core::io << core::io::msg(L"kewl", L"WARN: skipping recursive macro call");
			return;
		}
		break;
	default:
		if (arg[0].size() == 0 && arg.size() == 1)
			return;
		if (arg[0].size() > 15) {
			core::io << core::io::msg(L"kewl", L"ERR: cmdz hav limited length to 15 charz");
			return;
		}
		if (core::exec.cmdz.find(arg[0]) == core::exec.cmdz.end()) {
			core::io << core::io::msg(L"kewl", L"ERR: command not found: \"" + arg[0] + L'"');
			return;
		}

		if (arg[0] == L"macroz" && core::exec.macroz.called.size() != 0) {
			core::io << core::io::msg(L"kewl", L"ERR: macroz cannot manage themselvez");
			return;
		}

		if (stfu)
			core::io.stfu.toggle();
		int rtn = core::exec.cmdz[arg[0]]->gptr()->usr(arg);
		if (stfu)
			core::io.stfu.toggle();

		switch (rtn) {
		case 0:
			break;
		case 2:
			core::io << core::io::msg(L"kewl", L"ERR: invalid input");
			break;
		default:
			break;
		}
		break;
	}
}

void exec::serv::operator<<(std::wstring input)
{
	std::vector<std::wstring> arg;
	core::exec.interpreter(input, arg);
	if (arg.size() == 0 || arg[0].size() == 0 || input[0] == 32)
		return;
	if (core::exec.cmdz.find(arg[0]) == core::exec.cmdz.end())
		return;
	core::exec.cmdz[arg[0]]->gptr()->serv(arg);
}

exec::macroz::macroz()
{
}

void exec::macroz::init()
{
	passwd *pw = getpwuid(getuid());
	location = pw->pw_dir;
	location += "/.kewl_macroz";
	load();;
	if (existz(L"rc"))
		da_macroz[L"rc"]->do_it();
}

void exec::macroz::load()
{
	for (const std::pair<std::wstring, macro *> &da_macro: da_macroz)
		delete da_macro.second;
	da_macroz.clear();
	std::wifstream fd(location);
	std::wstring buf, tmp_name(L"");
	std::vector<std::wstring> tmp_content;
	if (!fd.is_open())
		goto da_return;

	int line;
	for (line = 1; std::getline(fd, buf); line++) {
		wint_t ch0 = buf[0];
		buf = core::io.trim(buf);
		if (buf.size() == 0)
			continue;
		if (buf[0] == L'#')
			continue;
		if (!core::io.iz_k(buf)) {
			core::io << core::io::msg(L"kewl", L"ERR: contains forbidden character/z: line " + std::to_wstring(line));
			goto da_return;
		}
		switch (ch0) {
		case 9:
			if (tmp_name.size() == 0) {
				core::io << core::io::msg(L"kewl", L"ERR: no macro name associated: line " + std::to_wstring(line));
				goto da_return;
			}
			tmp_content.push_back(buf);
			break;
		default:
			if (tmp_name.size() != 0) {
				if (tmp_content.size() != 0) {
					new macro(tmp_name, tmp_content);
					tmp_content.clear();
				} else {
					core::io << core::io::msg(L"kewl", L"WARN: skipping empty macro definition: line " + std::to_wstring(line));
				}
			}

			if (buf[buf.size() - 1] != L':') {
				core::io << core::io::msg(L"kewl", L"ERR: there must be ':' after macro name: line " + std::to_wstring(line));
				goto da_return;
			}
			buf.erase(buf.size() - 1);
			if (buf.size() == 0) {
				core::io << core::io::msg(L"kewl", L"ERR: empty macro name: line " + std::to_wstring(line));
				goto da_return;
			}
			if (core::exec.macroz.existz(buf)) {
				core::io << core::io::msg(L"kewl", L"ERR: macro already defined: line " + std::to_wstring(line));
				goto da_return;
			}
			tmp_name = buf;
			break;
		}
	}
	if (tmp_name.size() != 0) {
		if (tmp_content.size() != 0) {
			new macro(tmp_name, tmp_content);
		} else {
			core::io << core::io::msg(L"kewl", L"WARN: skipping empty macro definition: line " + std::to_wstring(line));
		}
	}
	da_return:
	fd.close();
	core::io << core::io::msg(L"kewl", std::to_wstring(da_macroz.size()) + L" macro/z imported");
}

void exec::macroz::gmacroz(std::vector<std::wstring> &trg)
{
	trg.clear();
	for (const std::pair<std::wstring, macro *> &da_macro: da_macroz)
		trg.push_back(da_macro.first);
}

bool exec::macroz::existz(std::wstring da_macro)
{
	return da_macroz.find(da_macro) == da_macroz.end() ? false : true;
}

exec::macroz::macro::macro(std::wstring da_name, std::vector<std::wstring> &da_content)
{
	if (da_name.size() < 2 || da_name.size() > 15 || core::exec.macroz.existz(da_name) || da_content.size() == 0)
		return;
	content = da_content;
	core::exec.macroz.da_macroz[da_name] = this;
}

void exec::macroz::macro::do_it()
{
	for (int i = 0; i < content.size(); i++) {
		core::exec.usr << content[i];
	}
	return;
}
