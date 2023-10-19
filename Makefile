TARGET = client
CC = g++

SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))

$(TARGET) : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o : %.c
	$(CC) -c $< -o $@

clean:
	rm -f *.o