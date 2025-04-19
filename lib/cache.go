package lib

import (
	"c2-server/types"
	"fmt"
	"sync"
	"time"
)

type Cache struct {
	cache  map[string]*[]types.TCPCache
	logger Logger
	mu     sync.Mutex
}

func NewCache() *Cache {
	return &Cache{
		cache:  make(map[string]*[]types.TCPCache),
		logger: *NewLogger(),
	}
}

func (this *Cache) NewInitial(mac string) {
	this.mu.Lock()
	defer this.mu.Unlock()

	if mac == "" {
		this.logger.Warning("Mac address is not valid 'is empty'")
		return
	}

	if _, exists := this.cache[mac]; exists {
		this.logger.Warning(fmt.Sprintf("Cache is already exists for (%s)", mac))
		this.cache[mac] = &[]types.TCPCache{
			{LastActivity: time.Now()},
		}
		return
	}

	this.cache[mac] = &[]types.TCPCache{
		{LastActivity: time.Now()},
	}

	this.logger.Cache(fmt.Sprintf("Create new cache for (%s)", mac))
}

func (this *Cache) RemoveCache(mac string) {
	this.mu.Lock()
	defer this.mu.Unlock()

	_, exists := this.cache[mac]
	if !exists {
		this.logger.Warning(fmt.Sprintf("Tried to remove non-existent cache for (%s)", mac))
		return
	}

	delete(this.cache, mac)
	this.logger.Cache(fmt.Sprintf("Delete cache for (%s)", mac))
}

func (this *Cache) SetCache(mac, unique string, cache interface{}, duration time.Duration) {
	this.mu.Lock()
	defer this.mu.Unlock()

	if _, exists := this.cache[mac]; !exists {
		this.logger.Warning(fmt.Sprintf("Not found cache for (%s)", mac))
		return
	}

	*this.cache[mac] = append(*this.cache[mac], *&types.TCPCache{
		Unique:       unique,
		Cache:        cache,
		LastActivity: time.Now(),
	})

	this.logger.Cache(fmt.Sprintf("Set new cache for (%s) -> '%s'", mac, cache))
}

func (this *Cache) GetCache(mac string, unique string) interface{} {
	this.mu.Lock()
	defer this.mu.Unlock()

	if _, exists := this.cache[mac]; !exists {
		this.logger.Warning(fmt.Sprintf("Not found cache for (%s)", mac))
		return nil
	}

	for _, cache := range *this.cache[mac] {
		if cache.Unique == unique {
			return cache.Cache
		}
	}

	this.logger.Warning(fmt.Sprintf("Cache with Unique '%s' not found for (%s)", unique, mac))
	return nil
}
