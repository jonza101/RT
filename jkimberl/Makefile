# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/16 17:44:46 by zjeyne-l          #+#    #+#              #
#    Updated: 2019/10/23 18:14:41 by zjeyne-l         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = main.c render.c funcs.c intersect.c normal.c
SRC += gnl/libft/libft.a

INC = -I gnl/

FLAGS = -g -Ofast

all:
	gcc $(FLAGS) $(SRC) $(INC) -lmlx -framework OpenGL -framework AppKit
