###########################################
#Makefile for simple programs
###########################################
INC=
LIB= -lpthread

CC=g++
CC_FLAG=-Wall

DEST=test
OBJ=test.o

$(DEST):$(OBJ)
	$(CC) $(INC) $(LIB) -o $@ $(OBJ)
	
.SUFFIXES: .c .o .cpp
.cpp.o:
	$(CC) $(CC_FLAG) $(INC) -c $*.cpp -o $*.o

.PRONY:clean
clean:
	@echo "Removing linked and compiled files......"
	rm -f $(OBJ) $(DEST)
