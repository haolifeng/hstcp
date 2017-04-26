EXES=hstcp
B_HOME=.
INCDIR=-I$(B_HOME)

OUTDIR=$(B_HOME)/lib

TO=o
LIB_PRE=lib
LIB_EXT=.so

RM=rm -f

CC=g++
LD=$(CC)

CFLAGS = -c -g -Wall -fPIC $(DEFS)
LFLAGS = -shared
LIBPATH=-L$(B_HOME)/lib

STDLIBS=-lpthread -lc -lnsl -ldl -lm -lstdc++ -lrt

RMOBJ = -$(RM) *.$(TO)
OBJS=notify.$(TO)\
	 thread.$(TO)\
	 time_tools.$(TO)\
	 tcp_base_connect.$(TO)\
	 tcp_thread.$(TO)\
	 tcp_active_connect.$(TO)\
	 tcp_client_creator.$(TO)\
	 tcp_client.$(TO)\
	 tcp_reconnect_thread.$(TO)\
	 tcp_accept_connect.$(TO)\
	 tcp_accept_thread.$(TO)\
	 tcp_connect_pool.$(TO)\
	 tcp_server.$(TO)
	 
all:$(EXES) clean

notify.$(TO):./sys/notify.cpp
	$(CC) $(CFLAGS) ./sys/notify.cpp $(INCDIR)
thread.$(TO):./sys/thread.cpp
	$(CC) $(CFLAGS) ./sys/thread.cpp $(INCDIR)
time_tools.$(TO):./sys/time_tools.cpp
	$(CC) $(CFLAGS) ./sys/time_tools.cpp $(INCDIR)
tcp_base_connect.$(TO):./base/tcp_base_connect.cpp
	$(CC) $(CFLAGS) ./base/tcp_base_connect.cpp $(INCDIR)
tcp_thread.$(TO):./base/tcp_thread.cpp
	$(CC) $(CFLAGS) ./base/tcp_thread.cpp $(INCDIR)
tcp_active_connect.$(TO):./tcp/client/tcp_active_connect.cpp
	$(CC) $(CFLAGS) ./tcp/client/tcp_active_connect.cpp $(INCDIR)
tcp_client_creator.$(TO):./tcp/client/tcp_client_creator.cpp
	$(CC) $(CFLAGS) ./tcp/client/tcp_client_creator.cpp $(INCDIR)
tcp_client.$(TO):./tcp/client/tcp_client.cpp
	$(CC) $(CFLAGS) ./tcp/client/tcp_client.cpp $(INCDIR)
tcp_reconnect_thread.$(TO):./tcp/client/tcp_reconnect_thread.cpp
	$(CC) $(CFLAGS) ./tcp/client/tcp_reconnect_thread.cpp $(INCDIR)
tcp_accept_connect.$(TO):./tcp/server/tcp_accept_connect.cpp
	$(CC) $(CFLAGS) ./tcp/server/tcp_accept_connect.cpp $(INCDIR)
tcp_accept_thread.$(TO):./tcp/server/tcp_accept_thread.cpp
	$(CC) $(CFLAGS) ./tcp/server/tcp_accept_thread.cpp $(INCDIR)
tcp_connect_pool.$(TO):./tcp/server/tcp_connect_pool.cpp
	$(CC) $(CFLAGS) ./tcp/server/tcp_connect_pool.cpp $(INCDIR)
tcp_server.$(TO):./tcp/server/tcp_server.cpp
	$(CC) $(CFLAGS) ./tcp/server/tcp_server.cpp $(INCDIR)

$(EXES):$(OBJS)
	$(LD) $(LFLAGS) -o$(OUTDIR)/$(LIB_PRE)$(EXES)$(LIB_EXT) $(OBJS) $(STDLIBS) $(DEFLIBS) $(LIBPATH)

cleanobj:
	$(RMOBJ)
clean:cleanobj
	@echo clean all
	$(RMALL)

install:
	cp -rf $(B_HOME)/include/*  /usr/local/include/
	cp -rf $(B_HOME)/lib/*      /usr/local/lib/


