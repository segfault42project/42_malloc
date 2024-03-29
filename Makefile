# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rabougue <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/10 19:16:51 by rabougue          #+#    #+#              #
#    Updated: 2017/07/23 18:52:56 by rabougue         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

####################################_COLOR_#####################################
GREEN = \033[38;5;10m
GREY = \033[38;5;60m
RED = \033[38;5;9m
END = \033[0m
##################################_COMPILATION_#################################
NAME = libft_malloc_$(HOSTTYPE).so
CC = clang
#FLAG = -Weverything -g
FLAG = -Wall -Wextra -Werror -g
LFT = ./libft/libft.a
SRCS =	./source/malloc.c\
		./source/debug.c\
		./source/realloc.c\
		./source/free.c\
		./source/tiny_small.c\
		./source/large.c\
		./source/lst.c\
		./source/memory.c\
		./source/show_alloc_mem.c\

OBJS = $(SRCS:.c=.o)

INCLUDE = -I ./include \
		  -I ./libft
###########################_RELINK_MODIFY_.h####################################
RELINK = ./include/malloc.h
################################################################################


all: $(NAME)

$(NAME): $(OBJS)
	@rm -rf libft_malloc.so $(NAME)
	@make -j8 -s -C ./libft/
	@ln -s $(NAME) libft_malloc.so
	@$(CC) $(FLAG) $(OBJS) -shared -o $(NAME) $(LFT)
	@printf "✅  Compilation done.\n"

%.o : %.c $(RELINK)
	@printf " ✅                                                              \r"
	@printf "✅  $(notdir $<)\r"
	@$(CC) -c $(FLAG) $< -o $@ $(INCLUDE)

clean:
	@printf "                                                               \r"
	@printf "✅  all .o deleted\n"
	@rm -f $(OBJS)
	@make -s clean -C ./libft/

fclean:
	@printf "                                                               \r"
	@printf "✅  libft.a, all .o and malloc deleted\n"
	@rm -f $(NAME) $(OBJS)
	@rm -rf libft_malloc.so $(NAME)
	@make -s fclean -C ./libft/

re: fclean all
