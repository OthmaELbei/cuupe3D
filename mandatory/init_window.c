/* khasni n hot fix map */

#include "../include/cub3d.h"

// Helper function to get a pixel color from a texture image
// This is crucial for sampling individual pixels from loaded textures
unsigned int get_pixel_color(t_img *img, int x, int y)
{
	char	*pixel_addr;

	// Check bounds to prevent reading outside the image data
	// printf("img->width: %d | img->height: %d\n", img->width, img->height);
	if (x < 0 || x >= img->width || y < 0 || y >= img->height)
		return (0); // Return black or transparent for out-of-bounds access

	// Calculate the memory address of the pixel
	pixel_addr = img->addr + (y * img->line_len + x * (img->bpp / 8));
	return (*(unsigned int *)pixel_addr);
}

void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char *dst;

	// Check if coordinates are within the window bounds
	if (x < 0 || x >= data->WIDTH || y < 0 || y >= data->HEIGHT)
		return ;
	// Calculate the memory address of the pixel
	dst = data->img.addr + (y * data->img.line_len + x * (data->img.bpp / 8));
	// Write the color to the pixel address
	*(unsigned int *)dst = color;
}

// Draws a wall tile at the given tile coordinates.
void	draw_wall(t_data *data, int tile_x, int tile_y)
{
	int				x;
	int				y;
	unsigned int	color;

	y = 0;
	while (y < TILE_SIZE)
	{
		x = 0;
		while (x < TILE_SIZE)
		{
			// Get pixel color from the loaded wall texture
            // We assume TILE_SIZE matches the texture's width/height for 1:1 mapping
            color = get_pixel_color(&data->wall, x, y);
			// Put this pixel onto the main image buffer at the correct screen position
			my_mlx_pixel_put(data, tile_x + x, tile_y + y, color); // Wall color
			x++;
		}
		y++;
	}
}

// Draws a background tile (floor) at the given tile coordinates.
void draw_background(t_data *data, int tile_x, int tile_y)
{
	int	x;
	int	y;

	y = 0;
	while (y < TILE_SIZE)
	{
		x = 0;
		while (x < TILE_SIZE)
		{
			my_mlx_pixel_put(data, ((tile_x + x)), ((tile_y + y)), WHITE); // Floor color
			x++;
		}
		y++;
	}
}

void draw(t_data *data, int i, int j)
{
	int tile_x;
	int tile_y;
	
	tile_x = j * TILE_SIZE;
	tile_y = i * TILE_SIZE;

	draw_background(data, tile_x, tile_y);

	if (data->map[i][j] == '1')
	{
		draw_wall(data, tile_x, tile_y);
	}
	// Player 'N' character is used for initial position, but not for drawing here.
}

void draw_line(t_data *data, int x0, int y0, float angle, int length)
{
	int i;
	int x;
	int y;

	i = 0;
	while (i < length)
	{
		// Calculate the next point on the line using trigonometry
		x = x0 + cos(angle) * i;
		y = y0 + sin(angle) * i;
		// Draw the pixel if it's within bounds
		if (x >= 0 && x < data->WIDTH && y >= 0 && y < data->HEIGHT)
			my_mlx_pixel_put(data, x, y, RED); // Player direction line color
		i++;
	}
}

// Draws the player's representation (the red line) on the map.
void draw_player(t_data *data)
{
	// Convert player's world coordinates to screen coordinates for drawing
	data->player.screen_x = (int)(data->player.x * TILE_SIZE);
	data->player.screen_y = (int)(data->player.y * TILE_SIZE);

	// Draw player direction line
	draw_line(data,
			  data->player.screen_x,
			  data->player.screen_y,
			  data->player.rotationangle,
			  TILE_SIZE / 2);
}

// Renders the entire 2D map view.
void render_map(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (data->map[i])
	{
		j = 0;
		while (data->map[i][j])
		{
			draw(data, i, j); // Draw each tile
			j++;
		}
		i++;
	}
	// The player is drawn separately in game_loop after rendering the map.
}

int key_press(int keycode, t_data *data)
{
	if (keycode == 13 || keycode == 126)
		data->player.walkdirection = 1; // up
	else if (keycode == 1 || keycode == 125)
		data->player.walkdirection = -1; // down
	else if (keycode == 124)
		data->player.turndirection = 1; // ->
	else if (keycode == 123)
		data->player.turndirection = -1; // <-
	else if (keycode == 53)		// ESC
		(mlx_destroy_window(data->mlx_ptr, data->win_ptr), exit(0));
	return (0);
}

// Handles key release events. Resets player's walk and turn directions.
int key_release(int keycode, t_data *data)
{
	if ((keycode == 13 || keycode == 126) && data->player.walkdirection == 1)
		data->player.walkdirection = 0;
	else if ((keycode == 1 || keycode == 125) && data->player.walkdirection == -1)
		data->player.walkdirection = 0;
	else if (keycode == 124 && data->player.turndirection == 1)
		data->player.turndirection = 0;
	else if (keycode == 123 && data->player.turndirection == -1)
		data->player.turndirection = 0;
	return (0);
}

int close_window(t_data *data)
{
	mlx_destroy_image(data->mlx_ptr, data->img.img_ptr);
	mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	exit(0);
}

void handle_event(t_data *data)
{
	mlx_hook(data->win_ptr, 2, 1L << 0, key_press, data);
	mlx_hook(data->win_ptr, 3, 1L << 1, key_release, data);
	mlx_hook(data->win_ptr, 17, 0, close_window, data);
}

void cleanup(t_data *data)
{
	if (data->wall.img_ptr)
		mlx_destroy_image(data->mlx_ptr, data->wall.img_ptr);
	if (data->img.img_ptr)
		mlx_destroy_image(data->mlx_ptr, data->img.img_ptr);
	if (data->win_ptr)
		mlx_destroy_window(data->mlx_ptr, data->win_ptr);
}

// Calculates the total width of the window based on the widest map row.
int calculate_width(char **map)
{
	int max_width = 0;
	int current_width;
	int i = 0;

	while (map[i])
	{
		current_width = 0;
		while (map[i][current_width])
			current_width++;
		if (current_width > max_width)
			max_width = current_width;
		i++;
	}
	return (max_width * TILE_SIZE); // Return width in pixels
}

// Calculates the total height of the window based on the number of map rows.
int calculate_height(char **map)
{
	int height;

	height = 0;
	while (map[height])
		height++;
	return (height * TILE_SIZE); // Return height in pixels
}


// Moves the player based on input and handles basic collision detection.
void move_player(t_data *data)
{
	float	move_step;
	float	new_x;
	float	new_y;
	int		x;
	int		y;

	// Update player rotation based on turn direction and speed
	data->player.rotationangle += data->player.turndirection * data->player.turnspeed;

	// Calculate potential movement vector
	move_step = data->player.walkdirection * data->player.walkspeed;
	new_x = data->player.x + cos(data->player.rotationangle) * move_step;
	new_y = data->player.y + sin(data->player.rotationangle) * move_step;

	// Basic collision detection: Check if the new position is a wall
	// Convert world coordinates to map grid coordinates for checking
	x = (int)new_x;
	y = (int)new_y;

	// Ensure x and y are within map bounds
	if (y >= 0 && y < data->HEIGHT &&
		x >= 0 && x < calculate_width(data->map))
	{
		// Only update position if the new tile is not a wall ('1')
		if (data->map[y][x] != '1')
		{
			data->player.x = new_x;
			data->player.y = new_y;
		}
	}
}

// Initializes the t_data and t_player structures, and loads textures.
void	init_struct(t_data *data, char **map)
{
	int	i;
	int	j;

	data->WIDTH = calculate_width(map);
	data->HEIGHT = calculate_height(map);
	data->player.x = 0;
	data->player.y = 0;
	data->player.screen_x = 0;
	data->player.screen_y = 0;
	data->player.radius = 3;
	data->player.turndirection = 0;
	data->player.walkdirection = 0;
	data->player.rotationangle = 0; // Will be set based on player char
	data->player.walkspeed = 0.05f;
	data->player.turnspeed = 0.05f;

	// Find player's initial position and set rotation angle
	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == 'N' || map[i][j] == 'S' || map[i][j] == 'E' || map[i][j] == 'W')
			{
				// Player position centered within the tile
				data->player.x = j + 0.5f;
				data->player.y = i + 0.5f;
				if (map[i][j] == 'N')
					data->player.rotationangle = 3 * M_PI / 2; // Facing North (up)
				else if (map[i][j] == 'S')
					data->player.rotationangle = M_PI / 2;    // Facing South (down)
				else if (map[i][j] == 'E')
					data->player.rotationangle = 0;           // Facing East (right)
				else if (map[i][j] == 'W')
					data->player.rotationangle = M_PI;        // Facing West (left)
				break; // Found player, no need to continue map scan for player pos
			}
			j++;
		}
		i++;
	}

	// Load the wall.xpm texture for 2D map rendering
	data->wall.img_ptr = mlx_xpm_file_to_image(data->mlx_ptr, "texture/wall.xpm", &data->wall.width, &data->wall.height);
	if (!data->wall.img_ptr)
	{
		ft_putstr_fd("Error: Failed to load wall.xpm texture for 2D map. Check path: ", STDERR_FILENO);
		ft_putstr_fd("texture/wall.xpm", STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		cleanup(data);
		exit(1);
	}
	data->wall.addr = mlx_get_data_addr(data->wall.img_ptr, &data->wall.bpp, &data->wall.line_len, &data->wall.endian);
}

// The main game loop, called repeatedly by MLX.
int game_loop(t_data *data)
{
	// Update player position and rotation before rendering
	move_player(data);

	render_map(data);  // Draw the static map elements
	draw_player(data); // Draw the player on top of the map

	// Put the updated image to the window
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.img_ptr, 0, 0);
	return (0);
}

void init_window(char **map)
{
	t_data data;

	ft_memset(&data, 0, sizeof(t_data));
	data.map = map;
	data.mlx_ptr = mlx_init();
	if (!data.mlx_ptr)
		exit(1);
	init_struct(&data, map);
	data.win_ptr = mlx_new_window(data.mlx_ptr, data.WIDTH, data.HEIGHT, "CUB3D");
	if (!data.win_ptr)
		return (cleanup(&data), exit(1));
	data.img.img_ptr = mlx_new_image(data.mlx_ptr, data.WIDTH, data.HEIGHT);
	if (!data.img.img_ptr)
		return (cleanup(&data), exit(1));
	data.img.addr = mlx_get_data_addr(data.img.img_ptr, &data.img.bpp,
									&data.img.line_len, &data.img.endian);
	if (!data.img.addr)
		return (cleanup(&data), exit(1));
	handle_event(&data);
	mlx_loop_hook(data.mlx_ptr, game_loop, &data);
	mlx_loop(data.mlx_ptr);
}
