## Dependencies 

go get github.com/influxdata/influxdb-client-go/v2
go get -u github.com/gorilla/mux
go get github.com/joho/godotenv
go get -u go.uber.org/zap

## Run

```bash
docker build -t go-api-weather-station .
docker run --rm --name go-api --network="host" -d go-api-weather-station
```

## References

https://github.com/gorilla/mux?tab=readme-ov-file#install
https://github.com/uber-go/zap

