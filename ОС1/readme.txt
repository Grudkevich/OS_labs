===============================================================================
                      MYSHELL - Simple UNIX Shell
                        User Manual (readme)
===============================================================================

1. INTRODUCTION
---------------
MYSHELL is a simple command-line shell for UNIX systems. It provides a set
of built-in commands and the ability to execute external programs. The shell
can operate in interactive mode, where it accepts commands from the user via
a prompt, or in batch mode, where it reads commands from a file.

2. GETTING STARTED
------------------
After compilation, the shell can be started in two ways:

   Interactive mode:
       ./myshell

   Batch mode:
       ./myshell <batchfile>

   Where <batchfile> is a text file containing shell commands, one per line.

3. BUILT-IN COMMANDS
--------------------
The following commands are built into the shell:

   cd [directory]
       Change the current working directory to the specified directory.
       If no directory is given, the current directory is displayed.
       Example: cd /home/user
       Example: cd ..

   clr
       Clear the terminal screen.
       Example: clr

   dir [directory]
       List the contents of the specified directory (one file per line).
       If no directory is given, the contents of the current directory
       are displayed.
       Example: dir /tmp
       Example: dir

   environ
       Display all environment variables and their values.
       Example: environ

   echo <text>
       Display the given text on the screen. Multiple spaces and tabs
       are reduced to a single space. A newline is added at the end.
       Example: echo Hello World

   help
       Display a help message with information about all built-in commands.
       Example: help

   pause
       Pause the shell and wait for the user to press the Enter key.
       Useful in batch files to stop execution temporarily.
       Example: pause

   quit
       Exit the shell. In batch mode, this command can be used to exit
       before the end of the file is reached.
       Example: quit

4. EXTERNAL COMMANDS
--------------------
Any command that is not a built-in command is treated as an external
program. The shell uses the fork() and exec() system calls to create a
child process and execute the program. The child process inherits the
environment of the parent shell.

   Example:
       myshell> ls -l
       myshell> grep pattern file.txt
       myshell> /bin/date

5. ENVIRONMENT VARIABLES
-------------------------
The shell manages two special environment variables:

   shell
       Contains the full path to the myshell executable. This is set
       automatically when the shell starts.

   parent
       Set in child processes launched by the shell. It contains the
       same path as the shell variable, indicating which shell created
       the child process.

   The built-in command 'environ' displays all environment variables.

   The 'cd' command updates the PWD environment variable to reflect
   the current working directory.

6. CONCEPTS EXPLAINED
---------------------
For users new to UNIX, here are explanations of important concepts:

   Input/Output Redirection:
       UNIX shells typically support redirecting input and output using
       the characters '<', '>', and '|'. MYSHELL is a simple shell that
       does not directly implement redirection. However, external programs
       launched by the shell can use their own redirection if supported.
       The shell itself focuses on basic command execution.

   Program Environment:
       Every program in UNIX runs within an "environment" - a set of
       variables that provide information to the program. Common variables
       include PATH (where to find executables), HOME (user's home
       directory), and USER (current username). The 'environ' command
       displays all these variables.

   Background Execution:
       Some shells allow programs to run in the background by adding '&'
       at the end of the command. This allows the user to continue using
       the shell while the program runs. MYSHELL does not support
       background execution - all programs run in the foreground, and
       the shell waits for them to complete.

   Batch Files:
       A batch file is a text file containing shell commands. When you
       run './myshell batchfile', the shell reads and executes each line
       of the file as if it were typed at the prompt. This is useful for
       automating repetitive tasks or running a series of commands.

7. EXAMPLES
-----------
   Start the shell interactively:
       $ ./myshell
       myshell> echo Hello from myshell
       Hello from myshell
       myshell> cd /tmp
       myshell> dir
       ... (list of files in /tmp)
       myshell> environ
       ... (list of environment variables)
       myshell> quit

   Create a batch file (test.txt):
       echo Starting batch process
       cd /home/user
       dir
       pause
       echo Batch process complete
       quit

   Run the batch file:
       $ ./myshell test.txt
       Starting batch process
       ... (list of files)
       Press Enter to continue...
       Batch process complete

8. NOTES
--------
   - In batch mode, the shell exits when it reaches the end of the file
     or when a 'quit' command is executed.
   - In interactive mode, the prompt is 'myshell> '.
   - The shell does not support command history or line editing features
     found in more advanced shells like bash or tcsh.
   - All commands are case-sensitive.
   - Error messages are printed to stderr when commands fail.

9. TROUBLESHOOTING
------------------
   Problem: "myshell: command not found"
   Solution: Make sure the command is in your PATH or specify the full
             path to the command.

   Problem: "myshell: cd: permission denied"
   Solution: Check that you have execute permission for the directory.

   Problem: "myshell: cannot open batch file"
   Solution: Verify that the batch file exists and is readable.

===============================================================================
                           End of User Manual
===============================================================================