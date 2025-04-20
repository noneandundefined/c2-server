OUT_DIR = source

<<<<<<< HEAD
EXE_SRC = c2-conn.c
EXE_DLL = c2-dll.c
EXE_INJECT = injector.c
EXE_OUT = $(OUT_DIR)/c2-conn.exe
DLL_OUT = $(OUT_DIR)/c2.dll
INJECT_OUT = $(OUT_DIR)/injector.exe
=======
SRC_MAIN = x86_x64.c
SRC_BOT = c2-bot.c
SRC_IPGEO = c2-ipgeo.c
SRC_PACKET= c2-packet.c

MAIN_OUT = $(OUT_DIR)/x86_x64.exe
>>>>>>> 6c44eb09365f0b0346e49ddb2e96fbcaa39d763c

CC = gcc
CFLAGS = -mwindows
LDFLAGS = -lws2_32
<<<<<<< HEAD

all: $(EXE_OUT)

$(EXE_OUT): $(EXE_SRC)
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) $(EXE_SRC) -o $(EXE_OUT) $(CFLAGS) $(LDFLAGS)

dll: $(DLL_OUT)

$(DLL_OUT): $(EXE_DLL)
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) -shared -o $(DLL_OUT) $(EXE_DLL) -Wall -Wextra $(LDFLAGS)

injector: $(INJECT_OUT)

$(INJECT_OUT): $(EXE_INJECT)
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) $(EXE_INJECT) -o $(INJECT_OUT) -lkernel32 -luser32
=======
HTTPFLAGS = -lwinhttp
LAPIFLAGS = -liphlpapi

exe: $(MAIN_OUT)

$(MAIN_OUT): $(SRC_MAIN)
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) $(SRC_MAIN) -o $(MAIN_OUT) $(SRC_BOT) $(SRC_IPGEO) $(SRC_PACKET) $(CFLAGS) $(LDFLAGS) $(HTTPFLAGS) $(LAPIFLAGS)

ipgeo-test: $(OUT_DIR)/ipgeo.exe

$(OUT_DIR)/ipgeo.exe: ipgeo.c
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) ipgeo.c -o $(OUT_DIR)/ipgeo.exe -lwinhttp -liphlpapi
>>>>>>> 6c44eb09365f0b0346e49ddb2e96fbcaa39d763c

clean:
	rm -f $(EXE_OUT) $(DLL_OUT)
