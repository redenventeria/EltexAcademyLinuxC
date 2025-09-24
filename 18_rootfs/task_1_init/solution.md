Для запуска ядра создал файл init (исходник init.c)

```bash
# компиляция init-процесса:
arm-gnueabihf-gcc -ststic init.c -o init 

# Вынес файл vexpress-v2p-ca9.dtb (device tree) в одну папку с сокпилированным в прошлом задании ядром

# Архивирование файла init в файловую систему:
echo init | cpio -o -H newc | gzip > initramfs.cpio.gz

# Запуск: 
QEMU_AUDIO_DRV=none qemu-system-arm -M vexpress-a9 -kernel zImage -dtb vexpress-v2p-ca9.dtb -initrd initramfs.cpio.gz -append "console=ttyAMA0" -nographic

Результаты запуска - в файле qemu.jpg
```