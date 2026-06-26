# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: dachin <dachin@student.codam.nl>             +#+                      #
#                                                    +#+                       #
#    Created: 2026/03/18 08:09:26 by dachin        #+#    #+#                  #
#    Updated: 2026/06/26 08:53:51 by dachin        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# List of source files
SRCS = 	src/main.c \
		src/utils.c \
		src/utils2.c \
		src/init.c \
		src/philo.c \
		src/monitor.c \
		src/parse.c \

# Creates object files by replacing .c with .o. $ references variables or automatic variables
OBJ_DIR = o_files
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

# Compiler and compiler flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Detect OS
UNAME_S := $(shell uname -s)

# Main executable name
NAME = philo

# Default target. Always put this in first place
all: $(NAME)

# Build main executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# Pattern rule. Compiles any .c file into .o using specified compiler and flags
# % is a wildcard
# $@ - name of the target
# $< = name of the first prerequisite
# $^ = name of all prerequisites
# -I flag tells the compiler where to look for header files
$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# $(RM) is prefined in make and equals rm -f. -f force flag avoids errors if files do not exist
clean:
	$(RM) -r $(OBJ_DIR)

# Removes all object files and the library file.
fclean:	clean
	$(RM) $(NAME)

# Rebuilds everything by clean and all
re:	fclean all

# Declares these targets as phony (not actual files), so Make always runs them when requested.
.PHONY: all clean fclean re valgrind helgrind

# ===== TEST SECTION =====

valgrind: $(NAME)
	@bash tests/valgrind_test.sh valgrind

helgrind: $(NAME)
	@bash tests/valgrind_test.sh helgrind
