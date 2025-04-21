package main

import (
	"fmt"
	"icu/lib"
	modules "icu/modules"

	"github.com/joho/godotenv"
)

func main() {
	cache := lib.NewCache()
	logger := lib.NewLogger()

	server := modules.NewTCPServer(cache)

	if err := godotenv.Load(); err != nil {
		logger.Error(err.Error())
	}

	if err := server.StartServer(); err != nil {
		logger.Error(fmt.Sprintf("Error starting server: %v", err))
	}
}
