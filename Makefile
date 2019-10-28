# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/16 17:44:46 by zjeyne-l          #+#    #+#              #
#    Updated: 2019/10/29 01:03:02 by zjeyne-l         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = main.c render.c funcs.c intersect.c normal.c thread.c camera.c keys.c
SRC += gnl/libft/libft.a

INC = -I gnl/

FLAGS = -g -Ofast

#all:
	#gcc $(FLAGS) $(SRC) $(INC) -lm -lpthread -lmlx -lXext -lX11 -I minilibx/ -L minilibx/

all:
	gcc $(FLAGS) $(SRC) $(INC) -lm -lpthread -lmlx -framework OpenGL -framework AppKit
