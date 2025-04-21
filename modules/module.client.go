package modules

import (
	"fmt"
	"icu/common/constants"
	"icu/common/utils"
	"icu/lib"
	"net"
	"sync"

	jsoniter "github.com/json-iterator/go"
)

var json = jsoniter.ConfigCompatibleWithStandardLibrary

var receiveBufPool = sync.Pool{
	New: func() any {
		return make([]byte, 0, constants.MAX_PACKET_SIZE)
	},
}

type BotClient struct {
	sock           net.Conn
	mac            [6]byte
	version        uint8
	type_is        uint8
	ipgeo          *string
	ipgeo_length   uint16
	crc            uint
	isBotConnected bool
	receiveBuf     []byte
	logger         *lib.Logger
	cache          *lib.Cache
	emitter        *lib.EventEmitter
}

func NewBotClient(sock net.Conn, cache *lib.Cache, emitter *lib.EventEmitter) *BotClient {
	return &BotClient{
		sock:           sock,
		mac:            [6]byte{},
		version:        0x00,
		type_is:        0x00,
		ipgeo:          new(string),
		ipgeo_length:   0x00,
		crc:            0,
		isBotConnected: false,
		logger:         lib.NewLogger(),
		receiveBuf:     []byte{},
		cache:          cache,
		emitter:        emitter,
	}
}

func (this *BotClient) IsConnected() bool {
	return this.isBotConnected
}

func (this *BotClient) parseData(socketBuf []byte) {
	if len(socketBuf) == 0 {
		this.emitter.Emit("error", fmt.Sprintf("Received bytes (%d) is NULL!", len(socketBuf)))
		return
	}

	this.receiveBuf = receiveBufPool.Get().([]byte)[:0]
	this.receiveBuf = append(this.receiveBuf, socketBuf...)

	packetType := this.receiveBuf[7]
	switch packetType {
	case 0x01:
		copy(this.mac[:], this.receiveBuf[0:6])
		this.version = this.receiveBuf[6]
		this.type_is = packetType
		this.ipgeo_length = uint16(this.receiveBuf[8]) | uint16(this.receiveBuf[9])<<8

		ipgeo_prepare := string(this.receiveBuf[10 : 10+this.ipgeo_length])
		this.ipgeo = &ipgeo_prepare

		this.emitter.Emit("hello-pack-received", "")

	default:
		this.emitter.Emit("error", fmt.Sprintf("Failed to get/parse type_is (%d)", packetType))
	}
}

func (this *BotClient) OnBotDisconnect() {
	if err := this.sock.Close(); err != nil {
		this.logger.Error(fmt.Sprintf("Error close connection for bot (%s): %v", utils.MacPrettyConvert(this.mac), err))
		return
	}

	this.mac = [6]byte{}
	this.version = 0x00
	this.type_is = 0x00
	this.ipgeo = nil
	this.ipgeo_length = 0x00
	this.crc = 0
	this.isBotConnected = false
}
