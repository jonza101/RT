# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/16 17:44:46 by zjeyne-l          #+#    #+#              #
#    Updated: 2020/02/03 17:00:02 by zjeyne-l         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = main.c render.c funcs.c intersect.c normal.c camera.c keys.c gpu.c init.c texture.c
SRC += gnl/get_next_line.c gnl/libft/libft.a

INC = -I gnl/

FLAGS = -g -Ofast

#all:
	#gcc $(FLAGS) $(SRC) $(INC) -lm -lpthread -lOpenCL -lmlx -lXext -lX11 -I minilibx/ -L minilibx/

all:
	gcc $(FLAGS) $(SRC) $(INC) -lm -lpthread -lmlx -framework OpenGL -framework AppKit -framework OpenCL
