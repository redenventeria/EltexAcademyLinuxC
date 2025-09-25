Для того, чотбы скомпилировать openssh, необходимо собрать openssl-1.1.1 и zlib>=1.2.1.2

```bash
# Сборка zlib
# В папке с мсходниками zlib-1.2.13:
СС=arm-linux-gnueabihf-gcc make
#Изначально эта команда провалилась, поиск похожих проблем показал, что в этой версии были выставлены макросы _FILE_OFFSET_BITS и _TIME_BITS, связанные с разрядностью переменной для хранения времени. Они конфликтовали с новым кросс-компилятором, при этом zlib не использовал этот функционал вообще. Их отключение позволило собрать библиотеку без проблем.

# После этого через mc добавил содержимое папки "_install" к сборке.

# Сборка openssl
# После долгих неудач была найдена следующая рабочая последовательность команд:
./Configure linux-generic32 shared  -DL_ENDIAN --prefix=/home/ivan/TMP/openssl-1.1.1-build --openssldir=/home/ivan/TMP/openssl-1.1.1-build
make CC=arm-linux-gnueabihf-gcc RANLIB=arm-linux-gnueabihf-ranlib LD=arm-linux-gnueabihf-ld MAKEDEPPROG=arm-linux-gnueabihf-gcc PROCESSOR=ARM -j 3
make install

# После установки добавил содержимое папки openssl-1.1.1-build к сборке.

# Для использования в сборке перенёс директории с собранными zlib и openssl в папку с исходниками openssh.

# Сборка openssh
./configure --disable-strip --prefix=$PWD/_install --host=arm-linux-gnueabihf --with-zlib=$PWD/zlib-1.2.13 --with-ssl-dir=$PWD/openssl-1.1.1-build
make -j 3
make install

# Архивация файловой системы:
find . | cpio -o -H newc | gzip > initramfs.cpio.gz

# Запуск в QEMU:
QEMU_AUDIO_DRV=none qemu-system-arm -M vexpress-a9 -kernel zImage -dtb vexpress-v2p-ca9.dtb -initrd initramfs.cpio.gz  -append "console=ttyAMA0 rdinit=/bin/ash" -nographic

Подтверждение наличия openssl и openssh в сборке - в скриншоте QEMU.jpg



```