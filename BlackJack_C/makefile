TOOLCHAINPATH=C:/TDM-GCC-32/
LIBPATH=$(TOOLCHAINPATH)lib/
CC=$(TOOLCHAINPATH)bin/gcc.exe
LIBS= \
	$(LIBPATH)libkernel32.a \
	$(LIBPATH)libuser32.a \
	$(LIBPATH)libgdi32.a
OBJS=Program.o GenList.o MyWin32.o BJack.o
EXE_NAME=BlackJack.exe
FLAGS=-mwindows -s -std=gnu11

BlackJack : $(OBJS)
	$(CC) $(FLAGS) -o $(EXE_NAME) $(OBJS) $(LIBS)

Program.o :
	$(CC) -c Program.c

GenList.o :
	$(CC) -c GenList.c

MyWin32.o :
	$(CC) -c MyWin32.c

BJack.o :
	$(CC) -c BJack.c

.PHONY: clean
clean :
	del /f /q *.o
