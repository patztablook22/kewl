class ascii: public core::cmdl::opt {

  public:

    ascii()
    {
      core::cmdl.add(L"ascii", L"use only base-ascii charz 4 environment", this);
    }

    void usr (std::wstring self)
    {
      core::io.sascii();
      return;
    }

} ascii;
