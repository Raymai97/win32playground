TOOLCHAINPATH=C:/TDM-GCC-32/
LIBPATH=$(TOOLCHAINPATH)lib/
CC=$(TOOLCHAINPATH)bin/gcc.exe
LIBS= \
	$(LIBPATH)libkernel32.a \
	$(LIBPATH)libuser32.a \
	$(LIBPATH)libgdi32.a
OBJS=Program.o DpiAware.o GenList.o
EXE_NAME=BlackJack.exe
FLAGS=-mwindows -s

BlackJack : $(OBJS)
	$(CC) $(FLAGS) -o $(EXE_NAME) $(OBJS) $(LIBS)

Program.o :
	$(CC) -c Program.c

DpiAware.o :
	$(CC) -c DpiAware.c

GenList.o :
	$(CC) -c GenList.c

.PHONY: clean
clean :
	del /f /q *.o
