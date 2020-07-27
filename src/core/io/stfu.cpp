io::stfu::stfu()
{
	on[0] = false;
	on[1] = false;
}

bool io::stfu::gon()
{
	if (core::io.myside == true)
		return on[0];
	else
		return on[1];
}

void io::stfu::toggle()
{
	if (core::io.myside == true)
		on[0] = !on[0];
	else
		on[1] = !on[1];
}
