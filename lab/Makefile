TARGET = example3
CFLAGS = -g -fno-stack-protector -pedantic #-m32
GCC = gcc $(CFLAGS)
$(TARGET): $(TARGET).o
	$(GCC) -o $@ $^
$(TARGET).o: $(TARGET).c
	$(GCC) -o $@ -c $<
.PHONY: clean dis
clean:
	$(RM) $(TARGET) $(TARGET).o
dis: $(TARGET).c
	gcc -S -fno-asynchronous-unwind-tables $^
################################################################
#
#
