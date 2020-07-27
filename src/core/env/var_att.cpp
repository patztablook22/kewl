env::ATT::ATT(std::wstring name, std::vector<std::wstring> da_defaultz)
{
	if (da_defaultz.size() != 3) {
		delete this;
		return;
	}
	if ( \
		core::env.existz(L"attr_" + name) || \
		core::env.colorz.find(da_defaultz[0]) == core::env.colorz.end() || \
		core::env.colorz.find(da_defaultz[1]) == core::env.colorz.end() || \
		core::env.weightz.find(da_defaultz[2]) == core::env.weightz.end() \
		) {
		delete this;
		return;
	}
	defaultz[0] = core::env.colorz[da_defaultz[0]];
	defaultz[1] = core::env.colorz[da_defaultz[1]];
	defaultz[2] = core::env.weightz[da_defaultz[2]];

	core::env.ATTz[L"attr_" + name] = this;
	id = COLORS - core::env.ATTz.size();
	svalz(da_defaultz);
}

uint8_t env::ATT::svalz(std::vector<std::wstring> da_valz)
{
	if (da_valz.size() != 3)
		return 2;
	if (core::env.colorz.find(da_valz[0]) == core::env.colorz.end())
		return 3;
	if (core::env.colorz.find(da_valz[1]) == core::env.colorz.end())
		return 4;
	if (core::env.weightz.find(da_valz[2]) == core::env.weightz.end())
		return 5;
	unsigned int new_valz[3];
	new_valz[0] = core::env.colorz[da_valz[0]];
	new_valz[1] = core::env.colorz[da_valz[1]];
	new_valz[2] = core::env.weightz[da_valz[2]];
	bool sacv = false;
	if (valz[0] == new_valz[0] && valz[1] == new_valz[1] && valz[2] == new_valz[2]) 
		sacv = true;
	for (int i = 0; i < 3; i++)
		valz[i] = new_valz[i];
	init_pair(id, valz[0], valz[1]);
	return sacv ? -1 : 0;
}

void env::ATT::reset()
{
	for (int i = 0; i < 3; i++)
		valz[i] = defaultz[i];
	init_pair(id, valz[0], valz[1]);
	core::io.mkwin();
}

void env::ATT::gvalz(std::vector<unsigned int> &trg)
{
	trg = {valz[0], valz[1], valz[2]};
}

void env::ATT::on()
{
	attron(COLOR_PAIR(id));
	attron(valz[2]);
}

void env::ATT::off()
{
	attroff(COLOR_PAIR(id));
	attroff(valz[2]);
}
