package lib

import (
	"fmt"
	"icu/common/types"
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

func (this *Cache) NewInitial(address string) {
	this.mu.Lock()
	defer this.mu.Unlock()

	if address == "" {
		this.logger.Warning("Address is not valid 'is empty'")
		return
	}

	if _, exists := this.cache[address]; exists {
		this.logger.Warning(fmt.Sprintf("Cache is already exists for (%s)", address))
		this.cache[address] = &[]types.TCPCache{
			{LastActivity: time.Now()},
		}
		return
	}

	this.cache[address] = &[]types.TCPCache{
		{LastActivity: time.Now()},
	}

	this.logger.Cache(fmt.Sprintf("Create new cache for (%s)", address))
}

func (this *Cache) RemoveCache(address string) {
	this.mu.Lock()
	defer this.mu.Unlock()

	_, exists := this.cache[address]
	if !exists {
		this.logger.Warning(fmt.Sprintf("Tried to remove non-existent cache for (%s)", address))
		return
	}

	delete(this.cache, address)
	this.logger.Cache(fmt.Sprintf("Delete cache for (%s)", address))
}

func (this *Cache) SetCache(address, key string, cache interface{}, duration time.Duration) {
	this.mu.Lock()
	defer this.mu.Unlock()

	if _, exists := this.cache[address]; !exists {
		this.logger.Warning(fmt.Sprintf("Not found cache for (%s)", address))
		return
	}

	*this.cache[address] = append(*this.cache[address], *&types.TCPCache{
		Key:          key,
		Cache:        cache,
		LastActivity: time.Now(),
	})

	this.logger.Cache(fmt.Sprintf("Set new cache for (%s)", address))
}

func (this *Cache) GetCache(address string, key string) interface{} {
	this.mu.Lock()
	defer this.mu.Unlock()

	if _, exists := this.cache[address]; !exists {
		this.logger.Warning(fmt.Sprintf("Not found cache for (%s)", address))
		return nil
	}

	for _, cache := range *this.cache[address] {
		if cache.Key == key {
			return cache.Cache
		}
	}

	this.logger.Warning(fmt.Sprintf("Cache with Key '%s' not found for (%s)", key, address))
	return nil
}
