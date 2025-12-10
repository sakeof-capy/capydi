ifndef MAKEFILE_DOCKER_MK_
MAKEFILE_DOCKER_MK_ := 1

include ./make/constants/Constants.mk

.PHONY: all
all:
	@$(DOCKER_COMPOSE_DEV) run --rm all

.PHONY: configure
configure:
	@$(DOCKER_COMPOSE_DEV) run --rm configure

.PHONY: run
run:
	@$(DOCKER_COMPOSE_DEV) run --rm run

.PHONY: test
test:
	@$(DOCKER_COMPOSE_DEV) run --rm test

.PHONY: docs
docs:
	@doxygen ${DOCS_DIR}/Doxyfile

.PHONY: rebuild-image
rebuild-image:
	@$(DOCKER_COMPOSE_DEV) build --no-cache all

endif # !MAKEFILE_DOCKER_MK_
