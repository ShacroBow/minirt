NAME				=	minirt

SRCS_PARSER			=	parser/parser.c parser/parser_utils.c parser/parser_read.c \
						parser/parse_elements.c parser/scene_add.c \
						parser/parse_sphere.c parser/parse_plane.c \
						parser/parse_cylinder.c parser/parse_cone.c \
						parser/parse_extra_args.c \
						parser/linter.c parser/linter_elements.c parser/linter_utils.c \
						parser/linter_arg_validation.c parser/linter_arg_validation_2.c \
						parser/lint_sphere.c parser/lint_plane.c \
						parser/lint_cylinder.c parser/lint_cone.c \
						parser/parse_mesh.c parser/lint_mesh.c
SRCS_VECTORS		=	vectors/vec_operations_1.c vectors/vec_operations_2.c \
						vectors/vec_operations_3.c
SRCS_INTERSECTIONS	=	intersections/hit_sphere.c intersections/hit_plane.c \
						intersections/hit_cylinder.c intersections/hit_cone.c \
						intersections/hit_cone2.c intersections/intersections.c
SRCS_RENDER			=	render/render.c render/render_utils.c render/render_threads.c \
						render/shading.c render/shading_utils.c render/shadows.c \
						render/specular_only.c render/transparency.c \
						render/color_ops.c render/color.c render/background.c \
						render/display.c
SRCS_TEXTURE		=	texture/texture.c texture/texture_loader.c \
						texture/texture_loader_utils.c texture/uv_mapping.c \
						texture/bump.c
SRCS_UTILS			=	utils/error.c utils/memory.c utils/string_utils.c \
						utils/utils.c utils/read_file.c
SRCS_CAMERA			=	camera/camera.c camera/camera_utils.c camera/camera_utils2.c \
						camera/camera_creation.c camera/camera_actions.c
SRCS_MESH			=	mesh/obj_load.c mesh/obj_read.c mesh/obj_array.c \
						mesh/obj_lex.c mesh/obj_number.c mesh/obj_parse.c \
						mesh/obj_face.c mesh/obj_triangle.c \
						mesh/mtl_load.c mesh/mtl_parse.c \
						mesh/mesh_adjacency.c mesh/mesh_normals.c mesh/mesh_build.c \
						mesh/bvh_aabb.c mesh/bvh_split.c mesh/bvh_partition.c \
						mesh/bvh_build.c mesh/bvh_walk.c mesh/tri_hit.c \
						mesh/mesh_hit.c mesh/mesh_shade.c mesh/mesh_transform.c \
						mesh/mesh_free.c

SRC					=	main.c $(SRCS_PARSER) $(SRCS_VECTORS) $(SRCS_INTERSECTIONS) \
						$(SRCS_RENDER) $(SRCS_TEXTURE) $(SRCS_UTILS) $(SRCS_CAMERA) \
						$(SRCS_MESH)

OBJ_DIR				=	./dir/
LIBFT_PATH			=	./libft/
LIBFT				=	$(LIBFT_PATH)libft.a

CC					=	cc
# gnu11: MLX declares its hook callbacks as int (*)(), which C23 (the
# default of new compilers) reads as "no arguments" and rejects.
CFLAGS				=	-Wall -Wextra -Werror -MMD -MP -O3 -flto -march=native -g \
						-std=gnu11 -pthread -I./include
LDLIBS				=	-L$(LIBFT_PATH) -lft -lm -pthread

all: $(NAME)

# Platform specific part: which MLX to build, how to link it, and the
# platform source file (SRCS_PLATFORM). Each file defines MLX, MLX_LDLIBS,
# the $(MLX) build rule and the mlx_clean / mlx_fclean rules.
UNAME_S				:=	$(shell uname -s)
ifeq ($(UNAME_S),Darwin)
include Makefile.macos
else
include Makefile.linux
endif

SRC					+=	$(SRCS_PLATFORM)
OBJ					=	$(SRC:%.c=$(OBJ_DIR)%.o)
DEPS				=	$(OBJ:.o=.d)

$(NAME): $(OBJ) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDLIBS) $(MLX_LDLIBS)

$(OBJ_DIR)%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_PATH) all

clean: mlx_clean
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_PATH) clean

fclean: clean mlx_fclean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_PATH) fclean

re: fclean
	$(MAKE) all

-include $(DEPS)

.PHONY: all clean fclean re mlx_clean mlx_fclean
