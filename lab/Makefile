TARGET = execstack
CFLAGS = -static -pedantic -fno-stack-protector -no-pie -z execstack
GCC = gcc $(CFLAGS)
$(TARGET): $(TARGET).o
	$(GCC) -ggdb -o $@ $^
$(TARGET).o: $(TARGET).c
	$(GCC) -ggdb -o $@ -c $<
.PHONY: clean dis
clean:
	@$(RM) -v $(TARGET) $(TARGET).o
dis: $(TARGET).c
	$(GCC) -S -fno-asynchronous-unwind-tables $^
################################################################
