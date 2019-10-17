# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zjeyne-l <zjeyne-l@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/16 17:44:46 by zjeyne-l          #+#    #+#              #
#    Updated: 2019/10/17 14:34:40 by zjeyne-l         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = main.c render.c funcs.c
SRC += gnl/libft/libft.a

INC = -I gnl/

all:
	gcc -g -Ofast $(SRC) $(INC) -lmlx -framework OpenGL -framework AppKit
