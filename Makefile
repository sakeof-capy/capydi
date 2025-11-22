BUILD_DIR := build
SRC_DIR := .

all: configure build

configure:
	@cmake -S $(SRC_DIR) -B $(BUILD_DIR)

build:
	@cmake --build $(BUILD_DIR) -j $(shell nproc)

run: all
	@$(BUILD_DIR)/0_root/capydi

test: all
	@$(BUILD_DIR)/9_testing/testing

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all configure build run clean
