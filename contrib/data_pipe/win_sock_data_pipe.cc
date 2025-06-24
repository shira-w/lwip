
#include "win_sock_data_pipe.h"
#include "data_pipe_c_api.h"
//TODO: some uniform debugging (jclogger?)
#include <iostream>
//TODO think about port
DataPipeWinSock::DataPipeWinSock(const std::string& address, bool is_server, int port)
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
    /*// Set destination address
	memset(&destAddr, 0, sizeof(destAddr));
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(12345);
	inet_pton(AF_INET, "192.168.41.3", &destAddr.sin_addr);*/
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;

    //think if have better way deal client-server
    if (is_server_) {
        addr_.sin_port = htons(port);
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
        addr_.sin_port = htons(port);
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

int DataPipeWinSock::SendTo(const void* data, size_t size) {
    if(!is_connected_ || socket_ == INVALID_SOCKET) return -2;

    int result = sendto(socket_,
        static_cast<const char*>(data),
        static_cast<int>(size),
        0,
        reinterpret_cast<const sockaddr*>(&addr_),
        sizeof(addr_));

    if (result == SOCKET_ERROR) {
        std::cerr << "sendto failed: " << WSAGetLastError() << std::endl;
    }
    return result;
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



std::unique_ptr<DataPipe> DataPipeFactory::CreateDataPipe(const std::string& address, bool is_server, int port) {
	return std::make_unique<DataPipeWinSock>(address, is_server,port);
}


datapip_handle datapip_create(const char* address, int is_server, int port) {
    auto pipe = DataPipeFactory::CreateDataPipe(address, is_server != 0, port);
    return new std::unique_ptr<DataPipe>(std::move(pipe));
}

void datapip_destroy(datapip_handle h) {
    delete static_cast<std::unique_ptr<DataPipe>*>(h);
}

int datapip_sendto(datapip_handle h, const void* data, size_t size) {
    auto* ptr = static_cast<std::unique_ptr<DataPipe>*>(h);
    return (*ptr)->SendTo(data, size);
}

void datapip_receivefrom(datapip_handle h, void* buffer, size_t size) {
    auto* ptr = static_cast<std::unique_ptr<DataPipe>*>(h);
    (*ptr)->ReceiveFrom(buffer, size);
}
