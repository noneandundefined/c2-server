package models

import "net"

type IBot interface {
	IsConnected() bool
	TransitData(conn net.Conn, commandType uint8, commandData *string)
	OnSockDisconnect()
}
