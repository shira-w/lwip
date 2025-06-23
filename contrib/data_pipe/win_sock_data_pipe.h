#pragma once

#include "data_pipe.h"
#include <string>

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class DataPipeWinSock : public DataPipe {
public:
    DataPipeWinSock(const std::string& address, bool is_server);
    ~DataPipeWinSock() override;

    void SendTo(const void* data, size_t size) override;
    void ReceiveFrom(void* buffer, size_t size) override;

private:
    bool is_server_;
    std::string full_address_;
    bool is_connected_;


    SOCKET socket_;
    sockaddr_in addr_;         // for server: bind address, for client: target
    sockaddr_in client_addr_;   // used when receiving
    int client_addr_len_;

    WSADATA wsa_data_;

};