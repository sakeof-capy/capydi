include ./make/constants/Constants.mk

ifdef NATIVE_MODE
include ./make/constants/NativeConstants.mk
include ./make/Makefile.native.mk
$(info Running in the native mode)
else
include ./make/constants/DockerConstants.mk
include ./make/Makefile.docker.mk
$(info Running in the docker mode)
endif

ifdef USE_NATIVE_BUILD_DIR
include ./make/constants/NativeBuildDir.mk
else
include ./make/constants/DockerBuildDir.mk
endif

ifdef BUILD_DIR
$(info Using build dir: $(BUILD_DIR))
else
$(error Build dir not defined)
endif

.PHONY: clean
clean:
	@echo "Cleaning: $(BUILD_ROOT_DIR)"
	@rm -rf $(BUILD_ROOT_DIR)
	@echo "Cleaning: $(DOCS_OUTPUT_DIR)"
	@rm -rf $(DOCS_OUTPUT_DIR)
