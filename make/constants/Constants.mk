ifndef CONSTANTS_MK_
CONSTANTS_MK_ := 1

include ./make/constants/Flags.mk

ROOT_DIR := .
SCRIPTS_DIR := $(ROOT_DIR)/scripts
BUILD_ROOT_DIR := $(ROOT_DIR)/build
DOCS_DIR := $(ROOT_DIR)/docs
DOCS_OUTPUT_DIR := $(DOCS_DIR)/output
CMAKE_CONFIGURE_FLAGS := -Wall -Wextra
ARTIFACTS_DIR := $(ROOT_DIR)/artifacts

ifeq ($(USE_BOOST),1)
GCC_TIME_REPORT_FILE_PATH := $(ARTIFACTS_DIR)/use_boost_time_report.log
EXTERNIS_TRACE_FILE_PATH := $(ARTIFACTS_DIR)/use_boost_benchmark.json
CMAKE_CONFIGURE_FLAGS += -DCAPYMETA_WITH_BOOST=ON
else
GCC_TIME_REPORT_FILE_PATH := $(ARTIFACTS_DIR)/no_boost_time_report.log
EXTERNIS_TRACE_FILE_PATH := $(ARTIFACTS_DIR)/no_boost_benchmark.json
CMAKE_CONFIGURE_FLAGS += -DCAPYMETA_WITH_BOOST=OFF
endif

endif # !CONSTANTS_MK_