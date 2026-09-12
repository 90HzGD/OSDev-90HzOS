CFLAGS= -Wall -Wextra -Iinclude

compile:
	clear
	mkdir -p ~/OSDev/90HzOS/kernel/src ~/OSDev/90HzOS/kernel/bin ~/OSDev/90HzOS/OS ~/OSDev/90HzOS/img ~/OSDev/90HzOS/boot/bin ~/OSDev/90HzOS/boot/src/vga ~/OSDev/90HzOS/kernel/bin/full ~/OSDev/90HzOS/kernel/bin/build ~/OSDev/90HzOS/prog/bin/build/ ~/OSDev/90HzOS/kernel/bin/build/vga ~/OSDev/90HzOS/kernel/src/mem/ ~/OSDev/90HzOS/kernel/bin/build/mem
	nasm -f bin ~/OSDev/90HzOS/boot/src/boot_stage1.asm -o ~/OSDev/90HzOS/boot/bin/boot_stage1
	nasm -f bin ~/OSDev/90HzOS/boot/src/boot_stage2.asm -o ~/OSDev/90HzOS/boot/bin/boot_stage2
	nasm -f elf ~/OSDev/90HzOS/kernel/src/entry.asm -o ~/OSDev/90HzOS/kernel/bin/build/entry.o
	nasm -f elf ~/OSDev/90HzOS/kernel/src/drivers/ports/ports.asm -o ~/OSDev/90HzOS/kernel/bin/build/ports.o
	nasm -f elf ~/OSDev/90HzOS/kernel/src/include/vga/local.asm -o ~/OSDev/90HzOS/kernel/bin/build/local_vga.o
	nasm -f elf ~/OSDev/90HzOS/prog/src/shellUtil.asm -o ~/OSDev/90HzOS/prog/bin/build/shellUtil.o
	nasm -f elf ~/OSDev/90HzOS/kernel/src/vga/VGAText.asm -o ~/OSDev/90HzOS/kernel/bin/build/VGAText.o
	i386-elf-gcc $(CFLAGS) -ffreestanding -m32 -g -c ~/OSDev/90HzOS/kernel/src/kernel.c -o ~/OSDev/90HzOS/kernel/bin/build/kernel.o
	i386-elf-gcc $(CFLAGS) -ffreestanding -m32 -g -c ~/OSDev/90HzOS/kernel/src/drivers/keyboard/ps2.c -o ~/OSDev/90HzOS/kernel/bin/build/ps2.o
	i386-elf-gcc $(CFLAGS) -ffreestanding -m32 -g -c ~/OSDev/90HzOS/prog/src/shell.c -o ~/OSDev/90HzOS/prog/bin/build/shell.o
	i386-elf-gcc $(CFLAGS) -ffreestanding -m32 -g -c ~/OSDev/90HzOS/kernel/src/vga/stdio.c -o ~/OSDev/90HzOS/kernel/bin/build/vga/stdio.o
	i386-elf-gcc $(CFLAGS) -ffreestanding -m32 -g -c ~/OSDev/90HzOS/kernel/src/mem/mem_alloc.c -o ~/OSDev/90HzOS/kernel/bin/build/mem/mem_alloc.o
	i386-elf-gcc $(CFLAGS) -ffreestanding -m32 -g -c ~/OSDev/90HzOS/kernel/src/string.c -o ~/OSDev/90HzOS/kernel/bin/build/string.o
	i386-elf-gcc $(CFLAGS) -ffreestanding -m32 -g -c ~/OSDev/90HzOS/kernel/src/vga/menu.c -o ~/OSDev/90HzOS/kernel/bin/build/menu.o
	i386-elf-gcc $(CFLAGS) -ffreestanding -m32 -g -c ~/OSDev/90HzOS/kernel/src/drivers/keyboard/api/kb_tools.c -o ~/OSDev/90HzOS/kernel/bin/build/kb_tools.o
	i386-elf-gcc $(CFLAGS) -ffreestanding -m32 -g -c ~/OSDev/90HzOS/kernel/src/drivers/PCI/PCI.c -o ~/OSDev/90HzOS/kernel/bin/build/PCI.o
	i386-elf-gcc $(CFLAGS) -ffreestanding -m32 -g -c ~/OSDev/90HzOS/kernel/src/drivers/ATA/atapio.c -o ~/OSDev/90HzOS/kernel/bin/build/atapio.o
	i386-elf-ld -T linker.ld -o ~/OSDev/90HzOS/kernel/bin/full/kernel.bin ~/OSDev/90HzOS/kernel/bin/build/entry.o ~/OSDev/90HzOS/kernel/bin/build/menu.o ~/OSDev/90HzOS/kernel/bin/build/VGAText.o ~/OSDev/90HzOS/kernel/bin/build/ports.o ~/OSDev/90HzOS/kernel/bin/build/atapio.o ~/OSDev/90HzOS/kernel/bin/build/local_vga.o ~/OSDev/90HzOS/prog/bin/build/shellUtil.o ~/OSDev/90HzOS/kernel/bin/build/kernel.o ~/OSDev/90HzOS/kernel/bin/build/kb_tools.o ~/OSDev/90HzOS/kernel/bin/build/PCI.o ~/OSDev/90HzOS/kernel/bin/build/ps2.o ~/OSDev/90HzOS/kernel/bin/build/string.o ~/OSDev/90HzOS/kernel/bin/build/vga/stdio.o ~/OSDev/90HzOS/kernel/bin/build/mem/mem_alloc.o ~/OSDev/90HzOS/prog/bin/build/shell.o --oformat binary -Map layout.map
	rm -f ~/OSDev/90HzOS/OS/90HzOS.bin ~/OSDev/90HzOS/img/disk.img
	touch ~/OSDev/90HzOS/img/disk.img ~/OSDev/90HzOS/OS/90HzOS.bin
	dd if=/dev/zero of=$$HOME/OSDev/90HzOS/img/disk.img bs=512 count=100
	dd if=$$HOME/OSDev/90HzOS/boot/bin/boot_stage1 of=$$HOME/OSDev/90HzOS/img/disk.img conv=notrunc
	dd if=$$HOME/OSDev/90HzOS/boot/bin/boot_stage2 of=$$HOME/OSDev/90HzOS/img/disk.img bs=512 seek=2048 conv=notrunc
	dd if=$$HOME/OSDev/90HzOS/kernel/bin/full/kernel.bin of=$$HOME/OSDev/90HzOS/img/disk.img bs=512 seek=2050 conv=notrunc
	dd if=/dev/zero >> $$HOME/OSDev/90HzOS/img/disk.img count=30 bs=512
	dd if=$$HOME/OSDev/90HzOS/img/disk.img of=$$HOME/OSDev/90HzOS/img/disk.img bs=512 conv=notrunc
	dd if=$$HOME/OSDev/90HzOS/img/disk.img of=$$HOME/OSDev/90HzOS/OS/90HzOS.bin bs=512 conv=notrunc

floppy:
	echo "--- MAKING FLOPPY DISK IMAGE ---"
	dd if=/dev/zero of=90HzOS/img/disk.img bs=1M count=16
	dd if=90HzOS/OS/90HzOS.bin of=90HzOS/img/disk.img conv=notrunc

convert_vdi_static:
	mkdir -p ./90HzOS/vdi/temp/
	rm -f ./90HzOS/vdi/temp/90HzOS.vdi
	truncate -s %512 ~/OSDev/90HzOS/img/disk.img
	VBoxManage convertdd ~/OSDev/90HzOS/img/disk.img ~/OSDev/90HzOS/vdi/temp/90HzOS.vdi
	dd if=./90HzOS/vdi/temp/90HzOS.vdi of=./90HzOS/vdi/90HzOS.vdi conv=notrunc

run:
	qemu-system-x86_64 -m 4096 -monitor stdio -hda ~/OSDev/90HzOS/OS/90HzOS.bin

clear:
	mkdir -p ~/OSDev/90HzOS/kernel/src ~/OSDev/90HzOS/kernel/bin ~/OSDev/90HzOS/OS ~/OSDev/90HzOS/img ~/OSDev/90HzOS/boot/bin ~/OSDev/90HzOS/boot/src ~/OSDev/90HzOS/kernel/bin/full ~/OSDev/90HzOS/kernel/bin/build ~/OSDev/90HzOS/prog/bin/build/ ~/OSDev/90HzOS/kernel/bin/build/vga ~/OSDev/90HzOS/kernel/bin/build/mem/
	rm -rf ./90HzOS/boot/bin ./90HzOS/kernel/bin ./90HzOS/prog/bin ./90HzOS/img/* ./90HzOS/OS/* ./90HzOS/vdi
	mkdir -p ~/OSDev/90HzOS/kernel/bin ~/OSDev/90HzOS/OS ~/OSDev/90HzOS/img ~/OSDev/90HzOS/boot/bin  ~/OSDev/90HzOS/kernel/bin/full ~/OSDev/90HzOS/kernel/bin/build ~/OSDev/90HzOS/prog/bin/build/ ~/OSDev/90HzOS/vdi/temp ~/OSDev/90HzOS/kernel/bin/build/vga ~/OSDev/90HzOS/kernel/bin/build/mem
