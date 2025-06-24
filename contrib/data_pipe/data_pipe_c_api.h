// datapip_c_api.h
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	typedef void* datapip_handle;

	datapip_handle datapip_create(const char* address, int is_server, int port);
	void datapip_destroy(datapip_handle h);
	int datapip_sendto(datapip_handle h, const void* data, size_t size);
	void datapip_receivefrom(datapip_handle h, void* buffer, size_t size);

#ifdef __cplusplus
}
#endif
