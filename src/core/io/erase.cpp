void io::da_erase()
{
	o.pos0 = -2;
	o.pos1 = 0;
	o.pos2 = 0;
	o.msgz.clear();
	if (!ascii) {
		core::io << L"┏━┓                 ┏━━┓ ";
		core::io << L"┃ ┃                 ┗┓ ┃ ";
		core::io << L"┃ ┃┏━┓ ┏━━━┓ ┏━┳━┳━┓ ┃ ┃ ";
		core::io << L"┃ ┗┛ ┃ ┃ ━ ┃ ┃ ┃ ┃ ┃ ┃ ┃ ";
		core::io << L"┃ ┏┓ ┃ ┃ ╺━┫ ┃ ╹ ╹ ┃ ┃ ┗┓";
		core::io << L"┗━┛┗━┛ ┗━━━┛ ┗━━━━━┛ ┗━━┛";
	} else {
		core::io << L"\\2┌─┐                 ┌──┐ ";
		core::io << L"\\2│ │                 └┐ │ ";
		core::io << L"\\2│ │┌─┐ ┌───┐ ┌─┬─┬─┐ │ │ ";
		core::io << L"\\2│ └┘ │ │ ─ │ │ │ │ │ │ │ ";
		core::io << L"\\2│ ┌┐ │ │ ──┤ │ └ └ │ │ └┐";
		core::io << L"\\2└─┘└─┘ └───┘ └─────┘ └──┘";
	}
	core::io << L"Konverzace Everybody Will Like \\2[" + ver_echo(VERSION) + L"]";
	core::io << core::io::msg(L"hr", L"");
}

