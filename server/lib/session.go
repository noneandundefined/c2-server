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
func (this *SessionManager) NewBotSession(mac string, client models.IClient) {
	this.mu.Lock()
	defer this.mu.Unlock()

	if _, exists := this.sessions[mac]; exists {
		this.logger.Warning(fmt.Sprintf("Session is already exists for (%s)", mac))
		this.sessions[mac].Client = client
		this.sessions[mac].LastActivity = time.Now()
		return
	}

	this.sessions[mac] = &types.TCPSession{
		Client:       client,
		LastActivity: time.Now(),
	}

	this.logger.Session(fmt.Sprintf("Create new bot session for (%s)", mac))
}

// NewBotSession добавляет новую сессию для админа
func (this *SessionManager) NewAdminSession(mac string, client models.IClient) {
	this.mu.Lock()
	defer this.mu.Unlock()

	if _, exists := this.sessions[mac]; exists {
		this.logger.Warning(fmt.Sprintf("Session is already exists for (%s)", mac))
		this.sessions[mac].Client = client
		this.sessions[mac].LastActivity = time.Now()
		return
	}

	this.sessions[mac] = &types.TCPSession{
		Client:       client,
		LastActivity: time.Now(),
	}

	this.logger.Session(fmt.Sprintf("Create new admin session for (%s)", mac))
}

// RemoveBotSession удаляет сессию бота
func (this *SessionManager) RemoveBotSession(mac string) {
	this.mu.Lock()
	defer this.mu.Unlock()

	session, exists := this.sessions[mac]
	if !exists {
		this.logger.Warning(fmt.Sprintf("Tried to remove non-existent session for (%s)", mac))
		return
	}

	if session.Client != nil {
		session.Client.OnBotDisconnect()
	}

	delete(this.sessions, mac)
	this.logger.Session(fmt.Sprintf("Delete session for (%s)", mac))
}

// RemoveAdminSession удаляет сессию админа
func (this *SessionManager) RemoveAdminSession(mac string) {
	this.mu.Lock()
	defer this.mu.Unlock()

	session, exists := this.sessions[mac]
	if !exists {
		this.logger.Warning(fmt.Sprintf("Tried to remove non-existent session for (%s)", mac))
		return
	}

	if session.Client != nil {
		session.Client.OnBotDisconnect()
	}

	delete(this.sessions, mac)
	this.logger.Session(fmt.Sprintf("Delete session for (%s)", mac))
}

// TransitData отпарвляет команду на устройство
func (this *SessionManager) TransitData(mac string, commandType uint8, commandData *string) {
	session, exists := this.sessions[mac]
	if !exists {
		this.logger.Error(fmt.Sprintf("Session not found for MAC: (%s)", mac))
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
