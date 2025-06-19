
#include "win_sock_data_pipe.h"


DataPipeWinSock::DataPipeWinSock(const std::string& address, bool is_server)
	: is_server_(is_server), full_adress_(address), is_connected_(false) {
	// Constructor stub
}

DataPipeWinSock::~DataPipeWinSock() {
	// Destructor stub
}

void DataPipeWinSock::SendTo(const void* data, size_t size) {
	// SendTo implementation stub
}

void DataPipeWinSock::ReceiveFrom(void* buffer, size_t size) {
	// ReceiveFrom implementation stub
}



std::unique_ptr<DataPipe> DataPipeFactory::CreateDataPipe(const std::string& address, bool is_server) {
	return std::make_unique<DataPipeWinSock>(address, is_server);
}