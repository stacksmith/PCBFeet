default: fcad
document.o: document.c document.h element.h view.h types.h
	gcc document.c -c `pkg-config --cflags  gtk+-3.0` -O3
	
line.o: line.c line.h view.h types.h
	gcc line.c -c `pkg-config --cflags  gtk+-3.0` -O3

pad.o:  pad.c pad.h view.h types.h
	gcc pad.c -c `pkg-config --cflags  gtk+-3.0` -O3

pin.o:  pin.c pin.h view.h types.h
	gcc pin.c -c `pkg-config --cflags  gtk+-3.0` -O3

element.o: element.c element.h view.h pad.h types.h
	gcc element.c -c `pkg-config --cflags  gtk+-3.0` -O3

grid.o: grid.c view.h types.h
	gcc grid.c -c `pkg-config --cflags  gtk+-3.0` -O3

view.o: view.c view.h types.h
	gcc view.c -c `pkg-config --cflags  gtk+-3.0` -O3

ruler.o: ruler.c ruler.h view.h types.h
	gcc ruler.c -c `pkg-config --cflags  gtk+-3.0` -O3

fcad.o: fcad.c view.h ruler.h pad.h types.h
	gcc fcad.c -c `pkg-config --cflags  gtk+-3.0` -O3
	
fcad: fcad.o view.o  ruler.o grid.o element.o pad.o pin.o line.o document.o
	gcc -g -o fcad fcad.o view.o  ruler.o grid.o element.o pad.o \
	pin.o line.o document.o \
	  `pkg-config --libs gtk+-3.0` -export-dynamic -O3

clean:
	rm -f *~ *.s *.o *.map *.cho *.bin
