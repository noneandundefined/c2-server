OUT_DIR = source

SRC_MAIN = main.c
SRC_ADMIN = core/admin.c
SRC_CONN = core/conn.c
SRC_ENV = stdlib/env.c
SRC_CRC = stdlib/crc.c
SRC_PACKET = stdlib/packet.c
SRC_SYSTEM = process/system.c

CC = gcc
LDFLAGS = -lws2_32
HTTPFLAGS = -lwinhttp
LAPIFLAGS = -liphlpapi

build:
	@if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) $(SRC_MAIN) -o "$(OUT_DIR)/icuadmin-x86_x64.exe" $(SRC_ENV) $(SRC_ADMIN) $(SRC_CONN) $(SRC_CRC) $(SRC_PACKET) $(SRC_SYSTEM) $(LDFLAGS) $(HTTPFLAGS) $(LAPIFLAGS)
