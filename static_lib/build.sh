mkdir -p build
mkdir -p lib
cd src
gcc add.c -c
gcc sub.c -c
gcc mul.c -c
gcc div.c -c
gcc main.c -c

ar rcs libcalc.a add.o sub.o mul.o div.o
gcc main.o libcalc.a -o main
mv libcalc.a ../lib
mv main ../build
rm *.o
