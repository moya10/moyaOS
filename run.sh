export PATH=$PATH:/usr/local/i386elfgcc/bin

nasm "boot.asm" -f bin -o "Binaries/boot.bin"
nasm "kernel_entry.asm" -f elf -o "Binaries/kernel_entry.o"

i386-elf-gcc -ffreestanding -m32 -g -c "kernel.cpp" -o "Binaries/kernel.o"
i386-elf-gcc -ffreestanding -m32 -g -c "CPU/idt.cpp" -o "Binaries/idt.o"
i386-elf-gcc -ffreestanding -m32 -g -c "CPU/isr.cpp" -o "Binaries/isr.o" -I CPU
i386-elf-gcc -ffreestanding -m32 -g -c "CPU/irq.cpp" -o "Binaries/irq.o" -I CPU
i386-elf-gcc -ffreestanding -m32 -g -c "CPU/timer.cpp" -o "Binaries/timer.o" -I CPU
nasm "zeroes.asm" -f bin -o "Binaries/zeroes.bin"

i386-elf-ld -o "Binaries/system.bin" -Ttext 0x1000 "Binaries/kernel_entry.o" "Binaries/kernel.o" "Binaries/idt.o" "Binaries/isr.o" "Binaries/irq.o" "Binaries/timer.o" --oformat binary

cat "Binaries/boot.bin" "Binaries/system.bin" "Binaries/zeroes.bin"  > "Binaries/moyaOS.bin"

rm Binaries/*.o  Binaries/boot.bin Binaries/system.bin Binaries/zeroes.bin

qemu-system-x86_64 -drive format=raw,file="Binaries/moyaOS.bin",index=0,if=floppy,  -m 128M