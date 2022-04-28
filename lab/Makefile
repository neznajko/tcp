# Compile with TARGET=overflow-2 make, for example
CFLAGS := -pedantic -fno-stack-protector -no-pie -z execstack
GCC    := gcc $(CFLAGS)
################################################################
$(TARGET): $(TARGET).o
	$(GCC) -ggdb -o $@ $^
$(TARGET).o: $(TARGET).c
	$(GCC) -ggdb -o $@ -c $<
################################################################
.PHONY: clean dis gitignore nasm
clean:
	@$(RM) -v $(TARGET) $(TARGET).o
dis: $(TARGET).c
	$(GCC) -S -fno-asynchronous-unwind-tables $^
gitignore:
	@grep $(TARGET) .gitignore > tmp || true
	@[ -s tmp ] || echo $(TARGET) >> .gitignore; rm tmp
nasm: $(TARGET).asm
	nasm -f elf64 -g -F dwarf $< -l $(TARGET).lst
	ld $(TARGET).o -o $(TARGET)
################################################################
