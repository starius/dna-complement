SHELL = /bin/bash # for time

all: dna-complement-char-same.exe \
	dna-complement-char-nonsame.exe \
	dna-complement-short-same.exe \
	dna-complement-short-nonsame.exe \
	make-int-table.exe \
	dna-complement-int-same.exe \
	dna-complement-int-nonsame.exe

dna-complement-char-same.exe: dna-complement.c
	gcc $< -o $@ -O3 -DSAME_BUFFER

dna-complement-char-nonsame.exe: dna-complement.c
	gcc $< -o $@ -O3

dna-complement-short-same.exe: dna-complement.c
	gcc $< -o $@ -O3 -DSAME_BUFFER -DUSE_SHORT

dna-complement-short-nonsame.exe: dna-complement.c
	gcc $< -o $@ -O3 -DUSE_SHORT

make-int-table.exe: make-int-table.c
	gcc $< -o $@ -O3

dna-complement-int-same.exe: dna-complement-int.c
	gcc $< -o $@ -O3 -DSAME_BUFFER

dna-complement-int-nonsame.exe: dna-complement-int.c
	gcc $< -o $@ -O3

input.txt:
	xxd /dev/urandom | head --bytes 4G > $@

table16g.dat: make-int-table.exe
	./$< > $@

test: all input.txt table16g.dat
	time ./dna-complement-char-same.exe \
		input.txt output-char-same.txt
	time ./dna-complement-char-nonsame.exe \
		input.txt output-char-nonsame.txt
	time ./dna-complement-short-same.exe \
		input.txt output-short-same.txt
	time ./dna-complement-short-nonsame.exe \
		input.txt output-short-nonsame.txt
	time ./dna-complement-int-same.exe \
		table16g.dat input.txt output-int-same.txt
	time ./dna-complement-int-nonsame.exe \
		table16g.dat input.txt output-int-nonsame.txt
	sha1sum *.txt
	time ./dna-complement-char-same.exe \
		input.txt /dev/null
	time ./dna-complement-char-nonsame.exe \
		input.txt /dev/null
	time ./dna-complement-short-same.exe \
		input.txt /dev/null
	time ./dna-complement-short-nonsame.exe \
		input.txt /dev/null
	time ./dna-complement-int-same.exe \
		table16g.dat input.txt /dev/null
	time ./dna-complement-int-nonsame.exe \
		table16g.dat input.txt /dev/null

clean:
	rm *.exe *.txt *.dat
