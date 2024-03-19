FROM golang:alpine as builder
WORKDIR /src/app
COPY go.mod main.go go.sum .env ./
RUN go mod download
RUN go build -o server

FROM alpine
WORKDIR /root/app
COPY --from=builder /src/app/.env ./
COPY --from=builder /src/app/server ./server
CMD ["./server"]
EXPOSE 8000
