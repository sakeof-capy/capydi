ifndef MAKEFILE_NATIVE_MK_
MAKEFILE_NATIVE_MK_ := 1

include ./make/constants/Constants.mk

define cmake_build
	@cmake --build $(BUILD_DIR) --target $(1) -j $(JOBS)
endef

.PHONY: all
all: configure
	$(call cmake_build,all)

.PHONY: configure
configure:
	@cmake -S $(ROOT_DIR) -B $(BUILD_DIR)

.PHONY: run
run: configure
	$(call cmake_build,capydi)
	@$(BUILD_DIR)/0_root/capydi

.PHONY: test
test: configure
	$(call cmake_build,testing)
	@$(BUILD_DIR)/9_testing/testing

.PHONY: docs
docs:
	@doxygen ${DOCS_DIR}/Doxyfile

endif # !MAKEFILE_NATIVE_MK_
