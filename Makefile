gol.exe: gol.c 
	gcc -mwindows gol.c -o $@

gol.res: gol.rc resource.h
	windres $< -O coff -o $@

clean:
	rm -f *.exe
	rm -f *.o
	rm -f *.res

