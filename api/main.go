package main

import (
	"context"
	"encoding/json"
	"fmt"
	"net/http"
	"os"
	"time"

	"github.com/gorilla/mux"
	influxdb2 "github.com/influxdata/influxdb-client-go/v2"
	"github.com/influxdata/influxdb-client-go/v2/api"
	"github.com/joho/godotenv"
	"go.uber.org/zap"
)

type Weather struct {
	Temperature float64 `json:"temperature"`
	Humidity    float64 `json:"humidity"`
	Time        string  `json:"time"`
	Location    string  `json:"location"`
}

type WeatherService struct {
	logger   *zap.SugaredLogger
	queryAPI api.QueryAPI
}

func NewWeatherService(logger *zap.SugaredLogger, queryAPI api.QueryAPI) *WeatherService {
	return &WeatherService{
		logger:   logger,
		queryAPI: queryAPI,
	}
}

func (ws *WeatherService) WeatherHandler(w http.ResponseWriter, r *http.Request) {
	weather, err := ws.getWeather()

	if err != nil {
		ws.logger.Error("Error getting weather: ", zap.Error(err))

		errorResponse := map[string]string{
			"error": "Error getting weather: " + err.Error(),
		}
		w.WriteHeader(http.StatusInternalServerError)
		json.NewEncoder(w).Encode(errorResponse)
		return
	}

	weatherJson, err := json.Marshal(weather)
	if err != nil {
		ws.logger.Error("Error marshalling weather: ", zap.Error(err))

		errorResponse := map[string]string{
			"error": "Error marshalling weather: " + err.Error(),
		}
		w.WriteHeader(http.StatusInternalServerError)
		json.NewEncoder(w).Encode(errorResponse)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusOK)
	w.Write(weatherJson)
}

func (ws *WeatherService) getWeather() ([]Weather, error) {
	query := `from(bucket: "my-bucket")
    |> range(start: -1000h)
    |> filter(fn: (r) => r["_measurement"] == "weather")
    |> filter(fn: (r) => r["_field"] == "humidity" or r["_field"] == "temperature")
    |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value")
    |> limit(n: 10)`

	results, err := ws.queryAPI.Query(context.Background(), query)
	if err != nil {
		return []Weather{}, fmt.Errorf("error querying data: %w", err)
	}
	defer results.Close()

	var weather_data []Weather

	for results.Next() {
		record := results.Record()
		temperature, ok := record.ValueByKey("temperature").(float64)
		if !ok {
			return []Weather{}, fmt.Errorf("error parsing temperature")
		}
		humidity, ok := record.ValueByKey("humidity").(float64)
		if !ok {
			return []Weather{}, fmt.Errorf("error parsing humidity")
		}
		time := record.Time().Format(time.RFC3339)
		location, ok := record.ValueByKey("location").(string)
		if !ok {
			return []Weather{}, fmt.Errorf("error parsing location")
		}

		weather_data = append(weather_data, Weather{
			Temperature: temperature,
			Humidity:    humidity,
			Time:        time,
			Location:    location,
		})
	}

	return weather_data, nil
}

func main() {
	// Create a new logger
	logger, _ := zap.NewDevelopment()
	defer logger.Sync()
	sugar := logger.Sugar()

	err := godotenv.Load()
	if err != nil {
		sugar.Fatal("Error loading .env file")
	}
	token := os.Getenv("INFLUX_TOKEN")
	url := os.Getenv("INFLUX_URL")
	org := os.Getenv("INFLUX_ORG")
	sugar.Info("Starting server")
	sugar.Infof("Token: %s", token)
	sugar.Infof("URL: %s", url)
	sugar.Infof("Org: %s", org)

	client := influxdb2.NewClient(url, token)
	queryAPI := client.QueryAPI(org)

	ws := NewWeatherService(sugar, queryAPI)

	router := mux.NewRouter()
	router.HandleFunc("/weather", ws.WeatherHandler)
	sugar.Fatal(http.ListenAndServe(":8000", router))
}
