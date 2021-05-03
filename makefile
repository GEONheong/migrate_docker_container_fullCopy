CC =gcc

all: controller user vm1

controller:./host_controller/controller.c
	$(CC) -o ./host_controller/controller ./host_controller/controller.c
user: ./host_user/user.c
	$(CC) -o ./host_user/user ./host_user/user.c
vm1: ./host_vm1/vm1.c ./host_vm1/sendfile.c
	$(CC) -o ./host_vm1/vm1 ./host_vm1/vm1.c ./host_vm1/sendfile.c