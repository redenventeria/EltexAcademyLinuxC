Установил busybox-1.37.0

```bash

# В директории busybox:
ARCH=arm make defconfig
ARCH=arm make menuconfig

# В меню выставил опции статической сборки и указал префикс кросс компилятора(см. menuconfig.jpg)

#После этого скомпилировал
ARCH=arm make -j 3

# Компиляция завершилась ошибкой. Поиск проблем показал, что в версии 1.37
# Сломаны опции CONFIG_TC для утилиты tc и SHA1 Hardware Acceleration. Их отключение решило проблему.

# Запуск через Qemu:
QEMU_AUDIO_DRV=none qemu-system-arm -M vexpress-a9 -kernel zImage -dtb vexpress-v2p-ca9.dtb -initrd initramfs.cpio.gz  -append "console=ttyAMA0 rdinit=/bin/ash" -nographic

# Результаты с тестом ash - gemu.jpg
```