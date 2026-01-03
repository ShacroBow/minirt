#define TINYOBJ_LOADER_C_IMPLEMENTATION
// #include "../include/minirt.h"
#include "../include/tinyobj_loader_c.h"
#include <stdio.h>
#include <memory.h>
#include <stdbool.h>
#include <string.h>

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include <float.h>
#include <limits.h>


#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>



static char* mmap_file(size_t* len, const char* filename) {
#ifdef _WIN64
  HANDLE file =
      CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

  if (file == INVALID_HANDLE_VALUE) { /* E.g. Model may not have materials. */
    return NULL;
  }

  HANDLE fileMapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);
  assert(fileMapping != INVALID_HANDLE_VALUE);

  LPVOID fileMapView = MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0);
  char* fileMapViewChar = (char*)fileMapView;
  assert(fileMapView != NULL);

  DWORD file_size = GetFileSize(file, NULL);
  (*len) = (size_t)file_size;

  return fileMapViewChar;
#else

  struct stat sb;
  char* p;
  int fd;

  fd = open(filename, O_RDONLY);
  if (fd == -1) {
    perror("open");
    return NULL;
  }

  if (fstat(fd, &sb) == -1) {
    perror("fstat");
    return NULL;
  }

  if (!S_ISREG(sb.st_mode)) {
    fprintf(stderr, "%s is not a file\n", filename);
    return NULL;
  }

  p = (char*)mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

  if (p == MAP_FAILED) {
    perror("mmap");
    return NULL;
  }

  if (close(fd) == -1) {
    perror("close");
    return NULL;
  }

  (*len) = sb.st_size;

  return p;

#endif
}

static void get_file_data(void* ctx, const char* filename, const int is_mtl,
                          const char* obj_filename, char** data, size_t* len) {
  // NOTE: If you allocate the buffer with malloc(),
  // You can define your own memory management struct and pass it through `ctx`
  // to store the pointer and free memories at clean up stage(when you quit an
  // app)
  // This example uses mmap(), so no free() required.
  (void)ctx;

  if (!filename) {
    fprintf(stderr, "null filename\n");
    (*data) = NULL;
    (*len) = 0;
    return;
  }

  size_t data_len = 0;

  *data = mmap_file(&data_len, filename);
  (*len) = data_len;
}

int	main(void)
{
	const char	*filename = "cube.obj";
	const char	*search_path = NULL;

	tinyobj_shape_t * shape = NULL;
	tinyobj_material_t * material = NULL;
	tinyobj_attrib_t attrib;

	unsigned long num_shapes;
	unsigned long num_materials;

	tinyobj_attrib_init(&attrib);

	int result = tinyobj_parse_obj(&attrib, &shape, &num_shapes, &material, &num_materials, filename, get_file_data, NULL, TINYOBJ_FLAG_TRIANGULATE);

	for (int i = 0; i < attrib.num_vertices; i++)
		printf("attrib.faces is %f\n", attrib.vertices[i]);
	
	tinyobj_attrib_free(&attrib);
	if (shape)
		tinyobj_shapes_free(shape, num_shapes);
	if (material)
		tinyobj_materials_free(material, num_materials);
}
