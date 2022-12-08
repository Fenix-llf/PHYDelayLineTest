#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>
#include "mem.h"


#define MMAP_MEM_PAGEALIGN   (4*1024-1)
typedef struct
{
  unsigned int memAddr;
  unsigned int memSize;
  unsigned int mmapMemAddr;
  unsigned int mmapMemSize;  
  unsigned int memOffset;
 
  int    memDevFd;
  volatile unsigned int *pMemVirtAddr;
  
} MMapCtrl;
MMapCtrl gMMapCtrl;

int32 common_MMap(uint32_t physAddr, uint32_t memSize , uint32_t *pMemVirtAddr){
    gMMapCtrl.memDevFd = open("/dev/mem",O_RDWR|O_SYNC);
    if(gMMapCtrl.memDevFd < 0)
    {
      printf(" ERROR: /dev/mem open failed !!!\n");
      return -1;
    }
 
    gMMapCtrl.memOffset   = physAddr & MMAP_MEM_PAGEALIGN;
 
    gMMapCtrl.mmapMemAddr = physAddr - gMMapCtrl.memOffset;
 
    gMMapCtrl.mmapMemSize = memSize + gMMapCtrl.memOffset;
 
    gMMapCtrl.pMemVirtAddr = mmap(	
           (void	*)gMMapCtrl.mmapMemAddr,
           gMMapCtrl.mmapMemSize,
           PROT_READ|PROT_WRITE|PROT_EXEC,MAP_SHARED,
           gMMapCtrl.memDevFd,
           gMMapCtrl.mmapMemAddr
           );
 
   if (gMMapCtrl.pMemVirtAddr==NULL)
   {
     printf(" ERROR: mmap() failed !!!\n");
     return -1;
   }
    *pMemVirtAddr = (UInt32)((UInt32)gMMapCtrl.pMemVirtAddr + gMMapCtrl.memOffset);
 
    return 0;
}
 
Int32 common_unmapMem()
{
    if(gMMapCtrl.pMemVirtAddr)
      munmap((void*)gMMapCtrl.pMemVirtAddr, gMMapCtrl.mmapMemSize);
      
    if(gMMapCtrl.memDevFd >= 0)
      close(gMMapCtrl.memDevFd);
      
    return 0;
}

int devmem_init(uint32_t base, uint32_t offset ,devmem_data_t* data)
{
	data = malloc(sizeof(*data));
	if (!data) {
		perror("malloc fail!");
		return -1;	
	}

	data->fd = -1;
	data->reg = NULL;

	data->fd = open(DEVMEM_PATH, O_RDWR | O_SYNC);
	if (data->fd < 0) {
		perror("open devmem fail!");
		free(data);
		return -1;	
	}
	printf("111\n");
	data->reg = (uint32_t*) mmap(NULL, 4096, PROT_READ | PROT_READ | PROT_EXEC, MAP_SHARED, data->fd, offset);	
	printf("222\n");
out:
	return 0;
}

void devmem_deinit(devmem_data_t* data)
{
	if(data == NULL)
		return;
	if (data->reg != NULL)
		munmap(data->reg, 4);
	if (data->fd > 0)
		close(data->fd);
	free(data);
}

int devmem_write(devmem_data_t *data, uint32_t val)
{
	if(data->reg == NULL)
		return -1;
	*(data->reg) = val;	
	return 0;
}

uint32_t devmem_read(devmem_data_t *data)
{
	if(data->reg == NULL)
		return 0;
	return *(data->reg);
}
