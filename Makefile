default: fcad
parser.o: parser.c parser.h
	gcc parser.c -c `pkg-config --cflags  gtk+-3.0` -O3

document.o: document.c document.h element.h view.h types.h parser.h
	gcc document.c -c `pkg-config --cflags  gtk+-3.0` -O3
	
object.o: object.c object.h view.h parser.h
	gcc object.c -c `pkg-config --cflags  gtk+-3.0` -O3

element.o: element.c element.h view.h pad.h pin.h line.h vtab.h parser.h types.h
	gcc element.c -c `pkg-config --cflags  gtk+-3.0` -O3

grid.o: grid.c view.h types.h
	gcc grid.c -c `pkg-config --cflags  gtk+-3.0` -O3

view.o: view.c view.h types.h
	gcc view.c -c `pkg-config --cflags  gtk+-3.0` -O3

ruler.o: ruler.c ruler.h view.h types.h
	gcc ruler.c -c `pkg-config --cflags  gtk+-3.0` -O3

fcad.o: fcad.c view.h ruler.h pad.h types.h
	gcc fcad.c -c `pkg-config --cflags  gtk+-3.0` -O3
	
fcad: fcad.o view.o  ruler.o grid.o element.o document.o parser.o object.o
	gcc -g -o fcad fcad.o view.o  ruler.o grid.o element.o \
	document.o parser.o object.o\
	  `pkg-config --libs gtk+-3.0` -export-dynamic -O3

clean:
	rm -f *~ *.s *.o *.map *.cho *.bin
