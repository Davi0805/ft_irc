# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: davi <davi@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/28 17:41:07 by davi              #+#    #+#              #
#    Updated: 2025/03/09 12:24:04 by davi             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler settings
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

# File names
SRCS = main.cpp ./Core/Socket.cpp ./Core/Events.cpp ./Handlers/MessageHandler.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = irc

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

# Clean and rebuild
re: clean all

.PHONY: all clean rebuild