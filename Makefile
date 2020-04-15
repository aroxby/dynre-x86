EXT_DIR=external

ZYDIS_REPO=https://github.com/zyantific/zydis.git
ZYDIS_TAG=v3.1.0
ZYDIS_DIR=$(EXT_DIR)/zydis
ZYDIS_BUILD_DIR=$(ZYDIS_DIR)/build
ZYDIS_MAKEFILE=$(ZYDIS_BUILD_DIR)/Makefile
ZYDIS_OUT=$(ZYDIS_BUILD_DIR)/libZydis.a

DEPENDS=$(ZYDIS_OUT)

EXT_INC=$(ZYDIS_INC)
SRC_INC=src
INC=$(SRC_INC) $(EXT_INC)
CPPFLAGS=$(foreach d, $(INC), -I$d)
GIT_FLAGS=--recursive -c advice.detachedHead=false --depth=1
SRC_DIR=src
SRCS=$(shell find $(SRC_DIR) -name *.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

AR=ar
CMAKE=cmake
CPP=g++
GIT=git

.PHONY: all depend tidy clean

$(ZYDIS_DIR):
	$(GIT) clone $(GIT_FLAGS) $(ZYDIS_REPO) -b $(ZYDIS_TAG) $@

$(ZYDIS_MAKEFILE): $(ZYDIS_DIR)
	$(CMAKE) -S $< -B $(shell dirname $@) -G 'Unix Makefiles'

$(ZYDIS_OUT): $(ZYDIS_MAKEFILE)
	$(MAKE) -C $(shell dirname $<)

depend: $(DEPENDS)

%.o: %.cpp $(DEPENDS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

tidy:
	rm -f $(OBJS)

clean: tidy

dist-clean: clean
	rm -rf $(EXT_DIR)
