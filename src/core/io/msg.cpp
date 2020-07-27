io::msg::msg()
:valid(false)
{
}

io::msg::msg(std::wstring da_from, std::wstring da_body)
:valid(false)
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
			if (da_body[i] == L'\\' || da_body[i] == L'"' || da_body[i] == L'/') {
				body += L'\\';
				last = da_body[i];
				body += last;
				len++;
			} else if (!otherz) {
				body += '\\';
				last = da_body[i];
				body += last;
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
