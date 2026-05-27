#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <libgen.h>

/* Максимальные размеры для буферов */
#define MAX_LINE 1024  /* максимальная длина строки ввода */
#define MAX_ARGS 64    /* максимальное количество аргументов команды */

/*
 * Основные функции оболочки
 */
void shell_loop(int batch_mode, FILE *input);
char *read_line(FILE *input);
char **parse_line(char *line);
int execute_command(char **args);
int launch_program(char **args);

/*
 * Встроенные команды оболочки
 * Каждая функция принимает массив аргументов и возвращает 1,
 * если оболочка должна продолжить работу, или 0 для выхода
 */
int cd_command(char **args);
int clr_command(char **args);
int dir_command(char **args);
int environ_command(char **args);
int echo_command(char **args);
int help_command(char **args);
int pause_command(char **args);
int quit_command(char **args);

/* Глобальная переменная для хранения пути к оболочке */
extern char shell_path[];

#endif /* MYSHELL_H */