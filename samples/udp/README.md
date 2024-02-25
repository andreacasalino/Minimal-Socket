## UDP samples

This folder stores some examples showing how to set up and use the objects inside **MinimalSocket** in order to use **udp** connections.
Each sample requires to run in parallel multiple processes at the same time. 
This is automatically done by for you by the [Monitor.py](../utils/Monitor.py) script. Which in essence:
- reads a configuration file with the list of processes to run together with the options to pass and run them concurrently
- wait for all processes to complete and gather their outputs
- generate an **hmtl report** with the outputs taken from all processes

Indeed **UdpSample01_asker_responder**, **UdpSample02_asker_connected_responer** and **UdpSample03_2_askers_responder** are special [**cmake custom targets**](https://cmake.org/cmake/help/latest/command/add_custom_target.html) that you can "compile" in order to run **Monitor.py** in the proper for executing and controlling the processes involved in the corresponfing sample. After the **hmtl report** is generated, you can inspect it with any browser. 

ATTENTION!!! The Samples execution might be blocked the first time by your firewall: set up properly your firewall or run the samples with the [administrator privileges](https://www.techopedia.com/definition/4961/administrative-privileges#:~:text=Administrative%20privileges%20are%20the%20ability,as%20a%20database%20management%20system.)

The above classes of samples can be described as follows:

- **UdpSample01_asker_responder**: 
    - related config file is [Sample01_asker_responder](./Sample01_asker_responder)
    - runs **UdpResponder**, creating a udp socket that binds a specified port
    - runs **UdpAsker**, creating a udp socket that binds another port and exchanges messages with the first spawned udp socket. 
    - the following sequence diagram summarizes this sample
    ```mermaid
    sequenceDiagram
        UdpResponder->>UdpResponder: bind a port
        UdpAsker->>UdpAsker: bind a port
        UdpAsker->>UdpResponder: request 1
        UdpResponder->>UdpAsker: response 1
        UdpAsker->>UdpResponder: request 2
        UdpResponder->>UdpAsker: response 2
    ```

- **UdpSample02_asker_connected_responer**: 
    - related config file is [Sample02_asker_connected_responer](./Sample02_asker_connected_responer)
    - runs **UdpResponder**, creating a udp socket that binds a specified port, then "connects" (this is an udp so connect means only to filter out messages incoming from other not connected upds) to the first udp socket reaching it.
    - runs **UdpAsker**, creating a udp socket that binds another port and exchanges messages with the first spawned udp socket. 
    After the connection, the first socket can't exchange messages with other peers. You can verify this by running a second **UdpAsker** sending requests to the port specified for the first **UdpResponder**: they will be always not satisfied.
    - the following sequence diagram summarizes this sample
    ```mermaid
    sequenceDiagram
        UdpResponder->>UdpResponder: bind a port
        UdpAsker->>UdpAsker: bind a port
        UdpAsker->>UdpResponder: request 1
        UdpResponder->>UdpResponder: "connect" address of UdpAsker
        UdpResponder->>UdpAsker: response 1
        UdpAsker->>UdpResponder: request 2
        UdpResponder->>UdpAsker: response 2
    ```

- **UdpSample03_2_askers_responder**:
    - related config file is [Sample03_2_askers_responder](./Sample03_2_askers_responder)
    - runs **UdpResponder**, creating a udp socket that binds a specified port
    - runs **UdpAsker**, creating a second udp socket that binds another port and exchanges messages with the first spawned udp socket. 
    - runs **UdpAsker**, creating a third udp socket that binds another port and exchanges messages with the first spawned udp socket.
    The socket created in **UdpResponder** is able fufll the requests of both clients, as it is an "un-connected" udp socket, i.e. accepts incoming messages from anyone. On the opposite, the one created in **UdpSample02_asker_connected_responer** connects to the first peer sending a message and that's why it would not be able to answer the requests of a third udp socket.
    - the following sequence diagram summarizes this sample
    ```mermaid
    sequenceDiagram
        UdpResponder->>UdpResponder: bind a port
        UdpAsker1->>UdpAsker1: bind a port
        UdpAsker2->>UdpAsker2: bind a port
        UdpAsker1->>UdpResponder: request 1
        UdpResponder->>UdpAsker1: response 1
        UdpAsker2->>UdpResponder: request 1
        UdpResponder->>UdpAsker2: response 1
        UdpAsker1->>UdpResponder: request 2
        UdpResponder->>UdpAsker1: response 2
    ```

- **UdpSample04_2_askers_2_nn_block_responders**: is an example of non blocking udp. The application uses one single thread to spin multiple connections. More in detail: 
    - related config file is [Sample04_2_askers_2_nn_block_responders](./Sample04_2_askers_2_nn_block_responders)
    - runs **UdpResponderNonBlocking**, creating two non blocking udp sockets that binds two distinct specified ports. Then, a single thread is used to spin such sockets, checking, at each iteration and one socket at a time, if something was received and eventually respond
    - runs **UdpAsker**, creating a udp socket that binds another port and exchanges messages with one of the two udp spawned in **UdpResponderNonBlocking**
    - runs another **UdpAsker**, creating a udp socket that binds another port and exchanges messages with the other udp spawned in **UdpResponderNonBlocking**
    - the following sequence diagram summarizes this sample
    ```mermaid
    sequenceDiagram
        UdpResponder->>UdpResponder: bind port_A
        UdpResponder->>UdpResponder: bind port_B
        UdpAsker->>UdpAsker: bind a port
        UdpResponder->>UdpResponder: was a message delivered to the first udp? if so send response
        UdpResponder->>UdpResponder: was a message delivered to the second udp? if so send response
        UdpAsker->>UdpResponder: request 1
        UdpResponder->>UdpResponder: was a message delivered to the first udp? if so send response
        UdpResponder->>UdpResponder: was a message delivered to the second udp? if so send response
        UdpResponder->>UdpAsker: response 1
        UdpResponder->>UdpResponder: was a message delivered to the first udp? if so send response
        UdpResponder->>UdpResponder: was a message delivered to the second udp? if so send response
        UdpAsker->>UdpResponder: request 2
        UdpResponder->>UdpResponder: was a message delivered to the first udp? if so send response
        UdpResponder->>UdpResponder: was a message delivered to the second udp? if so send response
        UdpResponder->>UdpAsker: response 2
    ```

**UdpAsker** and **UdpResponder** can be also used as stand alone processes, in order to check connections on local processes or the ones stored in a different host. Check the sources (or the scripts generated by **UdpScriptsGenerator**) for the syntax of the accepted arguments.

