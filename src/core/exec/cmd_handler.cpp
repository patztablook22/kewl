void exec::cmd::acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
{
}

uint8_t exec::cmd::serv(std::vector<std::wstring>)
{
	return 0;
}

exec::cmd_handler::cmd_handler(std::vector<std::vector<std::wstring>> da_man, class cmd *da_ptr)
{
	_man = da_man;
	_ptr = da_ptr;
}

void exec::cmd_handler::man(std::vector<std::vector<std::wstring>> &trg)
{
	trg = _man;
}

void exec::cmd_handler::acompl(std::vector<std::wstring> arg, std::vector<std::wstring> &trg)
{
	_ptr->acompl(arg, trg);
}

class exec::cmd *exec::cmd_handler::ptr()
{
	return _ptr;
}

