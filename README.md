# kewl

```
   ┏━┓                 ┏━━┓                                          
   ┃ ┃                 ┗┓ ┃             W   E   L   C   O   M   E                     
   ┃ ┃┏━┓ ┏━━━┓ ┏━┳━┳━┓ ┃ ┃           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                          
   ┃ ┗┛ ┃ ┃ ━ ┃ ┃ ┃ ┃ ┃ ┃ ┃                                      
   ┃ ┏┓ ┃ ┃ ╺━┫ ┃ ╹ ╹ ┃ ┃ ┗┓           A super kewl chatroom-based                              
   ┗━┛┗━┛ ┗━━━┛ ┗━━━━━┛ ┗━━┛          Linux C++ Internet Relay Chat                            
 Konverzace Everybody Will Like                                      
```

# kewld

Kewl server [here](https://github.com/patztablook22/kewld).

# Installation

Kewl is available on the [AUR](https://aur.archlinux.org/packages/kewl). \
You can use e.g. [AURA](https://github.com/patztablook22/aura) to install it!
```md
aura kewl

kewl help
```

# Configuration
```ruby
# Kewl provides macros to be configured with...
# content of $HOME/.config/kewl_macroz

rc:
   # this macro is being run every time
   _my_theme
   conn myserver:1234 mynick
   
my_theme:
   # this is my wholesome theme
   # NOTE: **stfu after a command inhibits its output
   #       alternatively, use "erase" command when
   #       all of your init macros have been called
   env define ultimate_color #123456                         **stfu
   env set bool_show_time FALZ                               **stfu
   env set attr_body_hl_3 ultimate_color term_default bold   **stfu
```

# Themed examples
**v1.0.0:** _the modern kewl look_ \
![default](https://raw.githubusercontent.com/patztablook22/meta/master/kewl/100_1.png) \
![dark](https://raw.githubusercontent.com/patztablook22/meta/master/kewl/100_2.png)

**v0.8.8:** _the early kewl look_ \
![default](https://raw.githubusercontent.com/patztablook22/meta/master/kewl/88.png)

