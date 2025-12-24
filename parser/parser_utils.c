#include "../minirt.h"

int	count_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens && tokens[i])
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
