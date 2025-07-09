/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 16:57:03 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/14 16:02:40 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static size_t	ft_strlen_putstr(const char *str)
{
	size_t	i;

	i = 0;
	while (*str != '\0')
	{
		i++;
		str++;
	}
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = ft_strlen_putstr(s);
	write(fd, s, i);
}
/* 
#include <stdio.h>
#include <fcntl.h>
#define BUFF_SIZE 1024
int main()
{
	char buf[BUFF_SIZE];
	int fd = open("ft_putstr_fd.txt", O_RDWR | O_CREAT | O_APPEND, 0777);
	ft_putstr_fd("heyo", fd);
	printf ("fd: %d\n", fd);

	if (fd != -1)
	{
		lseek(fd, 0, SEEK_SET); 
		ssize_t readsize = read(fd, buf, BUFF_SIZE - 1);
		buf[readsize] = '\0';
		printf("file content: '%s", buf);
		while (readsize > 0)
		{
			readsize = read(fd, buf, BUFF_SIZE - 1);
			buf[readsize] = '\0';
			printf("%s'\n", buf);
		}
	}
	return 0;
} */