env::STR::STR(std::wstring name, unsigned int min, unsigned int max, std::wstring ddd)
{
	if ( \
		core::env.existz(L"txt_" + name) || \
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
	core::env.STRz[L"txt_" + name] = this;
}

uint8_t env::set(std::wstring name, std::wstring da_val)
{
	if (STRz.find(name) != STRz.end())
		return STRz[name]->sval(da_val);
	if (TRUz.find(name) != TRUz.end())
		return TRUz[name]->sval(da_val);
	return 1;
}

uint8_t env::STR::sval(std::wstring da_val)
{
	if (da_val.size() < range[0] || da_val.size() > range[1] || !core::io.iz_k(da_val))
		return 3;
	bool sacv = false;
	if (da_val == val)
		sacv = true;
	val = da_val;
	return sacv ? -1 : 0;
}

void env::STR::reset()
{
	val = da_default;
	core::io.mkwin();
}

std::wstring env::STR::gval()
{
	return val;
}

std::wstring env::STR::grange()
{
	return std::to_wstring(range[0]) + L" - " + std::to_wstring(range[1]);
}
