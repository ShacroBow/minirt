NAME = minirt
SRC =	main.c $(SRCS_PARSER) $(SRCS_VECTORS) $(SRCS_INTERSECTIONS) $(SRCS_RENDER) \
		$(SRCS_UTILS)

SRCS_PARSER = parser/parser.c parser/parser_utils.c parser/validation.c \
              parser/parse_elements.c parser/parse_utils_extra.c
SRCS_VECTORS = vectors/vec_operations_1.c vectors/vec_operations_2.c
SRCS_INTERSECTIONS = intersections/hit_sphere.c intersections/hit_plane.c \
                     intersections/hit_cylinder.c intersections/intersections.c
SRCS_RENDER = render/render.c render/shading.c render/camera.c render/color.c
SRCS_UTILS = utils/error.c utils/memory.c utils/string_utils.c

OBJ_DIR = ./dir/
OBJ = $(SRC:%.c=$(OBJ_DIR)%.o)
OBJ_BONUS = $(SRC_BONUS:%.c=$(OBJ_DIR)%.o)

LIBFT_PATH = ./libft/
LIBFT = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LIBFLAGS = -L. $(LIBFT_PATH)$(LIBFT) -lmlx -lX11 -lXext -lm


all: $(NAME)

#bonus: $(NAME_BONUS)

$(NAME): $(OBJ_DIR) $(OBJ) $(LIBFT_PATH)$(LIBFT)
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME) $(LIBFLAGS)

$(NAME_BONUS): $(OBJ_DIR) $(OBJ_BONUS) $(LIBFT_PATH)$(LIBFT)
	$(CC) $(OBJ_BONUS) $(CFLAGS) -o $@ $(LIBFLAGS)

$(OBJ_DIR)%.o : %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

$(LIBFT_PATH)$(LIBFT):
	make -C $(LIBFT_PATH) all -j8

clean:
	rm -rf $(OBJ)
	rm -rf $(OBJ_BONUS)
	make -C $(LIBFT_PATH) clean

fclean: clean
	rm -rf $(NAME) $(NAME_BONUS) a.out $(OBJ_DIR) files.supp
	make -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re #bonus