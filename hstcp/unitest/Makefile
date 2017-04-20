all:clean testm
OBJS=main.o\
	 notify.o\
	 thread.o\
	 time_tools.o\
	 tcp_base_connect.o\
	 tcp_thread.o\
	 tcp_active_connect.o\
	 tcp_client_creator.o\
	 tcp_client.o\
	 tcp_reconnect_thread.o\
	 tcp_accept_connect.o\
	 tcp_accept_thread.o\
	 tcp_connect_pool.o\
	 tcp_server.o

LIBS=-lpthread -lc -lm -lstdc++ -lrt
SOURCES=..
INCLUDE=-I${SOURCES}
testm:${OBJS}
	g++ -o testm ${OBJS} ${LIBS}
main.o:main.cpp
	g++ -c -g main.cpp ${INCLUDE}
notify.o:../sys/notify.cpp
	g++ -c -g ../sys/notify.cpp ${INCLUDE}
thread.o:../sys/thread.cpp
	g++ -c -g ../sys/thread.cpp ${INCLUDE}
time_tools.o:../sys/time_tools.cpp
	g++ -c -g ../sys/time_tools.cpp ${INCLUDE}
tcp_base_connect.o:../base/tcp_base_connect.cpp
	g++ -c -g ../base/tcp_base_connect.cpp ${INCLUDE}
tcp_thread.o:../base/tcp_thread.cpp
	g++ -c -g ../base/tcp_thread.cpp ${INCLUDE}
tcp_client_creator.o:../tcp/client/tcp_client_creator.cpp
	g++ -c -g ../tcp/client/tcp_client_creator.cpp ${INCLUDE}
tcp_client.o:../tcp/client/tcp_client.cpp
	g++ -c -g ../tcp/client/tcp_client.cpp ${INCLUDE}
tcp_active_connect.o:../tcp/client/tcp_active_connect.cpp
	g++ -c -g ../tcp/client/tcp_active_connect.cpp ${INCLUDE}
tcp_reconnect_thread.o:../tcp/client/tcp_reconnect_thread.cpp
	g++ -c -g ../tcp/client/tcp_reconnect_thread.cpp ${INCLUDE}
tcp_accept_connect.o:../tcp/server/tcp_accept_connect.cpp
	g++ -c -g ../tcp/server/tcp_accept_connect.cpp ${INCLUDE}
tcp_accept_thread.o:../tcp/server/tcp_accept_thread.cpp
	g++ -c -g ../tcp/server/tcp_accept_thread.cpp ${INCLUDE}
tcp_connect_pool.o:../tcp/server/tcp_connect_pool.cpp
	g++ -c -g ../tcp/server/tcp_connect_pool.cpp ${INCLUDE}
tcp_server.o:../tcp/server/tcp_server.cpp
	g++ -c -g ../tcp/server/tcp_server.cpp ${INCLUDE}
clean:
	rm -rf *.o testm
