#include "../include/minirt.h"
# include <stdio.h>
# include <ctype.h>
# include <stdlib.h>

static int	read_token(FILE *f, char *buf, size_t size)
{
	int	c;
	size_t i;

	i = 0;
	for (;;)
	{
		c = fgetc(f);
		if (c == '#')
		{
			while ((c = fgetc(f)) != EOF && c != '\n')
				;
			continue;
		}
		if (c == EOF)
			return (0);
		if (!isspace(c))
		{
			ungetc(c, f);
			break;
		}
	}
	while ((c = fgetc(f)) != EOF && !isspace(c))
	{
		if (i + 1 < size)
			buf[i++] = (char)c;
	}
	buf[i] = '\0';
	return (1);
}

static void	skip_whitespace_and_comments(FILE *f)
{
	int c;

	for (;;)
	{
		c = fgetc(f);
		if (c == EOF)
			return;
		if (c == '#')
		{
			while ((c = fgetc(f)) != EOF && c != '\n')
				;
			continue;
		}
		if (isspace(c))
			continue;
		ungetc(c, f);
		return;
	}
}

t_texture	*load_ppm(const char *path)
{
	FILE	*fp;
	char	magic[8];
	char	buf[64];
	int	w;
	int	h;
	int	maxval;
	size_t	size;
	unsigned char *data;
	t_texture *tex;

	fp = fopen(path, "rb");
	if (!fp)
		return (NULL);
	if (!read_token(fp, magic, sizeof(magic)))
	{
		fclose(fp);
		return (NULL);
	}
	if (magic[0] != 'P' || magic[1] != '6')
	{
		fclose(fp);
		return (NULL);
	}
	if (!read_token(fp, buf, sizeof(buf))) { fclose(fp); return (NULL); }
	w = atoi(buf);
	if (!read_token(fp, buf, sizeof(buf))) { fclose(fp); return (NULL); }
	h = atoi(buf);
	if (!read_token(fp, buf, sizeof(buf))) { fclose(fp); return (NULL); }
	maxval = atoi(buf);
	if (maxval <= 0 || maxval > 255)
	{
		fclose(fp);
		return (NULL);
	}
	 /* Skip any whitespace or comments between header and binary data,
		 but do not consume the first data byte. */
	 skip_whitespace_and_comments(fp);
	size = (size_t)w * (size_t)h * 3;
	data = malloc(size);
	if (!data)
	{
		fclose(fp);
		return (NULL);
	}
	if (fread(data, 1, size, fp) != size)
	{
		free(data);
		fclose(fp);
		return (NULL);
	}
	fclose(fp);
	tex = malloc(sizeof(t_texture));
	if (!tex)
	{
		free(data);
		return (NULL);
	}
	tex->width = w;
	tex->height = h;
	tex->channels = 3;
	tex->data = data;
	return (tex);
}

void	free_texture(t_texture *tex)
{
	if (!tex)
		return;
	if (tex->data)
		free(tex->data);
	free(tex);
}

t_color	sample_texture(const t_texture *tex, double u, double v)
{
	int	x;
	int	y;
	size_t idx;
	unsigned char r;
	unsigned char g;
	unsigned char b;

	if (!tex || !tex->data)
		return ((t_color){0.0, 0.0, 0.0});
	if (u < 0.0)
		u = 0.0;
	else if (u > 1.0)
		u = 1.0;
	if (v < 0.0)
		v = 0.0;
	else if (v > 1.0)
		v = 1.0;
	x = (int)(u * (tex->width - 1));
	y = (int)((1.0 - v) * (tex->height - 1));
	idx = ((size_t)y * tex->width + (size_t)x) * tex->channels;
	r = tex->data[idx + 0];
	g = tex->data[idx + 1];
	b = tex->data[idx + 2];
	return ((t_color){(double)r, (double)g, (double)b});
}
