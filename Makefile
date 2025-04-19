.PHONY: fmt lint test

fmt:
	gofmt -w .
	goimports -w .

lint:
	golangci-lint run

check: fmt lint test
	@echo "All checks passed!"
build:
	@echo "Running build..."
	@go build -o bin/c2-server main.go

test:
	@go test -v ./...

run: build
	@./bin/c2-server
