io::ui::ui()
{
	colorz[L"blacc"] = COLOR_BLACK;
	colorz[L"red"] = COLOR_RED;
	colorz[L"green"] = COLOR_GREEN;
	colorz[L"yolow"] = COLOR_YELLOW;
	colorz[L"blu"] = COLOR_BLUE;
	colorz[L"magenta"] = COLOR_MAGENTA;
	colorz[L"cyan"] = COLOR_CYAN;
	colorz[L"wheit"] = COLOR_WHITE;

	weightz[L"dim"] = A_DIM;
	weightz[L"normal"] = A_NORMAL;
	weightz[L"bold"] = A_BOLD;
	weightz[L"italic"] = A_ITALIC;
	init();
}

void io::ui::init()
{
	propertiez0.clear();
	propertiez1.clear();
	propertiez2.clear();

	new property0(L"frame_base", {L"blacc", L"wheit", L"normal"});
	new property0(L"frame_hl", {L"blacc", L"wheit", L"bold"});
	new property0(L"new_tag", {L"blacc", L"wheit", L"normal"});

	new property0(L"usr_kewl", {L"wheit", L"blacc", L"bold"});
	new property0(L"usr_serv", {L"blacc", L"blacc", L"bold"});
	new property0(L"usr_otherz", {L"wheit", L"blacc", L"dim"});

	new property0(L"hr", {L"wheit", L"blacc", L"dim"});
	new property0(L"err", {L"wheit", L"blacc", L"bold"});
	new property0(L"warn", {L"wheit", L"blacc", L"bold"});
	new property0(L"private", {L"blacc", L"blacc", L"bold"});

	new property0(L"prompt_normal", {L"wheit", L"blacc", L"normal"});
	new property0(L"prompt_passwd", {L"wheit", L"blacc", L"bold"});

	new property1(L"prompt_normal", 0, 8, L"PROMPT> ");
	new property1(L"prompt_passwd", 0, 8, L"PASSWD> ");

	new property2(L"passwd_input", L'*');
	new property2(L"hr", L'-');

	if (!has_colors())
		core::io << core::io::msg(L"kewl", L"WARN: ur terminal doez not support colorz");
	else if (!can_change_color())
		core::io << core::io::msg(L"kewl", L"WARN: ur terminal cannot redefine colorz");
}

uint8_t io::ui::set(std::wstring name, std::vector<std::wstring> da_valuez)
{
	if (propertiez0.find(name) == propertiez0.end())
		return 1;

	return propertiez0[name]->svaluez(da_valuez);
}

uint8_t io::ui::def_col(std::wstring name, std::vector<unsigned int> da_value)
{
	if (da_value.size() != 3)
		return 2;
	if (name.size() < 3 || name.size() > 8)
		return 3;
	for (int i = 0; i < 3; i++) {
		if (da_value[i] < 0 || da_value[i] > 1000)
			return 4 + i;
	}
	if (!can_change_color())
		return -1;

	if (colorz.find(name) == colorz.end()) {
		if (colorz.size() > COLORS)
			return 1;
		init_color(colorz.size(), da_value[0], da_value[1], da_value[2]);
		colorz[name] = colorz.size();
	} else {
		if (init_color(colorz[name], da_value[0], da_value[1], da_value[2]) == ERR)
			return -1;
	}
	return 0;	
}

uint8_t io::ui::reset(std::wstring name)
{
	if (propertiez0.find(name) != propertiez0.end())
		propertiez0[name]->reset();
	else if (propertiez1.find(name) != propertiez1.end())
		propertiez1[name]->reset();
	else if (propertiez2.find(name) != propertiez2.end())
		propertiez2[name]->reset();
	else
		return 1;
	return 0;
}

void io::ui::reset()
{
	for (const std::pair<std::wstring, property0 *> &name: propertiez0)
		name.second->reset();
	for (const std::pair<std::wstring, property1 *> &name: propertiez1)
		name.second->reset();
	for (const std::pair<std::wstring, property2 *> &name: propertiez2)
		name.second->reset();
}

void io::ui::gcolorz(std::vector<std::wstring> &trg)
{
	trg.clear();
	for (const std::pair<std::wstring, unsigned int> &color: colorz)
		trg.push_back(color.first);
}

void io::ui::gweightz(std::vector<std::wstring> &trg)
{
	trg.clear();
	for (const std::pair<std::wstring, unsigned int> &weight: weightz)
		trg.push_back(weight.first);
}

void io::ui::gpropertiez(std::vector<std::wstring> &trg)
{
	trg.clear();
	for (const std::pair<std::wstring, property0 *> property: propertiez0)
		trg.push_back(property.first);
	for (const std::pair<std::wstring, property1 *> property: propertiez1)
		trg.push_back(property.first);
	for (const std::pair<std::wstring, property2 *> property: propertiez2)
		trg.push_back(property.first);
}

io::ui::property0::property0(std::wstring name, std::vector<std::wstring> da_defaultz)
{
	if (da_defaultz.size() != 3) {
		delete this;
		return;
	}
	if ( \
		core::io.ui.existz(L"attr_" + name) || \
		core::io.ui.colorz.find(da_defaultz[0]) == core::io.ui.colorz.end() || \
		core::io.ui.colorz.find(da_defaultz[1]) == core::io.ui.colorz.end() || \
		core::io.ui.weightz.find(da_defaultz[2]) == core::io.ui.weightz.end() \
		) {
		delete this;
		return;
	}
	defaultz[0] = core::io.ui.colorz[da_defaultz[0]];
	defaultz[1] = core::io.ui.colorz[da_defaultz[1]];
	defaultz[2] = core::io.ui.weightz[da_defaultz[2]];

	core::io.ui.propertiez0[L"attr_" + name] = this;
	id = core::io.ui.propertiez0.size();
	svaluez(da_defaultz);
}

uint8_t io::ui::property0::svaluez(std::vector<std::wstring> da_valuez)
{
	if (da_valuez.size() != 3)
		return 2;
	if (core::io.ui.colorz.find(da_valuez[0]) == core::io.ui.colorz.end())
		return 3;
	if (core::io.ui.colorz.find(da_valuez[1]) == core::io.ui.colorz.end())
		return 4;
	if (core::io.ui.weightz.find(da_valuez[2]) == core::io.ui.weightz.end())
		return 5;
	unsigned int new_valuez[3];
	new_valuez[0] = core::io.ui.colorz[da_valuez[0]];
	new_valuez[1] = core::io.ui.colorz[da_valuez[1]];
	new_valuez[2] = core::io.ui.weightz[da_valuez[2]];
	bool sacv = false;
	if (valuez[0] == new_valuez[0] && valuez[1] == new_valuez[1] && valuez[2] == new_valuez[2]) 
		sacv = true;
	for (int i = 0; i < 3; i++)
		valuez[i] = new_valuez[i];
	init_pair(id, valuez[0], valuez[1]);
	core::io.mkwin();
	return sacv ? -1 : 0;
}

void io::ui::property0::reset()
{
	for (int i = 0; i < 3; i++)
		valuez[i] = defaultz[i];
	init_pair(id, valuez[0], valuez[1]);
	core::io.mkwin();
}

void io::ui::property0::on()
{
	attron(COLOR_PAIR(id));
	attron(valuez[2]);
}

void io::ui::property0::off()
{
	attroff(COLOR_PAIR(id));
	attroff(valuez[2]);
}

void io::ui::on(std::wstring name)
{
	if (propertiez0.find(name) == propertiez0.end())
		return;
	propertiez0[name]->on();
}

void io::ui::off(std::wstring name)
{
	if (propertiez0.find(name) == propertiez0.end())
		return;
	propertiez0[name]->off();
}

std::wstring io::ui::gstr(std::wstring name)
{
	if (propertiez1.find(name) == propertiez1.end())
		return L"";
	return propertiez1[name]->gstr();
}

std::wstring io::ui::grange(std::wstring name)
{
	if (propertiez1.find(name) == propertiez1.end())
		return L"";
	return propertiez1[name]->grange();
}

io::ui::property1::property1(std::wstring name, unsigned int min, unsigned int max, std::wstring ddd)
{
	if ( \
		core::io.ui.existz(L"txt_" + name) || \
		ddd.size() < min || \
		ddd.size() > max || \
		!core::io.iz_k(ddd) \
		) {
		delete this;
		return;
	}
	range[0] = min;
	range[1] = max;
	da_default = ddd;
	svalue(ddd);
	core::io.ui.propertiez1[L"txt_" + name] = this;
}

uint8_t io::ui::set(std::wstring name, std::wstring da_value)
{
	if (propertiez1.find(name) == propertiez1.end())
		return 1;
	return propertiez1[name]->svalue(da_value);
}

uint8_t io::ui::property1::svalue(std::wstring da_value)
{
	if (da_value.size() < range[0] || da_value.size() > range[1] || !core::io.iz_k(da_value))
		return 3;
	bool sacv = false;
	if (da_value == value)
		sacv = true;
	value = da_value;
	core::io.mkwin();
	return sacv ? -1 : 0;
}

void io::ui::property1::reset()
{
	value = da_default;
	core::io.mkwin();
}

std::wstring io::ui::property1::gstr()
{
	return value;
}

std::wstring io::ui::property1::grange()
{
	return std::to_wstring(range[0]) + L" - " + std::to_wstring(range[1]);
}

io::ui::property2::property2(std::wstring name, wint_t ddd)
{
	if ( \
		core::io.ui.existz(L"char_" + name) || \
		!core::io.iz_k(ddd) \
		) {
		return;
	}
	da_default = ddd;
	svalue(da_default);
	core::io.ui.propertiez2[L"char_" + name] = this;
}

void io::ui::echo(std::wstring name, unsigned int n)
{
	if (propertiez2.find(name) == propertiez2.end())
		return;
	propertiez2[name]->echo(n);
}

void io::ui::property2::echo(unsigned int n)
{
	addwstr(std::wstring(n, value).c_str());
}

uint8_t io::ui::set(std::wstring name, wint_t da_value)
{
	if (propertiez2.find(name) == propertiez2.end())
		return 1;
	return propertiez2[name]->svalue(da_value);
}

uint8_t io::ui::property2::svalue(wint_t da_value)
{
	if (!core::io.iz_k(da_value))
		return 3;
	bool sacv = false;
	if (da_value == value)
		sacv = true;
	value = da_value;
	core::io.mkwin();
	return sacv ? -1 : 0;
}

void io::ui::property2::reset()
{
	value = da_default;
}

bool io::ui::existz(std::wstring name)
{
	if (propertiez0.find(name) != propertiez0.end())
		return true;
	if (propertiez1.find(name) != propertiez1.end())
		return true;
	if (propertiez2.find(name) != propertiez2.end())
		return true;
	return false;
}

io::io()
:beep_on(true), beep_delay(667)
{
	std::locale::global(std::locale("en_US.UTF-8"));
}

void io::init(bool init_ui)
{
	initscr();
	noecho();
	keypad(stdscr, true);
	raw();
	start_color();
	use_default_colors();

	if (init_ui) {
		core::io.ui.init();
		for (int i = 0; i < 8; i++)
			color_content(i, &default_contentz[i][0], &default_contentz[i][1], &default_contentz[i][2]);
		init_color(COLOR_WHITE, 742, 742, 742);
	}

	beep_tm.tv_sec = 0;
	beep_tm.tv_usec = 0;

	mkwin();
}

io::~io()
{
	for (int i = 0; i < 8; i++)
		init_color(i, default_contentz[i][0], default_contentz[i][1], default_contentz[i][2]);
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

std::wstring io::sha256(std::wstring input)
{
	std::string tmp(input.begin(), input.end());
	const char *string = tmp.c_str();
	char outputBuffer[65];
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, string, strlen(string));
	SHA256_Final(hash, &sha256);
	for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
	}
	outputBuffer[64] = 0;
	tmp = outputBuffer;
	return std::wstring(tmp.begin(), tmp.end());
}

std::wstring io::trim(std::wstring s)
{
	size_t i, done = 0;
	std::replace(s.begin(), s.end(), 9, 32);
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
	if (da_from == L"hr") 
	{
		from = L"hr";
		body = L"";
		prop0 = L"";
		prop1 = L"";
		w0 = 0;
		valid = true;
		return;
	}

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
		prop0 = L"attr_usr_kewl";
	else if (from == std::wstring(L"serv"))
		prop0 = L"attr_usr_serv";
	else
		prop0 = L"attr_usr_otherz";

	w0 = body.find(' ');
	if (w0 == std::wstring::npos)
		w0 = da_body.size();

	if (prop0 != L"attr_usr_otherz" && core::io.trim(body).substr(0, 5) == std::wstring(L"ERR: "))
		prop1 = L"attr_err";
	else if (prop0 != L"attr_usr_otherz" && core::io.trim(body).substr(0, 6) == std::wstring(L"WARN: "))
		prop1 = L"attr_warn";
	else if (prop0 != L"attr_usr_kewl" && body[0] == L'@' && w0 > 3 && w0 < 17)
		prop1 = L"attr_private";
	else
		prop1 = L"";
	valid = true;
}

void io::msg::operator=(msg da_msg)
{
	this->from = da_msg.from;
	this->body = da_msg.body;
	this->prop0 = da_msg.prop0;
	this->prop1 = da_msg.prop1;
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

std::wstring io::msg::gprop0()
{
	return prop0;
}

std::wstring io::msg::gprop1()
{
	return prop1;
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
		i.use_passwd = true;
	i.buf.erase();
	i.pos = 0;
	i.draw();
}

void io::operator>>(std::wstring &trg)
{
	if (i.use_normal)
		return;
	else
		i.use_normal = true;
	wint_t ch;
	i.buf.clear();
	i.pos = 0;
	bool max = (i.buf.size() > 255);

	for (;;) {
		get_wch(&ch);
		switch (ch) {
		case -1:
		case 0:
			// ERR
			std::wcerr << L"ERR: failed to capture keypress\n";
			exit(1);
		case 10:
			// ENTER
			if (!i.use_passwd) {
				trg = i.buf;
				i.hist.buftohist();
				i.buf.clear();
				i.pos = 0;
				i.acompl.reset();
				i.use_normal = false;
				i.draw();
				return;
			} else {
				try {
					core::serv << L'|' + sha256(i.buf);
				} catch (...) {}
				i.use_passwd = false;
				i.buf.clear();
				i.pos = 0;
			}
			break;
		case 410:
		// RESIZE
			o.scrll_resize();
			continue;
		case 258:
			// KEY_DOWN
			if (!i.use_passwd)
				i.hist.histtobuf(true);
			break;
		case 259:
			// KEY_UP
			if (!i.use_passwd)
				i.hist.histtobuf(false);
			break;
		case 260:
			// KEY_LEFT
			if (i.pos > 0)
				i.pos--;
			break;
		case 261:
			// KEY_RIGHT
			if (i.pos < i.buf.size())
				i.pos++;
			break;
		case 338:
			// PG_DN
			o.scrll(true);
			break;
		case 339:
			// PG_UP
			o.scrll(false);
			break;
		case 360:
			// END
			o.jump(true);
			break;
		case 262:
			// HOME
			o.jump(false);
			break;
		case 263:
		case 127:
			// BKSP
			if (i.pos > 0) {
				i.buf.erase(--i.pos, 1);
				if (i.buf[i.pos] == 32 && i.buf[i.pos - 1] == 32)
					i.buf.erase(--i.pos, 1);
			}
			break;
		case 330:
			// DEL
			if (i.pos < i.buf.size()) {
				i.buf.erase(i.pos, 1);
				if (i.buf[i.pos] == 32 && i.buf[i.pos - 1] == 32)
					i.buf.erase(i.pos, 1);
			}
			break;
		case 21:
			// CTRL+U
			if (!i.use_passwd)
				i.clpbrd.cut(true);
			break;
		case 23:
			// CTRL+W
			if (!i.use_passwd) {
				i.clpbrd.cut(false);
				if (i.buf[i.pos] == 32 && i.buf[i.pos - 1] == 32)
					i.buf.erase(i.pos, 1);
			}
			break;
		case 25:
			// CTRL+Y
			if (!i.use_passwd)
				i.clpbrd.paste();
			break;
		case 1:
			// CTRL+A
			i.pos = 0;
			i.begin = 0;
			break;
		case 5:
			// CTRL+E
			i.pos = i.buf.size();
			i.begin = 0;
			break;
		case 9:
			// TAB
			if (!i.use_passwd)
				i.acompl.get();
			break;
		case 32:
			// SPACE
			if ((i.pos > 0 && i.buf[i.pos - 1] != 32 && i.buf[i.pos] != 32) || i.use_passwd)
				i.buf.insert(i.pos++, 1, 32);
			break;
		default:
			// "NORMAL" char
			if (iz_k(ch))
				i.buf.insert(i.pos++, 1, ch);
			else
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
	std::wstring prompt = core::io.ui.gstr(use_passwd ? L"txt_prompt_passwd" : L"txt_prompt_normal");
	if (pos >= begin + max_x - prompt.size() - 2)
		begin = pos - max_x + prompt.size() + 2;
	else if (pos < begin)
		begin = pos;
		
	mvprintw(max_y - 1, 0, "%*s", max_x, "");
	core::io.ui.on(use_passwd ? L"attr_prompt_passwd" : L"attr_prompt_normal");
	mvaddwstr(max_y - 1, 0, prompt.substr(0, max_x).c_str());
	core::io.ui.off(use_passwd ? L"attr_prompt_passwd" : L"attr_prompt_normal");
	move(max_y - 1, prompt.substr(0, max_x).size());
	if (!use_passwd)
		addwstr(buf.substr(begin, max_x - prompt.substr(0, max_x).size()).c_str());
	else
		core::io.ui.echo(L"char_passwd_input", buf.substr(begin, max_x - prompt.substr(0, max_x).size()).size());
	move(max_y - 1, pos - begin + prompt.substr(0, max_x).size());
	refresh();
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
}

void io::i::acompl::get()
{
	bool first = possibz.size() == 0;
	if (first) {
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
					core::exec.macroz.gmacroz(possibz);
					for (int i = 0; i < possibz.size(); i++)
						possibz[i].insert(0, 1, L'_');
				} else {
					core::exec.gcmdz(possibz);
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
			core::exec.cmd_gacompl(arg, possibz);
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
			if (j == search.size())
				i++;
			else
				possibz.erase(possibz.begin() + i);
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
	iter = (iter + 1) % possibz.size();
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
	if (stfu.gon())
		return;
	int max_x, max_y, tmp_lcount, tmp_omit, i, j;
	getmaxyx(stdscr, max_y, max_x);
	if (!da_msg.gvalid())
		return;
	if (o.len > 0) {
		if (da_msg.gfrom() == L"hr" && o.msgz[0].gfrom() == L"hr") {
			if (o.pos2 == -1) {
				if (o.pos0 == -1)
					o.pos0 = 0;
				o.pos2 = 0;
				mkwin();
			}
			return;
		}
	}
	tmp_lcount = -o.pos1;
	for (i = o.pos0; i >= 0 && tmp_lcount < max_y - 3 && i < o.len; i--)
		tmp_lcount += o.glinez(i, max_x, max_y);
	tmp_omit = tmp_lcount - max_y + 3;
	if (o.len < 320)
		o.len++;
	for (int i = o.len - 1; i > 0; i--)
		o.msgz[i] = o.msgz[i - 1];
	o.msgz[0] = da_msg;
	if (o.pos0 != -2 || o.pos1 != 0) {
		if (o.pos0 + 1 < o.len)
			o.pos0++;
		else
			o.pos1 = 0;
	}
	if (o.pos2 != o.len - 2)
		o.pos2++;
	o.new_msg = true;
	
	tmp_lcount = -o.pos1;
	for (j = o.pos0; j >= 0 && j < o.len && tmp_lcount < max_y - 3; j--)
		tmp_lcount += o.glinez(j, max_x, max_y);
	
	 if ((tmp_omit < 0 || (tmp_omit == 0 && i == -1)) && tmp_lcount - max_y + 3 >= 0) {
		o.pos0 = -2;
		o.pos1 = 0;
	}
	mkwin();
}

void io::cls()
{
	o.pos0 = -1;
	o.pos1 = 0;
	o.pos2 = -1;
	mkwin();
}

void io::da_erase()
{
	o.len = 0;
	o.pos0 = -2;
	o.pos1 = 0;
	o.pos2 = 0;
	for (int i = 0; i < 320; i++)
		o.msgz[i] = core::io::msg();
	core::io << core::io::msg(L"kewl", L" _              _ ");
	core::io << core::io::msg(L"kewl", L"| |_______ __ _| |");
	core::io << core::io::msg(L"kewl", L"| / / -_) V  V / |");
	core::io << core::io::msg(L"kewl", L"|_\\_\\___|\\_/\\_/|_|");
	core::io << core::io::msg(L"kewl", o.title);
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

io::stfu::stfu()
:main_tid(std::this_thread::get_id())
{
	on[0] = false;
	on[1] = false;
}

bool io::stfu::gon()
{
	return on[std::this_thread::get_id() == main_tid ? 0 : 1];
}

void io::stfu::toggle()
{
	if (std::this_thread::get_id() == main_tid)
		on[0] = !on[0];
	else
		on[1] = !on[1];
}

io::o::o()
{
	title = L"Konverzace Everybody Will Like ";
	title += core::io.ver_echo(VERSION);
	core::io.da_erase();
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
	if (msgz[id].gfrom() == L"hr")
		return 1;
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
	if (msgz[id].gfrom() == L"hr") {
		printw("\n ");
		core::io.ui.on(L"attr_hr");
		core::io.ui.echo(L"char_hr", max_x - 2);
		core::io.ui.off(L"attr_hr");
		return;
	}
	int tmp_pos, ltotal = glinez(id, max_x, max_y), lcount = 0, tmp_begin, tmp_inc;
	std::wstring tmp_from(L"<");
	tmp_from += msgz[id].gfrom();
	tmp_from += L"> ";
	core::io.ui.on(msgz[id].gprop0());
	
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

	core::io.ui.off(msgz[id].gprop0());
	core::io.ui.on(msgz[id].gprop1());

	for (tmp_pos = 0; tmp_pos < msgz[id].gbody().size();) {
		if (tmp_pos == 0) {
			tmp_inc = max_x - tmp_begin - 1;
		} else {
			tmp_inc = max_x - 4;
			lcount++;
		}
		bool end_prop1 = (msgz[id].gw0() > tmp_pos && msgz[id].gw0() <= tmp_pos + tmp_inc) ? true : false;
		if (lcount > omit0 && lcount <= ltotal - omit1) {
			if (tmp_pos != 0)
				printw("\n   ");
			if (!end_prop1) {
				addwstr(msgz[id].gbody().substr(tmp_pos, tmp_inc).c_str());
			} else {
				addwstr(msgz[id].gbody().substr(tmp_pos, msgz[id].gw0() - tmp_pos).c_str());
				core::io.ui.off(msgz[id].gprop1());
				addwstr(msgz[id].gbody().substr(msgz[id].gw0(), tmp_inc + tmp_pos - msgz[id].gw0()).c_str());
			}
		}
		if (end_prop1)
			core::io.ui.off(msgz[id].gprop1());
		tmp_pos += tmp_inc;
	}
}

void io::o::scrll(bool bacc)
{
	int max_x, max_y, ldiff, tmp_lcount = 0, i, way = (bacc ? -1 : 1);
	getmaxyx(stdscr, max_y, max_x);
	ldiff = (max_y - 3) / 4;
	if (ldiff <= 0)
		ldiff = 1;

	if (pos0 == -2 && pos1 == 0) {
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
		pos0 = i - 1;
		pos1 = tmp_lcount - ldiff;
	} else {
		if (pos0 == -1)
			return;
		tmp_lcount = 0;
		for (i = pos0; i >=0 && i < glen(); i--) {
			tmp_lcount += glinez(i, max_x, max_y);
			if (i == pos0)
				tmp_lcount -= pos1;
			if (tmp_lcount > ldiff)
				break;
		}
		pos0 = i;
		if (i >= 0)
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
	getmaxyx(stdscr, max_y, max_x);
	if (bacc) {
		int tmp_lcount = 0, i;
		for (i = 0; i < len && tmp_lcount < max_y - 3; i++)
			tmp_lcount += glinez(i, max_x, max_y);
		if (i - 1 <= pos2)
			pos0 = -2;
		else
			pos0 = pos2;
		pos1 = 0;
	} else {
		pos0 = len - 1;
		pos1 = 0;
	}
	core::io.mkwin();
}

void io::o::scrll_resize()
{
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	if (pos0 != -2 || pos1 != 0)
		scrll_chk(max_x, max_y);
	core::io.mkwin();
}

void io::o::scrll_chk(int max_x, int max_y)
{
	int i, tmp_lcount;
	tmp_lcount = -pos1;
	for (i = pos0; i >= 0 && tmp_lcount < max_y - 3; i--)
		tmp_lcount += glinez(i, max_x, max_y);
	
	if (tmp_lcount < max_y - 3 && pos0 <= pos2)
		jump(true);
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
	if (max_x <= 4 || max_y < 1)
		return;
	erase();
	int tmp_lcount, j, begin, end, omit0, omit1;
	if (o.gpos0() == -2 && o.gpos1() == 0) {
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

	core::io.ui.on(L"attr_frame_base");
	mvprintw(0, 0, "%*s", max_x, "");
	mvaddwstr(0, 1, o.gtitle().substr(0, max_x - 1).c_str());
	core::io.ui.off(L"attr_frame_base");
	core::serv.status.draw(max_x, max_y);
	core::io.ui.on(L"attr_new_tag");
	if (!(end == 0 && omit1 <= 0) && o.new_msg)
		mvaddwstr(max_y - 2, max_x - 5, std::wstring(L"_NEW_").substr(0, max_x).c_str());
	else
		o.new_msg = false;
	core::io.ui.off(L"attr_new_tag");
	i.draw(max_x, max_y);
}
