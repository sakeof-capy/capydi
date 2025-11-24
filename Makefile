BUILD_DIR := build
SRC_DIR := .
JOBS := $(shell nproc)

define cmake_build
	@cmake --build $(BUILD_DIR) --target $(1) -j $(JOBS)
endef

all: configure
	$(call cmake_build,all)

configure:
	@cmake -S $(SRC_DIR) -B $(BUILD_DIR)

run: configure
	$(call cmake_build,capydi)
	@$(BUILD_DIR)/0_root/capydi

test: configure
	$(call cmake_build,testing)
	@$(BUILD_DIR)/9_testing/testing

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all configure run test clean
