UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
include Makefile.macos
else ifeq ($(UNAME_S),Linux)
include Makefile.linux
endif
