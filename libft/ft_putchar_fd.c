/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:14:05 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/14 15:59:54 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
/* 
#include <stdio.h>
#include <fcntl.h>
#define BUFF_SIZE 1024
int main()
{
	char buf[BUFF_SIZE];
	int fd = open("ft_putchar_fd.txt", O_RDWR | O_CREAT | O_APPEND, 0777);
	ft_putchar_fd('2', fd);
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