OUT_DIR = source

SRC_MAIN = x86_x64.c
SRC_BOT = core/ICU_bot.c
SRC_IPGEO = stdlib/ICU_ipgeo.c
SRC_PACKET = stdlib/ICU_packet.c
SRC_WIN_REESTR = process/windowsprocess.c
SRC_WIN_APP = ICU_app.c

MAIN_OUT = $(OUT_DIR)/x86_x64.exe

CC = gcc
CFLAGS = -mwindows
LDFLAGS = -lws2_32
HTTPFLAGS = -lwinhttp
LAPIFLAGS = -liphlpapi

LIB_SDL = C:\SDL3\x86_64-w64-mingw32\lib
INCLUDE_SDL = C:\SDL3\x86_64-w64-mingw32\include

exe: $(MAIN_OUT)

$(MAIN_OUT): $(SRC_MAIN)
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) $(SRC_MAIN) -o $(MAIN_OUT) $(SRC_BOT) $(SRC_IPGEO) $(SRC_PACKET) $(SRC_WIN_REESTR) $(CFLAGS) $(LDFLAGS) $(HTTPFLAGS) $(LAPIFLAGS)
	$(CC) -std=c17 $(SRC_WIN_APP) -I$(INCLUDE_SDL) -L$(LIB_SDL) -o $(OUT_DIR)/windows_x86_x64.exe -Wall -lmingw32 -lSDL3 $(CFLAGS)

clean:
	rm -f $(EXE_OUT) $(DLL_OUT)
