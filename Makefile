LIBS =  -lvlc \
		   -lpthread \

EXE_NAME = exe

build: sources
	gcc *.o $(LIBS) -o $(EXE_NAME)

debug: sources
	gcc *.o $(LIBS) -o $(EXE_NAME) -g
	gdb ./$(EXE_NAME)

sources: *.c
	gcc -c $?
	
run:
	./$(EXE_NAME) example.mp3

clean:
	rm $(EXE_NAME) *.o