g++ -o ./bin/Server ./src/Server.cpp ../../src/Stream_Socket.cpp -lpthread -lpthread
g++ -o ./bin/Client_A ./src/Client_A_slow.cpp ../../src/Stream_Socket.cpp
g++ -o ./bin/Client_B ./src/Client_B_fast.cpp ../../src/Stream_Socket.cpp
