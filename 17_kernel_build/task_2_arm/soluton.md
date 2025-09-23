Установил qemu и кросс-компилятор.
```bash
# Сборка конфига
ARCH=arm make defconfig
ARCH=arm make menuconfig

# Компиляция
ARCH=arm CROSS-COMPILE=arm-linux-gnueabihf- make -j 3 zImage
ARCH=arm eabihf- make -j 3 dtbs

# Перенёс файлы zImage и vexpress-v2p-ca9.dtb в отдельную папку.

# Запуск при помощи qemu:
QEMU_AUDIO_DRV=none qemu-system-arm -M vexpress-a9 -kernel zImage -dtb vexpress-v2p-ca9.dtb -append "console=ttyAMA0" -nographic

Результат запуска в файле qemu.jpg
```