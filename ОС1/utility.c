#include "myshell.h"

/**
 * cd_command - встроенная команда для смены текущего каталога
 * @args: аргументы команды (args[1] - путь к каталогу)
 *
 * Если каталог не указан, выводит текущий каталог.
 * При смене каталога обновляет переменную окружения PWD.
 * Возвращает 1 для продолжения работы оболочки.
 */
int cd_command(char **args)
{
    char cwd[MAX_LINE];

    if (args[1] == NULL) {
        /* Выводим текущий каталог */
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("myshell: cd");
        }
    } else {
        /* Меняем каталог */
        if (chdir(args[1]) != 0) {
            fprintf(stderr, "myshell: cd: %s: %s\n", 
                    args[1], strerror(errno));
        } else {
            /* Обновляем переменную PWD */
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                setenv("PWD", cwd, 1);
            }
        }
    }
    return 1;
}

/**
 * clr_command - очищает экран терминала
 * @args: аргументы (не используются)
 *
 * Использует escape-последовательность для очистки экрана.
 * Возвращает 1 для продолжения работы оболочки.
 */
int clr_command(char **args)
{
    (void)args;  /* Подавляем предупреждение о неиспользуемом параметре */
    printf("\033[2J\033[H");
    fflush(stdout);
    return 1;
}

/**
 * dir_command - выводит содержимое указанного каталога
 * @args: аргументы (args[1] - путь к каталогу, по умолчанию ".")
 *
 * Выводит список файлов в каталоге, по одному на строку.
 * Возвращает 1 для продолжения работы оболочки.
 */
int dir_command(char **args)
{
    DIR *dir;
    struct dirent *entry;
    char *path;

    /* Определяем путь к каталогу */
    path = (args[1] != NULL) ? args[1] : ".";

    /* Открываем каталог */
    dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "myshell: dir: %s: %s\n", 
                path, strerror(errno));
        return 1;
    }

    /* Читаем и выводим содержимое каталога */
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
    return 1;
}

/**
 * environ_command - выводит все переменные окружения
 * @args: аргументы (не используются)
 *
 * Использует внешнюю переменную environ для доступа к окружению.
 * Возвращает 1 для продолжения работы оболочки.
 */
int environ_command(char **args)
{
    extern char **environ;
    char **env = environ;
    
    (void)args;
    
    while (*env != NULL) {
        printf("%s\n", *env);
        env++;
    }
    
    return 1;
}

/**
 * echo_command - выводит текст на экран
 * @args: аргументы для вывода
 *
 * Выводит все аргументы, разделяя их одним пробелом,
 * и завершает вывод символом новой строки.
 * Возвращает 1 для продолжения работы оболочки.
 */
int echo_command(char **args)
{
    int i = 1;
    int first = 1;

    while (args[i] != NULL) {
        if (!first) {
            printf(" ");
        }
        printf("%s", args[i]);
        first = 0;
        i++;
    }
    printf("\n");
    
    return 1;
}

/**
 * help_command - выводит справочную информацию об оболочке
 * @args: аргументы (не используются)
 *
 * Показывает список доступных встроенных команд и краткое описание.
 * Возвращает 1 для продолжения работы оболочки.
 */
int help_command(char **args)
{
    (void)args;
    
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║           MYSHELL - Simple UNIX Shell                   ║\n");
    printf("╠══════════════════════════════════════════════════════════╣\n");
    printf("║ Built-in commands:                                      ║\n");
    printf("║   cd [directory]  - Change or display current directory ║\n");
    printf("║   clr             - Clear the screen                   ║\n");
    printf("║   dir [directory] - List directory contents            ║\n");
    printf("║   environ         - Display environment variables      ║\n");
    printf("║   echo <text>     - Display text                       ║\n");
    printf("║   help            - Show this help message             ║\n");
    printf("║   pause           - Wait for Enter key                 ║\n");
    printf("║   quit            - Exit the shell                     ║\n");
    printf("║                                                        ║\n");
    printf("║ Environment variables:                                  ║\n");
    printf("║   shell  - Full path to the shell executable           ║\n");
    printf("║   parent - Full path to the parent shell               ║\n");
    printf("║                                                        ║\n");
    printf("║ Usage modes:                                           ║\n");
    printf("║   Interactive: ./myshell                               ║\n");
    printf("║   Batch mode:  ./myshell batchfile                     ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    
    return 1;
}

/**
 * pause_command - приостанавливает работу до нажатия Enter
 * @args: аргументы (не используются)
 *
 * Возвращает 1 для продолжения работы оболочки.
 */
int pause_command(char **args)
{
    (void)args;
    
    printf("Press Enter to continue...");
    fflush(stdout);
    
    /* Читаем символ новой строки (Enter) */
    while (getchar() != '\n') {
        /* Игнорируем все символы до Enter */
    }
    
    return 1;
}

/**
 * quit_command - завершает работу оболочки
 * @args: аргументы (не используются)
 *
 * Возвращает 0 для завершения главного цикла оболочки.
 */
int quit_command(char **args)
{
    (void)args;
    return 0;
}