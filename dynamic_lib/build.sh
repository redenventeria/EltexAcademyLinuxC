mkdir -p build
mkdir -p lib
cd src
gcc add.c -Wall -Wextra -c -fpic
gcc sub.c -Wall -Wextra -c -fpic
gcc mul.c -Wall -Wextra -c -fpic
gcc div.c -Wall -Wextra -c -fpic

gcc -shared -o libcalc.so add.o sub.o mul.o div.o
mv libcalc.so ../lib

gcc -L../lib -o main main.c -lcalc
mv main ../build

rm *.o
