.PHONY: all clean mount umount

all:
	gcc -Wall passthrough.c `pkg-config fuse3 --cflags --libs` -o passthrough

clean:
	rm -rf passthrough

mount: all
	mkdir -p mount_point
	./passthrough mount_point

umount:
	umount mount_point
