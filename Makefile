TMP = tmps
SRC = src
BLD = BUILD

OBJ = $(TMP)/obj
DEP = $(TMP)/dep
CC = gcc

BASE = ggluemap
LIB = lib$(BASE)
EXE = $(BASE).out

DPR = d_
PIC = pic_

files := glMap.c
test := main.c
ft = $(files) $(test)

prod_sta_OBJS = $(addprefix $(OBJ)/, $(files:.c=.o))
dbug_sta_OBJS = $(addprefix $(OBJ)/$(DPR), $(files:.c=.o))
prod_pic_OBJS = $(addprefix $(OBJ)/$(PIC), $(files:.c=.o))
dbug_pic_OBJS = $(addprefix $(OBJ)/$(DPR)$(PIC), $(files:.c=.o))

prod_sta_DEPS = $(addprefix $(DEP)/, $(ft:.c=.d))
dbug_sta_DEPS = $(addprefix $(DEP)/$(DPR), $(ft:.c=.d))
prod_pic_DEPS = $(addprefix $(DEP)/$(PIC), $(ft:.c=.d))
dbug_pic_DEPS = $(addprefix $(DEP)/$(DPR)$(PIC), $(ft:.c=.d))

prodCompilerFlags = -O3 -m64 -Wall -Wpedantic -Wextra
dbugCompilerFlags = -O0 -g -m64 -Wall -Wpedantic -Wextra
linkerFlags = 

.PHONY: target statDbug dinDbug statProd dinProd 

target: dbug

testDebug: dbug

testProduction: prod

staticDebug: dbug_sta

staticProduction: prod_sta

dinamicDebug: dbug_pic

dinamicProduction: prod_pic

.SECONDEXPANSION:
dbug prod: $$@_sta
	$(CC) -o $(BLD)/$(EXE) $(linkerFlags) $($@CompilerFlags) \
	$(addprefix $(SRC)/, $(test)) -L$(BLD) -l:$(LIB).a

dbug_sta prod_sta: $$($$@_DEPS) $$($$@_OBJS)
	ar rcs $(BLD)/$(LIB).a \
	$($@_OBJS)

dbug_pic prod_pic: $$($$@_DEPS) $$($$@_OBJS)
	$(CC) -o $(BLD)/$(LIB).so -shared \
	$($@_OBJS)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(prodCompilerFlags) \
	-o $@ $< -c 

$(OBJ)/$(PIC)%.o: $(SRC)/%.c
	$(CC) $(prodCompilerFlags) \
	-o $@ $< -c -fPIC 

$(OBJ)/$(DPR)%.o: $(SRC)/%.c
	$(CC) $(dbugCompilerFlags) \
	-o $@ $< -c

$(OBJ)/$(DPR)$(PIC)%.o: $(SRC)/%.c
	$(CC) $(dbugCompilerFlags) \
	-o $@ $< -c -fPIC

$(DEP)/%.d $(DEP)/$(DPR)%.d $(DEP)/$(PIC)%.d $(DEP)/$(DPR)$(PIC)%.d : $(SRC)/%.c
	$(CC) $< -M \
	-MT $(OBJ)/$(basename $(notdir $@)).o \
	> $@

readSymbols: dbug_pic
	readelf --symbols $(BLD)/$(LIB).so

test:
	ln -sf $(BLD)/$(EXE) test.out

.PHONY:clean
clean: 
	rm -f $(OBJ)/*
	rm -f $(DEP)/*
	rm -f $(BLD)/*

-include $(DEP)/*
