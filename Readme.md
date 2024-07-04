# Minishell

## Overview

Minishell is a project that involves creating a simple shell. This shell aims to replicate basic behaviors of real shell environments, providing a platform for executing commands and managing processes.
## Features

-   Basic shell functionality: executing commands, handling paths, and redirection.
-   Built-in commands like cd, echo, exit, and more.
-   Implementation of pipes (|) and redirections (>, <).
-   Signal handling to manage interrupts and termination signals.

## Prerequisites

-   A Unix-like operating system.
-   GCC (GNU Compiler Collection) installed.
-   Basic knowledge of C programming and shell commands.

Installation
```
git clone https://github.com/jbettini/Minishell.git
cd Minishell
make
```

## Usage

Run the compiled executable:
```
./minishell
````

You can then enter shell commands as you would in a regular shell. For example:
```
ls -l
cd /path/to/directory
echo "Hello, World!"
```
## Project Structure

-   src/: Source files.
-   include/: Header files.
-   Makefile: Build configuration.