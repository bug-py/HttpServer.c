FLAGS = -Werror -Wall -Wextra
FILE = httpserver

all: $(FILE).exe
	./$(FILE).exe

$(FILE).exe : $(FILE).c
	gcc $(FLAGS) $^ -o $@
