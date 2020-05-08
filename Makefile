SRC = main.c render.c funcs.c intersect.c normal.c camera.c keys.c gpu.c init.c texture.c
SRC += gnl/get_next_line.c gnl/libft/libft.a
INC = -I gnl/ -I /usr/X11/include

FLAGS = -g -Ofast

# #all:
# 	#gcc $(FLAGS) $(SRC) $(INC) -lm -lpthread -lOpenCL -lmlx -lXext -lX11 -I minilibx/ -L minilibx/

 all:
	gcc $(FLAGS) $(SRC) $(INC) -lm -lpthread -lOpenCL -L/usr/X11/lib /usr/X11/lib/libmlx.a -lXext -lX11 -lbsd
