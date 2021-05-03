CC =gcc

all: controller user vm1

controller:controller.c
	$(CC) -o controller controller.c
user: user.c
	$(CC) -o user user.c
vm1: vm1.c sendfile.c
	$(CC) -o vm1 vm1.c sendfile.c