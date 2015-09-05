SHELL = /bin/bash # for time

all: dna-complement-char-same.exe \
	dna-complement-char-nonsame.exe \
	dna-complement-short-same.exe \
	dna-complement-short-nonsame.exe

dna-complement-char-same.exe: dna-complement.c
	gcc $< -o $@ -O3 -DSAME_BUFFER

dna-complement-char-nonsame.exe: dna-complement.c
	gcc $< -o $@ -O3

dna-complement-short-same.exe: dna-complement.c
	gcc $< -o $@ -O3 -DSAME_BUFFER -DUSE_SHORT

dna-complement-short-nonsame.exe: dna-complement.c
	gcc $< -o $@ -O3 -DUSE_SHORT

input.txt:
	xxd /dev/urandom | head --bytes 1G > $@

test: input.txt all
	./dna-complement-char-same.exe \
		$< output-char-same.txt
	./dna-complement-char-nonsame.exe \
		$< output-char-nonsame.txt
	./dna-complement-short-same.exe \
		$< output-short-same.txt
	./dna-complement-short-nonsame.exe \
		$< output-short-nonsame.txt
	sha1sum *.txt
	time ./dna-complement-char-same.exe \
		$< /dev/null
	time ./dna-complement-char-nonsame.exe \
		$< /dev/null
	time ./dna-complement-short-same.exe \
		$< /dev/null
	time ./dna-complement-short-nonsame.exe \
		$< /dev/null

clean:
	rm *.exe *.txt
