# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/16 17:44:46 by zjeyne-l          #+#    #+#              #
#    Updated: 2019/10/26 16:28:05 by zjeyne-l         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = main.c render.c funcs.c intersect.c normal.c
SRC += gnl/libft/libft.a

INC = -I gnl/

FLAGS = -g -Ofast

all:
	gcc $(FLAGS) $(SRC) $(INC)  -lmlx -lXext -lX11 -I minilibx/ -L minilibx/ -lm

#all:
	#gcc $(FLAGS) $(SRC) $(INC) -lmlx -framework OpenGL -framework AppKit
