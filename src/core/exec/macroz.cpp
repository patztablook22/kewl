exec::macroz::macroz()
{
}

void exec::macroz::init()
{
	passwd *pw = getpwuid(getuid());
	location = pw->pw_dir;
	location += "/.config/kewl_macroz";
	load();
	core::io.ready();
	if (_mcrz.find(L"rc") != _mcrz.end())
		_mcrz[L"rc"]->do_it();
}

void exec::macroz::load()
{
	for (const std::pair<std::wstring, macro *> &da_macro: _mcrz)
		delete da_macro.second;
	_mcrz.clear();
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
			core::io << L"ERR: contains forbidden character/z: \\1line " + std::to_wstring(line);
			goto da_return;
		}
		switch (ch0) {
		case 9:
    case 32:
			if (tmp_name.size() == 0) {
				core::io << L"ERR: no macro name associated: \\1line " + std::to_wstring(line);
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
					core::io << L"WARN: skipping empty macro definition: \\1line " + std::to_wstring(line);
				}
			}

			if (buf[buf.size() - 1] != L':') {
				core::io << L"ERR: there must be ':' after macro name: \\1line " + std::to_wstring(line);
				goto da_return;
			}
			buf.erase(buf.size() - 1);
			if (buf.size() == 0) {
				core::io << L"ERR: empty macro name: \\1line " + std::to_wstring(line);
				goto da_return;
			}
			if (core::exec.macroz.mcr(buf)) {
				core::io << L"ERR: macro already defined: \\1line " + std::to_wstring(line);
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
			core::io << L"WARN: skipping empty macro definition: \\1line " + std::to_wstring(line);
		}
	}
	da_return:
	fd.close();
	core::io << L"\\1" + std::to_wstring(_mcrz.size()) + L"\\0 macro/z imported";
}

void exec::macroz::mcrz(std::vector<std::wstring> &trg)
{
	trg.clear();
	for (const std::pair<std::wstring, macro *> &da_macro: _mcrz)
		trg.push_back(da_macro.first);
}

bool exec::macroz::mcr(std::wstring da_macro)
{
	return _mcrz.find(da_macro) == _mcrz.end() ? false : true;
}

exec::macroz::macro::macro(std::wstring da_name, std::vector<std::wstring> &da_content)
{
	if (da_name.size() < 2 || da_name.size() > 15 || core::exec.macroz.mcr(da_name) || da_content.size() == 0)
		return;
	content = da_content;
	core::exec.macroz._mcrz[da_name] = this;
}

void exec::macroz::macro::do_it()
{
	for (int i = 0; i < content.size(); i++) {
		core::exec << content[i];
	}
	return;
}
