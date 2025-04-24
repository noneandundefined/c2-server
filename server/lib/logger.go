package lib

import (
	"fmt"
	"os"
	"path/filepath"
	"time"
)

type Logger struct {
	logDir string
}

func NewLogger() *Logger {
	now := time.Now()
	day := fmt.Sprintf("%02d", now.Day())
	month := fmt.Sprintf("%02d", now.Month())

	logDir := filepath.Join("logs", fmt.Sprintf("%s.%s", day, month))

	if err := os.MkdirAll(logDir, os.ModePerm); err != nil {
		fmt.Println("Error creating log directory:", err)
	}

	return &Logger{logDir: logDir}
}

func (l *Logger) logFile(fileName string, message string) {
	filePath := filepath.Join(l.logDir, fileName)

	file, err := os.OpenFile(filePath, os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0666)
	if err != nil {
		fmt.Println("Error opening log file:", err)
		return
	}
	defer file.Close()

	if _, err := file.WriteString(message + "\n"); err != nil {
		fmt.Println("Error writing log to file:", err)
	}
}

func (l *Logger) Info(message string) {
	location, err := time.LoadLocation("Asia/Yekaterinburg")
	if err != nil {
		return
	}

	now := time.Now().In(location)
	logMessage := fmt.Sprintf("[\033[94m%02d.%02d.%d %02d:%02d:%02d\033[0m] [\033[32mINFO\033[0m] \033[32m%s\033[0m",
		now.Day(), now.Month(), now.Year(),
		now.Hour(), now.Minute(), now.Second(), message)
	logFile := fmt.Sprintf("[%02d.%02d.%d %02d:%02d:%02d] [INFO] %s",
		now.Day(), now.Month(), now.Year(),
		now.Hour(), now.Minute(), now.Second(), message)

	fmt.Println(logMessage)
	l.logFile("server.log", logFile)
}

func (l *Logger) Warning(message string) {
	location, err := time.LoadLocation("Asia/Yekaterinburg")
	if err != nil {
		return
	}

	now := time.Now().In(location)
	logMessage := fmt.Sprintf("[\033[94m%02d.%02d.%d %02d:%02d:%02d\033[0m] [\033[33mWARN\033[0m] \033[33m%s\033[0m",
		now.Day(), now.Month(), now.Year(),
		now.Hour(), now.Minute(), now.Second(), message)
	logFile := fmt.Sprintf("[%02d.%02d.%d %02d:%02d:%02d] [WARN] %s",
		now.Day(), now.Month(), now.Year(),
		now.Hour(), now.Minute(), now.Second(), message)

	fmt.Println(logMessage)
	l.logFile("warning.log", logFile)
}

func (l *Logger) Cache(message string) {
	location, err := time.LoadLocation("Asia/Yekaterinburg")
	if err != nil {
		return
	}

	now := time.Now().In(location)
	logMessage := fmt.Sprintf("[\033[94m%02d.%02d.%d %02d:%02d:%02d\033[0m] [\033[35mCACH\033[0m] \033[35m%s\033[0m",
		now.Day(), now.Month(), now.Year(),
		now.Hour(), now.Minute(), now.Second(), message)
	logFile := fmt.Sprintf("[%02d.%02d.%d %02d:%02d:%02d] [CACH] %s",
		now.Day(), now.Month(), now.Year(),
		now.Hour(), now.Minute(), now.Second(), message)

	fmt.Println(logMessage)
	l.logFile("cache.log", logFile)
}

func (l *Logger) Session(message string) {
	location, err := time.LoadLocation("Asia/Yekaterinburg")
	if err != nil {
		return
	}

	now := time.Now().In(location)
	logMessage := fmt.Sprintf("[\033[94m%02d.%02d.%d %02d:%02d:%02d\033[0m] [\033[35mSESS\033[0m] \033[35m%s\033[0m",
		now.Day(), now.Month(), now.Year(),
		now.Hour(), now.Minute(), now.Second(), message)
	logFile := fmt.Sprintf("[%02d.%02d.%d %02d:%02d:%02d] [SESS] %s",
		now.Day(), now.Month(), now.Year(),
		now.Hour(), now.Minute(), now.Second(), message)

	fmt.Println(logMessage)
	l.logFile("session.log", logFile)
}

func (l *Logger) Error(message string) {
	location, err := time.LoadLocation("Asia/Yekaterinburg")
	if err != nil {
		return
	}

	now := time.Now().In(location)
	logMessage := fmt.Sprintf("[\033[94m%02d.%02d.%d %02d:%02d:%02d\033[0m] [\033[31mERROR\033[0m] \033[31m%s\033[0m",
		now.Day(), now.Month(), now.Year(),
		now.Hour(), now.Minute(), now.Second(), message)
	logFile := fmt.Sprintf("[%02d.%02d.%d %02d:%02d:%02d] [ERROR] %s",
		now.Day(), now.Month(), now.Year(),
		now.Hour(), now.Minute(), now.Second(), message)

	fmt.Println(logMessage)
	l.logFile("errors.log", logFile)
}
