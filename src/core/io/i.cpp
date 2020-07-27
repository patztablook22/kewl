void io::i::draw()
{
	core::io.scr_mtx.lock();
	int max_x, max_y;
	basis::termxy(max_x, max_y);
	draw(max_x, max_y);
	core::io.scr_mtx.unlock();
}

void io::i::draw(int max_x, int max_y)
{
	_draw(max_x, max_y);
}

void io::i::_draw(int max_x, int max_y)
{
	acompl.popup(max_x, max_y);
	std::wstring prompt = core::env.gstr(use_passwd ? L"txt_prompt_passwd" : L"txt_prompt_normal");
	if (pos >= begin + max_x - prompt.size() - 2)
		begin = pos - max_x + prompt.size() + 2;
	else if (pos < begin)
		begin = pos;

	basis::mvrubber(0, max_y - 1, max_x);
	core::env.on(use_passwd ? L"attr_prompt_passwd" : L"attr_prompt_normal");
	basis::mvprint(0, max_y - 1, prompt.substr(0, max_x).c_str());
	core::env.off();
	core::env.on(use_passwd ? L"attr_input_passwd" : L"attr_input_normal");
	basis::mv(prompt.substr(0, max_x).size(), max_y - 1);
	if (!use_passwd)
		basis::print(buf.substr(begin, max_x - prompt.substr(0, max_x).size()));
	else
		basis::print(std::wstring(buf.substr(begin, max_x - prompt.substr(0, max_x).size()).size(), core::env.gchar(L"char_passwd_input")));
	core::env.off();
	basis::mv(pos - begin + prompt.substr(0, max_x).size(), max_y - 1);
	refresh();
}

void io::i::help()
{
	if (buf.size() < 2 || buf[0] != L'/')
		return;
	std::wstring tmp(buf.begin() + 1, buf.end());
	std::vector<std::wstring> arg;
	core::exec.interpreter(tmp, arg);
	if (arg[0].size() < 2 || arg[0].size() > 15 || arg[0][0] == L'_')
		return;
	core::exec << L"help \"" + arg[0] + L'"';
}

io::i::acompl::acompl()
{
	reset();
}

void io::i::acompl::reset()
{
	possibz.clear();
	iter = 0;
	pos = 0;
	len = 0;
	size = 0;
	core::io.mkwin();
}

bool io::i::acompl::move(bool next)
{
	if (possibz.size() == 0)
		return false;
	next ? iter++ : iter--;
	iter += possibz.size();
	iter %= possibz.size();
	return true;
}

void io::i::acompl::get()
{
	if (possibz.size() == 0)
		quarry();
	if (possibz.size() == 0)
		return;
	core::io.i.buf.erase(pos, len);
	core::io.i.buf.insert(pos, possibz[iter]);
	len = possibz[iter].size();
	core::io.i.pos = pos + len;
	if (pos >= 0 && core::io.i.buf[pos - 1] == L'"') {
		if (core::io.i.pos >= core::io.i.buf.size() || core::io.i.buf[core::io.i.pos] != L'"')
			core::io.i.buf.insert(core::io.i.pos, 1, L'"');
		core::io.i.pos++;
	}
	if (core::io.i.pos >= core::io.i.buf.size() || core::io.i.buf[core::io.i.pos] != 32)
		core::io.i.buf.insert(core::io.i.pos, 1, 32);
	core::io.i.pos++;
	if (possibz.size() == 1)
		reset();
}

void io::i::acompl::quarry()
{
	if (possibz.size() != 0)
		return;
	std::vector<std::wstring> arg;
	std::wstring search(core::io.i.buf);
	if (search.size() == 0)
		return;
	bool iz_cmd = (search.size() > 1 && search[0] == L'/' && search[1] != L' ');
	int tmp_end = core::io.i.pos - iz_cmd;
	if (iz_cmd)
		search.erase(0, 1);
	int tmp = core::exec.interpreter(search, arg, tmp_end);
	if (tmp == -1 || arg.size() == 0)
		return;
	if (iz_cmd) {
		arg[0].insert(0, 1, L'/');
		tmp++;
	}
	if (tmp == core::io.i.pos && core::io.i.buf[tmp - 1] == 32) {
		search = L"";
	} else {
		search = arg[arg.size() - 1];
		arg.erase(arg.end() - 1);
	}
			if (arg.size() == 0) {
		if (search.size() == 0 || search[0] != core::io.i.buf[0])
			return;
		switch (search[0]) {
		case L'/':
			search.erase(0, 1);
			if (search.size() > 0 && search[0] == L'_') {
				core::exec.macroz.mcrz(possibz);
				for (int i = 0; i < possibz.size(); i++)
					possibz[i].insert(0, 1, L'_');
			} else {
				core::exec.cmdz(possibz);
			}
			break;
		case L'@':
			search.erase(0, 1);
			if (!core::serv.status.gactive())
				return;
			core::serv.status.gotherz(possibz);	
			break;
		default:
			return;
		}
	} else if (iz_cmd) {
		core::exec.acompl(arg, possibz);
	}
		int i = 0;
	while (i < possibz.size()) {
		if (possibz[i].size() < search.size()) {
			possibz.erase(possibz.begin() + i);
			continue;
		}
		int j;
		for (j = 0; j < search.size(); j++) {
			if (possibz[i][j] != search[j]) {
				break;
			}
		}
		if (j == search.size()) {
			if (possibz[i].size() > size)
				size = possibz[i].size();
			i++;
		} else {
			possibz.erase(possibz.begin() + i);
		}
	}
	if (search.size() > 0) {
		pos = core::io.i.buf.rfind(search, tmp);
		if (pos == std::wstring::npos) {
			reset();
		}
	} else {
		for (int i = core::io.i.pos - 1; i >= 0; i--) {
			if (core::io.i.buf[i] == 32) {
				pos = i + 1;
				break;
			}
		}
		if (pos == 0 && (core::io.i.buf[pos] == L'/' || core::io.i.buf[pos] == L'@'))
			pos++;
		if (core::io.i.buf[pos] == L'"')
			pos++;
	}
	len = search.size();
}

void io::i::acompl::popup(int max_x, int max_y)
{
	if (possibz.size() < 2 || !core::env.gtru(L"bool_do_popup"))
		return;
	int x = core::env.gstr(L"txt_prompt_normal").size() +  pos - core::io.i.begin - 2;
	if (x < 0)
		x = 0;
	int w = size + 2;
	if (x + w + 3 > max_x)
		w = max_x - x - 3;
	if (w < 6)
		return;
	int h = possibz.size(), tmp = 0;
	if (h > max_y / 4) {
		h = max_y / 4;
		tmp = iter - h / 2;
		if (tmp < 0)
			tmp = 0;
		else if (tmp + h > possibz.size())
			tmp = possibz.size() - h;
	}
	basis::mvprint(x, max_y - h - 3, "┌");
	basis::print(std::wstring(w, L'─'));
	basis::print("┐");
	for (int i = 0; i < h; i++) {
		basis::mvprint(x, max_y - h - 2 + i, "│");
		if (tmp + i == iter)
			core::env.on(L"attr_popup_hl");
		basis::print(" ");
		if (possibz[tmp + i].size() > w - 2) {
			basis::print(possibz[tmp + i].substr(0, w - 5));
			basis::print("... ");
		} else {
			basis::print(possibz[tmp + i]);
			basis::rubber(w - possibz[tmp + i].size() - 1);
		}
		core::env.off();
		basis::print("│");
	}
	basis::mvprint(x, max_y - 2, "│ ");
	basis::print(tmp + h == possibz.size() ? "[R]" : "...");
	basis::rubber(w - 4);
	basis::print("│");
}

void io::i::clpbrd::cut(bool all)
{
	if (core::io.i.pos == 0)
		return;
	size_t cut_begin;
	if (all) {
		cut_begin = 0;
	} else {
		int i, j = core::io.i.pos - 1;
		while ((i = core::io.i.buf.substr(0, j).find_last_of(32)) != std::wstring::npos) {
			if (core::io.i.buf[i + 1] == 32 || i + 1 > core::io.i.pos)
				j = i;
			else
				break;
		}
		cut_begin = (i == std::wstring::npos) ? 0 : i + 1;
	}
	buf = core::io.i.buf.substr(cut_begin, core::io.i.pos - cut_begin);
	core::io.i.buf.erase(cut_begin, core::io.i.pos - cut_begin);
	core::io.i.pos = cut_begin;
}

void io::i::clpbrd::paste()
{
	core::io.i.buf.insert(core::io.i.pos, buf);
	core::io.i.pos += buf.size();
}

io::i::hist::hist()
:len(1)
{
}

void io::i::hist::prepare()
{
	if (da_hist.size() == 0)
		da_hist.push_back(L"");
	if ((int)da_hist.size() - 1 - core::env.gint(L"int_input_scrollbucc") > 0)
		da_hist.erase(da_hist.begin(), da_hist.end() - 1 - core::env.gint(L"int_input_scrollbucc"));
	pos = da_hist.size() - 1;
}

void io::i::hist::buftohist()
{
	if (core::io.trim(core::io.i.buf).size() == 0)
		return;
	if (da_hist.size())
		da_hist[da_hist.size() - 1] = core::io.i.buf;
	else
		da_hist.push_back(core::io.i.buf);
	da_hist.push_back(L"");
}

void io::i::hist::histtobuf(bool bacc)
{
	if (bacc && pos == da_hist.size() - 1 || !bacc && pos == 0)
		return;
	if (pos == da_hist.size() - 1)
		da_hist[da_hist.size() - 1] = core::io.i.buf;
	
	short int direction = bacc ? 1 : -1;
	core::io.i.buf = da_hist[pos += direction];
	core::io.i.pos = core::io.i.buf.size();
	core::io.i.begin = 0;
}
