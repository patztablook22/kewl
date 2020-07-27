env::env()
{
	colorz[L"blacc"] = COLOR_BLACK;
	colorz[L"red"] = COLOR_RED;
	colorz[L"green"] = COLOR_GREEN;
	colorz[L"yolow"] = COLOR_YELLOW;
	colorz[L"blu"] = COLOR_BLUE;
	colorz[L"magenta"] = COLOR_MAGENTA;
	colorz[L"cyan"] = COLOR_CYAN;
	colorz[L"wheit"] = COLOR_WHITE;
	colorz[L"term_default"] = -1;

	weightz[L"dim"] = A_DIM;
	weightz[L"normal"] = A_NORMAL;
	weightz[L"bold"] = A_BOLD;
	weightz[L"italic"] = A_ITALIC;
	init();
}

void env::init()
{
	ATTz.clear();
	STRz.clear();
	CHRz.clear();

	new ATT(L"header", {L"blacc", L"wheit", L"normal"});
	new ATT(L"statusbar_placeholder", {L"blacc", L"wheit", L"normal"});
	new ATT(L"statusbar_base", {L"blacc", L"wheit", L"normal"});
	new ATT(L"statusbar_hl", {L"blacc", L"wheit", L"bold"});
	new ATT(L"new_tag", {L"blacc", L"wheit", L"normal"});
	new ATT(L"hr", {L"wheit", L"term_default", L"dim"});
	new ATT(L"popup_hl", {L"blacc", L"wheit", L"bold"});

	new ATT(L"time", {L"blacc", L"term_default", L"bold"});
	new ATT(L"usr_kewl", {L"wheit", L"term_default", L"bold"});
	new ATT(L"usr_serv", {L"blacc", L"term_default", L"bold"});
	new ATT(L"usr_u", {L"wheit", L"term_default", L"bold"});
	new ATT(L"usr_otherz", {L"wheit", L"term_default", L"dim"});
	new ATT(L"body_kewl", {L"wheit", L"term_default", L"normal"});
	new ATT(L"body_serv", {L"wheit", L"term_default", L"normal"});
	new ATT(L"body_u", {L"wheit", L"term_default", L"normal"});
	new ATT(L"body_otherz", {L"wheit", L"term_default", L"normal"});
	new ATT(L"body_hl_1", {L"wheit", L"term_default", L"bold"});
	new ATT(L"body_hl_2", {L"wheit", L"term_default", L"bold"});
	new ATT(L"body_hl_3", {L"wheit", L"term_default", L"bold"});
	new ATT(L"body_hl_4", {L"wheit", L"term_default", L"bold"});
	new ATT(L"body_hl_5", {L"wheit", L"term_default", L"italic"});
	new ATT(L"body_hl_6", {L"wheit", L"term_default", L"bold"});
	new ATT(L"body_hl_7", {L"wheit", L"term_default", L"bold"});
	new ATT(L"body_hl_err", {L"wheit", L"term_default", L"bold"});
	new ATT(L"body_hl_warn", {L"wheit", L"term_default", L"bold"});
	new ATT(L"body_hl_private", {L"blacc", L"term_default", L"bold"});

	new ATT(L"prompt_normal", {L"wheit", L"term_default", L"normal"});
	new ATT(L"prompt_passwd", {L"wheit", L"term_default", L"bold"});
	new ATT(L"input_normal", {L"wheit", L"term_default", L"normal"});
	new ATT(L"input_passwd", {L"wheit", L"term_default", L"normal"});

	new STR(L"prompt_normal", 0, 8, L"PROMPT> ");
	new STR(L"prompt_passwd", 0, 8, L"PASSWD> ");
	new STR(L"time_before", 0, 2, L"");
	new STR(L"time_after", 0, 2, L"");
	new STR(L"usr_before", 0, 4, L" <");
	new STR(L"usr_after", 0, 4, L"> ");

	new CHR(L"passwd_input", L'*');

	new NUM(L"output_scrollbucc", 128, 4096, 512);
	new NUM(L"input_scrollbucc", 0, 256, 64);
	new NUM(L"usr_max_padding", 0, 15, 0);
	new NUM(L"beep_ms_delay", 0, 65536, 100);

	new TRU(L"unicode", L"TRU");
	new TRU(L"show_time", L"TRU");
	new TRU(L"do_popup", L"TRU");
	new TRU(L"beep_beep", L"TRU");

	new CHR(L"hr", core::env.gtru(L"bool_unicode") ?  L'┄' : L'-');

	if (!has_colors())
		core::io << L"WARN: ur terminal doez not support colorz";
	else if (!can_change_color())
		core::io << L"WARN: ur terminal cannot redefine colorz";
}
