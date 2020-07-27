size_t exec::interpreter(std::wstring input, std::vector<std::wstring> &trg, size_t end)
{
	bool in = false, par = false;
	int iter = 0;
	std::wstring tmp;
	while (iter < input.size()) {
		wint_t ch = input[iter++];
		switch (ch) {
		case L'\\':
			if (iter == input.size())
				break;
			switch (input[iter++]) {
			case L'\\':
				tmp += L'\\';
				break;
			case L'"':
				tmp += L'"';
				break;
			case L'\'':
				tmp += L'\'';
				break;
			case L'/':
				tmp += L'/';
				break;
			default:
				break;
			}
			break;
		case L'"':
			switch (in) {
			case true:
				if (iter < input.size() && input[iter++] != 32)
					return (end == -1 ? iter - 1 : -1);
				trg.push_back(tmp);
				if (iter > end)
					return iter;
				tmp.clear();
				in = false;
				break;
			case false:
				if (iter > 1 && input[iter - 2] != 32)
					return (end == -1 ? iter - 1 : -1);
				in = true;
			}	
			break;
		case L' ':
			switch (in) {
			case true:
				if (tmp.size() == 0 || tmp[tmp.size() - 1] != L' ')
					tmp += L' ';
				break;
			case false:
				if (tmp.size() != 0) {
					trg.push_back(tmp);
					if (iter > end)
						return iter;
					tmp.clear();
				}
				break;
			}
			break;
		default:
			tmp += ch;
		}
	}
	if (tmp.size() != 0 || in)
		trg.push_back(tmp);
	if (in)
		return iter + 1;
	return iter;
}

size_t exec::escape(std::vector<std::wstring> arg, std::wstring &trg, size_t end)
{
	trg.clear();
	for (int i = 0; i <= (end < arg.size() - 1 ? end : arg.size() - 1); i++) {
		std::wstring tmp;
		bool parz = false;
		for (int j = 0; j < arg[i].size(); j++) {
			wint_t ch = arg[i][j];
			switch (ch) {
			case L'\\':
				tmp += L"\\\\";
				break;
			case L'"':
				tmp += L"\\\"";
				break;
			case L'/':
				tmp += L"\\/";
				break;
			case 32:
				tmp += L' ';
				parz = true;
				break;
			default:
				tmp += ch;
				break;
			}
		}
		
		if (i != 0)
			trg += L' ';
		if (parz || tmp.size() == 0)
			trg += L'"' + tmp + L'"';
		else
			trg += tmp;
		tmp.clear();
	}
	return trg.size();
}
