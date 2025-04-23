package lib

import "sync"

type EventEmitter struct {
	listeners map[string][]func(interface{})
	mu        sync.Mutex
}

func NewEventEmitter() *EventEmitter {
	return &EventEmitter{
		listeners: make(map[string][]func(interface{})),
	}
}

func (this *EventEmitter) On(event string, handler func(interface{})) {
	this.mu.Lock()
	defer this.mu.Unlock()

	this.listeners[event] = append(this.listeners[event], handler)
}

func (this *EventEmitter) Emit(event string, data interface{}) {
	this.mu.Lock()
	defer this.mu.Unlock()

	if handlers, found := this.listeners[event]; found {
		for _, handler := range handlers {
			go handler(data)
		}
	}
}
