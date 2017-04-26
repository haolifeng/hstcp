all:client_sample service_sample
client_sample:client_sample.cpp
	g++ -o client_sample  -g client_sample.cpp -I/usr/local/include -L/usr/local/lib -lhstcp
service_sample:service_sample.cpp
	g++ -o service_sample -g service_sample.cpp -I/usr/local/include -L/usr/local/lib -lhstcp

clean:
	rm -rf *.o client_sample service_sample
