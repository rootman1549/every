cmd_/root/workspace/test_bin/hello.ko := ld -r -m elf_i386 -T /usr/src/linux-headers-3.18.0-kali3-common/scripts/module-common.lds --build-id  -o /root/workspace/test_bin/hello.ko /root/workspace/test_bin/hello.o /root/workspace/test_bin/hello.mod.o
