env::NUM::NUM(std::wstring name, int min, int max, int ddd)
{
	if ( \
		core::env.existz(L"int_" + name) || \
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
	core::env.NUMz[L"int_" + name] = this;
}

uint8_t env::set(std::wstring name, int da_val)
{
	if (NUMz.find(name) == NUMz.end())
		return 1;
	return NUMz[name]->sval(da_val);
}

uint8_t env::NUM::sval(int da_val)
{
	if (da_val < range[0] || da_val > range[1])
		return 3;
	bool sacv = false;
	if (da_val == val)
		sacv = true;
	val = da_val;
	return sacv ? -1 : 0;
}

void env::NUM::reset()
{
	val = da_default;
	core::io.mkwin();
}

int env::NUM::gval()
{
	return val;
}

std::wstring env::NUM::grange()
{
	return std::to_wstring(range[0]) + L" - " + std::to_wstring(range[1]);
}
