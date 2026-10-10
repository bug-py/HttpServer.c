FLAGS = -Werror -Wall -Wextra -g

PATH_SRC=src
PATH_HEADER=include
PATH_OBJ = build/obj
PATH_BIN = build/bin
PATH_TEST = tests

TESTS =  $(wildcard $(PATH_TEST)/*.c)
BINS = $(subst $(PATH_TEST)/,$(PATH_BIN)/,$(TESTS:.c=.exe))

LIB_NAME = http
LIB_PATH = build/lib
LIB = $(LIB_PATH)/lib$(LIB_NAME).a

SRCS = $(wildcard $(PATH_SRC)/*/*.c)
OBJS =$(subst $(PATH_SRC)/,$(PATH_OBJ)/,$(SRCS:.c=.o))

define COMPILE_RULES
$(1) : $(2) $(3)
	gcc $(FLAGS) -I$(PATH_HEADER) -c $$< -o $$@
endef

.PHONY : all clean test
all : $(LIB)

$(foreach src,$(SRCS),$(eval $(call COMPILE_RULES , $(subst $(PATH_SRC)/,$(PATH_OBJ)/,$(src:.c=.o)) ,$(src),$(subst $(PATH_SRC)/,$(PATH_HEADER)/,$(src:.c=.h)) )))

$(LIB) : $(OBJS)
	ar -crs $@ $^
	
$(PATH_BIN)/%.exe : $(PATH_TEST)/%.c $(LIB)
	gcc $(FLAGS) -I$(PATH_HEADER) $< -L$(LIB_PATH) -l$(LIB_NAME) -o $@

test  : $(BINS)

clean :
	rm -rf build/obj/*/*.o
	rm -rf build/bin/*.exe
	rm -rf $(LIB)
