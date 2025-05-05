# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/10 16:02:17 by artuda-s          #+#    #+#              #
#    Updated: 2025/04/25 13:41:11 by dmelo-ca         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Binary name
NAME = ircserv

# Compiler
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address,undefined -fno-omit-frame-pointer -O1

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
	Commands/ModeCommand.cpp \
	Commands/InviteCommand.cpp \
	Commands/KickCommand.cpp \
	Commands/TopicCommand.cpp \
	Commands/PartCommand.cpp \
	Commands/PingCommand.cpp \
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



############################################################################################################################################
############################################################################################################################################
############################################################################################################################################
# Bot compilation details
BOT_NAME = UselessBot
BOT_SRC = Bot/bot_main.cpp \
          Bot/Bot.cpp
BOT_OBJ_DIR = $(OBJ_DIR)/Bot
BOT_OBJ = $(addprefix $(BOT_OBJ_DIR)/, $(notdir $(BOT_SRC:.cpp=.o)))

# Bot compilation rule
bot: $(BOT_NAME)

$(BOT_NAME): $(BOT_OBJ)
	@$(CC) $(CFLAGS) $(BOT_OBJ) -o $@
	@echo "\n$(GRN)➾ [ BOT ] created$(RES)"

# Rule to create object files for bot
$(BOT_OBJ_DIR)/%.o: Bot/%.cpp | $(BOT_OBJ_DIR)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(YEL)➾ Compiling Bot$(RES)\r"

# Create bot object directory
$(BOT_OBJ_DIR):
	@mkdir -p $@



############################################################################################################################################
############################################################################################################################################
############################################################################################################################################
# Test compilation details
TEST_NAME = integration_test
TEST_SRC = Tests/Integration.cpp
TEST_OBJ = $(addprefix $(OBJ_DIR)/, $(TEST_SRC:.cpp=.o))

# Rule for compiling
test: $(TEST_OBJ) $(filter-out $(OBJ_DIR)/main.o, $(OBJ))
	$(CC) $(CFLAGS) $^ -o $(TEST_NAME)

# Rule to compile test object files without flags
# fucking c++98 xD
$(OBJ_DIR)/Tests/%.o: Tests/%.cpp | $(OBJ_DIR)/Tests
	@mkdir -p $(@D)
	@$(CC) -c $< -o $@

# Create test object directory
$(OBJ_DIR)/Tests:
	@mkdir -p $@

# Update clean and fclean to handle bot
clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
		rm -rf $(OBJ_DIR); \
		echo "${RED}➾ Cleaned the workspace${RES}"; \
	fi

fclean: clean
	@if [ -f "$(NAME)" ]; then \
		rm -f $(NAME); \
	fi
	@if [ -f "$(BOT_NAME)" ]; then \
		rm -f $(BOT_NAME); \
	fi
		@if [ -f "$(TEST_NAME)" ]; then \
		rm -f $(TEST_NAME); \
		echo "${RED}➾ Fully cleaned the workspace${RES}"; \
	fi

re: fclean all

# Phony targets
.PHONY: all clean fclean re bot

TOTAL_FILES := $(words $(SRC))
COMPILED_FILES := $(shell if [ -d "$(OBJ_DIR)" ]; then find $(OBJ_DIR) -name "*.o" | wc -l;	 else echo 0; fi)

# Color variables
RED = \033[0;31m
GRN = \033[0;32m
YEL = \033[0;33m
BLU = \033[0;34m
MAG = \033[0;35m
CYN = \033[0;36m
WHT = \033[0;37m
RES = \033[0m
