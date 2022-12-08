#ifndef __MEM_H__
#define __MEM_H__

#include <stdint.h>
#define DEVMEM_PATH "/dev/mem"


typedef struct devmem_data {
	int fd;
	uint32_t *reg;
}devmem_data_t;

int devmem_init(uint32_t addr, uint32_t offset, devmem_data_t* data);
void devmem_deinit(devmem_data_t *data);
int devmem_write(devmem_data_t *data, uint32_t val);
uint32_t devmem_read(devmem_data_t *data);

#endif 
