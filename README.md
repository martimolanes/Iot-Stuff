# Iot final Project
[mosquitto docker](https://hub.docker.com/_/eclipse-mosquitto/)

# Run the mosquitto broker
```bash
sudo docker run -it -p 1883:1883 -p 9001:9001 -v ./mosquitto.conf:/mosquitto/config/mosquitto.conf -v ./mosquitto/data -v ./mosquitto/log -d eclipse-mosquitto
```

# Troubleshooting Commands
```bash
# send data to the broker
mosquitto_pub -h localhost -t weatherData -m '{"temperature": 22, "humidity": 50}'
# subscribe to the some topic of the broker
mosquitto_sub -h localhost -t weatherData
```
