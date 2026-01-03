ifndef DOCKER_CONSTANTS_MK_
DOCKER_CONSTANTS_MK_ := 1

include ./make/constants/Flags.mk
include ./make/constants/Constants.mk

DOCKER_COMPOSE := docker compose
DOCKER_COMPOSE_ENV := MAKE_COMMON_ARGS="$(MAKE_COMMON_ARGS)"
DOCKER_COMPOSE_DEV := 									\
	$(DOCKER_COMPOSE_ENV) $(DOCKER_COMPOSE) 			\
		-f ${ROOT_DIR}/docker/docker-compose-dev.yml

endif
