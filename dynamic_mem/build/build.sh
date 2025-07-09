gcc ../src/tui.c -c
gcc ../src/dl_list.c -c
gcc ../src/contact.c -c
gcc ../src/phone_book.c -c
gcc ../src/main.c -c
gcc contact.o dl_list.o tui.o phone_book.o main.o -o main -g
rm *.o
