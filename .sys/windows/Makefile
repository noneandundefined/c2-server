OUT_DIR = source

SRC_MAIN= x86_x64.c
MAIN_OUT = $(OUT_DIR)/x86_x64.exe

CC = gcc
CFLAGS = -mwindows
LDFLAGS = -lws2_32

exe: $(MAIN_OUT)

$(MAIN_OUT): $(SRC_MAIN)
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) $(SRC_MAIN) -o $(MAIN_OUT) $(SRC_BOT) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(EXE_OUT) $(DLL_OUT)
