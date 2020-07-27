io::o::o()
:new_msg(false)
{
	title = L"kewl [";
	title += core::io.ver_echo(VERSION);
	title += L"] - https://github.com/patztablook22/kewl";
}

int io::o::glen()
{
	return msgz.size();
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
	if (msgz[id].gfrom() == L"hr")
		return 1;
	uint8_t before = core::env.gstr(L"txt_usr_before").size(), after = core::env.gstr(L"txt_usr_after").size(), padding = core::env.gint(L"int_usr_max_padding");
	int l0 = 0, l1 = 0, rest = msgz[id].gfrom().size();
	if (padding > rest)
		rest = padding;
	uint8_t show_time = 0;
	if (core::env.gtru(L"bool_show_time")) {
		show_time = 5 + core::env.gstr(L"txt_time_before").size() + core::env.gstr(L"txt_time_after").size();
		before += show_time;
	}
	rest += after;
	while (rest > 0) {
		rest -= max_x - 1 - before;
		l0++;
	}
	l0--;
	rest += max_x - (l0 ? 1 + before : 1);	
	rest += msgz[id].glen();
	padding += before + after;
	if (l0)
		rest += before;
	rest -= padding;
	while (rest > 0) {
		rest -= max_x - padding - 1;
		l1++;
	}
	return l0 + l1;
}

std::wstring io::o::gtitle()
{
	return title;
}

void io::o::echo(int id, int max_x, int max_y, int omit0, int omit1)
{
	if (msgz[id].gfrom() == L"hr") {
		basis::print("\n ");
		core::env.on(L"attr_hr");
		basis::print(std::wstring(max_x - 2, core::env.gchar(L"char_hr")));
		core::env.off();
		return;
	}
	int padding = core::env.gint(L"int_usr_max_padding"), before = core::env.gstr(L"txt_usr_before").size(), after = core::env.gstr(L"txt_usr_after").size();
	int tmp_pos = msgz[id].gfrom().size(), ltotal = glinez(id, max_x, max_y), lcount = 1, tmp_begin = 0, tmp_inc;
	std::wstring tmp;
	tmp = core::env.gstr(L"txt_usr_before");
	if (padding > tmp_pos)
		tmp += std::wstring(padding - tmp_pos, 32);
	tmp += msgz[id].gfrom();
	tmp += core::env.gstr(L"txt_usr_after");
	uint8_t show_time = core::env.gtru(L"bool_show_time");
	if (lcount > omit0 && lcount <= ltotal - omit1)
		basis::print("\n");
	if (show_time) {
		std::wstring todo = core::env.gstr(L"txt_time_before") + msgz[id].grecv() + core::env.gstr(L"txt_time_after");
		before += todo.size();
		show_time = todo.size();
		if (lcount > omit0 && lcount <= ltotal - omit1) {
			core::env.on(L"attr_time");
			basis::print(todo);
			core::env.off();
		}
	}

	core::env.on(L"attr_usr_" + msgz[id].gprop0());
	for (tmp_pos = 0;;) {
		tmp_inc = max_x - (tmp_pos ? 1 + before : 1 + show_time);
		if (lcount > omit0 && lcount <= ltotal - omit1) {
			if (tmp_pos != 0) {
				basis::print("\n");
				if (before > 0)
					basis::print(std::wstring(before, 32));
			}
			basis::print(tmp.substr(tmp_pos, tmp_inc));
		}
		if (tmp_pos + tmp_inc >= tmp.size()) {
			tmp_begin = tmp.size() - tmp_pos + (tmp_pos == 0 ? show_time : before);
			break;
		}
		tmp_pos += tmp_inc;
		lcount++;
	}
	
	core::env.off();
	core::env.on(L"attr_body_" + msgz[id].gprop0());
	padding += before + after;
	tmp_pos = tmp_begin;
	std::wstring buf;
	for (int i = 0; i < msgz[id].gbody().size(); i++) {
		if (tmp_pos >= max_x - 1) {
			if (lcount > omit0 && lcount <= ltotal - omit1)
				basis::print(buf);
			lcount++;
			buf = L"\n";
			tmp_pos = 0;
			if (padding > 0) {
				buf += std::wstring(padding, 32);
				tmp_pos += padding;
			}
		}
		wint_t ch = msgz[id].gbody()[i];
		switch (ch) {
		case L'\\':
			if (lcount > omit0 && lcount <= ltotal - omit1)
				basis::print(buf);
			buf.clear();
			i++;
			switch (msgz[id].gbody()[i]) {
			case L'0':
				core::env.off();
				core::env.on(L"attr_body_" + msgz[id].gprop0());
				break;
			case L'1':
				core::env.off();
				core::env.on(L"attr_body_hl_1");
				break;
			case L'2':
				core::env.off();
				core::env.on(L"attr_body_hl_2");
				break;
			case L'3':
				core::env.off();
				core::env.on(L"attr_body_hl_3");
				break;
			case L'4':
				core::env.off();
				core::env.on(L"attr_body_hl_4");
				break;
			case L'5':
				core::env.off();
				core::env.on(L"attr_body_hl_5");
				break;
			case L'6':
				core::env.off();
				core::env.on(L"attr_body_hl_6");
				break;
			case L'7':
				core::env.off();
				core::env.on(L"attr_body_hl_7");
				break;
			case L'e':
				core::env.off();
				core::env.on(L"attr_body_hl_err");
				break;
			case L'w':
				core::env.off();
				core::env.on(L"attr_body_hl_warn");
				break;
			case L'p':
				core::env.off();
				core::env.on(L"attr_body_hl_private");
				break;
			case L'\\':
			case L'"':
			case L'/':
				tmp_pos++;
				buf += msgz[id].gbody()[i];
				break;
			}
			break;
		default:
			buf += ch;
			tmp_pos++;
			break;
		}
			
	}
	if (lcount > omit0 && lcount <= ltotal - omit1)
		basis::print(buf);
	core::env.off();
}

void io::o::scrll(bool bacc)
{
	int max_x, max_y, ldiff, tmp_lcount = 0, i, way = (bacc ? -1 : 1);
	basis::termxy(max_x, max_y);
	if (max_x <= core::env.gint(L"int_usr_max_padding") + core::env.gstr(L"txt_usr_before").size() + core::env.gstr(L"txt_usr_after").size() + 1 + (core::env.gtru(L"bool_show_time") ? 5 + core::env.gstr(L"txt_time_before").size() + core::env.gstr(L"txt_time_after").size() : 0) || max_y < 1)
		return;
	ldiff = (max_y - 3) / 4;
	if (ldiff <= 0)
		ldiff = 1;

	if (pos0 == -2 && pos1 == 0) {
		for (i = glen() - 1; i >= 0 && tmp_lcount < max_y - 3; i--)
			tmp_lcount += glinez(i, max_x, max_y);
		pos0 = i + 1;
		pos1 = tmp_lcount - max_y + 3;
		if (pos1 < 0)
			pos1 = 0;
	}

	if (way == 1) {
		tmp_lcount = pos1;
		for (i = pos0 - 1; i >= 0 && i < glen() && tmp_lcount < ldiff; i--)
			tmp_lcount += glinez(i, max_x, max_y);
		pos0 = i + 1;
		pos1 = tmp_lcount - ldiff;
	} else {
		if (pos0 == -1)
			return;
		tmp_lcount = 0;
		for (i = pos0; i >=0 && i < glen(); i++) {
			tmp_lcount += glinez(i, max_x, max_y);
			if (i == pos0)
				tmp_lcount -= pos1;
			if (tmp_lcount > ldiff)
				break;
		}
		pos0 = i;
		if (i < glen())
			pos1 = glinez(pos0, max_x, max_y) - tmp_lcount + ldiff;
		else
			pos1 = 0;
	}

	if (pos1 < 0)
		pos1 = 0;
	if (way == -1)
		scrll_chk(max_x, max_y);
	core::io.mkwin();
}

void io::o::jump(bool bacc)
{
	int max_x, max_y;
	basis::termxy(max_x, max_y);
	if (max_x <= core::env.gint(L"int_usr_max_padding") + core::env.gstr(L"txt_usr_before").size() + core::env.gstr(L"txt_usr_after").size() + 1 + (core::env.gtru(L"bool_show_time") ? 5 + core::env.gstr(L"txt_time_before").size() + core::env.gstr(L"txt_time_after").size() : 0) || max_y < 1)
		return;
	if (bacc) {
		int tmp_lcount = 0, i;
		for (i = msgz.size() - 1; i >= 0 && tmp_lcount < max_y - 3; i--)
			tmp_lcount += glinez(i, max_x, max_y);
		if (i + 1 >= pos2)
			pos0 = -2;
		else
			pos0 = pos2;
		pos1 = 0;
	} else {
		pos0 = 0;
		pos1 = 0;
	}
	core::io.mkwin();
}

void io::o::scrll_resize()
{
	int max_x, max_y;
	basis::termxy(max_x, max_y);
	if (pos0 != -2 || pos1 != 0)
		scrll_chk(max_x, max_y);
	core::io.mkwin();
}

void io::o::scrll_chk(int max_x, int max_y)
{
	if (max_x <= core::env.gint(L"int_usr_max_padding") + core::env.gstr(L"txt_usr_before").size() + core::env.gstr(L"txt_usr_after").size() + 1 + (core::env.gtru(L"bool_show_time") ? 5 + core::env.gstr(L"txt_time_before").size() + core::env.gstr(L"txt_time_after").size() : 0) || max_y < 1)
		return;
	int i, tmp_lcount;
	tmp_lcount = -pos1;
	for (i = pos0; i < glen() && tmp_lcount < max_y - 3; i++)
		tmp_lcount += glinez(i, max_x, max_y);
	if (tmp_lcount < max_y - 3 && pos0 >= pos2) {
		jump(true);
	}
}
