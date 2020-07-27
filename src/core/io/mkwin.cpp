void io::mkwin()
{
	scr_mtx.lock();
	_mkwin();
	scr_mtx.unlock();
}

void io::_mkwin()
{
	int max_x, max_y;
	basis::termxy(max_x, max_y);
	if (max_x <= core::env.gint(L"int_usr_max_padding") + core::env.gstr(L"txt_usr_before").size() + core::env.gstr(L"txt_usr_after").size() + 1 + (core::env.gtru(L"bool_show_time") ? 5 + core::env.gstr(L"txt_time_before").size() + core::env.gstr(L"txt_time_after").size() : 0) || max_y < 1)
		return;
	erase();
	int tmp_lcount, j, begin, end, omit0, omit1;
	if (o.gpos0() == -2 && o.gpos1() == 0) {
		tmp_lcount = 0;
		for (j = o.glen() - 1; j >= 0 && tmp_lcount < max_y - 3; j--)
			tmp_lcount += o.glinez(j, max_x, max_y);
		begin = j + 1;
		end = o.glen();
		omit0 = tmp_lcount - max_y + 3;
		if (omit0 < 0)
			omit0 = 0;
		omit1 = 0;
	} else {
		tmp_lcount = -o.gpos1();
		for (j = o.gpos0(); j >= 0 && j < o.glen() && tmp_lcount < max_y - 3; j++)
			tmp_lcount += o.glinez(j, max_x, max_y);
		begin = o.gpos0();
		end = j;
		omit0 = o.gpos1();
		omit1 = tmp_lcount - max_y + 3;
	}

	for (int j = begin; j < end; j++) {
		int tmp_omit0 = 0, tmp_omit1 = 0;
		if (j == begin)
			tmp_omit0 = omit0;
		if (j == end)
			tmp_omit1 = omit1;
		o.echo(j, max_x, max_y, tmp_omit0, tmp_omit1);
	}

	core::env.on(L"attr_header");
	basis::mvrubber(0, 0, max_x);
	basis::mvprint(1, 0, o.gtitle().substr(0, max_x - 1).c_str());
	core::env.off();
	core::serv.status.draw(max_x, max_y);
	core::env.on(L"attr_new_tag");
	if (!(end == o.glen() && omit1 <= 0) && o.new_msg)
		basis::mvprint(max_x - 5, max_y - 2, std::wstring(L"*NEW*").substr(0, max_x).c_str());
	else
		o.new_msg = false;
	core::env.off();
	i.draw(max_x, max_y);
}

