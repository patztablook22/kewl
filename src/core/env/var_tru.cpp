env::TRU::TRU(std::wstring name, std::wstring ddd)
{
	if (core::env.existz(L"bool_ " + name) || (ddd != L"TRU" && ddd != L"FALZ")) {
		delete this;
		return;
	}
	if (ddd == L"TRU")
		da_default = true;
	else
		da_default = false;
	val = da_default;
	core::env.TRUz[L"bool_" + name] = this;
}

bool env::TRU::gval()
{
	return val;
}

uint8_t env::TRU::sval(std::wstring da_val)
{
	bool tmp;
  std::wcout << da_val << std::endl;
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
	return sacv ? -1 : 0;
}

void env::TRU::reset()
{
	val = da_default;
	core::io.mkwin();
}
