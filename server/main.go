package main

import (
	"fmt"
	"icu/lib"
	modules "icu/modules"

	"github.com/joho/godotenv"
)

func main() {
	logger := lib.NewLogger()

	if err := godotenv.Load(); err != nil {
		logger.Error(err.Error())
	}

	cache := lib.NewCache()

	server := modules.NewTCPServer(cache)

	if err := server.StartServer(); err != nil {
		logger.Error(fmt.Sprintf("Error starting server: %v", err))
	}
}
