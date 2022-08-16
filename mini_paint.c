#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define NONE		0 	//원이아님
#define	OUTLINE 	1	//원의 외곽
#define INNER		2	//원의 내부

#define ERROR		1
#define SUCCESS		0

typedef struct s_board
{
	int	w;
	int h;
	char color;
	char *map;
}	t_board;

typedef struct t_circle
{
	char type;
	float	x;
	float	y;
	float	r;
	char	color;
}	t_circle;

void	ft_putstr(char *str)
{
	while (*str)
	{
		write(1, str, 1);
		++str;
	}
}

//x, y좌표는 원의 NONE, OUTLINE, INNER 중 무엇인가?
int	is_circle(t_circle *c, float x, float y)
{
	// x y와 원의 중점의 거리
	float distance = sqrtf((c-> x - x) * (c->x - x) + (c-> y - y) * (c->y - y));
	if (distance > c->r)
		return NONE;
	if (c->r - distance < 1.0000f)
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

int init_circle(FILE *f, t_board *b)
{
	t_circle c;
	int		check;
	while (1)
	{
		check = fscanf(f, "%c %f %f %f %c\n", &c.type, &c.x, &c.y, &c.r ,&c.color);
		if (check != 5)
			break ;
		if (c.type != 'c' && c.type != 'C')
			return ERROR;
		if (c.r <= 0.00000f )
			return ERROR;
		for (int y = 0; y < b->h; y++)
		{
			for (int x = 0; x < b->w; x++)
			{
				if (c.type == 'c' && is_circle(&c, (float)x, (float)y) == OUTLINE)
					b->map[b->w * y + x] = c.color;
				else if (c.type == 'C' && is_circle(&c, (float)x, (float)y) != NONE)
					b->map[b->w * y + x] = c.color;
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
	if (init_circle(f, &b) == ERROR)
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
