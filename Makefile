# Makefile for IBM TCP/IP for OS/2 telnet login replacement

CC = icc -q -O -I. -DOS2
AR = lib /nologo

OBJP = passwd.obj pwd.obj crypt.obj crypt_util.obj getopt.obj getpass.obj 
OBJ1 = login.obj
OBJ2 = login2.obj pwd.obj crypt.obj crypt_util.obj sleep.obj getopt.obj getpass.obj 
DEF  = default.def

.SUFFIXES: .c .obj
.c.obj:
	$(CC) $(CFLAGS) -c $<

all: passwd.exe login.exe login2.exe

passwd.exe: $(OBJP)
	$(CC) -Fe $@ $(OBJP) $(DEF)

login.exe: $(OBJ1)
	$(CC) -Fe $@ $(OBJ1) $(DEF)

login2.exe: $(OBJ2)
	$(CC) -Fe $@ $(OBJ2) $(DEF)

crypt.obj: ufc/crypt.c
	$(CC) $(CFLAGS) -c ufc/$*.c

crypt_util.obj: ufc/crypt_util.c
	$(CC) $(CFLAGS) -c ufc/$*.c
