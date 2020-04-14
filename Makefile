EXT_DIR=external

ZYDIS_REPO=https://github.com/zyantific/zydis.git
ZYDIS_TAG=v3.1.0
ZYDIS_DIR=$(EXT_DIR)/zydis
ZYDIS_BUILD_DIR=$(ZYDIS_DIR)/build
ZYDIS_OUT=$(ZYDIS_DIR)/$(TBD)

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

.PHONY: all depend tidu clean

$(ZYDIS_DIR):
	$(GIT) clone $(GIT_FLAGS) $(ZYDIS_REPO) -b $(ZYDIS_TAG) $@

$(ZYDIS_BUILD_DIR): $(ZYDIS_DIR)
	$(CMAKE) -S $< -B $@ -G 'Unix Makefiles'

$(ZYDIS_OUT): $(ZYDIS_BUILD_DIR)
	false  # TBD

depend: $(DEPENDS)

%.o: %.cpp $(DEPENDS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

tidy:
	rm -f $(OBJS)

clean: tidy

dist-clean: clean
	rm -rf $(EXT_DIR)
