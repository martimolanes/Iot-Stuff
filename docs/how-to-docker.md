# How to Docker

This are some useful commands to manage docker containers:

```bash
# list all running containers
docker ps
# list all containers
docker ps -a
# stop a container
docker stop <container_id>
# remove a container
docker rm <container_id>
# stop all containers in the system
docker stop $(docker ps -aq)
# remove all containers in the system (use with caution)
docker rm $(docker ps -aq)
# see logs of a container
docker logs <container_id>
```

> We are using docker-compose to manage the containers and we are using the
`start-service.sh` to run the docker-compose files and stop them.
Try `start-service.sh -h`.

This two are useful for deleting the configurations of the influxDB:

```bash
# see the volumes
docker volume ls
# remove a volume. In this case, influx-db_influx-config influx-db_influx-data.
docker volume rm <volume_name>
```
