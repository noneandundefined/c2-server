OUT_DIR = source

EXE_SRC = c2-conn.c
EXE_DLL = c2-dll.c
EXE_INJECT = injector.c
EXE_OUT = $(OUT_DIR)/c2-conn.exe
DLL_OUT = $(OUT_DIR)/c2.dll
INJECT_OUT = $(OUT_DIR)/injector.exe

CC = gcc
CFLAGS = -mwindows
LDFLAGS = -lws2_32

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

clean:
	rm -f $(EXE_OUT) $(DLL_OUT)
