/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sidrissi <sidrissi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:34:11 by sidrissi          #+#    #+#             */
/*   Updated: 2025/07/22 11:36:45 by sidrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**/
#include "../include/cub3d.h"

int check_extention(char *map)
{
	char *ext;
	int i;
	int j;

	ext = ".cub";
	i = ft_strlen(map) - 1;
	j = ft_strlen(ext) - 1;
	while (j > -1)
	{
		if (map[i] != ext[j] && j > -1)
			return (1);
		else if (map[i] == ext[j])
			((i--), (j--));
	}
	return (0);
}

char *get_word(char *line, int count)
{
	char *str;
	int i;
	int j;

	i = 0;
	while (line[i] == ' ')
		i++;
	str = malloc(sizeof(char) * 4);
	if (NULL == str)
		return (NULL);
	j = 0;
	while (j < count && line[i] != '\n')
	{
		if (line[i] == '\t')
			return (printf("there are tab\n"), free(str), NULL);
		str[j++] = line[i++];
	}
	str[j] = '\0';
	return (str);
}

int check_element(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			return (1337);
		i++;
	}
	return (0);
}

int check_res(char **res)
{
	int i;

	if (ft_count_2d_array(res) != 3)
		return (1);
	i = 0;
	while (res[i] != NULL)
	{
		if (check_element(res[i]))
			return (1337);
		i++;
	}
	return (0);
}

int	check_number(char *color, char *path, t_vmap *v_map)
{
	char **res;
	int r;
	int g;
	int b;

	res = NULL;
	res = ft_split(path, ", ");
	if (NULL == res)
		return (1337);
	if (check_res(res))
		return (free_2d_array(res), 1);
	r = ft_atoi(res[0]);
	g = ft_atoi(res[1]);
	b = ft_atoi(res[2]);
	if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255))
		return (free_2d_array(res), 1337);
	if (!ft_strcmp(color, "F "))
		v_map->n_floor = (r << 16 | g << 8 | b << 0);
	else if (!ft_strcmp(color, "C "))
		v_map->n_ceil = (r << 16 | g << 8 | b << 0);;

	return (free_2d_array(res), 0);
}

int	check_comma(char *path)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (path[i])
	{
		if (path[i] == ',')
			count++;
		if (path[i] == ',' && path[i + 1] == ',')
			return (1337);
		i++;
	}
	if (count !=  2)
		return (1337);
	return (0);
}

int	check_v_color(char *color, char *path, t_vmap *v_map)
{
	if (!ft_strcmp(color, "F ") || !ft_strcmp(color, "C "))
	{
		if (check_comma(path) || check_number(color, path, v_map))
			return (1337);
	}
	return (0);
}

int	check_v_path(char *str, char *path)
{
	int	fd;

	if (!ft_strcmp(str, "NO ") || !ft_strcmp(str, "SO ")
		|| !ft_strcmp(str, "WE ") || !ft_strcmp(str, "EA "))
	{
		if (((fd = open(path, O_RDONLY)) < 0))
			return (ft_putstr_fd("path not valid\n", 2), 1337);
	}
	return (0);
}

int index_start(char *line)
{
	int i;

	i = 0;
	while (line[i] && (line[i] != ' '))
		i++;
	while (line[i] && (line[i] == ' '))
		i++;
	return (i);
}

int index_end(char *line)
{
	int start;

	start = index_start(line);
	while (line[start])
		start++;
	return (start);
}

int check_dublicate(t_vmap *v_map, char *line, int *count)
{
	char	*path;

	path = NULL;
	if (!ft_strcmp(line, "\0"))
		;
	else if (!ft_strcmp(v_map->str, "NO ") || !ft_strcmp(v_map->str, "SO ")
		|| !ft_strcmp(v_map->str, "WE ") || !ft_strcmp(v_map->str, "EA ")
		|| ft_strcmp(v_map->color, "F ") || ft_strcmp(v_map->color, "C "))
	{
		path = ft_substr(line, index_start(line), index_end(line));
		if (NULL == path)
			return (ft_putstr_fd("path is NULL\n", 2), 1337);
		if (!ft_strcmp(v_map->str, "NO ") && !v_map->north && ft_strcmp(path, "\0"))
		{
			v_map->north = path;
			(*count)++;
		}
		else if (!ft_strcmp(v_map->str, "SO ") && !v_map->south && ft_strcmp(path, "\0"))
		{
			v_map->south = path;
			(*count)++;
		}
		else if (!ft_strcmp(v_map->str, "WE ") && !v_map->west && ft_strcmp(path, "\0"))
		{
			v_map->west = path;
			(*count)++;
		}
		else if (!ft_strcmp(v_map->str, "EA ") && !v_map->east && ft_strcmp(path, "\0"))
		{
			v_map->east = path;
			(*count)++;
		}
		else if (!ft_strcmp(v_map->color, "F ") && !v_map->floor && ft_strcmp(path, "\0"))
		{
			v_map->floor = path;
			(*count)++;
		}
		else if (!ft_strcmp(v_map->color, "C ") && !v_map->ceil && ft_strcmp(path, "\0"))
		{
			v_map->ceil = path;
			(*count)++;
		}
		else
			return (ft_putstr_fd("path not correct\n", 2), free(path), 1337);
	}
	else
		return (1337);
	if (check_v_path(v_map->str, path) || check_v_color(v_map->color, path, v_map))
		return (free(path), 1337);
	return (free(path), (*count));
}

int check_direction(t_vmap *v_map, char *line, int *count)
{
	if (check_dublicate(v_map, line, count) == 1337)
		return (1337);
	return (*(count));
}

int check_line(t_vmap *v_map, char *line, int *count)
{
	int		check;
	char	*new_line;

	new_line = ft_strtrim(line, " ");
	v_map->str = get_word(line, 3);
	v_map->color = get_word(line, 2);
	if ((NULL == v_map->str) || (NULL == v_map->color))
		return (printf("error: get_word\n"), 1337);
	check = check_direction(v_map, new_line, count);
	free(new_line);
	if (check == 1337)
		return (printf("error: 1337\n"), free(v_map->str),free(v_map->color), 1337);
	return (free(v_map->str), free(v_map->color), *(count));
}

int	check_first_part(int fd, int count)
{
	t_vmap	v_map;
	char	*line;
	int		n_count;
	char	*new_line;

	ft_memset(&v_map, 0, sizeof(v_map));
	fd = open("map/map.cub", O_RDONLY);
	if (fd < 0)
		return (ft_putstr_fd("fd error\n", STDERR_FILENO), 1337);
	while ((line = get_next_line(fd)) != NULL)
	{
		if (ft_strcmp(line, "\n"))
		{
			new_line =  ft_strtrim(line, "\n");
			n_count = check_line(&v_map, new_line, &count);
			if (n_count == 1337)
				return (free(line), free(new_line), 1337);
			(free(line), free(new_line));
		}
		else
			free(line);
	}
	if (n_count != 6)
		return (free(line), 1337);
	return (close(fd), free(line), 0);
}

int ft_parsing(char *map)
{
	int		fd;
	int		count;

	fd = 0;
	count = 0;
	if (check_extention(map))
		return (printf("error: invalid extension\n"), 1);
	if (check_first_part(fd, count))
		return (printf("error: map header invalid\n"), 1); // remove it when finish
	return (0);
}

/**/
