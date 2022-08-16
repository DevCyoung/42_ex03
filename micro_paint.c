#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define NONE		0 	
#define	OUTLINE 	1	
#define INNER		2	

#define ERROR		1
#define SUCCESS		0

typedef struct s_board
{
	int	w;                 
	int h;
	char color;
	char *map;
}	t_board;

typedef struct t_rect
{
	char type;
	float	x;
	float	y;
	float	w;
	float	h;
	char	color;
}	t_rect;

void	ft_putstr(char *str)
{
	while (*str)
	{
		write(1, str, 1);
		++str;
	}
}

int	is_rect(t_rect *r, float x, float y)
{
	if (x < r->x)
		return NONE;
	if (x > r->x + r->w)
		return NONE;
	if (y < r->y)
		return NONE;
	if (y > r->y + r->h)
		return NONE;

	if (x - r->x < 1.0000f)
		return OUTLINE;	
	if (r->x + r->w - x < 1.0000f)
		return OUTLINE;
	if (y - r->y < 1.000f)
		return OUTLINE;
	if (r->y + r->h - y < 1.0000f)
		return OUTLINE;
	return INNER;
}

int init_board(FILE *f, t_board *b)
{
	int check = fscanf(f, "%d %d %c\n", &b->w, &b->h, &b->color);
	if (check != 3)
		return ERROR;
	if (b->w <= 0 || b->w > 300 || b->h <= 0 || b->h > 300)
		return ERROR;
	b->map = (char *)malloc(sizeof(char) * (b->w * b->h));
	if (b->map == NULL)
		return ERROR;
	for (int i = 0; i < b->w * b->h; i++)
		b->map[i] = b->color;
	return SUCCESS;
}

int init_rect(FILE *f, t_board *b)
{
	t_rect r;
	int		check;
	while (1)
	{
		check = fscanf(f, "%c %f %f %f %f %c\n", &r.type, &r.x, &r.y, &r.w , &r.h ,&r.color);
		if (check != 6)
			break ;
		if (r.type != 'r' && r.type != 'R')
			return ERROR;
		if (r.w <= 0 || r.h <= 0)
			return ERROR;
		for (int y = 0; y < b->h; y++)
		{
			for (int x = 0; x < b->w; x++)
			{
				if (r.type == 'r' && is_rect(&r, (float)x, (float)y) == OUTLINE)
					b->map[b->w * y + x] = r.color;
				else if (r.type == 'R' && is_rect(&r, (float)x, (float)y) != NONE)
					b->map[b->w * y + x] = r.color;
			}
		}
	}
	if (check != -1)
		return ERROR;
	return SUCCESS;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_putstr("Error: argument\n");
		return ERROR;
	}
	FILE *f = fopen(argv[1], "r");
	if (f == NULL)
	{
		ft_putstr("Error: Operation file corrupted");
		return ERROR;
	}
	t_board b;
	b.map = NULL;
	if (init_board(f, &b) == ERROR)
	{
		fclose(f);
		ft_putstr("Error: Operation file corrupted");
		return ERROR;
	}
	if (init_rect(f, &b) == ERROR)
	{
		fclose(f);
		free(b.map);
		ft_putstr("Error: Operation file corrupted");
		return ERROR;
	}
	for (int y = 0; y < b.h; y++)
	{
		for (int x = 0; x < b.w; x++)
		{
			write(1, &b.map[b.w * y + x], 1);
		}
		ft_putstr("\n");
	}
	return SUCCESS;
}
