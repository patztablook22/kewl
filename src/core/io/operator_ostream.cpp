void io::operator>>(std::wstring &trg)
{
	if (i.use_normal)
		return;
	else
		i.use_normal = true;
	wint_t ch;
	i.hist.prepare();
	i.buf.clear();
	i.pos = 0;
	bool max = (i.buf.size() > 255);

	for (;;) {
		get_wch(&ch);
		switch (ch) {
		case 10:
			// ENTER
			if (!i.use_passwd) {
				if (i.acompl.move(true))
					break;
				trg = i.buf;
				i.hist.buftohist();
				i.buf.clear();
				i.pos = 0;
				i.acompl.reset();
				i.use_normal = false;
				i.draw();
				return;
			} else {
				try {
					core::serv << L'|' + sha256(i.buf);
				} catch (...) {}
				i.use_passwd = false;
				i.buf.clear();
				i.pos = 0;
			}
			break;
		case 410:
		// RESIZE
			o.scrll_resize();
			continue;
		case 258:
			// KEY_DOWN
			if (i.use_passwd)
				break;
			if (i.acompl.move(true))
				ch = 9;
			else
				i.hist.histtobuf(true);
			break;
		case 259:
			// KEY_UP
			if (i.use_passwd)
				break;
			if (i.acompl.move(false))
				ch = 9;
			else
				i.hist.histtobuf(false);
			break;
		case 260:
			// KEY_LEFT
			if (i.pos > 0)
				i.pos--;
			break;
		case 261:
			// KEY_RIGHT
			if (i.pos < i.buf.size())
				i.pos++;
			break;
		case 338:
			// PG_DN
			o.scrll(true);
			break;
		case 339:
			// PG_UP
			o.scrll(false);
			break;
		case 360:
			// END
			o.jump(true);
			break;
		case 262:
			// HOME
			o.jump(false);
			break;
		case KEY_F(1):
			// F1
			if (!i.use_passwd)
				i.help();
			break;
		case 263:
		case 127:
			// BKSP
			if (i.pos > 0) {
				i.buf.erase(--i.pos, 1);
				if (i.buf[i.pos] == 32 && i.buf[i.pos - 1] == 32)
					i.buf.erase(--i.pos, 1);
			}
			break;
		case 330:
			// DEL
			if (i.pos < i.buf.size()) {
				i.buf.erase(i.pos, 1);
				if (i.buf[i.pos] == 32 && i.buf[i.pos - 1] == 32)
					i.buf.erase(i.pos, 1);
			}
			break;
		case 21:
			// CTRL+U
			if (!i.use_passwd)
				i.clpbrd.cut(true);
			break;
		case 23:
			// CTRL+W
			if (!i.use_passwd) {
				i.clpbrd.cut(false);
				if (i.buf[i.pos] == 32 && i.buf[i.pos - 1] == 32)
					i.buf.erase(i.pos, 1);
			}
			break;
		case 25:
			// CTRL+Y
			if (!i.use_passwd)
				i.clpbrd.paste();
			break;
		case 1:
			// CTRL+A
			i.pos = 0;
			i.begin = 0;
			break;
		case 5:
			// CTRL+E
			i.pos = i.buf.size();
			i.begin = 0;
			break;
		case 9:
			// TAB
			if (!i.use_passwd)
				i.acompl.move(true);
			break;
		case 32:
			// SPACE
			if ((i.pos > 0 && i.buf[i.pos - 1] != 32 && i.buf[i.pos] != 32) || i.use_passwd)
				i.buf.insert(i.pos++, 1, 32);
			break;
		default:
			// "NORMAL" char
			if (iz_k(ch))
				i.buf.insert(i.pos++, 1, ch);
			else
				continue;
		}

		if (ch == 9)
			i.acompl.get();
		else
			i.acompl.reset();

		if (i.buf.size() > 255) {
			i.buf.erase(i.pos -= i.buf.size() - 255, i.buf.size() - 255);
			if (!max) {
				core::io << L"ERR: length limit already reached";
				max = true;
			} else {
				i.draw();
			}
		} else {
			i.draw();
		}
	}
}
