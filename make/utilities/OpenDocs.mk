ifndef OPEN_DOCS_MK_
OPEN_DOCS_MK_ := 1

include ./make/constants/Constants.mk

ifeq ($(OS),Windows_NT)
    OPEN_CMD := start
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        OPEN_CMD := open
    else
        OPEN_CMD := xdg-open
    endif
endif

endif # !OPEN_DOCS_MK_
