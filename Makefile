ifndef TARGET
TARGET := test_zone
endif
CC = gcc
LDFLAGS = -ggdb
CFLAGS = -ggdb
TMP = /tmp/tags
SHELL = /bin/bash
$(TARGET): $(TARGET).o inet.o
	$(CC) $(LDFLAGS) $^ -o $@
$(TARGET).o: $(TARGET).c inet.h
	$(CC) $(CFLAGS) $< -c -o $@
inet.o: inet.c inet.h
	$(CC) $(CFLAGS) $< -c -o $@
.PHONY: clean etags ctags debug 
clean:
	$(RM) $(TARGET) $(TARGET).o inet.o
ctags etags:
	@$(CC) -M $(TARGET).c | sed 's/.\+\.c /$(@) /g' > $(TMP)
	@source $(TMP)
	@$(RM) $(TMP)
debug: CFLAGS += -DDEBUG
debug: $(TARGET)
