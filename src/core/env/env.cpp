uint8_t env::set(std::wstring name, std::vector<std::wstring> da_valz)
{
	if (ATTz.find(name) == ATTz.end())
		return 1;
	return ATTz[name]->svalz(da_valz);
}

uint8_t env::def_col(std::wstring name, std::vector<unsigned int> da_val)
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
		init_color(COLORS - colorz.size(), da_val[0], da_val[1], da_val[2]);
		colorz[name] = COLORS - colorz.size();
	} else {
		if (init_color(colorz[name], da_val[0], da_val[1], da_val[2]) == ERR)
			return -1;
	}
	return 0;	
}

uint8_t env::reset(std::wstring name)
{
	if (ATTz.find(name) != ATTz.end())
		ATTz[name]->reset();
	else if (STRz.find(name) != STRz.end())
		STRz[name]->reset();
	else if (CHRz.find(name) != CHRz.end())
		CHRz[name]->reset();
	else if (NUMz.find(name) != NUMz.end())
		NUMz[name]->reset();
	else if (TRUz.find(name) != TRUz.end())
		TRUz[name]->reset();
	else
		return 1;
	return 0;
}

void env::reset()
{
	for (const std::pair<std::wstring, ATT *> &name: ATTz)
		name.second->reset();
	for (const std::pair<std::wstring, STR *> &name: STRz)
		name.second->reset();
	for (const std::pair<std::wstring, CHR *> &name: CHRz)
		name.second->reset();
	for (const std::pair<std::wstring, NUM *> &name: NUMz)
		name.second->reset();
	for (const std::pair<std::wstring, TRU *> &name: TRUz)
		name.second->reset();
}

void env::gcolorz(std::vector<std::wstring> &trg)
{
	trg.clear();
	for (const std::pair<std::wstring, unsigned int> &color: colorz)
		trg.push_back(color.first);
}

void env::gweightz(std::vector<std::wstring> &trg)
{
	trg.clear();
	for (const std::pair<std::wstring, unsigned int> &weight: weightz)
		trg.push_back(weight.first);
}

void env::gpropertiez(std::vector<std::wstring> &trg)
{
	trg.clear();
	for (const std::pair<std::wstring, ATT *> property: ATTz)
		trg.push_back(property.first);
	for (const std::pair<std::wstring, STR *> property: STRz)
		trg.push_back(property.first);
	for (const std::pair<std::wstring, CHR *> property: CHRz)
		trg.push_back(property.first);
	for (const std::pair<std::wstring, NUM *> property: NUMz)
		trg.push_back(property.first);
	for (const std::pair<std::wstring, TRU *> property: TRUz)
		trg.push_back(property.first);
}

void env::on(std::wstring name)
{
	if (ATTz.find(name) == ATTz.end())
		return;
	active = name;
	ATTz[name]->on();
}

void env::off()
{
	if (active.size() != 0)
		ATTz[active]->off();
	active.clear();
}

std::wstring env::gstr(std::wstring name)
{
	if (STRz.find(name) == STRz.end())
		return L"";
	return STRz[name]->gval();
}

std::wstring env::grange(std::wstring name)
{
	if (STRz.find(name) != STRz.end())
		return STRz[name]->grange();
	else if (NUMz.find(name) != NUMz.end())
		return NUMz[name]->grange();
	return L"";
}

void env::gattrz(std::wstring name, std::vector<std::wstring> &trg)
{
	trg.clear();
	if (ATTz.find(name) == ATTz.end())
		return;
	trg.push_back(L"");
	trg.push_back(L"");
	trg.push_back(L"");
	std::vector<unsigned int> tmp;
	ATTz[name]->gvalz(tmp);
	for (const std::pair<std::wstring, unsigned int> &col: colorz) {
		if (col.second == tmp[0])
			trg[0] = col.first;
		if (col.second == tmp[1])
			trg[1] = col.first;
	}

	for (const std::pair<std::wstring, unsigned int> &weight: weightz) {
		if (weight.second == tmp[2]) {
			trg[2] = weight.first;
			break;
		}
	}
}

int env::gint(std::wstring name)
{
	if (NUMz.find(name) == NUMz.end())
		return 0;
	return NUMz[name]->gval();
}

bool env::gtru(std::wstring name)
{
	if (TRUz.find(name) == TRUz.end())
		return true;
	return TRUz[name]->gval();
}

bool env::existz(std::wstring name)
{
	if (ATTz.find(name) != ATTz.end())
		return true;
	if (STRz.find(name) != STRz.end())
		return true;
	if (CHRz.find(name) != CHRz.end())
		return true;
	if (NUMz.find(name) != NUMz.end())
		return true;
	if (TRUz.find(name) != TRUz.end())
		return true;
	return false;
}
