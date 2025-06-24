#pragma once

#include <memory>
#include <string>

// Abstract Base Class
class DataPipe {
public:
    virtual ~DataPipe() = default;

    virtual int SendTo(const void* data, size_t size) = 0;
    virtual void ReceiveFrom(void* buffer, size_t size) = 0;
};

// Factory class
class DataPipeFactory {
public:
    static std::unique_ptr<DataPipe> CreateDataPipe(const std::string& address, bool is_server, int port);
};