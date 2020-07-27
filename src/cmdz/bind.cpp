/*
class bind: public core::exec::cmd {
public:
	bind()
	{
		core::exec.add(L"bind", {{L"add key_unicode \"command arg1 arg2 ...\"", L"bind command to unicode keypress", L"(use \"kewl --debug\" to find the unicode value)", L"(u can bind macro by commanding \"_macro_name\")"}, {L"remove UTF-8", L"unbind the key"}, {L"list", L"list all bindings"}}, this);
	}

	uint8_t usr(std::vector<std::wstring> arg)
	{
		if (arg.size() != 1)
			return 1;
		return 0;
	}
} bind;
*/
