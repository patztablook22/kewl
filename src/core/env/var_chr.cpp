env::CHR::CHR(std::wstring name, wint_t ddd)
{
	if ( \
		core::env.existz(L"char_" + name) || \
		!core::io.iz_k(ddd) \
		) {
		return;
	}
	da_default = ddd;
	sval(da_default);
	core::env.CHRz[L"char_" + name] = this;
}

wint_t env::gchar(std::wstring name)
{
	if (CHRz.find(name) == CHRz.end())
		return 0;
	return CHRz[name]->gval();
}

wint_t env::CHR::gval()
{
	return val;
}	

uint8_t env::set(std::wstring name, wint_t da_val)
{
	if (CHRz.find(name) == CHRz.end())
		return 1;
	return CHRz[name]->sval(da_val);
}

uint8_t env::CHR::sval(wint_t da_val)
{
	if (!core::io.iz_k(da_val))
		return 3;
	bool sacv = false;
	if (da_val == val)
		sacv = true;
	val = da_val;
	return sacv ? -1 : 0;
}

void env::CHR::reset()
{
	val = da_default;
}
