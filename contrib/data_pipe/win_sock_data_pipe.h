#pragma once

#include "data_pipe.h"
#include <string>

class DataPipeWinSock : public DataPipe {
public:
    DataPipeWinSock(const std::string& address, bool is_server);
    ~DataPipeWinSock() override;

    void SendTo(const void* data, size_t size) override;
    void ReceiveFrom(void* buffer, size_t size) override;

private:
    bool is_server_;
    std::string full_adress_;
    bool is_connected_;
};