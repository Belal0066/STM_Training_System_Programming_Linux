# LQ_shell_optm

This is a basic shell implementation in C designed to execute both internal and external commands. It supports several built-in commands and can handle executing external programs.

## Internal Commands

- **pwdlq**: Print the current working directory.
- **echolq <message>**: Print the specified message to the terminal.
- **cplq <source> <destination>**: Copy a file from the source to the destination.
- **mvlq <source> <destination>**: Move a file from the source to the destination.
- **cd <directory>**: Change the current working directory.
- **type <command>**: Determine if a command is internal, external, or unsupported.
- **envir**: Print all environment variables.
- **exitlq**: Exit the shell.
- **help**: Display a list of supported commands.

## External Commands

- Supports executing any command not recognized as an internal command using its name directly (no need for the full path) if found in the system's PATH.

## Dynamic Allocation and Resource Management

- **Arguments Array**: The shell dynamically allocates memory for the argument array based on the number of arguments parsed from user input.
- **Input Buffer**: The input buffer is dynamically resized, handling memory allocation automatically.
- The shell uses `malloc` and `realloc` to allocate memory for argument arrays and user input dynamically.
- It frees allocated memory when it is no longer needed to prevent memory leaks.

### Waiting for Child Processes (external commands)

- The parent process waits for the child process to complete using `waitpid`. This ensures that the shell does not exit or prompt for new commands until the current command finishes execution.
- Handling child processes prevents zombie processes and ensures that system resources are released once the child process has terminated.

## Usage

```bash
belal@Ubuntu:~/Desktop/st/4$ gcc -o lq *.c
belal@Ubuntu:~/Desktop/st/4$ ./lq

THE SHELL IS ALIVE! Type 'help' for a list of commands.
eb3t ya ghaly >>> help
Supported commands are:
1- pwdlq = print working directory
2- echolq = print a user input string
3- cplq = copy a file to another file
4- mvlq = move a file to another dir
5- cd = change the current working dir
6- type = print the type of the command (internal, external, unsupported)
7- envir = print all the enviroment variable
8- exitlq = quit the shell
9- help = print all supported commands
10- external commands = type names directly

eb3t ya ghaly >>> pwd
/home/belal/Desktop/st/4

eb3t ya ghaly >>> cd /home/belal/Desktop/st
Directory changed to /home/belal/Desktop/st

eb3t ya ghaly >>> pwd
/home/belal/Desktop/st

eb3t ya ghaly >>> type pwd
pwd is an external command (executable)

eb3t ya ghaly >>> type pwdlq
pwdlq is an internal command (builtin)

eb3t ya ghaly >>> envir
SHELL=/bin/bash
SESSION_MANAGER=local/Ubuntu:@/tmp/.ICE-unix/925,unix/Ubuntu:/tmp/.ICE-unix/925
QT_ACCESSIBILITY=1
COLORTERM=truecolor
XDG_CONFIG_DIRS=/etc/xdg/xdg-ubuntu:/etc/xdg
SSH_AGENT_LAUNCHER=gnome-keyring
XDG_MENU_PREFIX=gnome-
GNOME_DESKTOP_SESSION_ID=this-is-deprecated
LANGUAGE=en_CA:en
LC_ADDRESS=en_GB.UTF-8
GNOME_SHELL_SESSION_MODE=ubuntu
LC_NAME=en_GB.UTF-8
SSH_AUTH_SOCK=/run/user/1000/keyring/ssh
XMODIFIERS=@im=ibus
DESKTOP_SESSION=ubuntu
LC_MONETARY=en_GB.UTF-8
GTK_MODULES=gail:atk-bridge
DBUS_STARTER_BUS_TYPE=session
PWD=/home/belal/Desktop/st/4
LOGNAME=belal
XDG_SESSION_DESKTOP=ubuntu
XDG_SESSION_TYPE=wayland
SYSTEMD_EXEC_PID=925
XAUTHORITY=/run/user/1000/.mutter-Xwaylandauth.NXBZR2
HOME=/home/belal
USERNAME=belal
IM_CONFIG_PHASE=1
LC_PAPER=en_GB.UTF-8
LANG=en_CA.UTF-8
LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.webp=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:
XDG_CURRENT_DESKTOP=ubuntu:GNOME
VTE_VERSION=6800
WAYLAND_DISPLAY=wayland-0
GNOME_TERMINAL_SCREEN=/org/gnome/Terminal/screen/3bd7dcd6_71e2_4ac0_8140_2b868aab10da
GNOME_SETUP_DISPLAY=:1
LESSCLOSE=/usr/bin/lesspipe %s %s
XDG_SESSION_CLASS=user
TERM=xterm-256color
LC_IDENTIFICATION=en_GB.UTF-8
LESSOPEN=| /usr/bin/lesspipe %s
USER=belal
GNOME_TERMINAL_SERVICE=:1.156
DISPLAY=:0
SHLVL=1
LC_TELEPHONE=en_GB.UTF-8
QT_IM_MODULE=ibus
LC_MEASUREMENT=en_GB.UTF-8
DBUS_STARTER_ADDRESS=unix:path=/run/user/1000/bus,guid=54104cfbe1d822be1aeeda1466acaa8f
PAPERSIZE=a4
XDG_RUNTIME_DIR=/run/user/1000
LC_TIME=en_GB.UTF-8
XDG_DATA_DIRS=/usr/share/ubuntu:/usr/local/share/:/usr/share/:/var/lib/snapd/desktop
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin
GDMSESSION=ubuntu
DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus,guid=54104cfbe1d822be1aeeda1466acaa8f
LC_NUMERIC=en_GB.UTF-8
_=./lq

eb3t ya ghaly >>> exitlq
goodbye
