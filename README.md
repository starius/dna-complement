# dna-complement
Replace `A<->T` `G<->C` in a file

Run in tmpfs not to freeze a system because of high IO usage.

```
$ cd /tmp
$ make
$ make test
```

## Results

Make sure output files are equal:
```
sha1sum *.txt
2d28accd35d4982f4b4a4e9e33b7aae4561528b7  input.txt
ea285587e049d4d9db1f04137334ce3084149572  output-char-nonsame.txt
ea285587e049d4d9db1f04137334ce3084149572  output-char-same.txt
ea285587e049d4d9db1f04137334ce3084149572  output-int-nonsame.txt
ea285587e049d4d9db1f04137334ce3084149572  output-int-same.txt
ea285587e049d4d9db1f04137334ce3084149572  output-short-nonsame.txt
ea285587e049d4d9db1f04137334ce3084149572  output-short-same.txt
```

Launch with an output file:
```
dna-complement-char-same.exe
    real    0m4.941s
    user    0m2.816s
    sys     0m2.112s

dna-complement-char-nonsame.exe
    real    0m5.057s
    user    0m2.776s
    sys     0m2.192s

dna-complement-short-same.exe
    real    0m3.580s
    user    0m1.332s
    sys     0m2.236s

dna-complement-short-nonsame.exe
    real    0m3.573s
    user    0m1.184s
    sys     0m2.376s

dna-complement-int-same.exe
    real    0m19.399s
    user    0m16.613s
    sys     0m2.740s

dna-complement-int-nonsame.exe
    real    0m37.583s
    user    0m18.413s
    sys     0m8.285s
```

Launch without an output file:
```
dna-complement-char-same.exe
    real    0m3.545s
    user    0m2.780s
    sys     0m0.756s

dna-complement-char-nonsame.exe
    real    0m3.567s
    user    0m2.840s
    sys     0m0.716s

dna-complement-short-same.exe
    real    0m2.162s
    user    0m1.444s
    sys     0m0.712s

dna-complement-short-nonsame.exe
    real    0m2.164s
    user    0m1.432s
    sys     0m0.724s

dna-complement-int-same.exe
    real    0m15.439s
    user    0m14.353s
    sys     0m1.052s

dna-complement-int-nonsame.exe
    real    0m16.737s
    user    0m15.657s
    sys     0m1.044s
```

Standings:
  * short
  * char
  * int
