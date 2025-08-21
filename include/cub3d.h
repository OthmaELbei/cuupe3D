/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbied <oelbied@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:21:16 by sidrissi          #+#    #+#             */
/*   Updated: 2025/07/28 15:37:25 by oelbied          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H

# include "mlx.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MLX_ERROR 1

#define GREEN 0x00FF00
#define BLACK 0x4d4d4d
#define WHITE 0xFFFFFF
#define RED 0XFF0000
# define TILE_SIZE 32

#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


/*============= Parsing ===================*/

typedef struct	s_vmap
{
	char	*path;
	char	*str;
	char	*color;
	char	*north;
	char	*south;
	char	*west;
	char	*east;
	char	*floor;
	char	*ceil;
	int		n_floor;
	int		n_ceil;
}	t_vmap;

/*============= Parsing ===================*/


/*============= Execution ===================*/
typedef struct	s_img
{
	void	*img_ptr;
	char	*addr;
	int 	bpp;
	int 	line_len;
	int 	endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_player {
	float	x;
	float	y;
	int		screen_x;
	int		screen_y;
	float	radius;
	float	turndirection;
	float	walkdirection;
	float	rotationangle;
	float	walkspeed;
	float	turnspeed;
}	t_player;


typedef struct	s_data
{
	void		*win_ptr;
	void		*mlx_ptr;
	int			WIDTH;
	int			HEIGHT;
	char		**map;
	t_img		img;
	t_img		wall;
	t_vmap		v_map;
	t_player	player;
}	t_data;


/*============= Execution ===================*/
int men();

/*=============get_next_line (Start)==========================*/
char	*get_next_line(int fd);

char	*read_file(int fd, char *left_over, char *buffer);
char	*extr_line(char *line);
char	*ft_left_over(char *line);
/*=============get_next_line (End)============================*/

/*=============libft (Start)============================*/
void	ft_putstr_fd(char *s, int fd);
int		ft_strlen(const char *s);
char	*ft_strdup(char *s1);
char	*ft_substr(char *s, unsigned int start, size_t len);
char	*ft_strjoin(char *s1, char *s2);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
char	*ft_strchr(char *s, char c);
char	**ft_split(char const *s, char *sep);
void	free_2d_array(char **arr);
int		ft_count_2d_array(char **arr);
void	*ft_memset(void *b, int c, size_t len);
int		ft_strcmp(char *s1, char *s2);
long	ft_atoi(char *str);
int		ft_isdigit(int c);
char	*ft_strtrim(char *s1, char *set);
t_vmap	*ft_lstnew(char *path);
void	ft_lstadd_back(t_vmap **lst, t_vmap *new);
t_vmap	*ft_lstlast(t_vmap *lst);
/*=============libft (End)============================*/

/*=============Parsing (End)============================*/
int		ft_parsing(char *map);
int		check_extention(char *map);
int		check_first_part(int fd, int count);
/*=============Parsing (Start)============================*/



/*=============start_drawing (Start)============================*/
void	init_window(char **map);
/*=============start_drawing (End)============================*/
#endif
