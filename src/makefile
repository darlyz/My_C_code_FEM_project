CC = gcc
FLAG = 
target = tinyfem
aztecoo = /mnt/d/Allo_Test/test.fem/AztecOO
clapack = /mnt/d/Allo_Test/test.fem/CLAPACK
solvlib = $(aztecoo)
solvinc = $(aztecoo)
lpcklib = $(clapack)
lpckinc = $(clapack)

src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))

$(target):$(obj)
	$(CC) $^ -o $@ $(solvlib)/libaztec.a $(lpcklib)/liblapack.a -lm
	mv ./$(target) ..
	rm *.o

%.o:%.c
	$(CC) $(FLAG) -c $< -I$(solvinc) -I$(lpckinc) -o $@

.PHONY:clean
clean:
	rm $(obj) $(target) -f
wclean:
	del $(obj) $(target)

