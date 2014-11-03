CFLAGS=-std=c99 -Wall -Wextra -Werror -pedantic -pedantic-errors -g
LDLIBS=-lglut -lGLU -lGL -lm -lrt

.PHONY: clean

outback: main.o const.o text.o render.o state.o
	$(CC) -o $@ $^ $(LDLIBS)

main.o: main.c main.h

const.o: const.c const.h

text.o: text.c text.h

render.o: render.c render.h

state.o: state.c state.h

clean:
	rm -f outback *.o *.aux *.out *.log *.toc

report.pdf: report.tex
	pdflatex report.tex
