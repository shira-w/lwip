
#include "win_sock_data_pipe.h"
//TODO: some uniform debugging (jclogger?)
#include <iostream>

DataPipeWinSock::DataPipeWinSock(const std::string& address, bool is_server)
	:is_server_(is_server),
    full_address_(address),
    is_connected_(false),
    socket_(INVALID_SOCKET),
    client_addr_len_(sizeof(client_addr_)) {
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data_);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return;
    }

    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_ == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;

    if (is_server_) {
        addr_.sin_port = htons(12323);
        addr_.sin_addr.s_addr = INADDR_ANY;

        if (bind(socket_, reinterpret_cast<SOCKADDR*>(&addr_), sizeof(addr_)) ==
            SOCKET_ERROR) {
            std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
            closesocket(socket_);
            WSACleanup();
            return;
        }
    }
    else {
        addr_.sin_port = htons(12323);
        inet_pton(AF_INET, address.c_str(), &addr_.sin_addr);
    }

    is_connected_ = true;
}

DataPipeWinSock::~DataPipeWinSock() {
    if (socket_ != INVALID_SOCKET) {
        closesocket(socket_);
    }
    WSACleanup();
}

void DataPipeWinSock::SendTo(const void* data, size_t size) {
    if(!is_connected_ || socket_ == INVALID_SOCKET) return;

    int result = sendto(socket_,
        static_cast<const char*>(data),
        static_cast<int>(size),
        0,
        reinterpret_cast<const sockaddr*>(&addr_),
        sizeof(addr_));

    if (result == SOCKET_ERROR) {
        std::cerr << "sendto failed: " << WSAGetLastError() << std::endl;
    }
}

void DataPipeWinSock::ReceiveFrom(void* buffer, size_t size) {
    if (!is_connected_ || socket_ == INVALID_SOCKET) return;

    int result = recvfrom(socket_,
        static_cast<char*>(buffer),
        static_cast<int>(size),
        0,
        reinterpret_cast<sockaddr*>(&client_addr_),
        &client_addr_len_);

    if (result == SOCKET_ERROR) {
        std::cerr << "recvfrom failed: " << WSAGetLastError() << std::endl;
    }
}



std::unique_ptr<DataPipe> DataPipeFactory::CreateDataPipe(const std::string& address, bool is_server) {
	return std::make_unique<DataPipeWinSock>(address, is_server);
}