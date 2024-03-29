CCFLAGS = -Wall -Wextra -pedantic -ansi

BUILD_DIR = .
SRC_DIR = src
BIN = fmmodel.exe

all: $(BUILD_DIR)/$(BIN)

$(BUILD_DIR)/$(BIN):
	$(CC) $(CCFLAGS) -o $@ $(SRC_DIR)/fmmodel.c

$(BUILD_DIR):
	mkdir $@

clean:
	rm -f $(BUILD_DIR)/$(BIN)
