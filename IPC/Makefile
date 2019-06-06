
BuildOpt = -g -std=c++11
objService = service.o
objClient = msgClient.o

all : service client

rebulid: clean service client

service : $(objService)
	g++ -o service $(objService)

client: $(objClient)
	g++ -o client $(objClient)

service.o : log.h ipc.h
	g++ -c -g -std=c++11 service.cpp

msgClient.o : log.h ipc.h
	g++ -c -g -std=c++11 msgClient.cpp

.PHONY : clean
clean :
	rm $(objService) $(objClient) service client