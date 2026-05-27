# Makefile для myshell
# Простая оболочка командной строки для UNIX

# Компилятор и флаги
CC = gcc
CFLAGS = -Wall

# Исходные файлы и целевой исполняемый файл
SRCS = myshell.c utility.c
OBJS = $(SRCS:.c=.o)
TARGET = myshell

# Правило по умолчанию - сборка программы
all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Компиляция исходных файлов в объектные
%.o: %.c myshell.h
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка скомпилированных файлов
clean:
	rm -f $(OBJS) $(TARGET)

# Удаление всех созданных файлов, включая резервные копии
distclean: clean
	rm -f *~

# Информация о целях
.PHONY: all clean distclean