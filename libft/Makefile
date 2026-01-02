NAME = libft.a
SRC = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isspace.c\
	ft_isascii.c ft_isprint.c ft_strlen.c ft_memset.c \
	ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c \
	ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
	ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c \
	ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c \
	ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c \
	ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c \
	ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	ft_lstnew.c ft_lstadd_front.c ft_lstsize.c \
	ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c \
	ft_lstiter.c ft_lstmap.c ft_waste_time.c ft_quicksort.c \
	ft_realloc.c ft_strcmp.c ft_atoi_base.c ft_strndup.c \
	ft_atol.c ft_atof.c ft_split_inplace.c

OBJ = $(SRC:.c=.o)
CC = cc
AR = ar
ARFLAGS = rcs
CFLAGS = -Wall -Wextra -Werror -g #-g -fsanitize=address,bounds,undefined,float-divide-by-zero

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(ARFLAGS) $(NAME) $?

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

BONUS_FLAG = .bonus

$(BONUS_FLAG) : $(OBJ_BONUS)
	$(AR) $(ARFLAGS) $(NAME) $?
	@touch $(BONUS_FLAG)

bonus: $(NAME) $(BONUS_FLAG)

clean:
	rm -f $(OBJ) $(OBJ_BONUS)

fclean: clean
	rm -f $(NAME) a.out $(BONUS_FLAG)

re: fclean all

.PHONY: all clean fclean re bonus