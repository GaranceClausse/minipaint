# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <string.h>

typedef struct s_zone 
{
	int width;
	int height;
	char bc;
}	t_zone;

typedef struct s_draw
{
	char type;
	char c;
	float rad;
	float x;
	float y;
}	t_draw;

int ft_strlen(char *str)
{
	int i;
	i = 0;
	while(str[i])
	i++;
	return i;
}

int ft_putstr(char *str)
{
	int i;
	i = 0;
	write(1, str, ft_strlen(str));
	return 1;
}

void ft_puttab(char **tab)
{
	int i;
	i = 0;
	while(tab[i])
	{
		ft_putstr(tab[i]);
		write(1, "\n", 1);
		++i;
	}
}

int		ft_freetab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		++i;
	}
	free(tab);
	return (1);
}

static int set_paint(char **paint, t_zone zone)
{
	int i;
	i = 0;
	if(!paint)
		return 1;
	while ( i < zone.height)
	{
		if(!(paint[i] = calloc(1, zone.width + 1)))
			return 1;
		memset(paint[i], zone.bc, zone.width);
		++i;
	}
	paint[i] = 0;
	return 0;
}

static int isincircle(float xa, float ya, t_draw draw)
{
	float d;

	d = sqrtf((xa - draw.x) * (xa - draw.x) + (ya - draw.y) * (ya - draw.y));
	if (d <= draw.rad)
	{
		if ((draw.rad - d) < 1.0)
			return 1;
		return 0;
	}
	return -1;
}

static int drawcircle(t_draw draw, t_zone zone, char **paint)
{
	int i;
	int j;
	int k;

	i = 0;
	while(i < zone.height)
	{
		j = 0;
		while(j < zone.width)
		{
			k = isincircle(j, i, draw);
			if ( k == 1 || (draw.type == 'C' && k == 0))
			paint[i][j] = draw.c;
			++j;
		}
		++i;
	}
	return 0;
}

static int drawing(FILE *stream, t_zone zone)
{
	int ret;
	t_draw draw;
	char n;
	char **paint;

	paint = malloc(sizeof(char *) * (zone.height + 1));
	if (set_paint(paint, zone) == 1)
		return ft_freetab(paint);
	draw.type = 0;
	while((ret = fscanf(stream, "%c%c %f %f %f %c", &n, &draw.type, &draw.x, 
	&draw.y, &draw.rad, &draw.c)) == 6)
	{
		if ((draw.type != 'c' && draw.type != 'C') || n != '\n' 
		|| draw.rad <= 0.0 || draw.c == 0 || draw.c == '\n')
			return(ft_freetab(paint));
		drawcircle(draw, zone, paint);
		draw.type = 0;
	}
	if (draw.type != '\0' && ret != -1)
		return(ft_freetab(paint));
	ft_puttab(paint);
	ft_freetab(paint);
	return 0;
}

static int get_zone(FILE *stream, t_zone *zone)
{
	size_t ret;
	ret = fscanf(stream, "%d %d %c", &zone->width, &zone->height, &zone->bc);
	if (ret != 3)
		return 1;
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->width > 300 || zone->bc == '\n')
		return 1;
	return 0;
}

int main(int ac, char **av)
{
	FILE *stream;
	t_zone zone;

	if (ac != 2)
		return (ft_putstr("Error: argument\n"));
	if (!(stream = fopen(av[1], "r")))
		return (ft_putstr("Error: Operation file corrupted\n"));
	if (get_zone(stream, &zone) == 1)
		return (ft_putstr("Error: Operation file corrupted\n"));
	if (drawing(stream, zone) == 1)
		return (ft_putstr("Error: Operation file corrupted\n"));
	fclose(stream);
	return (0);
}

