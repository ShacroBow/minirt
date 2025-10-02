NAME = minirt

SRCS_PARSER = parser/parser.c parser/validation.c \
			  parser/parse_elements.c parser/parse_cylinder.c parser/parser_utils.c parser/parser_utils_extra.c
SRCS_VECTORS = vectors/vec_operations_1.c vectors/vec_operations_2.c
SRCS_INTERSECTIONS = intersections/hit_sphere.c intersections/hit_plane.c \
					 intersections/hit_cylinder.c intersections/intersections.c
SRCS_RENDER = render/render.c render/shading.c render/camera.c render/color.c render/color_ops.c render/background.c
SRCS_UTILS = utils/error.c utils/memory.c utils/string_utils.c
SRC = main.c $(SRCS_PARSER) $(SRCS_VECTORS) $(SRCS_INTERSECTIONS) $(SRCS_RENDER) \
	  $(SRCS_UTILS)

OBJ_DIR = ./dir/
OBJ = $(SRC:%.c=$(OBJ_DIR)%.o)

LIBFT_PATH = ./libft/
LIBFT = libft.a

MLX_PATH = ./minilibx-linux/

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I. -I$(LIBFT_PATH) -I$(MLX_PATH)
LDFLAGS = -L$(MLX_PATH) -lmlx -L$(LIBFT_PATH) -lft -lX11 -lXext -lm


all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ) $(LIBFT_PATH)$(LIBFT)
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME) $(LDFLAGS)

$(OBJ_DIR)%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@

$(LIBFT_PATH)$(LIBFT):
	$(MAKE) -C $(LIBFT_PATH) all -j8

clean:
	rm -rf $(OBJ)
	$(MAKE) -C $(LIBFT_PATH) clean

fclean: clean
	rm -rf $(NAME) $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re