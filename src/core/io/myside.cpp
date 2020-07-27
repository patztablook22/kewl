io::myside::myside()
:meesa(std::this_thread::get_id())
{
}

bool io::myside::operator==(const bool arg)
{
	bool res = std::this_thread::get_id() == meesa;
	if (arg == res)
		return true;
	return false;
}
