SRCS=main.s
OBJS=$(subst .s,.o,$(SRCS))
TARGET=example.exe

.PHONY: all tidy clean

all: $(TARGET)

%.o: %.s
	$(AS) $< -o $@

$(TARGET): $(OBJS)
	$(LD) $< -o $@

tidy:
	rm -f $(OBJS)

clean: tidy
	rm -f $(TARGET)