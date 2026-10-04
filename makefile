FLAGS = -Werror -Wall -Wextra


all: $(FILE).exe
	./$(FILE).exe

$(FILE).exe : $(FILE).c
	gcc $(FLAGS) $^ -o $@
