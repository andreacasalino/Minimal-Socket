## TCP samples

This folder stores some examples showing how to set up and use the objects inside **MinimalSocket** in order to use **tcp** connections.
Each sample requires to run in parallel multiple processes at the same time. 
This is automatically done by for you by the [Monitor.py](../utils/Monitor.py) script. Which in essence:
- reads a configuration file with the list of processes to run together with the options to pass and run them concurrently
- wait for all processes to complete and gather their outputs
- generate an **hmtl report** with the outputs taken from all processes

Indeed, **TcpSample01_server_client**, **TcpSample02_server_2_clients** and **TcpSample03_chain_with_2_repeaters** are special [**cmake custom targets**](https://cmake.org/cmake/help/latest/command/add_custom_target.html) that you can "compile" in order to run **Monitor.py** in the proper for executing and controlling the processes involved in the corresponfing sample. After the **hmtl report** is generated, you can inspect it with any browser. 

ATTENTION!!! The Samples execution might be blocked the first time by your firewall: set up properly your firewall or run the samples with the [administrator privileges](https://www.techopedia.com/definition/4961/administrative-privileges#:~:text=Administrative%20privileges%20are%20the%20ability,as%20a%20database%20management%20system.)

The above classes of samples can be described as follows:

- **TcpSample01_server_client**: 
    - related config file is [Sample01_server_client](./Sample01_server_client)
    - runs **TcpServer**, creating a tcp server that binds and listen to a specified port
    - runs **TcpClient**, creating a tcp client that connections to the previous server, exchanging messages with it. 
    - the following sequence diagram summarizes this sample
    ```mermaid
    sequenceDiagram
        TcpServer->>TcpServer: bind a port
        TcpClient->>TcpServer: ask for connection
        TcpServer->>TcpClient: connection done
        TcpClient->>TcpServer: request 1
        TcpServer->>TcpClient: response 1
        TcpClient->>TcpServer: request 2
        TcpServer->>TcpClient: response 2
    ```

- **TcpSample02_server_2_clients**: 
    - related config file is [Sample02_server_2_clients](./Sample02_server_2_clients)
    - runs **TcpServer**, creating a tcp server that binds and listen to a specified port
    - runs **TcpClient**, creating a first tcp client that connections to the previous server, exchanging messages with it. 
    - runs **TcpClient**, creating a second tcp client that connections to the previous server, exchanging messages with it with a different frequency. 
    - the following sequence diagram summarizes this sample
    ```mermaid
    sequenceDiagram
        TcpServer->>TcpServer: bind a port
        TcpClient1->>TcpServer: ask for connection
        TcpServer->>TcpClient1: connection done
        TcpClient2->>TcpServer: ask for connection
        TcpServer->>TcpClient2: connection done
        TcpClient1->>TcpServer: request 1
        TcpServer->>TcpClient1: response 1
        TcpClient2->>TcpServer: request 1
        TcpServer->>TcpClient2: response 1
        TcpClient1->>TcpServer: request 2
        TcpServer->>TcpClient1: response 2
    ```    

- **TcpSample03_chain_with_2_repeaters** has the aim of creating a chain of connected tcps. More in detail: 
    - related config file is [Sample03_chain_with_2_repeaters](./Sample03_chain_with_2_repeaters)
    - runs **TcpServer**, creating a tcp server that binds and listen to a specified port
    - runs a couple of **TcpRepeater**, creating a tcp client that connects to the previous process in the chain and a tcp server waitning for the connection request from the next element in the chain
    - runs **TcpClient**, connecting to the last spawned process of the chain
    The last client sends some requests, which are forward along the chain till the first server. This latter, sends a response that is backwarded along the chain till xoming back to the first client.
    - the following sequence diagram summarizes this sample
    ```mermaid
    sequenceDiagram
        TcpServer->>TcpServer: bind a port
        TcpRepeater1->>TcpRepeater1: bind a port
        TcpRepeater2->>TcpRepeater2: bind a port
        TcpRepeater1->>TcpServer: ask for connection
        TcpServer->>TcpRepeater1: connection done
        TcpRepeater2->>TcpRepeater1: ask for connection
        TcpRepeater1->>TcpRepeater2: connection done
        TcpClient->>TcpRepeater2: ask for connection
        TcpRepeater2->>TcpClient: connection done

        TcpClient->>TcpRepeater2: request 1
        TcpRepeater2->>TcpRepeater1: forward request 1
        TcpRepeater1->>TcpServer: forward request 1

        TcpServer->>TcpRepeater1: response 1
        TcpRepeater1->>TcpRepeater2: forawrd response 1
        TcpRepeater2->>TcpClient: forawrd response 1
    ```

- **TcpSample04_server_nn_block_2_clients** is an example of non blocking tcp server. The application uses one single thread to spin multiple connections. More in detail: 
    - related config file is [Sample04_server_nn_block_2_clients](./Sample04_server_nn_block_2_clients)
    - runs **TcpServerNonBlocking**, creating a tcp server that binds and listen to a specified port
    - runs **TcpClient**, creating a first tcp client that connections to the previous server, exchanging messages with it. 
    - runs **TcpClient**, creating a second tcp client that connections to the previous server, exchanging messages with it with a different frequency. 
    - the following sequence diagram summarizes this sample
    ```mermaid
    sequenceDiagram
        TcpServer->>TcpServer: bind a port
        TcpClient1->>TcpServer: ask for connection
        TcpServer->>TcpClient1: connection done
        TcpClient2->>TcpServer: ask for connection
        TcpServer->>TcpClient2: connection done
        TcpServer->>TcpServer: has something arrived from client 1? if so send response
        TcpServer->>TcpServer: has something arrived from client 2? if so send response
        TcpClient1->>TcpServer: request 1
        TcpServer->>TcpServer: has something arrived from client 1? if so send response
        TcpServer->>TcpServer: has something arrived from client 2? if so send response
        TcpServer->>TcpClient1: response 1
        TcpServer->>TcpServer: has something arrived from client 1? if so send response
        TcpServer->>TcpServer: has something arrived from client 2? if so send response
        TcpClient2->>TcpServer: request 1
        TcpServer->>TcpServer: has something arrived from client 1? if so send response
        TcpServer->>TcpServer: has something arrived from client 2? if so send response
        TcpServer->>TcpClient2: response 1
        TcpServer->>TcpServer: has something arrived from client 1? if so send response
        TcpServer->>TcpServer: has something arrived from client 2? if so send response
        TcpClient1->>TcpServer: request 2
        TcpServer->>TcpServer: has something arrived from client 1? if so send response
        TcpServer->>TcpServer: has something arrived from client 2? if so send response
        TcpServer->>TcpClient1: response 2

**TcpServer** and **TcpClient** can be also used as stand alone processes, in order to check connections locally or on a different host.
