/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbied <oelbied@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 10:08:33 by sidrissi          #+#    #+#             */
/*   Updated: 2025/08/01 15:11:49 by oelbied          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

char *_join_lines()
{
	int fd;
	char *line;
	char *temp;
	char *res;

	res = NULL;
	fd = open("map/map.cub", O_RDONLY, 0777);
	if (fd < 0)
		return (ft_putstr_fd("fd error\n", STDERR_FILENO), NULL);
	temp = NULL;
	while ((line = get_next_line(fd)) != NULL)
	{
		res = ft_strjoin(temp, line);
		if (!res)
			return (ft_putstr_fd("res fail\n", STDERR_FILENO),
					free(temp), free(line), NULL);
		free(line);
		free(temp);
		temp = res;
	}
	return (close(fd), res);
}
int ft_start()
{
	char *join_lines;
	char **_2darray;

	_2darray = NULL;
	join_lines = _join_lines();
	if (!join_lines)
		return (printf("map is empty\n"), 1);
	_2darray = ft_split(join_lines, "\n");
	if (!_2darray)
		return (free(join_lines), 1);
	init_window(_2darray);
	return (free_2d_array(_2darray), free(join_lines), 0);
}

void f()
{
	system("leaks Cub3d");
}

int main(int ac, char **av)
{
	(void)ac, (void)av;
	// atexit(f);
	// if ((!av[1]) || ft_parsing(av[1]))
	// 	return (ft_putstr_fd("map not valid!\n", STDERR_FILENO), 1);
	// if(!men())
	   return 0;
	printf("map valid!\n");
	if (ft_start())
		return (1);
	return (0);
}