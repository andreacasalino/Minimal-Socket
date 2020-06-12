g++ -o ./bin/Server ./src/Server.cpp ../../src/Stream_Socket.cpp -DMINGW_COMPILE  -lws2_32 
g++ -o ./bin/Client_A ./src/Client_A.cpp ../../src/Stream_Socket.cpp -DMINGW_COMPILE  -lws2_32 
g++ -o ./bin/Client_B ./src/Client_B.cpp ../../src/Stream_Socket.cpp -DMINGW_COMPILE  -lws2_32 

