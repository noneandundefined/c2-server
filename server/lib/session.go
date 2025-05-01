package lib

import (
	"fmt"
	"sync"
	"time"

	"icu/common/types"
	"icu/models"
)

type SessionManager struct {
	sessions map[string]*types.TCPSession
	logger   Logger
	mu       sync.Mutex
}

func NewSessionManager() *SessionManager {
	return &SessionManager{
		sessions: make(map[string]*types.TCPSession),
		logger:   *NewLogger(),
	}
}

// NewBotSession добавляет новую сессию для бота
func (this *SessionManager) NewBotSession(address string, client models.IClient) {
	this.mu.Lock()
	defer this.mu.Unlock()

	if _, exists := this.sessions[address]; exists {
		this.logger.Warning(fmt.Sprintf("Session bot is already exists for (%s)", address))
		this.sessions[address].Client = client
		this.sessions[address].LastActivity = time.Now()
		return
	}

	this.sessions[address] = &types.TCPSession{
		Client:       client,
		LastActivity: time.Now(),
	}

	this.logger.Session(fmt.Sprintf("Create new bot session for (%s)", address))
}

// NewAdminSession добавляет новую сессию для админа
func (this *SessionManager) NewAdminSession(address string, client models.IClient) {
	this.mu.Lock()
	defer this.mu.Unlock()

	if _, exists := this.sessions[address]; exists {
		this.logger.Warning(fmt.Sprintf("Session admin is already exists for (%s)", address))
		this.sessions[address].Client = client
		this.sessions[address].LastActivity = time.Now()
		return
	}

	this.sessions[address] = &types.TCPSession{
		Client:       client,
		LastActivity: time.Now(),
	}

	this.logger.Session(fmt.Sprintf("Create new admin session for (%s)", address))
}

// RemoveBotSession удаляет сессию бота
func (this *SessionManager) RemoveBotSession(address string) {
	this.mu.Lock()
	defer this.mu.Unlock()

	session, exists := this.sessions[address]
	if !exists {
		this.logger.Warning(fmt.Sprintf("Tried to remove non-existent session for (%s)", address))
		return
	}

	if session.Client != nil {
		session.Client.OnBotDisconnect()
	}

	delete(this.sessions, address)
	this.logger.Session(fmt.Sprintf("Delete bot session for (%s)", address))
}

// RemoveAdminSession удаляет сессию админа
func (this *SessionManager) RemoveAdminSession(address string) {
	this.mu.Lock()
	defer this.mu.Unlock()

	session, exists := this.sessions[address]
	if !exists {
		this.logger.Warning(fmt.Sprintf("Tried to remove non-existent session for (%s)", address))
		return
	}

	if session.Client != nil {
		session.Client.OnBotDisconnect()
	}

	delete(this.sessions, address)
	this.logger.Session(fmt.Sprintf("Delete admin session for (%s)", address))
}

// TransitData отпарвляет команду на устройство
func (this *SessionManager) TransitData(address string, commandType uint8, commandData *string) {
	session, exists := this.sessions[address]
	if !exists {
		this.logger.Error(fmt.Sprintf("Session not found for ADDRESS: (%s)", address))
		return
	}

	session.Client.TransitData(commandType, commandData)
}

// MonitorSessions мониторит все сессии и закрывает если нет активности > 6 минут
// func (this *SessionManager) PingPong() {
// 	ticker := time.NewTicker(1 * time.Minute)
// 	defer ticker.Stop()

// 	for range ticker.C {
// 		for imei, session := range this.sessions {
// 			if time.Since(session.LastActivity) > 6*time.Minute {
// 				this.logger.Warning(fmt.Sprintf("Session for (%s) is inactive for more than 6 minute", imei))

// 				if session.Device != nil {
// 					this.RemoveBotSession(imei)
// 				}
// 			}
// 		}
// 	}
// }
