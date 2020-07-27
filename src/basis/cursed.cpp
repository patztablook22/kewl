void termxy(int &max_x, int &max_y)
{
  getmaxyx(stdscr, max_y, max_x);
}

void print(const char *txt)
{
  addstr(txt);
}

void print(const wchar_t *txt)
{
  addwstr(txt);
}

void print(std::string txt)
{
  addstr(txt.c_str());
}

void print(std::wstring txt)
{
  addwstr(txt.c_str());
}

void rubber(int n)
{
  printw("%*c", n, ' ');
}

void mv(int x, int y)
{
  move(y, x);
}

  template<typename T>
void mvprint(int x, int y, T *txt)
{
  basis::mv(x, y);
  basis::print(txt);
}

void mvrubber(int x, int y, int n)
{
  basis::mv(x, y);
  basis::rubber(n);
}
