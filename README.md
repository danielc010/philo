*This project has been created as part of the 42 curriculum by dachin.*

# Philosophers

## DESCRIPTION

Minishell is a Unix shell implementation built as part of the 42 curriculum. The goal is to reproduce the core behaviour of bash — reading user input, parsing it, and executing commands — while handling pipes, redirections, environment variables, signals, and a set of built-in commands.

The project is divided between two authors: **luimarti** is responsible for the lexer, parser, expander, and the built-ins `echo`, `pwd`, and `env`; **dachin** is responsible for the executor, pipes, redirections, signals, and the built-ins `cd`, `export`, `unset`, and `exit`.

### How it works

The shell follows this pipeline for every command the user types:

1. **Lexer** — tokenises the raw input string into a linked list of tokens (words, pipes, redirections, heredocs).
2. **Parser** — converts the token list into a linked list of `t_cmd` structs, each representing one command with its arguments and redirections.
3. **Expander** — expands environment variables (`$VAR`, `$?`) and strips quotes from every argument and redirection filename.
4. **Executor** — executes each command: built-ins are called directly as C functions; external commands are run via `fork` + `execve`. Pipes connect processes, and redirections adjust `stdin`/`stdout` using `dup2`.

### Key Concepts

* **Tokenisation**
  The lexer uses a state machine (NORMAL, IN_WORD, IN_SINGLE_QUOTE, IN_DOUBLE_QUOTE) to classify each character and group them into typed tokens.

* **fork / execve**
  External commands are executed in a child process created with `fork()`. The child calls `execve()` to replace itself with the target program. The parent waits with `waitpid()` and collects the exit status.

* **Pipes**
  A pipe is an OS-level channel between two processes. `pipe()` creates two file descriptors; `dup2()` connects the write end to the first child's stdout and the read end to the second child's stdin.

* **Redirections**
  Input/output redirections (`<`, `>`, `>>`, `<<`) are applied in the child process using `dup2()` before `execve()`, so the parent's file descriptors are never affected.

* **Signals**
  `sigaction()` is used to handle `SIGINT` (Ctrl+C) and `SIGQUIT` (Ctrl+\). The shell uses three signal modes: interactive (waiting for input), waiting (parent waiting for a child), and child (default behaviour restored before `execve`). A single global variable `g_signal` (of type `volatile sig_atomic_t`) is used to communicate signal events from the handler to the main loop.

---

## INSTRUCTIONS

### Compilation

Compile the program using the Makefile:

    make

Remove object files:

    make clean

Remove object files and the executable:

    make fclean

Recompile everything:

    make re

### Execution

Run the shell:

    ./minishell

The shell supports the following built-in commands:

| Command | Description |
|---|---|
| `echo [-n]` | Print arguments to stdout. `-n` suppresses the trailing newline. |
| `pwd` | Print the current working directory. |
| `env` | Print all environment variables. |
| `cd [path]` | Change the current directory. |
| `export [NAME=value]` | Set or export an environment variable. |
| `unset [NAME]` | Remove an environment variable. |
| `exit [n]` | Exit the shell with optional exit code. |

### Supported features

* Single and double quotes
* Environment variable expansion (`$VAR`, `$?`)
* Input/output redirections: `<`, `>`, `>>`
* Heredoc: `<<`
* Pipes: `|`
* Command history (arrow keys)
* Ctrl+C — display a new prompt
* Ctrl+D — exit the shell
* Ctrl+\ — ignored

---

## RESOURCES

* GNU Bash Reference Manual
  https://www.gnu.org/software/bash/manual/bash.html

* The Open Group Base Specifications — Shell Command Language
  https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html

* Linux man pages: `fork`, `execve`, `pipe`, `dup2`, `waitpid`, `sigaction`
  https://man7.org/linux/man-pages/

* GNU Readline Library
  https://tiswww.case.edu/php/chet/readline/rltop.html

* Write a Shell in C — Stephen Brennan
  https://brennan.io/2015/01/16/write-a-shell-in-c/

### AI Usage

AI tools (Claude by Anthropic) were used extensively throughout this project for:

* Explaining Unix system calls (`fork`, `execve`, `pipe`, `dup2`, `waitpid`, `sigaction`) and their correct usage in a shell context.
* Reviewing and debugging the lexer state machine, parser logic, and expander behaviour.
* Identifying and fixing bugs in pipe handling, signal management, redirection application, and built-in exit codes.
* Discussing the correct behaviour of edge cases by comparing output against bash.
* Writing and iterating on test scripts to validate the shell against the evaluation sheet and a community tester.
* Formatting and structuring this README according to the subject requirements.
