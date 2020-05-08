NAME		=	RT

CC			=	gcc
FLAGS		=	-Wall -Wextra -Werror -g3 -Ofast

DELTA		=	$$(echo "$$(tput cols)-47"|bc)

LIBFT_DIR	=	libft/
LIBFT_LIB	=	$(LIBFT_DIR)libft.a
LIBFT_INC	=	$(LIBFT_DIR)/inc/

LIBS		=	-lm -lpthread -lOpenCL -L/usr/X11/lib /usr/X11/lib/libmlx.a -lXext -lX11 -lbsd

SRC_DIR		=	Sources/
INC_DIR		=	Includes/
OBJ_DIR		=	.obj/

SRC_BASE	=	main.c		\
				render.c	\
				funcs.c		\
				intersect.c	\
				normal.c	\
				camera.c	\
				keys.c		\
				gpu.c		\
				init.c		\
				texture.c	\

SRCS		=	$(addprefix $(SRC_DIR), $(SRC_BASE))
OBJS		=	$(addprefix $(OBJ_DIR), $(SRC_BASE:.c=.o))
NB			=	$(words $(SRC_BASE))
INDEX		=	0

all :
	@make -C $(LIBFT_DIR)
	@make -j $(NAME)

$(NAME):		$(LIBFT_LIB) $(OBJ_DIR) $(OBJS)
	@$(CC) $(OBJS) -o $(NAME) \
		-I $(INC_DIR) \
		-I $(LIBFT_INC) \
		$(LIBS) \
		$(LIBFT_LIB) \
		$(FLAGS)
	@printf "\r\033[48;5;15;38;5;25m✅ MAKE $(NAME)\033[0m\033[K\n"

$(LIBFT_LIB):
	@make -C $(LIBFT_DIR)

$(OBJ_DIR) :
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $(OBJS))

$(OBJ_DIR)%.o :	$(SRC_DIR)%.c | $(OBJ_DIR)
	@$(eval DONE=$(shell echo $$(($(INDEX)*20/$(NB)))))
	@$(eval PERCENT=$(shell echo $$(($(INDEX)*100/$(NB)))))
	@$(eval COLOR=$(shell echo $$(($(PERCENT)%35+196))))
	@$(eval TO_DO=$(shell echo $$((20-$(INDEX)*20/$(NB)))))
	@printf "\r\033[38;5;11m⌛ MAKE %10.10s : %2d%% \033[48;5;%dm%*s\033[0m%*s\033[48;5;255m \033[0m \033[38;5;11m %*.*s\033[0m\033[K" $(NAME) $(PERCENT) $(COLOR) $(DONE) "" $(TO_DO) "" $(DELTA) $(DELTA) "$@"
	@$(CC)  -MMD -c $< -o $@\
		-I $(INC_DIR)\
		-I $(LIBFT_INC)
	@$(eval INDEX=$(shell echo $$(($(INDEX)+1))))

clean:          cleanlib
	@rm -rf $(OBJ_DIR)
	@printf "\r\033[38;5;202m✖ clean $(NAME)\033[0m\033[K\n"

cleanlib:
	@make -C $(LIBFT_DIR) clean

cleansh:
	@rm -rf $(OBJ_DIR)
	@printf "\r\033[38;5;202m✖ clean $(NAME)\033[0m\033[K\n"

fclean:			fcleanlib cleansh
	@rm -f $(NAME)
	@printf "\r\033[38;5;196m❌ fclean $(NAME)\033[0m\033[K\n"

fcleanlib:
	@make -C $(LIBFT_DIR) fclean

re:				fclean all

.PHONY :		fclean clean re cleanlib fcleanlib

-include $(OBJS:.o=.d)


#SRC = main.c render.c funcs.c intersect.c normal.c camera.c keys.c gpu.c init.c texture.c
#SRC += libft/libft.a
#INC = -I libft/inc
#
#FLAGS = -g -Ofast
#
## #all:
## 	#gcc $(FLAGS) $(SRC) $(INC) -lm -lpthread -lOpenCL -lmlx -lXext -lX11 -I minilibx/ -L minilibx/
#
# all:
#	gcc $(FLAGS) $(SRC) $(INC) -lm -lpthread -lOpenCL -L/usr/X11/lib /usr/X11/lib/libmlx.a -lXext -lX11 -lbsd
