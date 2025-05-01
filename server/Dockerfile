FROM golang:1.23.0 AS builder

WORKDIR /build_app

COPY go.mod ./
COPY go.sum ./
RUN go mod download && go mod verify

COPY . .

RUN CGO_ENABLED=0 GOOS=linux GOARCH=amd64 go build -a -installsuffix cgo -ldflags '-extldflags "-static"' -o build_app

FROM scratch

COPY --from=builder /build_app/build_app /

EXPOSE 49435

CMD ["/build_app"]
