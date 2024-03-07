FROM golang
WORKDIR /src/app
COPY go.mod main.go go.sum .env ./
RUN go mod download
RUN go build -o /server
CMD ["/server"]
EXPOSE 8000
