NAME	= ircserv
bot 	= bot_bonus
bonussv = bonussv

CC		= c++
CFLAGS 	= -Wall -Wextra -Werror -std=c++98

SRCS	=			mandatory/srcs/Server.cpp\
					mandatory/srcs/Client.cpp\
					mandatory/srcs/Channel.cpp \
					mandatory/srcs/Parsing.cpp \
					mandatory/srcs/CommandHandler.cpp \
					main.cpp\

SRCS_BOT_BONUS =	bonus/srcs/Bot_bonus.cpp\
					bonus/srcs/main_bot_bonus.cpp\

SRCS_BONUS = 		bonus/srcs/Server_bonus.cpp\
					bonus/srcs/Client_bonus.cpp\
					bonus/srcs/Channel_bonus.cpp \
					bonus/srcs/Parsing_bonus.cpp \
					bonus/srcs/CommandHandler_bonus.cpp\
					bonus/srcs/main_bonus.cpp\

OBJS	= $(SRCS:.cpp=.o)
OBJS_BOT_BONUS = $(SRCS_BOT_BONUS:.cpp=.o)
OBJS_BONUS = $(SRCS_BONUS:.cpp=.o)

all: $(NAME)
bonus: $(bot) $(bonussv)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(bot): $(OBJS_BOT_BONUS)
	$(CC) $(CFLAGS) -o $(bot) $(OBJS_BOT_BONUS)

$(bonussv): $(OBJS_BONUS)
	$(CC) $(CFLAGS) -o $(bonussv) $(OBJS_BONUS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

clean_bonus:
	rm -f $(OBJS_BOT_BONUS) $(OBJS_BONUS)

fclean: clean
	rm -f $(NAME)

fclean_bonus: clean_bonus
	rm -f $(bot) $(bonussv)	

re: fclean all

re: fclean_bonus bonus

.PHONY: all clean fclean re

.PHONY: bonus clean_bonus fclean_bonus re