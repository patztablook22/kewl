class tree: public core::exec::cmd {
private:
	std::wstring branch(int i, int max)
	{
		if (0 == max - 1)
			return L"───";
		if (i == 0)
			return L"──┬";
		else if(i == max - 1)
			return L"  └";
		else
			return L"  ├";
	}
public:
	tree()
	{
		core::exec.add(L"tree", {{L"", L"kewl command tree scheme"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 1;
		std::vector<std::wstring> list;
		core::exec.cmdz(list);
		core::io << core::io::msg(L"hr", L"");
		core::io << L"";
		for (int i = 0; i < list.size(); i++) {
			std::vector<std::vector<std::wstring>> man;
			core::exec.man(list[i], man);
			core::io << branch(i, list.size()) + branch(0, man.size()) + L"─ \\1" + list[i];
			std::wstring tmp(i == list.size() - 1 ? L"  " : L" │");
			if (man.size() < 2)
				goto spc;
			for (int j = 0; j < man.size(); j++) {
				if (man[j][0] == L"")
					continue;
				core::io << L" " + tmp + branch(j + 1, man.size() + 1) + L"─── " + man[j][0];
			}
			spc:
			core::io << L" " + tmp;
		}
		core::exec.macroz.mcrz(list);
		for (int i = 0; i < list.size(); i++) {
			core::io << branch(i, list.size()) + L"─── _\\1" + list[i];
			core::io << (i == list.size() - 1 ? L" " : L"  │");
		}
		core::io << core::io::msg(L"hr", L"");
		return 0;
	}
} tree;
