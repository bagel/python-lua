# makefile for install 

PYTHON_VERSION=2.7
LUA_VERSION=5.1

MYCFLAGS=
MYLIBS=

CC= gcc

INSTALL_TOP=/usr

INSTALL= install -p
INSTALL_EXEC= $(INSTALL) -m 0755
INSTALL_DATA= $(INSTALL) -m 0644

#python include and config path, edit to your own
PYTHON_CFLAGS= -I$(INSTALL_TOP)/include/python$(PYTHON_VERSION) 
PYTHON_CONFIG= -I$(INSTALL_TOP)/lib/python$(PYTHON_VERSION)/config

#lua include path, edit to your own
LUA_CFLAGS= -I$(INSTALL_TOP)/include/lua5.1

CFLAGS= -I$(INSTALL_TOP)/include $(PYTHON_CFLAGS) $(PYTHON_CONFIG) $(LUA_CFLAGS) $(MYCFLAGS)

#lua lib, edit to your own
LUA_LIBS= -llua$(LUA_VERSION)
LIBS= -L$(INSTALL_TOP)/lib $(LUA_LIBS) $(MYLIBS)

LUA_SO= lua.so
LUA_O= lua.o
LUA_C= lua.c


all: $(LUA_SO)

$(LUA_SO): $(LUA_C)
	$(CC) -fPIC -shared -o $@ $? $(CFLAGS) $(LIBS)

$(LUA_O): $(LUA_C)

install:
	$(INSTALL_EXEC) $(LUA_SO) $(INSTALL_TOP)/lib/python$(PYTHON_VERSION)/site-packages/

clean:
	rm lua.so -rv
