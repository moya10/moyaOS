# moyaOS

## theory provided

Check the repository [here](https://github.com/moya10/os-fundamentals/) to better understand the code concepts and create smthing of your own.

## Building And Running

### Required packages
-----------------

First, install the required packages. On linux, use your package distribution. On a Mac, [install brew](http://brew.sh/) and get those packages with `brew install`

- nasm
- qemu
- gmp
- mpfr
- libmpc
- gcc

Yes, we will need `gcc` to build our cross-compiled `gcc`, especially on a Mac where gcc has been deprecated for `clang`

Once installed, find where your packaged gcc is (remember, not clang) and export it. For example:

```
export CC=/usr/local/bin/gcc-4.9
export LD=/usr/local/bin/gcc-4.9
```

We will need to build binutils and a cross-compiled gcc, and we will put them into `/usr/local/i386elfgcc`, so let's export some paths now.

```
export PREFIX="/usr/local/i386elfgcc"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"
```

### binutils
--------

Remember: always be careful before pasting walls of text from the internet. I recommend copying line by line.


```sh
mkdir /tmp/src
cd /tmp/src
curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.24.tar.gz # If the link 404's, look for a more recent version
tar xf binutils-2.24.tar.gz
mkdir binutils-build
cd binutils-build
../binutils-2.24/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log
make all install 2>&1 | tee make.log
```

### gcc
---
```sh
cd /tmp/src
curl -O https://ftp.gnu.org/gnu/gcc/gcc-4.9.1/gcc-4.9.1.tar.bz2
tar xf gcc-4.9.1.tar.bz2
mkdir gcc-build
cd gcc-build
../gcc-4.9.1/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-languages=c --without-headers
make all-gcc 
make all-target-libgcc 
make install-gcc 
make install-target-libgcc 
```

That's it! You should have all the GNU binutils and the compiler at `/usr/local/i386elfgcc/bin`, prefixed by `i386-elf-` to avoid
collisions with your system's compiler and binutils. From now on we will explicitly use the prefixes when using the cross-compiled gcc.

### The kernel
----------

Our C kernel will print a Splash screen and launch a shell. Go ahead
and open `kernel.cpp`.

You will notice a dummy function. That function will force us
to create a kernel entry routine which does not point to byte 0x0 in our kernel, but
to an actual label which we know that launches it. In our case, function `main()`.

`i386-elf-gcc -static-libgcc -lgcc -ffreestanding -m32 -g -c kernel.c -o kernel.o`

That routine is coded on `kernel_entry.asm`. Read it and you will learn how to
use `[extern]` declarations in assembly. To compile this file, instead of generating
a binary, we will generate an `elf` format file which will be linked with `kernel.o`

`nasm kernel_entry.asm -f elf -o kernel_entry.o`


### The linker
----------

A linker is a very powerful tool and we only started to benefit from it.

To link both object files into a single binary kernel and resolve label references,
run:

`i386-elf-ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary`

Notice how our kernel will be placed not at `0x0` in memory, but at `0x1000`. The
bootsector will need to know this address too.

### The bootsector
--------------

Open `boot.asm` and examine the code.

Compile it with `nasm boot.asm -f bin -o boot.bin`

Create binary file full of zeroes to read as many sectors as possible and avoid DISK_ERROR. In assembly write on `zeroes.asm` the code that generates zeroes `times 25600 db 0`. And finally compile it with `nasm zeroes.asm -f bin -o zeroes.bin`

### Putting it all together
-----------------------

Now what? We have two separate files for the bootsector and the kernel?

Can't we just "link" them together into a single file? Yes, we can, and it's easy,
just concatenate them:

`cat boot.bin kernel.bin zeroes.bin > moyaOS.bin`

### Run!
----

You can now run `moyaOS.bin` with qemu.

Remember that if you find disk load errors you may need to play with the disk numbers
or qemu parameters (floppy = `0x0`, hdd = `0x80`). Now, you may want to call `qemu-system-x86_64 moyaOS.bin`

-----
## Resources used:
* Osdev wiki: https://wiki.osdev.org
* os-tutorial: https://github.com/cfenollosa/os-tutorial
* osdever.net: http://www.osdever.net/tutorials/view/brans-kernel-development-tutorial 
* PonchoOS: https://github.com/Absurdponcho/PonchoOS
* Mell-0-tron: https://github.com/mell-o-tron/MellOs
* Daedalus Community: https://www.youtube.com/playlist?list=PLm3B56ql_akNcvH8vvJRYOc7TbYhRs19M