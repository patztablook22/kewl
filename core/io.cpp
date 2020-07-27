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
	colorz[L"term_default"] = -1;

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

	new property0(L"header", {L"blacc", L"wheit", L"normal"});
	new property0(L"statusbar_placeholder", {L"blacc", L"wheit", L"normal"});
	new property0(L"statusbar_base", {L"blacc", L"wheit", L"normal"});
	new property0(L"statusbar_hl", {L"blacc", L"wheit", L"bold"});
	new property0(L"new_tag", {L"blacc", L"wheit", L"normal"});
	new property0(L"hr", {L"wheit", L"term_default", L"dim"});

	new property0(L"time", {L"blacc", L"term_default", L"bold"});
	new property0(L"usr_kewl", {L"wheit", L"term_default", L"bold"});
	new property0(L"usr_serv", {L"blacc", L"term_default", L"bold"});
	new property0(L"usr_u", {L"wheit", L"term_default", L"bold"});
	new property0(L"usr_otherz", {L"wheit", L"term_default", L"dim"});
	new property0(L"body_kewl", {L"wheit", L"term_default", L"normal"});
	new property0(L"body_serv", {L"wheit", L"term_default", L"normal"});
	new property0(L"body_u", {L"wheit", L"term_default", L"normal"});
	new property0(L"body_otherz", {L"wheit", L"term_default", L"normal"});
	new property0(L"body_hl_1", {L"wheit", L"term_default", L"bold"});
	new property0(L"body_hl_2", {L"wheit", L"term_default", L"bold"});
	new property0(L"body_hl_3", {L"wheit", L"term_default", L"bold"});
	new property0(L"body_hl_4", {L"wheit", L"term_default", L"bold"});
	new property0(L"body_hl_5", {L"wheit", L"term_default", L"bold"});
	new property0(L"body_hl_6", {L"wheit", L"term_default", L"bold"});
	new property0(L"body_hl_7", {L"wheit", L"term_default", L"bold"});
	new property0(L"body_hl_err", {L"wheit", L"term_default", L"bold"});
	new property0(L"body_hl_warn", {L"wheit", L"term_default", L"bold"});
	new property0(L"body_hl_private", {L"blacc", L"term_default", L"bold"});

	new property0(L"prompt_normal", {L"wheit", L"term_default", L"normal"});
	new property0(L"prompt_passwd", {L"wheit", L"term_default", L"bold"});
	new property0(L"input_normal", {L"wheit", L"term_default", L"normal"});
	new property0(L"input_passwd", {L"wheit", L"term_default", L"normal"});

	new property1(L"prompt_normal", 0, 8, L"PROMPT> ");
	new property1(L"prompt_passwd", 0, 8, L"PASSWD> ");
	new property1(L"time_before", 0, 2, L"");
	new property1(L"time_after", 0, 2, L"");
	new property1(L"usr_before", 0, 4, L" <");
	new property1(L"usr_after", 0, 4, L"> ");

	new property2(L"passwd_input", L'*');
	new property2(L"hr", core::io.ascii ? L'-' : L'┄');

	new property3(L"output_scrollbucc", 128, 4096, 512);
	new property3(L"input_scrollbucc", 0, 256, 64);
	new property3(L"usr_max_padding", 0, 15, 0);

	new property4(L"show_time", L"TRU");

	if (!has_colors())
		core::io << core::io::msg(L"kewl", L"WARN: ur terminal doez not support colorz");
	else if (!can_change_color())
		core::io << core::io::msg(L"kewl", L"WARN: ur terminal cannot redefine colorz");
}

uint8_t io::ui::set(std::wstring name, std::vector<std::wstring> da_valz)
{
	if (propertiez0.find(name) == propertiez0.end())
		return 1;

	return propertiez0[name]->svalz(da_valz);
}

uint8_t io::ui::def_col(std::wstring name, std::vector<unsigned int> da_val)
{
	if (da_val.size() != 3)
		return 2;
	if (name.size() < 3 || name.size() > 15)
		return 3;
	for (int i = 0; i < 3; i++) {
		if (da_val[i] < 0 || da_val[i] > 1000)
			return 4 + i;
	}
	if (!can_change_color())
		return -1;

	if (colorz.find(name) == colorz.end()) {
		if (colorz.size() > COLORS)
			return 1;
		init_color(colorz.size(), da_val[0], da_val[1], da_val[2]);
		colorz[name] = colorz.size();
	} else {
		if (init_color(colorz[name], da_val[0], da_val[1], da_val[2]) == ERR)
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
	else if (propertiez3.find(name) != propertiez3.end())
		propertiez3[name]->reset();
	else if (propertiez4.find(name) != propertiez4.end())
		propertiez4[name]->reset();
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
	for (const std::pair<std::wstring, property3 *> &name: propertiez3)
		name.second->reset();
	for (const std::pair<std::wstring, property4 *> &name: propertiez4)
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
	for (const std::pair<std::wstring, property3 *> property: propertiez3)
		trg.push_back(property.first);
	for (const std::pair<std::wstring, property4 *> property: propertiez4)
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
	svalz(da_defaultz);
}

uint8_t io::ui::property0::svalz(std::vector<std::wstring> da_valz)
{
	if (da_valz.size() != 3)
		return 2;
	if (core::io.ui.colorz.find(da_valz[0]) == core::io.ui.colorz.end())
		return 3;
	if (core::io.ui.colorz.find(da_valz[1]) == core::io.ui.colorz.end())
		return 4;
	if (core::io.ui.weightz.find(da_valz[2]) == core::io.ui.weightz.end())
		return 5;
	unsigned int new_valz[3];
	new_valz[0] = core::io.ui.colorz[da_valz[0]];
	new_valz[1] = core::io.ui.colorz[da_valz[1]];
	new_valz[2] = core::io.ui.weightz[da_valz[2]];
	bool sacv = false;
	if (valz[0] == new_valz[0] && valz[1] == new_valz[1] && valz[2] == new_valz[2]) 
		sacv = true;
	for (int i = 0; i < 3; i++)
		valz[i] = new_valz[i];
	init_pair(id, valz[0], valz[1]);
	core::io.mkwin();
	return sacv ? -1 : 0;
}

void io::ui::property0::reset()
{
	for (int i = 0; i < 3; i++)
		valz[i] = defaultz[i];
	init_pair(id, valz[0], valz[1]);
	core::io.mkwin();
}

void io::ui::property0::on()
{
	attron(COLOR_PAIR(id));
	attron(valz[2]);
}

void io::ui::property0::off()
{
	attroff(COLOR_PAIR(id));
	attroff(valz[2]);
}

void io::ui::on(std::wstring name)
{
	if (propertiez0.find(name) == propertiez0.end())
		return;
	active = name;
	propertiez0[name]->on();
}

void io::ui::off()
{
	if (active.size() != 0)
		propertiez0[active]->off();
	active.clear();
}

std::wstring io::ui::gstr(std::wstring name)
{
	if (propertiez1.find(name) == propertiez1.end())
		return L"";
	return propertiez1[name]->gval();
}

std::wstring io::ui::grange(std::wstring name)
{
	if (propertiez1.find(name) != propertiez1.end())
		return propertiez1[name]->grange();
	else if (propertiez3.find(name) != propertiez3.end())
		return propertiez3[name]->grange();
	return L"";
}

int io::ui::gint(std::wstring name)
{
	if (propertiez3.find(name) == propertiez3.end())
		return 0;
	return propertiez3[name]->gval();
}

bool io::ui::gtru(std::wstring name)
{
	if (propertiez4.find(name) == propertiez4.end())
		return true;
	return propertiez4[name]->gval();
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
	sval(ddd);
	core::io.ui.propertiez1[L"txt_" + name] = this;
}

uint8_t io::ui::set(std::wstring name, std::wstring da_val)
{
	if (propertiez1.find(name) != propertiez1.end())
		return propertiez1[name]->sval(da_val);
	if (propertiez4.find(name) != propertiez4.end())
		return propertiez4[name]->sval(da_val);
	return 1;
}

uint8_t io::ui::property1::sval(std::wstring da_val)
{
	if (da_val.size() < range[0] || da_val.size() > range[1] || !core::io.iz_k(da_val))
		return 3;
	bool sacv = false;
	if (da_val == val)
		sacv = true;
	val = da_val;
	core::io.mkwin();
	return sacv ? -1 : 0;
}

void io::ui::property1::reset()
{
	val = da_default;
	core::io.mkwin();
}

std::wstring io::ui::property1::gval()
{
	return val;
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
	sval(da_default);
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
	addwstr(std::wstring(n, val).c_str());
}

uint8_t io::ui::set(std::wstring name, wint_t da_val)
{
	if (propertiez2.find(name) == propertiez2.end())
		return 1;
	return propertiez2[name]->sval(da_val);
}

uint8_t io::ui::property2::sval(wint_t da_val)
{
	if (!core::io.iz_k(da_val))
		return 3;
	bool sacv = false;
	if (da_val == val)
		sacv = true;
	val = da_val;
	core::io.mkwin();
	return sacv ? -1 : 0;
}

void io::ui::property2::reset()
{
	val = da_default;
}

io::ui::property3::property3(std::wstring name, int min, int max, int ddd)
{
	if ( \
		core::io.ui.existz(L"int_" + name) || \
		ddd < min || \
		ddd > max \
		) {
		delete this;
		return;
	}
	range[0] = min;
	range[1] = max;
	da_default = ddd;
	val = da_default;
	core::io.ui.propertiez3[L"int_" + name] = this;
}

uint8_t io::ui::set(std::wstring name, int da_val)
{
	if (propertiez3.find(name) == propertiez3.end())
		return 1;
	return propertiez3[name]->sval(da_val);
}

uint8_t io::ui::property3::sval(int da_val)
{
	if (da_val < range[0] || da_val > range[1])
		return 3;
	bool sacv = false;
	if (da_val == val)
		sacv = true;
	val = da_val;
	core::io.mkwin();
	return sacv ? -1 : 0;
}

void io::ui::property3::reset()
{
	val = da_default;
	core::io.mkwin();
}

int io::ui::property3::gval()
{
	return val;
}

std::wstring io::ui::property3::grange()
{
	return std::to_wstring(range[0]) + L" - " + std::to_wstring(range[1]);
}

io::ui::property4::property4(std::wstring name, std::wstring ddd)
{
	if (core::io.ui.existz(L"bool_ " + name) || (ddd != L"TRU" && ddd != L"FALZ")) {
		delete this;
		return;
	}
	if (ddd == L"TRU")
		da_default = true;
	else
		da_default = false;
	val = da_default;
	core::io.ui.propertiez4[L"bool_" + name] = this;
}

bool io::ui::property4::gval()
{
	return val;
}

uint8_t io::ui::property4::sval(std::wstring da_val)
{
	bool tmp;
	if (da_val == L"TRU")
		tmp = true;
	else if (da_val == L"FALZ")
		tmp = false;
	else
		return 3;
	bool sacv = false;
	if (tmp == val)
		sacv = true;
	val = tmp;
	core::io.mkwin();
	return sacv ? -1 : 0;
}

void io::ui::property4::reset()
{
	val = da_default;
	core::io.mkwin();
}

bool io::ui::existz(std::wstring name)
{
	if (propertiez0.find(name) != propertiez0.end())
		return true;
	if (propertiez1.find(name) != propertiez1.end())
		return true;
	if (propertiez2.find(name) != propertiez2.end())
		return true;
	if (propertiez3.find(name) != propertiez3.end())
		return true;
	if (propertiez4.find(name) != propertiez4.end())
		return true;
	return false;
}

io::io()
:beep_on(true), beep_delay(667), ascii(false)
{
	std::locale::global(std::locale("en_US.UTF-8"));
}

void io::init(bool init_ui)
{
	if (!isatty(STDIN_FILENO)) {
		std::wcerr << L"ERR: failed to read stdin" << std::endl;
		exit(EXIT_FAILURE);
	}

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

	if (init_ui)
		da_erase();
	mkwin();
}

void io::sascii()
{
	ascii = true;
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
		(chk >= 32 && chk <= 591) /* ASCII etc. */ || \
		(chk >= 1025 && chk <= 1105) /* cука блять etc. xd */ || \
		(chk >= 9472 && chk <= 9679) /* box-drawing charz etc. */ \
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
	time_t tm = time(NULL);
	if (da_from == L"hr") 
	{
		from = L"hr";
		body = L"";
		prop0 = L"";
		len = 0;
		valid = true;
		return;
	}
	if (da_from.size() > 15 || da_from.size() < 3 || da_body.size() == 0 || da_body.size() > 255)
		return;
	if (da_from.find(32) != std::wstring::npos)
		return;
	if (!core::io.iz_k(da_from) || !core::io.iz_k(da_body))
		return;

	from = da_from;
	body.clear();
	len = 0;
	bool otherz = from != L"kewl" && from != L"serv";
	wint_t last = 32;
	std::wstring w0;
	for (int i = 0; i < da_body.size(); i++) {
		switch (da_body[i]) {
		case L'\\':
			i++;
			if (da_body[i] == L'\\' || da_body[i] == L'"') {
				body += L'\\';
				body += da_body[i];
				len++;
			} else if (!otherz) {
				body += '\\';
				body += da_body[i];
			}
			break;
		default:
			if (da_body[i] == 32 && last == 32 && otherz)
				break;
			last = da_body[i];
			if (last == 32 && !w0.size())
				w0 = body;
			body += last;
			len++;
			break;
		}
	}
	if (!w0.size())
		w0 = body;
	if (len == 0 || body.size() > 255)
		return;

	if (otherz)
		prop0 = L"otherz";
	else if (from == L"kewl")
		prop0 = L"kewl";
	else
		prop0 = L"serv";
	
	if (w0 == L"ERR:" && !otherz) {
		body.insert(0, L"\\e");
		body.insert(6, L"\\0");
	} else if (w0 == L"WARN:" && !otherz) {
		body.insert(0, L"\\w");
		body.insert(7, L"\\0");
	} else if (w0[0] == L'@' && w0.size() >= 4 && prop0 != L"kewl") {
		body.insert(0, L"\\p");
		body.insert(w0.size() + 2, L"\\0");
	}

	if (core::serv.status.gactive() && core::serv.status.gnick() == from)
		prop0 = L"u";
	uint8_t tmp = localtime(&tm)->tm_hour;
	recv.clear();
	if (tmp < 10)
		recv += L'0';
	recv += std::to_wstring(tmp);
	recv += L':';
	tmp = localtime(&tm)->tm_min;
	if (tmp < 10)
		recv += L'0';
	recv += std::to_wstring(tmp);

	valid = true;
}

void io::msg::operator=(msg da_msg)
{
	this->from = da_msg.from;
	this->body = da_msg.body;
	this->prop0 = da_msg.prop0;
	this->len = da_msg.len;
	this->recv = da_msg.recv;
	this->valid = da_msg.valid;
}

std::wstring io::msg::grecv()
{
	return recv;
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

uint8_t io::msg::glen()
{
	return len;
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
	i.hist.prepare();
	i.buf.clear();
	i.pos = 0;
	bool max = (i.buf.size() > 255);

	for (;;) {
		get_wch(&ch);
		switch (ch) {
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
	core::io.ui.off();
	core::io.ui.on(use_passwd ? L"attr_input_passwd" : L"attr_input_normal");
	move(max_y - 1, prompt.substr(0, max_x).size());
	if (!use_passwd)
		addwstr(buf.substr(begin, max_x - prompt.substr(0, max_x).size()).c_str());
	else
		core::io.ui.echo(L"char_passwd_input", buf.substr(begin, max_x - prompt.substr(0, max_x).size()).size());
	core::io.ui.off();
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

void io::i::hist::prepare()
{
	if (da_hist.size() == 0)
		da_hist.push_back(L"");
	if ((int)da_hist.size() - 1 - core::io.ui.gint(L"int_input_scrollbucc") > 0)
		da_hist.erase(da_hist.begin(), da_hist.end() - 1 - core::io.ui.gint(L"int_input_scrollbucc"));
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
	
	short int way = bacc ? 1 : -1;
	core::io.i.buf = da_hist[pos += way];
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
	if (o.msgz.size() > 0) {
		if (da_msg.gfrom() == L"hr" && o.msgz[o.glen() - 1].gfrom() == L"hr") {
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

	if (o.msgz.size() + 1 > core::io.ui.gint(L"int_output_scrollbucc"))
		o.msgz.erase(o.msgz.begin(), o.msgz.end() + 1 - core::io.ui.gint(L"int_output_scrollbucc"));
	
	o.msgz.push_back(da_msg);
	if (o.pos0 != -2 || o.pos1 != 0) {
		if (o.msgz.size() > core::io.ui.gint(L"int_output_scrollbucc"))
			o.pos1 = 0;
	}
	o.new_msg = true;
	
	tmp_lcount = -o.pos1;
	for (j = o.pos0; j >= 0 && j < o.msgz.size() && tmp_lcount < max_y - 3; j++)
		tmp_lcount += o.glinez(j, max_x, max_y);
	
	 if ((tmp_omit < 0 || (tmp_omit == 0 && i == o.msgz.size() - 1)) && tmp_lcount - max_y + 3 >= 0) {
		o.pos0 = -2;
		o.pos1 = 0;
	}
	mkwin();
}

void io::cls()
{
	o.pos0 = o.glen();
	o.pos1 = 0;
	o.pos2 = o.glen();
	mkwin();
}

void io::da_erase()
{
	o.pos0 = -2;
	o.pos1 = 0;
	o.pos2 = 0;
	o.msgz.clear();
	if (!ascii) {
		core::io << core::io::msg(L"kewl", L"┏━┓                 ┏━━┓");
		core::io << core::io::msg(L"kewl", L"┃ ┃                 ┗┓ ┃");
		core::io << core::io::msg(L"kewl", L"┃ ┃┏━┓ ┏━━━┓ ┏━┳━┳━┓ ┃ ┃");
		core::io << core::io::msg(L"kewl", L"┃ ┗┛ ┃ ┃ ━ ┃ ┃ ┃ ┃ ┃ ┃ ┃");
		core::io << core::io::msg(L"kewl", L"┃ ┏┓ ┃ ┃ ╺━┫ ┃ ╹ ╹ ┃ ┃ ┗┓");
		core::io << core::io::msg(L"kewl", L"┗━┛┗━┛ ┗━━━┛ ┗━━━━━┛ ┗━━┛");
	} else {
		core::io << core::io::msg(L"kewl", L" _              _ ");
		core::io << core::io::msg(L"kewl", L"| |_______ __ _| |");
		core::io << core::io::msg(L"kewl", L"| / / -_) V  V / |");
		core::io << core::io::msg(L"kewl", L"|_\\\\_\\\\___|\\\\_/\\\\_/|_|");
	}
	core::io << core::io::msg(L"kewl", L"Konverzace Everybody Will Like \\2[" + ver_echo(VERSION) + L"]");
	core::io << core::io::msg(L"hr", L"");
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
	int s = tmp.tv_sec - beep_tm.tv_sec;
	bool elapsed;
	if (s < beep_delay / 1000)
		elapsed = false;
	else if (s > beep_delay / 1000 + 1)
		elapsed = true;
	else if (1000 * s + (tmp.tv_usec - beep_tm.tv_usec) / 1000 >= beep_delay)
		elapsed = true;
	else
		elapsed = false;

	if (!elapsed) {
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
	title = L"kewl [";
	title += core::io.ver_echo(VERSION);
	title += L"] - http://jirkavrba.net/kewl";
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
	uint8_t before = core::io.ui.gstr(L"txt_usr_before").size(), after = core::io.ui.gstr(L"txt_usr_after").size(), padding = core::io.ui.gint(L"int_usr_max_padding");
	int l0 = 0, l1 = 0, rest = msgz[id].gfrom().size();
	if (padding > rest)
		rest = padding;
	uint8_t show_time = 0;
	if (core::io.ui.gtru(L"bool_show_time")) {
		show_time = 5 + core::io.ui.gstr(L"txt_time_before").size() + core::io.ui.gstr(L"txt_time_after").size();
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
		printw("\n ");
		core::io.ui.on(L"attr_hr");
		core::io.ui.echo(L"char_hr", max_x - 2);
		core::io.ui.off();
		return;
	}
	int padding = core::io.ui.gint(L"int_usr_max_padding"), before = core::io.ui.gstr(L"txt_usr_before").size(), after = core::io.ui.gstr(L"txt_usr_after").size();
	int tmp_pos = msgz[id].gfrom().size(), ltotal = glinez(id, max_x, max_y), lcount = 1, tmp_begin = 0, tmp_inc;
	std::wstring tmp;
	tmp = core::io.ui.gstr(L"txt_usr_before");
	if (padding > tmp_pos)
		tmp += std::wstring(padding - tmp_pos, 32);
	tmp += msgz[id].gfrom();
	tmp += core::io.ui.gstr(L"txt_usr_after");
	uint8_t show_time = core::io.ui.gtru(L"bool_show_time");
	if (lcount > omit0 && lcount <= ltotal - omit1)
		printw("\n");
	if (show_time) {
		std::wstring todo = core::io.ui.gstr(L"txt_time_before") + msgz[id].grecv() + core::io.ui.gstr(L"txt_time_after");
		before += todo.size();
		show_time = todo.size();
		if (lcount > omit0 && lcount <= ltotal - omit1) {
			core::io.ui.on(L"attr_time");
			addwstr(todo.c_str());
			core::io.ui.off();
		}
	}

	core::io.ui.on(L"attr_usr_" + msgz[id].gprop0());
	for (tmp_pos = 0;;) {
		tmp_inc = max_x - (tmp_pos ? 1 + before : 1 + show_time);
		if (lcount > omit0 && lcount <= ltotal - omit1) {
			if (tmp_pos != 0) {
				printw("\n");
				if (before > 0)
					addwstr(std::wstring(before, 32).c_str());
			}
			addwstr(tmp.substr(tmp_pos, tmp_inc).c_str());
		}
		if (tmp_pos + tmp_inc >= tmp.size()) {
			tmp_begin = tmp.size() - tmp_pos + (tmp_pos == 0 ? show_time : before);
			break;
		}
		tmp_pos += tmp_inc;
		lcount++;
	}
	
	core::io.ui.off();
	core::io.ui.on(L"attr_body_" + msgz[id].gprop0());
	padding += before + after;
	tmp_pos = tmp_begin;
	std::wstring buf;
	for (int i = 0; i < msgz[id].gbody().size(); i++) {
		if (tmp_pos >= max_x - 1) {
			if (lcount > omit0 && lcount <= ltotal - omit1)
				addwstr(buf.c_str());
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
				addwstr(buf.c_str());
			buf.clear();
			i++;
			switch (msgz[id].gbody()[i]) {
			case L'0':
				core::io.ui.off();
				core::io.ui.on(L"attr_body_" + msgz[id].gprop0());
				break;
			case L'1':
				core::io.ui.off();
				core::io.ui.on(L"attr_body_hl_1");
				break;
			case L'2':
				core::io.ui.off();
				core::io.ui.on(L"attr_body_hl_2");
				break;
			case L'3':
				core::io.ui.off();
				core::io.ui.on(L"attr_body_hl_3");
				break;
			case L'4':
				core::io.ui.off();
				core::io.ui.on(L"attr_body_hl_4");
				break;
			case L'5':
				core::io.ui.off();
				core::io.ui.on(L"attr_body_hl_5");
				break;
			case L'6':
				core::io.ui.off();
				core::io.ui.on(L"attr_body_hl_6");
				break;
			case L'7':
				core::io.ui.off();
				core::io.ui.on(L"attr_body_hl_7");
				break;
			case L'e':
				core::io.ui.off();
				core::io.ui.on(L"attr_body_hl_err");
				break;
			case L'w':
				core::io.ui.off();
				core::io.ui.on(L"attr_body_hl_warn");
				break;
			case L'p':
				core::io.ui.off();
				core::io.ui.on(L"attr_body_hl_private");
				break;
			case L'\\':
			case L'"':
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
		addwstr(buf.c_str());
	core::io.ui.off();
}

void io::o::scrll(bool bacc)
{
	int max_x, max_y, ldiff, tmp_lcount = 0, i, way = (bacc ? -1 : 1);
	getmaxyx(stdscr, max_y, max_x);
	if (max_x <= core::io.ui.gint(L"int_usr_max_padding") + core::io.ui.gstr(L"txt_usr_before").size() + core::io.ui.gstr(L"txt_usr_after").size() + 1 + (core::io.ui.gtru(L"bool_show_time") ? 5 + core::io.ui.gstr(L"txt_time_before").size() + core::io.ui.gstr(L"txt_time_after").size() : 0) || max_y < 1)
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
	getmaxyx(stdscr, max_y, max_x);
	if (max_x <= core::io.ui.gint(L"int_usr_max_padding") + core::io.ui.gstr(L"txt_usr_before").size() + core::io.ui.gstr(L"txt_usr_after").size() + 1 + (core::io.ui.gtru(L"bool_show_time") ? 5 + core::io.ui.gstr(L"txt_time_before").size() + core::io.ui.gstr(L"txt_time_after").size() : 0) || max_y < 1)
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
	getmaxyx(stdscr, max_y, max_x);
	if (pos0 != -2 || pos1 != 0)
		scrll_chk(max_x, max_y);
	core::io.mkwin();
}

void io::o::scrll_chk(int max_x, int max_y)
{
	if (max_x <= core::io.ui.gint(L"int_usr_max_padding") + core::io.ui.gstr(L"txt_usr_before").size() + core::io.ui.gstr(L"txt_usr_after").size() + 1 + (core::io.ui.gtru(L"bool_show_time") ? 5 + core::io.ui.gstr(L"txt_time_before").size() + core::io.ui.gstr(L"txt_time_after").size() : 0) || max_y < 1)
		return;
	int i, tmp_lcount;
	tmp_lcount = -pos1;
	for (i = pos0; i < glen() && tmp_lcount < max_y - 3; i++)
		tmp_lcount += glinez(i, max_x, max_y);
	if (tmp_lcount < max_y - 3 && pos0 >= pos2) {
		jump(true);
	}
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
	if (max_x <= core::io.ui.gint(L"int_usr_max_padding") + core::io.ui.gstr(L"txt_usr_before").size() + core::io.ui.gstr(L"txt_usr_after").size() + 1 + (core::io.ui.gtru(L"bool_show_time") ? 5 + core::io.ui.gstr(L"txt_time_before").size() + core::io.ui.gstr(L"txt_time_after").size() : 0) || max_y < 1)
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

	core::io.ui.on(L"attr_header");
	mvprintw(0, 0, "%*s", max_x, "");
	mvaddwstr(0, 1, o.gtitle().substr(0, max_x - 1).c_str());
	core::io.ui.off();
	core::serv.status.draw(max_x, max_y);
	core::io.ui.on(L"attr_new_tag");
	if (!(end == o.glen() && omit1 <= 0) && o.new_msg)
		mvaddwstr(max_y - 2, max_x - 5, std::wstring(L"*NEW*").substr(0, max_x).c_str());
	else
		o.new_msg = false;
	core::io.ui.off();
	i.draw(max_x, max_y);
}
