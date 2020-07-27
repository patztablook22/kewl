std::wstring io::i::bind::ed(uint32_t key)
{
	if (_bindz.find(key) == _bindz.end())
		return L"";
	return _bindz[key];
}

void io::i::bind::set(uint32_t key, std::wstring cmd)
{
	_bindz[key] = cmd;
}

void io::i::bind::del(uint32_t key)
{
	if (_bindz.find(key) == _bindz.end())
		return;
	_bindz.erase(key);
}

void io::i::bind::z(std::vector<uint32_t> &trg)
{
	trg.clear();
	for (std::pair<uint32_t, std::wstring> key: _bindz)
		trg.push_back(key.first);
}
