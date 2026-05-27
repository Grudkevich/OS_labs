#include "myshell.h"

/* Хранение полного пути к исполняемому файлу оболочки */
char shell_path[MAX_LINE];

/**
 * main - точка входа в программу
 * @argc: количество аргументов командной строки
 * @argv: массив аргументов командной строки
 *
 * Возвращает 0 при нормальном завершении
 */
int main(int argc, char *argv[])
{
    FILE *input = NULL;
    int batch_mode = 0;

    /* Сохраняем полный путь к оболочке */
    if (realpath(argv[0], shell_path) == NULL) {
        perror("realpath");
        strncpy(shell_path, argv[0], MAX_LINE - 1);
    }
    
    /* Устанавливаем переменную окружения shell */
    setenv("shell", shell_path, 1);

    /* Проверяем, запущена ли оболочка в пакетном режиме */
    if (argc > 1) {
        /* Пакетный режим: читаем команды из файла */
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "myshell: cannot open batch file '%s': %s\n", 
                    argv[1], strerror(errno));
            return EXIT_FAILURE;
        }
        batch_mode = 1;
    } else {
        /* Интерактивный режим: читаем команды с stdin */
        input = stdin;
        batch_mode = 0;
    }

    /* Запускаем главный цикл оболочки */
    shell_loop(batch_mode, input);

    /* Закрываем файл, если он был открыт */
    if (input != stdin && input != NULL) {
        fclose(input);
    }

    return EXIT_SUCCESS;
}

/**
 * shell_loop - главный цикл обработки команд
 * @batch_mode: флаг пакетного режима (1 - пакетный, 0 - интерактивный)
 * @input: файловый поток для чтения команд
 *
 * Бесконечно читает и выполняет команды до получения команды quit
 * или достижения конца файла в пакетном режиме
 */
void shell_loop(int batch_mode, FILE *input)
{
    char *line;
    char **args;
    int status = 1;

    do {
        /* Выводим приглашение только в интерактивном режиме */
        if (!batch_mode) {
            printf("myshell> ");
            fflush(stdout);
        }

        /* Читаем строку команды */
        line = read_line(input);
        
        /* Проверяем на конец файла */
        if (line == NULL) {
            if (batch_mode) {
                /* В пакетном режиме конец файла - завершение работы */
                printf("\nEnd of batch file. Exiting...\n");
            }
            free(line);
            break;
        }

        /* Разбираем строку на аргументы */
        args = parse_line(line);
        
        /* Выполняем команду, если она не пустая */
        if (args[0] != NULL) {
            status = execute_command(args);
        }

        /* Освобождаем память */
        free(line);
        free(args);
    } while (status);
}

/**
 * read_line - читает строку из входного потока
 * @input: файловый поток для чтения
 *
 * Возвращает указатель на строку или NULL при ошибке/конце файла.
 * Выделяет память динамически, вызывающий код должен освободить её.
 */
char *read_line(FILE *input)
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t chars_read;

    chars_read = getline(&line, &bufsize, input);
    
    if (chars_read == -1) {
        /* Ошибка или конец файла */
        if (feof(input)) {
            free(line);
            return NULL;
        }
        perror("myshell: read error");
        free(line);
        return NULL;
    }

    /* Удаляем символ новой строки, если он есть */
    if (chars_read > 0 && line[chars_read - 1] == '\n') {
        line[chars_read - 1] = '\0';
    }

    return line;
}

/**
 * parse_line - разбивает строку на массив аргументов
 * @line: строка для разбора
 *
 * Разделителями являются пробелы и табуляции.
 * Возвращает массив строк, оканчивающийся NULL.
 * Выделяет память динамически, вызывающий код должен освободить её.
 */
char **parse_line(char *line)
{
    int bufsize = MAX_ARGS;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (tokens == NULL) {
        fprintf(stderr, "myshell: allocation error\n");
        exit(EXIT_FAILURE);
    }

    /* Получаем первый токен */
    token = strtok(line, " \t\r\n");
    
    /* Продолжаем разбор остальных токенов */
    while (token != NULL) {
        tokens[position] = token;
        position++;

        /* Расширяем буфер, если необходимо */
        if (position >= bufsize) {
            bufsize += MAX_ARGS;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (tokens == NULL) {
                fprintf(stderr, "myshell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " \t\r\n");
    }
    
    tokens[position] = NULL;
    return tokens;
}

/**
 * execute_command - выполняет команду (встроенную или внешнюю)
 * @args: массив аргументов команды
 *
 * Возвращает 1 для продолжения работы оболочки, 0 для выхода
 */
int execute_command(char **args)
{
    if (args[0] == NULL) {
        /* Пустая команда */
        return 1;
    }

    /* Проверяем встроенные команды */
    if (strcmp(args[0], "cd") == 0) {
        return cd_command(args);
    } else if (strcmp(args[0], "clr") == 0) {
        return clr_command(args);
    } else if (strcmp(args[0], "dir") == 0) {
        return dir_command(args);
    } else if (strcmp(args[0], "environ") == 0) {
        return environ_command(args);
    } else if (strcmp(args[0], "echo") == 0) {
        return echo_command(args);
    } else if (strcmp(args[0], "help") == 0) {
        return help_command(args);
    } else if (strcmp(args[0], "pause") == 0) {
        return pause_command(args);
    } else if (strcmp(args[0], "quit") == 0) {
        return quit_command(args);
    } else {
        /* Внешняя команда */
        return launch_program(args);
    }
}

/**
 * launch_program - запускает внешнюю программу
 * @args: массив аргументов для запускаемой программы
 *
 * Использует fork() для создания дочернего процесса и execvp() для
 * выполнения программы. Устанавливает переменную parent в дочернем процессе.
 * Возвращает 1 для продолжения работы оболочки.
 */
int launch_program(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    
    if (pid == 0) {
        /* Дочерний процесс */
        /* Устанавливаем переменную окружения parent */
        setenv("parent", shell_path, 1);
        
        /* Запускаем программу */
        if (execvp(args[0], args) == -1) {
            fprintf(stderr, "myshell: %s: command not found\n", args[0]);
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        /* Ошибка при создании процесса */
        perror("myshell: fork failed");
    } else {
        /* Родительский процесс ждет завершения дочернего */
        waitpid(pid, &status, 0);
    }

    return 1;
}