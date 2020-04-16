EXT_DIR=external

ZYDIS_REPO=https://github.com/zyantific/zydis.git
ZYDIS_TAG=v3.1.0
ZYDIS_DIR=$(EXT_DIR)/zydis
ZYDIS_BUILD_DIR=$(ZYDIS_DIR)/build
ZYDIS_MAKEFILE=$(ZYDIS_BUILD_DIR)/Makefile
ZYDIS_OUT=$(ZYDIS_BUILD_DIR)/libZydis.a
ZYCORE_DIR=$(ZYDIS_DIR)/dependencies/zycore
ZYCORE_BUILD_DIR=$(ZYDIS_BUILD_DIR)/zycore
ZYCORE_INC=$(ZYCORE_DIR)/include $(ZYCORE_BUILD_DIR)
ZYDIS_INC=$(ZYDIS_DIR)/include  $(ZYDIS_BUILD_DIR) $(ZYCORE_INC)

DEPENDS=$(ZYDIS_OUT)

EXT_INC=$(ZYDIS_INC)
EXT_LIB_DIRS=$(ZYDIS_BUILD_DIR)
SRC_INC=src
INC=$(SRC_INC) $(EXT_INC)
LIBS=Zydis
LIB_DIRS=$(EXT_LIB_DIRS)
CPPFLAGS=$(foreach d, $(INC), -I$d) $(foreach d, $(LIB_DIRS), -L$d) $(foreach l, $(LIBS), -l$l)
GIT_FLAGS=--recursive -c advice.detachedHead=false --depth=1
SRC_DIR=src
SRCS=$(shell find $(SRC_DIR) -name *.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))
TARGET=dynre-x86.exe

AR=ar
CMAKE=cmake
CPP=g++
GIT=git

.PHONY: all depend tidy clean

all: $(TARGET)

$(ZYDIS_DIR):
	$(GIT) clone $(GIT_FLAGS) $(ZYDIS_REPO) -b $(ZYDIS_TAG) $@

$(ZYDIS_MAKEFILE): $(ZYDIS_DIR)
	mkdir $(shell dirname $@)
	cd $(shell dirname $@) && $(CMAKE) -S $< .. -G 'Unix Makefiles'

$(ZYDIS_OUT): $(ZYDIS_MAKEFILE)
	$(MAKE) -C $(shell dirname $<)

depend: $(DEPENDS)

%.o: %.cpp $(DEPENDS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CPP) $< $(CPPFLAGS) -o $@

tidy:
	rm -f $(OBJS)

clean: tidy
	rm -f $(TARGET)

dist-clean: clean
	rm -rf $(EXT_DIR)
