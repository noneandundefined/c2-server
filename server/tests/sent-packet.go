package main

import (
	"fmt"
	"net"
	"time"

	jsoniter "github.com/json-iterator/go"
)

var json = jsoniter.ConfigCompatibleWithStandardLibrary

func main() {
	serverAddress := "127.0.0.1:12345"

	conn, err := net.Dial("tcp", serverAddress)
	if err != nil {
		fmt.Printf("Error connection to server: %v\n", err)
		return
	}
	defer conn.Close()

	reqData, err := json.Marshal("This is testing")
	if err != nil {
		fmt.Printf("Ошибка сериализации запроса: %v\n", err)
		return
	}

	_, err = conn.Write(reqData)
	if err != nil {
		fmt.Printf("Ошибка отправки данных на сервер: %v\n", err)
		return
	}

	if errTimeout := conn.SetReadDeadline(time.Now().Add(20 * time.Second)); errTimeout != nil {
		fmt.Printf("Ошибка установки тайм-аута: %v\n", errTimeout)
	}

	buffer := make([]byte, 4096)
	n, err := conn.Read(buffer)
	if err != nil {
		fmt.Printf("Ошибка чтения ответа от сервера: %v\n", err)
		return
	}

	// Выводим ответ от сервера
	fmt.Printf("Ответ от сервера: %s\n", string(buffer[:n]))
}
