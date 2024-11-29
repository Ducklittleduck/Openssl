# 编译器和编译选项
CC = gcc
CFLAGS = -Wall -g

# 目标文件
TARGET = test_program

# 源文件
SRC = test1.c
OBJ = $(SRC:.c=.o)

# 默认目标
all: $(TARGET)

# 链接目标
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# 编译源文件为目标文件
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

# 清理编译结果
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
