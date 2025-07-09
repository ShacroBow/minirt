/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmashkoo <kmashkoo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 17:53:09 by kmashkoo          #+#    #+#             */
/*   Updated: 2024/09/14 16:01:20 by kmashkoo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putnbr_fd(int n, int fd)
{
	int	digit;

	if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
		ft_putnbr_fd(n, fd);
	}
	else
	{
		if (n > 9)
		{
			ft_putnbr_fd(n / 10, fd);
			ft_putnbr_fd(n % 10, fd);
		}
		else
		{
			digit = n + 48;
			write(fd, &digit, 1);
		}
	}
}
/* 
#include <stdio.h>
#include <fcntl.h>
#define BUFF_SIZE 1024
int main()
{
	char buf[BUFF_SIZE];
	int fd = open("ft_putnbr_fd.txt", O_RDWR | O_CREAT | O_APPEND, 0777);
	ft_putnbr_fd(123435, fd);
	write(fd, "\n", 1);
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