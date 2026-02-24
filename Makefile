NAME	= ircserv

CC		= c++
CFLAGS 	= -Wall -Wextra -Werror -std=c++98

SRCS	=	src/Server.cpp\
			src/Client.cpp\
			src/Channel.cpp \
			main.cpp\

OBJS	= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re