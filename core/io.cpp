io::io()
:beep_on(true), beep_delay(667)
{
	std::locale::global(std::locale("en_US.UTF-8"));
}

void io::init()
{
	initscr();
	noecho();
	keypad(stdscr, true);
	raw();
	start_color();
	use_default_colors();

	init_pair(1, COLOR_YELLOW, COLOR_BLACK);	// msg err color
	init_pair(2, COLOR_GREEN, COLOR_BLACK);		// msg private color
	init_pair(10, COLOR_CYAN, COLOR_BLACK);		// <kewl> color
	init_pair(11, COLOR_BLUE, COLOR_BLACK);		// <serv> color
	init_pair(12, COLOR_WHITE, COLOR_BLACK);	// <someone> color
	init_pair(20, COLOR_WHITE, COLOR_BLUE);		// interface base color
	init_pair(21, COLOR_WHITE, COLOR_RED);		// interface warn color
	init_pair(22, COLOR_CYAN, COLOR_BLUE);		// interface decor color

	gettimeofday(&beep_tm, NULL);
	mkwin();
}

io::~io()
{
	endwin();
}

bool io::iz_k(wint_t chk)
{
	if ( \
		(chk >= 32 && chk <= 176) /* standard ASCII without da weird mess at beginning*/ || \
		(chk >= 192 && chk != 215 && chk != 247 && chk <= 591) /* some čžech crap */ || \
		(chk >= 1025 && chk <= 1105) /* cука блять etc. xd */ \
		)
		return true;
	return false;
}

bool io::iz_k(std::wstring chk)
{
	for (int i = 0; i < chk.size(); i++)
		if(!iz_k(chk[i]))
			return false;
	return true;
}

bool io::iz_k(std::string chk)
{
	return iz_k(std::wstring(chk.begin(), chk.end()));
}

std::wstring io::trim(std::wstring s)
{
	size_t i, done = 0;
	while ((i = s.find(32, done)) != std::wstring::npos) {
		if (i == 0 || i == s.size() - 1 || s[i - 1] == 32 || s[i + 1] == 32)
			s.erase(i, 1);
		else
			done++;
	}
	return s;
}

std::wstring io::ver_echo(int v)
{
	std::wstring w[3];
	for (int j = 2; j > 0; j--) {
		w[j] = std::to_wstring(v % 10);
		v /= 10;
	}
	w[0] = std::to_wstring(v);
	return std::wstring(L"v") + w[0] + std::wstring(L".") + w[1] + std::wstring(L".") + w[2];
}

io::msg::msg()
:valid(false)
{
}

io::msg::msg(std::wstring da_from, std::wstring da_body)
{
	set(da_from, da_body);
}

void io::msg::set(std::wstring da_from, std::wstring da_body)
{
	valid = false;
	if (da_from != std::wstring(L"kewl") && da_from != std::wstring(L"serv"))
		da_body = core::io.trim(da_body);
	if (da_from.size() > 15 || da_from.size() < 3 || da_body.size() == 0 || da_body.size() > 255)
		return;
	if (da_from.find(32) != std::wstring::npos)
		return;
	if (!core::io.iz_k(da_from) || !core::io.iz_k(da_body))
		return;

	from = da_from;
	body = da_body;

	if (from == std::wstring(L"kewl"))
		col0 = 0;
	else if (from == std::wstring(L"serv"))
		col0 = 1;
	else
		col0 = 2;

	w0 = body.find(' ');
	if (w0 == std::wstring::npos)
		w0 = da_body.size();

	if ((col0 == 0 || col0 == 1) && core::io.trim(body).substr(0, 5) == std::wstring(L"ERR: "))
		col1 = 1;
	else if (col0 > 0 && body[0] == L'@' && w0 > 3 && w0 < 17)
		col1 = 2;
	else
		col1 = 0;
	valid = true;
}

void io::msg::operator=(msg da_msg)
{
	this->from = da_msg.from;
	this->body = da_msg.body;
	this->col0 = da_msg.col0;
	this->col1 = da_msg.col1;
	this->w0 = da_msg.w0;
	this->valid = da_msg.valid;
}

std::wstring io::msg::gfrom()
{
	return from;
}

std::wstring io::msg::gbody()
{
	return body;
}

int io::msg::gcol0()
{
	return col0;
}

int io::msg::gcol1()
{
	return col1;
}

int io::msg::gw0()
{
	return w0;
}

bool io::msg::gvalid()
{
	return valid;
}

void io::send_passwd()
{
	if (i.use_passwd)
		return;
	else
		i.use_passwd = true;
	i.draw();
}

void io::operator>>(std::wstring &trg)
{
	if (i.use_normal)
		return;
	else
		i.use_normal = true;
	wint_t ch;
	bool upasswd = false;
	i.buf.clear();
	i.pos = 0;
	bool max = (i.buf.size() > 255);

	for (;;) {
		get_wch(&ch);
		if (upasswd != i.use_passwd) {
			i.buf.clear();
			i.pos = 0;
			upasswd = !upasswd;
		}
		if (ch == 0 | ch == -1) {	// ERR
			std::wcerr << L"ERR: failed to capture keypress\n";
			exit(1);
		} else if (ch == 10) {
			if (!upasswd) {
				break;
			} else if (i.buf.size() != 0) {
				try {
					core::serv << i.buf;
				} catch (...) {}
				i.use_passwd = false;
				upasswd = false;
				i.buf.clear();
				i.pos = 0;
			}
		} else if (ch == 410) { // RESIZE
			o.scrll_resize();
			continue;
		} else if (ch == 258) {	// KEY_DOWN
			if (!upasswd)
				i.hist.histtobuf(true);
		} else if (ch == 259) {	// KEY_UP
			if (!upasswd)
				i.hist.histtobuf(false);
		} else if (ch == 260) {	// KEY_LEFT
			if (i.pos > 0)
				i.pos--;
		} else if (ch == 261) {	// KEY_RIGHT
			if (i.pos < i.buf.size())
				i.pos++;
		} else if (ch == 338) {	// PG_DN
			o.scrll(true);
		} else if (ch == 339) {	// PG_UP
			o.scrll(false);
		} else if (ch == 360) {	// END
			o.jump(true);
		} else if (ch == 262) {	// HOME
			o.jump(false);
		} else if (ch == 263 || ch == 127) {	// BKSP
			if (i.pos > 0)
				i.buf.erase(--i.pos, 1);
		} else if (ch == 330) {	// DEL
			if (i.pos < i.buf.size())
				i.buf.erase(i.pos, 1);
		} else if (ch == 21) {	// CTRL+U
			if (!upasswd)
				i.clpbrd.cut(true);
		} else if (ch == 23) {	// CTRL+W
			if (!upasswd)
				i.clpbrd.cut(false);
		} else if (ch == 25) {	// CTRL+Y
			if (!upasswd)
				i.clpbrd.paste();
		} else if (ch == 1) {	// CTRL+A
			i.pos = 0;
			i.begin = 0;
		} else if (ch == 5) {	// CTRL+E
			i.pos = i.buf.size();
			i.begin = 0;
		} else if (ch == 9) {	// TAB
			if (!upasswd)
				i.acompl.get();
		} else if (ch == 32 && !upasswd) {	// SPACE
			if (i.pos > 0 && i.buf[i.pos - 1] != 32 && i.buf[i.pos] != 32)
				i.buf.insert(i.pos++, 1, 32);
		} else if (iz_k(ch)) {	// "NORMAL" char
			i.buf.insert(i.pos++, 1, ch);
		} else {
			continue;
		}
		if (i.buf.size() > 255) {
			i.buf.erase(i.pos -= i.buf.size() - 255, i.buf.size() - 255);
			if (!max) {
				core::io << core::io::msg(L"kewl", L"ERR: length limit already reached");
				max = true;
			} else {
				i.draw();
			}
		} else {
			i.draw();
		}
		if (ch != 9)
			i.acompl.reset();
	}
	trg = i.buf;
	i.hist.buftohist();
	i.buf.clear();
	i.pos = 0;
	i.acompl.reset();
	i.use_normal = false;
	i.draw();
}

io::i::i()
:use_normal(false), use_passwd(false), prompt_normal(L"[(u)]>"), prompt_passwd(L"PASSWD>")
{
}

void io::i::draw()
{
	core::io.scr_mtx.lock();
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	draw(max_x, max_y);
	core::io.scr_mtx.unlock();
}

void io::i::draw(int max_x, int max_y)
{
	_draw(max_x, max_y);
}

void io::i::_draw(int max_x, int max_y)
{
	std::wstring &prompt = (use_passwd ? prompt_passwd : prompt_normal);
	if (pos >= begin + max_x - prompt.size() - 3)
		begin = pos - max_x + prompt.size() + 3;
	else if (pos < begin)
		begin = pos;
		
	mvprintw(max_y - 1, 0, "%*s", max_x, "");
	mvaddwstr(max_y - 1, 0, prompt.substr(0, max_x).c_str());
	move(max_y - 1, prompt.substr(0, max_x).size() + 1);
	if (!use_passwd)
		addwstr(buf.substr(begin, max_x - prompt.substr(0, max_x).size() - 1).c_str());
	else
		addwstr(std::wstring(buf.substr(begin, max_x - prompt.substr(0, max_x).size() - 1).size(), 42).c_str());
	move(max_y - 1, pos - begin + prompt.substr(0, max_x).size() + 1);
	refresh();
}

io::i::acompl::acompl()
:prez(L"/@")
{
	reset();
}

void io::i::acompl::reset()
{
	possibz.clear();
	iter = 0;
	second = false;
}

void io::i::acompl::get()
{
	std::wstring search;
	wchar_t pre = 0;
	bool pre_hidden;
	if (possibz.size() == 0) {
		int tmp_begin = core::io.i.buf.substr(0, core::io.i.pos).find_last_of(32);
		if (tmp_begin == std::wstring::npos)
			tmp_begin = 0;
		else
			tmp_begin++;
		int tmp_end = core::io.i.buf.find(32, core::io.i.pos);
		if (tmp_end == std::wstring::npos)
			tmp_end = core::io.i.buf.size();
		search = core::io.i.buf.substr(tmp_begin, tmp_end - tmp_begin);

		if (search.size() > 0)
			pre = search[0];

		if (prez.find(pre) != std::wstring::npos) {
			search.erase(0, 1);
			pre_hidden = false;
		} else {
			int tmp_pos = (core::io.i.buf[core::io.i.pos - 1] == 32 ? 1 : 0);
			std::wstring tmp0 = core::io.trim(core::io.i.buf), tmp1 = tmp0.substr(0, core::io.i.pos);
			while (tmp1.size() > 0) {	// get current arg #
				int i = tmp1.find(32);
				if (i == std::wstring::npos)
					i = tmp1.size();
				else
					i++;
				tmp1.erase(0, i);
				tmp_pos++;
			}
			if (tmp_pos <= 1)
				return;
			int tmp_pos1 = tmp0.find(32);
			if (tmp_pos1 == std::wstring::npos)
				tmp_pos1 = tmp0.size();
			tmp0.erase(tmp_pos1);
			if (tmp0[0] == L'/')
				tmp0.erase(0, 1);
			else
				return;
			pre = core::exec.gacompl(tmp0, tmp_pos - 2);
			pre_hidden = true;
		}

		switch (pre) {
		case L'/':
			core::exec.gcmdz(possibz);
			break;
		case L'@':
			core::serv.status.gotherz(possibz);
			break;
		default:
			return;
		}
		int i = 0;
		while (true) {
			if (i >= possibz.size())
				break;
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
				if (!pre_hidden)
					possibz[i].insert(0, 1, pre);
				i++;
			} else {
				possibz.erase(possibz.begin() + i);
			}
		}
	}
	if (possibz.size() == 0)
		return;
	int tmp_pos_begin = core::io.i.buf.substr(0, core::io.i.pos - (second ? 1 : 0)).find_last_of(32);
	if (tmp_pos_begin == std::wstring::npos)
		tmp_pos_begin = 0;
	else
		tmp_pos_begin++;
	core::io.i.buf.erase(tmp_pos_begin, core::io.i.pos - tmp_pos_begin);
	core::io.i.pos -= core::io.i.pos - tmp_pos_begin;

	core::io.i.buf.insert(core::io.i.pos, possibz[iter]);
	core::io.i.pos += possibz[iter].size();
	if (core::io.i.buf[core::io.i.pos] != 32)
		core::io.i.buf.insert(core::io.i.pos, 1, 32);
	core::io.i.pos++;
	iter = (iter + 1) % possibz.size();
	second = true;
	if (possibz.size() == 1)
		reset();
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

void io::i::hist::buftohist()
{
	if (core::io.trim(core::io.i.buf).size() == 0)
		goto remk_win;
	if (len < 32)
		len++;
	for (int i = len - 1; i > 1; i--)
		body[i] = body[i - 1];

	body[1] = core::io.i.buf;
	body[0].clear();

	remk_win:
	pos = 0;
}

void io::i::hist::histtobuf(bool bacc)
{
	if (bacc && pos == 0 || !bacc && pos == len - 1)
		return;
	if (pos == 0)
		body[0] = core::io.i.buf;
	
	short int way = bacc ? -1 : 1;
	core::io.i.buf = body[pos += way];
	core::io.i.pos = core::io.i.buf.size();
	core::io.i.begin = 0;
}

void io::operator<<(msg da_msg)
{
	if (!da_msg.gvalid())
		return;
	if (o.len < 320)
		o.len++;
	for (int i = o.len - 1; i > 0; i--)
		o.msgz[i] = o.msgz[i - 1];
	o.msgz[0] = da_msg;
	if (o.pos0 != 0 || o.pos1 != 0) {
		if (o.pos0 + 1 < o.len)
			o.pos0++;
		else
			o.pos1 = 0;
	}
	o.new_msg = true;
	core::io.mkwin();
}

void io::cls()
{
	for (int i = 0; i < 320; i++)
		o.msgz[i] = core::io::msg();
	mkwin();
}

void io::beep(bool interactive)
{
	if (!beep_on) {
		if (interactive)
			core::io << core::io::msg(L"kewl", L"*disabled*");
		return;
	}
	timeval tmp;
	gettimeofday(&tmp, NULL);
	if (1000000 * (tmp.tv_sec - beep_tm.tv_sec) + tmp.tv_usec - beep_tm.tv_usec < 1000 * beep_delay) {
		if (interactive)
			core::io << core::io::msg(L"kewl", L"*delay not elapsed*");
		return;
	}
	beep_tm = tmp;
	std::wcout << L'\a' << std::flush;
	if (interactive)
		core::io << core::io::msg(L"kewl", L"*beep*");
}

void io::beep()
{
	beep(false);
}

void io::beep_sdelay(unsigned int da_delay)
{
	if (da_delay >= 0)
		beep_delay = da_delay;
}

void io::beep_son(bool da_on)
{
	beep_on = da_on;
}

bool io::beep_gon()
{
	return beep_on;
}

int io::beep_gdelay()
{
	return beep_delay;
}

io::o::o()
{
	title = L"Konverzace Everybody Will Like ";
	title += core::io.ver_echo(VERSION);
	core::io << core::io::msg(L"kewl", L" _              _ ");
	core::io << core::io::msg(L"kewl", L"| |_______ __ _| |");
	core::io << core::io::msg(L"kewl", L"| / / -_) V  V / |");
	core::io << core::io::msg(L"kewl", L"|_\\_\\___|\\_/\\_/|_|");
	core::io << core::io::msg(L"kewl", title);
}

int io::o::glen()
{
	return len;
}

int io::o::gpos0()
{
	return pos0;
}

int io::o::gpos1()
{
	return pos1;
}

int io::o::glinez(int id, int max_x, int max_y)
{
	int l = 0, rest = msgz[id].gfrom().size() + msgz[id].gbody().size();
	while (rest > 0) {
		rest -= max_x - 4;
		l++;
	}
	return l;
}

std::wstring io::o::gtitle()
{
	return title;
}

void io::o::echo(int id, int max_x, int max_y, int omit0, int omit1)
{
	int tmp_pos, ltotal = glinez(id, max_x, max_y), lcount = 0, tmp_begin, tmp_inc;
	std::wstring tmp_from(L"<");
	tmp_from += msgz[id].gfrom();
	tmp_from += L"> ";
	attron(A_BOLD);
	attron(COLOR_PAIR(10 + msgz[id].gcol0()));
	
	for (tmp_pos = 0;;) {
		lcount++;
		if (tmp_pos == 0)
			tmp_inc = max_x - 1;
		else
			tmp_inc = max_x - 4;
		if (lcount > omit0 && lcount <= ltotal - omit1) {
			printw("\n");
			if (tmp_pos != 0)
				printw("   ");
			addwstr(tmp_from.substr(tmp_pos, tmp_inc).c_str());
		}
		if (tmp_pos + tmp_inc > tmp_from.size()) {
			tmp_begin = tmp_from.substr(tmp_pos, tmp_inc).size() + (tmp_pos == 0 ? 0 : 3);
			break;
		}
		tmp_pos += tmp_inc;
	}

	attroff(COLOR_PAIR(10 + msgz[id].gcol0()));
	attroff(A_BOLD);
	attron(COLOR_PAIR(msgz[id].gcol1()));

	for (tmp_pos = 0; tmp_pos < msgz[id].gbody().size();) {
		if (tmp_pos == 0) {
			tmp_inc = max_x - tmp_begin - 1;
		} else {
			tmp_inc = max_x - 4;
			lcount++;
		}
		bool end_col1 = (msgz[id].gw0() > tmp_pos && msgz[id].gw0() <= tmp_pos + tmp_inc) ? true : false;
		if (lcount > omit0 && lcount <= ltotal - omit1) {
			if (tmp_pos != 0)
				printw("\n   ");
			if (!end_col1) {
				addwstr(msgz[id].gbody().substr(tmp_pos, tmp_inc).c_str());
			} else {
				addwstr(msgz[id].gbody().substr(tmp_pos, msgz[id].gw0() - tmp_pos).c_str());
				attroff(COLOR_PAIR(msgz[id].gcol1()));
				addwstr(msgz[id].gbody().substr(msgz[id].gw0(), tmp_inc + tmp_pos - msgz[id].gw0()).c_str());
			}
		}
		if (end_col1)
			attroff(COLOR_PAIR(msgz[id].gcol1()));
		tmp_pos += tmp_inc;
	}
}

void io::o::scrll(bool bacc)
{
	int max_x, max_y, ldiff, tmp_lcount = 0, i, way = (bacc ? -1 : 1), new_pos0, new_pos1;
	getmaxyx(stdscr, max_y, max_x);
	ldiff = (max_y - 3) / 4;
	if (ldiff <= 0)
		ldiff = 1;


	if (pos0 == 0 && pos1 == 0) {
		for (i = 0; i < glen() && tmp_lcount < max_y - 3; i++)
			tmp_lcount += glinez(i, max_x, max_y);
		pos0 = i - 1;
		pos1 = tmp_lcount - max_y + 3;
		if (pos1 < 0)
			pos1 = 0;
	}

	if (way == 1) {
		tmp_lcount = pos1;
		for (i = pos0 + 1; i >= 0 && i < glen() && tmp_lcount < ldiff; i++)
			tmp_lcount += glinez(i, max_x, max_y);
		new_pos0 = i - 1;
		new_pos1 = tmp_lcount - ldiff;
	} else {
		tmp_lcount = 0;
		for (i = pos0; i >=0 && i < glen(); i--) {
			tmp_lcount += glinez(i, max_x, max_y);
			if (i == pos0)
				tmp_lcount -= pos1;
			if (tmp_lcount > ldiff)
				break;
		}
		new_pos0 = i;
		new_pos1 = glinez(new_pos0, max_x, max_y) - tmp_lcount + ldiff;
	}

	if (new_pos1 < 0)
		new_pos1 = 0;
	tmp_lcount = -new_pos1;
	for (i = new_pos0; i >= 0 && tmp_lcount < max_y - 3; i--)
		tmp_lcount += glinez(i, max_x, max_y);

	if (i + 1 == 0 && tmp_lcount - max_y + 3 <= 0) {
		pos0 = 0;
		pos1 = 0;
	} else {
		pos0 = new_pos0;
		pos1 = new_pos1;
	}
	core::io.mkwin();
}

void io::o::jump(bool bacc)
{
	if (bacc) {
		pos0 = 0;
		pos1 = 0;
	} else {
		pos0 = len - 1;
		pos1 = 0;
	}
	core::io.mkwin();
}

void io::o::scrll_resize()
{
	int max_x, max_y, i, tmp_lcount;
	getmaxyx(stdscr, max_y, max_x);
	pos1 = 0;
	tmp_lcount = 0;
	for (i = pos0; i >= 0 && tmp_lcount < max_y - 3; i--)
		tmp_lcount += glinez(i, max_x, max_y);

	if (i + 1 == 0 && tmp_lcount - max_y + 3 <= 0) {
		pos0 = 0;
		pos1 = 0;
	}

	core::io.mkwin();
}

void io::mkwin()
{
	scr_mtx.lock();
	_mkwin();
	scr_mtx.unlock();
}

void io::_mkwin()
{
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	erase();
	int tmp_lcount, j, begin, end, omit0, omit1;
	if (o.gpos0() == 0 && o.gpos1() == 0) {
		tmp_lcount = 0;
		for (j = 0; j < o.glen() && tmp_lcount < max_y - 3; j++)
			tmp_lcount += o.glinez(j, max_x, max_y);
		begin = j - 1;
		end = 0;
		omit0 = tmp_lcount - max_y + 3;
		if (omit0 < 0)
			omit0 = 0;
		omit1 = 0;
	} else {
		tmp_lcount = -o.gpos1();
		for (j = o.gpos0(); j >= 0 && j < o.glen() && tmp_lcount < max_y - 3; j--)
			tmp_lcount += o.glinez(j, max_x, max_y);
		begin = o.gpos0();
		end = j + 1;
		omit0 = o.gpos1();
		omit1 = tmp_lcount - max_y + 3;
	}

	for (int j = begin; j >= end; j--) {
		int tmp_omit0 = 0, tmp_omit1 = 0;
		if (j == begin)
			tmp_omit0 = omit0;
		if (j == end)
			tmp_omit1 = omit1;
		o.echo(j, max_x, max_y, tmp_omit0, tmp_omit1);
	}

	attron(COLOR_PAIR(20));
	mvprintw(0, 0, "%*s", max_x, "");
	mvaddwstr(0, 1, o.gtitle().substr(0, max_x - 1).c_str());
	attroff(COLOR_PAIR(20));
	core::serv.status.draw(max_x, max_y);
	attron(COLOR_PAIR(20));
	if (end != 0 && o.new_msg)
		mvaddwstr(max_y - 2, max_x - 5, std::wstring(L"_NEW_").substr(0, max_x).c_str());
	else
		o.new_msg = false;
	attroff(COLOR_PAIR(20));
	i.draw(max_x, max_y);
}
