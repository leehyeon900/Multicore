CC = gcc
TARGET = mat_mul
OBJECTS = main.o mul.o timer.o

CFLAGS = -std=c99 -Wall -pthread -O2 -mavx2 -mfma
LDFLAGS = -lm -pthread

MODE = single
DEVICE = cpu

NDIM = 2048
MDIM = 2048
TEST = 2048

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)
	./$(TARGET) $(TEST) $(TEST) -v

.PHONY: clean run

clean:
	rm -rf $(TARGET) $(OBJECTS)

run: $(TARGET)
	@thorq --add --mode $(MODE) --device $(DEVICE) --timeout 60 ./$(TARGET) $(NDIM) $(MDIM) -v
