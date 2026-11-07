NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = philo.c \
	  check_args.c \
	  init_program.c \
	  init_philos.c \
	  init_routines.c \
	  utils_args.c \
	  routine_philos.c \
	  routine_philos_eat.c \
	  routine_waiter.c \
	  utils_routine.c \
	  free_structs.c
OBJ = $(SRC:.c=.o)

HEADER = philo.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

tsan: fclean
	$(MAKE) CFLAGS="$(CFLAGS) -fsanitize=thread -g"

.PHONY: all clean fclean re tsan
