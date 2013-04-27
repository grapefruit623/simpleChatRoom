cc=gcc
opt=-o myServer
obj=hw2.c login.c
all: 
	$(cc) $(obj) $(opt) 
run:
	$(CURDIR)/myServer
clean:
	rm myServer
