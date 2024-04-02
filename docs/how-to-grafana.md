# How to Grafana

In this project we deploy Grafana using a docker-compose file.

This docker-compose exposes a web interface that allows us to fully customize
our dashboards, create alerts, and explore our data.

To access the web interface, go to the address of your Grafana
(project default: `http://localhost:6868`) in your browser.

## Tutorial

This tutorial will guide you through the process of setting up a simple
dashboard in Grafana to visualize our temperature data and humidity data.

1. Open Menu > Connections > Add new Connection.

2. Select InfluxDB as the type of connection > Add new Data source.

3. InfluxDB has 3 query languages, in this example we will use Flux, because it
   is the most powerful and flexible query language.

4. To use Flux we need only 4 parameters
(`URL`, `Token`, `User`, `Database`).
This are the default values for our project, you can change them in the
`./influx-db/influx.yml` file.

- Query Language: Flux
- URL: [http://influxdb:8086](http://influxdb:8086)
- User: my-user
- Database: my-bucket

- Token: `you need to create a token in InfluxDB`

> To create a token in InfluxDB, go to the address of your InfluDB and
> create a new token in the one of the tutorials for client libraries.
