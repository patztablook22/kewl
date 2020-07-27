cmdl::opt_handler::opt_handler(std::wstring da_desc, opt *da_ptr)
{
	desc = da_desc;
	ptr = da_ptr;
}

cmdl::opt *cmdl::opt_handler::gptr()
{
	return ptr;
}

std::wstring cmdl::opt_handler::gdesc()
{
	return desc;
}

