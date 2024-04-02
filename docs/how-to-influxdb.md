# How to InfluxDB

In this project we deploy InfluxDB using a docker-compose file.

This docker-compose exposes a web interface that allows us to read through
very good documentation, also create new database access tokens and has a
user-friendly query builder with our data.

To access the web interface, go to the address of your influxDB
(project default: `http://localhost:8086`) in your browser.

The project default credentials are:

- User: `my-user`

- Password: `my-password`

> Note: You can change the credentials in the
`./influx-grafana/influx-grafana.yml` file.
