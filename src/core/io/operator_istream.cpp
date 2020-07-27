void io::operator<<(msg arg)
{
	if (stfu.gon())
		return;
	int max_x, max_y, tmp_lcount, tmp_omit, i, j;
	basis::termxy(max_x, max_y);
	if (!arg.gvalid())
		return;
	if (o.msgz.size() > 0) {
		if (arg.gfrom() == L"hr" && o.msgz[o.glen() - 1].gfrom() == L"hr") {
			if (o.pos2 == o.glen()) {
				if (o.pos0 == o.pos2)
					o.pos0 = o.glen() - 1;
				o.pos2 = o.glen() - 1;
				mkwin();
			}
			return;
		}
	}
	tmp_lcount = -o.pos1;
	for (i = o.pos0; i >= 0 && tmp_lcount < max_y - 3 && i < o.msgz.size(); i++)
		tmp_lcount += o.glinez(i, max_x, max_y);
	tmp_omit = tmp_lcount - max_y + 3;

	if (o.msgz.size() + 1 > core::env.gint(L"int_output_scrollbucc")) {
		o.msgz.erase(o.msgz.begin(), o.msgz.end() + 1 - core::env.gint(L"int_output_scrollbucc"));
		if (o.pos0 != -2 || o.pos1 != 0) {
			if (o.pos0 > 0)
				o.pos0--;
			else
				o.pos1 = 0;
			if (o.pos2 > 0)
				o.pos2--;
		}
	}
	
	o.msgz.push_back(arg);
	o.new_msg = true;
	
	tmp_lcount = -o.pos1;
	for (j = o.pos0; j >= 0 && j < o.msgz.size() && tmp_lcount < max_y - 3; j++)
		tmp_lcount += o.glinez(j, max_x, max_y);
	
	if ((tmp_omit < 0 || (tmp_omit == 0 && i == o.msgz.size() - 1)) && tmp_lcount - max_y + 3 >= 0) {
		o.pos0 = -2;
		o.pos1 = 0;
	}
	if (iz_ready)
		mkwin();
}

void io::operator<<(std::wstring arg)
{
	core::io << core::io::msg(L"kewl", arg);
}
