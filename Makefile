default: fcad

	
view.o: view.c view.h
	gcc view.c -c `pkg-config --cflags  gtk+-3.0` -O3

ruler.o: ruler.c ruler.h view.h
	gcc ruler.c -c `pkg-config --cflags  gtk+-3.0` -O3

pad.o:  pad.c pad.h view.h
	gcc pad.c -c `pkg-config --cflags  gtk+-3.0` -O3

fcad.o: fcad.c view.h ruler.h pad.h 
	gcc fcad.c -c `pkg-config --cflags  gtk+-3.0` -O3
	
fcad: fcad.o view.o pad.o ruler.o
	gcc -g -o fcad fcad.o view.o pad.o ruler.o `pkg-config --libs gtk+-3.0` -export-dynamic -O3

clean:
	rm -f *~ *.s *.o *.map *.cho *.bin