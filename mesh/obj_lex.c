#include "../include/mesh.h"

bool	is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\r' || c == '\v' || c == '\f');
}

char	*skip_ws(char *s)
{
	while (is_blank(*s))
		s++;
	return (s);
}

/**
 * True if s starts with the keyword kw followed by whitespace or the end of
 * the line. rest is set to the text after the keyword.
 */
bool	is_keyword(char *s, const char *kw, char **rest)
{
	size_t	len;

	len = ft_strlen(kw);
	if (ft_strncmp(s, kw, len) != 0 || (s[len] && !is_blank(s[len])))
		return (false);
	*rest = s + len;
	return (true);
}

/**
 * Statements that are valid .obj but have nothing to render here:
 * object / group names, lines, points and free-form parameter vertices.
 */
bool	is_ignored_statement(char *s)
{
	static const char	*ignored[] = {"o", "g", "l", "p", "vp", "mg", NULL};
	char				*rest;
	int					i;

	i = 0;
	while (ignored[i])
	{
		if (is_keyword(s, ignored[i], &rest))
			return (true);
		i++;
	}
	return (false);
}

/**
 * Cuts the next whitespace separated token out of *s (in place) and moves
 * *s past it. Returns NULL when there are no tokens left.
 */
char	*next_token(char **s)
{
	char	*start;
	char	*p;

	start = skip_ws(*s);
	if (*start == '\0')
		return (NULL);
	p = start;
	while (*p && !is_blank(*p))
		p++;
	if (*p)
	{
		*p = '\0';
		p++;
	}
	*s = p;
	return (start);
}
