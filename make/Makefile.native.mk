ifndef MAKEFILE_NATIVE_MK_
MAKEFILE_NATIVE_MK_ := 1

include ./make/constants/Constants.mk

define cmake_build
	@cmake --build $(BUILD_DIR) --target $(1) -j $(JOBS) -- $(2)
endef

.PHONY: all
all: configure
	$(call cmake_build,all)

.PHONY: configure
configure:
	$(info "CMAKE_CONFIGURE_FLAGS: ${CMAKE_CONFIGURE_FLAGS}")
	@cmake $(CMAKE_CONFIGURE_FLAGS) -S $(ROOT_DIR) -B $(BUILD_DIR)

.PHONY: run
run: configure
	$(call cmake_build,capydi)
	@$(BUILD_DIR)/core/root/capydi

.PHONY: test
test: configure
	$(call cmake_build,testing)
	@$(BUILD_DIR)/testing/testing

.PHONY: bench
bench: configure
	@if [ -z "$(TARGET)" ]; then \
		echo "Error: set TARGET=<benchmark_name>"; exit 1; \
	fi

	@mkdir -p artifacts
	$(call cmake_build,$(TARGET),-B) 2>&1 | tee $(GCC_TIME_REPORT_FILE_PATH)
	@cp $(BUILD_DIR)/benchmarks/trace.json $(EXTERNIS_TRACE_FILE_PATH)
	@$(BUILD_DIR)/benchmarks/$(TARGET)

.PHONY: docs
docs:
	@doxygen ${DOCS_DIR}/Doxyfile

endif # !MAKEFILE_NATIVE_MK_
