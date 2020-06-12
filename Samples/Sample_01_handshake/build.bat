g++ -o ./bin/Server ./src/Server.cpp ../../src/Stream_Socket.cpp -DMINGW_COMPILE  -lws2_32
g++ -o ./bin/Client ./src/Client.cpp ../../src/Stream_Socket.cpp -DMINGW_COMPILE  -lws2_32
