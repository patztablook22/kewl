uint8_t io::beep()
{
	if (!core::env.gtru(L"bool_beep_beep"))
		return 1;
	timeval tmp;
	gettimeofday(&tmp, NULL);
	int s = tmp.tv_sec - beep_tm.tv_sec;
	bool elapsed;
	int beep_delay = core::env.gint(L"int_beep_ms_delay");
	if (s < beep_delay / 1000)
		elapsed = false;
	else if (s > beep_delay / 1000 + 1)
		elapsed = true;
	else if (1000 * s + (tmp.tv_usec - beep_tm.tv_usec) / 1000 >= beep_delay)
		elapsed = true;
	else
		elapsed = false;

	if (!elapsed)
		return 2;
	beep_tm = tmp;
	std::wcout << L'\a' << std::flush;
	return 0;
}
