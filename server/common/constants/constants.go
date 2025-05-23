package constants

const (
	MAX_PACKET_SIZE uint16 = 4096

	BOT_TYPE_HELLO      uint8  = 0x01
	BOT_TYPE_REPLY      uint8  = 0x02
	BOT_TYPE_KEEP_ALIVE uint16 = 0xAA

	ADMIN_TYPE_HELLO                uint8  = 0x11
	ADMIN_TYPE_PING                 uint8  = 0x12
	ADMIN_TYPE_COMMAND              uint8  = 0x13
	ADMIN_PACKET_CTYPE_COMMAND_DDOS uint8  = 0x21
	ADMIN_PACKET_CTYPE_COMMAND_BASH uint8  = 0x22
	ADMIN_TYPE_REPLY                uint8  = 0x14
	ADMIN_TYPE_KEEP_ALIVE           uint16 = 0xAAA

	PACKET_TYPE_COMMAND_BASH uint8 = 0x33
	PACKET_TYPE_COMMAND_DDOS uint8 = 0x44
)
