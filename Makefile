# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/10 16:02:17 by artuda-s          #+#    #+#              #
#    Updated: 2025/03/17 15:41:53 by artuda-s         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Binary name
NAME = ircserv

# Compiler
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

# Files
SRC = main.cpp \
	Core/Socket.cpp \
	Core/Events.cpp \
	Handlers/MessageHandler.cpp \
	Commands/Command.cpp \
	Commands/PassCommand.cpp \
	Commands/NickCommand.cpp \
	Commands/UserCommand.cpp \
	Commands/JoinCommand.cpp \
	Commands/PrivMsgCommand.cpp \
	Commands/QuitCommand.cpp \
	Commands/WhoCommand.cpp \
	Models/Channel.cpp \
	Models/User.cpp \
	Models/Server.cpp \
	Services/UserService.cpp \
	Services/ChannelService.cpp \
	Utils/ServerMessages.cpp \
	Utils/Utils.cpp

OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

# Rules
all: $(NAME)

$(NAME): $(OBJ) 
	@$(CC) $(CFLAGS) $(OBJ) -o $@
	@printf "$(GRN)➾ Compilation progress: $$(echo "$(shell find $(OBJ_DIR) -name "*.o" | wc -l) $(TOTAL_FILES)" | awk '{printf "%.2f", $$1/$$2 * 100}')%% $(RES)\r"
	@echo "\n$(GRN)➾ [ ${NAME} ] created$(RES)"

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(GRN)➾ Compilation progress: $$(echo "$(shell find $(OBJ_DIR) -name "*.o" | wc -l) $(TOTAL_FILES)" | awk '{printf "%.2f", $$1/$$2 * 100}')%%$(RES)\r"

$(OBJ_DIR):
	@mkdir -p $@

clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
		rm -rf $(OBJ_DIR);\
		echo "${RED}➾ Cleaned the workspace${RES}";\
	fi

fclean: clean
	@if [ -f "$(NAME)" ]; then \
		rm -f $(NAME);\
		echo "${RED}➾ Fully cleaned the workspace${RES}";\
	fi

re: fclean all

TOTAL_FILES := $(words $(SRC))
COMPILED_FILES := $(shell if [ -d "$(OBJ_DIR)" ]; then find $(OBJ_DIR) -name "*.o" | wc -l;	 else echo 0; fi)

# Phony targets
.PHONY: all clean fclean re

# Color variables
RED = \033[0;31m
GRN = \033[0;32m
YEL = \033[0;33m
BLU = \033[0;34m
MAG = \033[0;35m
CYN = \033[0;36m
WHT = \033[0;37m
RES = \033[0m

