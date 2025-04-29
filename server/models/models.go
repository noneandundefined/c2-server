package models

type IBot interface {
	IsConnected() bool
	TransitData(commandType uint8, commandData *string)
	OnBotDisconnect()
}
