SRCS=main.s
OBJS=$(subst .s,.o,$(SRCS))
NATIVE_TARGET=example.exe
BIN_TARGET=example.bin

.PHONY: all tidy clean

all: $(BIN_TARGET)

%.o: %.s
	$(AS) $< -o $@

$(BIN_TARGET): $(NATIVE_TARGET)
	objcopy -j .text -O binary $< $@

$(NATIVE_TARGET): $(OBJS)
	$(LD) $< -o $@

tidy:
	rm -f $(OBJS)

clean: tidy
	rm -f $(BIN_TARGET) $(NATIVE_TARGET)
