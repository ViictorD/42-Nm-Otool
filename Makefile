NAME_NM = ft_nm

NAME_OTOOL = ft_otool

CC = gcc

CFLAGS = -Wall -Wextra -Werror

INCLUDE_DIR = ./includes

DIR_OBJ = ./obj/

DIR_LIB = ./libft/

SRC_DIR = ./srcs/

FILES_NM = ft_nm.c \
			ft_nm2.c \
			utils.c \
			sort.c 

FILES_OTOOL = ft_otool.c \
				utils.c \

SRCS_NM = $(addprefix $(SRC_DIR), $(FILES_NM))

OBJS_NM = $(patsubst %.c, $(DIR_OBJ)%.o, $(FILES_NM))

SRCS_OTOOL = $(addprefix $(SRC_DIR), $(FILES_OTOOL))

OBJS_OTOOL = $(patsubst %.c, $(DIR_OBJ)%.o, $(FILES_OTOOL))

RM = /bin/rm -rf

all: mkdirobj $(OBJS_NM) $(OBJS_OTOOL)
		@ make -C $(DIR_LIB)
		@ /bin/echo "Archiving object in $(NAME_NM)"
		@ $(CC) -o $(NAME_NM) $(OBJS_NM) $(MFLAGS) -L $(DIR_LIB) -lft
		@ /bin/echo "Archiving object in $(NAME_OTOOL)"
		@ $(CC) -o $(NAME_OTOOL) $(OBJS_OTOOL) $(MFLAGS) -L $(DIR_LIB) -lft
		@ echo "\033[32mAll done!\033[0m"

$(DIR_OBJ)%.o: $(SRC_DIR)%.c
	@ /bin/echo -n "    $(notdir $@)"
	@ $(CC) $(CFLAGS) -c -o $@ $< -I $(INCLUDE_DIR)
	@ echo " \033[32mOK\033[0m"

mkdirobj:
	@ mkdir -p $(DIR_OBJ)

clean:
	@ make clean -C $(DIR_LIB)
	@ /bin/echo -n "Removing object files:"
	@ $(RM) $(DIR_OBJ)
	@ echo " \033[32mdone\033[0m"

fclean: clean
	@ make fclean -C $(DIR_LIB)
	@ /bin/echo -n "Removing library:"
	@ $(RM) $(NAME_NM)
	@ $(RM) $(NAME_NM).dSYM
	@ $(RM) $(NAME_OTOOL)
	@ $(RM) $(NAME_OTOOL).dSYM
	@ echo " \033[32mdone\033[0m"

debug: mkdirobj
	@ make -C $(DIR_LIB)
	@ $(CC) -o $(NAME_NM) $(SRCS_NM) -I$(INCLUDE_DIR) $(MFLAGS) -L $(DIR_LIB) -lft -g
	@ $(CC) -o $(NAME_OTOOL) $(SRCS_OTOOL) -I$(INCLUDE_DIR) $(MFLAGS) -L $(DIR_LIB) -lft -g


re: fclean all

.PHONY : all clean fclean re

