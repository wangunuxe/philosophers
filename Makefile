NAME = philo
CC = cc
CFLAGS = -Werror -Wall -Wextra -pthread

#Run 'make re MODE=debug for debug mode
#Run 'make re MODE=debug_thread for debug mode and information of data race
MODE = none
ifeq ($(MODE), debug)
	CFLAGS += -D DEBUG_FORMATTING=1
endif
ifeq ($(MODE), debug_thread)
	CFLAGS += -D DEBUG_FORMATTING=1 -fsanitize=thread -g
endif

SRC_PATH = source/
OBJ_PATH = object/

SRC = exit.c \
	grim_reaper.c \
	init.c \
	main.c \
	output.c \
	parsing.c \
	philosopher.c \
	time.c
SRCS = $(addprefix $(SRC_PATH), $(SRC))
OBJ = $(SRC:.c=.o)
OBJS = $(addprefix $(OBJ_PATH), $(OBJ))

INC = -I ./include/

#COLORS
G = "\033[32m"
X = "\033[0m"

all: $(NAME)
	@printf "\n"
	@echo $(G)" PHILOSOPHER "$(X)
	@printf "\n\n"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJ_PATH)
fclean: clean
	rm -f $(NAME)
re: fclean all
.PHONY: all re clean fclean