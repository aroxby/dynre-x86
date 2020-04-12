DEMO_SRCS=demo.s
DEMO_OBJS=$(subst .s,.o,$(DEMO_SRCS))
DEMO_TARGET=demo.exe

.PHONY: default all demo demo-tidy demo-clean tidy clean

default: all

all: demo

%.o: %.s
	$(AS) $< -o $@

$(DEMO_TARGET): $(DEMO_OBJS)
	ld $< -o $@

demo: $(DEMO_TARGET)

demo-tidy:
	rm -f $(DEMO_OBJS)

demo-clean: demo-tidy
	rm -f $(DEMO_TARGET)

tidy: demo-tidy
	rm -f $(OBJS)

clean: tidy demo-clean
	rm -f $(TARGET)