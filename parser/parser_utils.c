#include "../minirt.h"

static int	count_words(char const *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i]!= c)
			i++;
	}
	return (count);
}

static char	*get_next_word(char const **s, char c)
{
	int		len;
	char	*word;
	int		i;

	while (**s == c)
		(*s)++;
	len = 0;
	while ((*s)[len] && (*s)[len]!= c)
		len++;
	word = (char *)safe_malloc(sizeof(char) * (len + 1));
	i = 0;
	while (i < len)
	{
		word[i] = (*s)[i];
		i++;
	}
	word[i] = '\0';
	*s += len;
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		word_count;
	int		i;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	result = (char **)safe_malloc(sizeof(char *) * (word_count + 1));
	i = 0;
	while (i < word_count)
	{
		result[i] = get_next_word(&s, c);
		i++;
	}
	result[i] = NULL;
	return (result);
}

int	count_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		i++;
	return (i);
}

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}