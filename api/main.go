package main

import (
    "context"
    "fmt"
    "log"
    "net/http"
    "encoding/json"
    "os"
    "time"

    "github.com/gorilla/mux"
    influxdb2 "github.com/influxdata/influxdb-client-go/v2"
    "github.com/influxdata/influxdb-client-go/v2/api"
    "github.com/joho/godotenv"
)

type Weather struct {
    Temperature float64 `json:"temperature"`
    Humidity    float64 `json:"humidity"`
    Time        string  `json:"time"`
    Location    string  `json:"location"`
}

type Temperature struct {
    Value float64 `json:"value"`
    Time  string  `json:"time"`
    Location string `json:"location"`
}

type Humidity struct {
    Value float64 `json:"value"`
    Time  string  `json:"time"`
    Location string `json:"location"`
}

var queryAPI api.QueryAPI

func main() {
    err := godotenv.Load()
    if err != nil {
        log.Fatal("Error loading .env file")
    }
    token := os.Getenv("INFLUX_TOKEN")
    url := os.Getenv("INFLUX_URL")
    org := os.Getenv("INFLUX_ORG")
    fmt.Println("Token: " + token)
    fmt.Println("URL: " + url)
    fmt.Println("Org: " + org)

    client := influxdb2.NewClient(url, token)
    queryAPI = client.QueryAPI(org)

    router := mux.NewRouter()
    router.HandleFunc("/weather", WeatherHandler)
    log.Fatal(http.ListenAndServe(":8000", router))
}

func WeatherHandler(w http.ResponseWriter, r *http.Request) {
    weather, err := getWeather(queryAPI)
    if err != nil {
        w.WriteHeader(http.StatusInternalServerError)
        w.Write([]byte(`{"error": "Error getting weather: ` + err.Error() + `"}`))
        return
    }

    weatherJson, err := json.Marshal(weather)
    if err != nil {
        w.WriteHeader(http.StatusInternalServerError)
        w.Write([]byte(`{"error": "Error marshalling weather: ` + err.Error() + `"}`))
        return
    }

    w.Header().Set("Content-Type", "application/json")
    w.WriteHeader(http.StatusOK)
    w.Write(weatherJson)
}

func getWeather(queryAPI api.QueryAPI) ([]Weather, error) {
    query := `from(bucket: "my-bucket")
    |> range(start: -1000h)
    |> filter(fn: (r) => r["_measurement"] == "weather")
    |> filter(fn: (r) => r["_field"] == "humidity" or r["_field"] == "temperature")
    |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value")
    |> limit(n: 10)`

    results, err := queryAPI.Query(context.Background(), query)
    if err != nil {
        return []Weather{}, fmt.Errorf("error querying database: %w", err)
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
            Humidity: humidity,
            Time: time,
            Location: location,
        })
    }

    return weather_data, nil
}
