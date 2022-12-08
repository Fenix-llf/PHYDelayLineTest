#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include "config.h"
#include "mem.h"
#include "ping.h"

static devmem_data_t *tx_delay_reg;
static devmem_data_t *rx_delay_reg;

static FILE *log_fp = NULL;

int log_init(void)
{
	log_fp = fopen(LOG_PATH, "w+");
	if (!log_fp) {
		return -1;
	}
	return 0;
}

void log_deinit(void)
{
	if(log_fp)
		fclose(log_fp);
}

#define write_log(fmt,...) fprintf(log_fp, fmt, ##__VA_ARGS__)

int reg_init(void)
{
	int ret = -1;
	
	ret = devmem_init(TX_DELAY_REG_BASE, TX_DELAY_REG_OFFSET, tx_delay_reg);
	if (!SAME_REG_FLAG) 
		ret += devmem_init(RX_DELAY_REG_BASE, RX_DELAY_REG_OFFSET, rx_delay_reg);

	if (ret < 0) {
		perror("reg init failed");
		return ret;
	}

	return 0;
}

void reg_deinit(void)
{
	devmem_deinit(tx_delay_reg);	
	if (!SAME_REG_FLAG) {
		devmem_deinit(rx_delay_reg);
	}
}

int set_delay(uint32_t tx_delay, uint32_t rx_delay)
{
	if ((rx_delay > RX_DELAY_MAX) ||
		(rx_delay < RX_DELAY_MIN) ||
		(tx_delay > TX_DELAY_MAX) ||
		(tx_delay < TX_DELAY_MIN)) {
		return -1;
	}
		
	uint32_t tx_reg_value = 0;
	uint32_t rx_reg_value = 0;

	tx_reg_value = devmem_read(tx_delay_reg);
	tx_reg_value &= ~(TX_DELAY_MASK << TX_DELAY_SHIFT);
	tx_reg_value |= (tx_delay & TX_DELAY_MASK) << TX_DELAY_SHIFT;
	devmem_write(tx_delay_reg, tx_reg_value);
	if (!SAME_REG_FLAG) {
		rx_reg_value = devmem_read(rx_delay_reg);
		rx_reg_value &= ~(RX_DELAY_MASK << RX_DELAY_SHIFT);
		rx_reg_value |= (rx_delay & RX_DELAY_MASK) << RX_DELAY_SHIFT;
		devmem_write(rx_delay_reg, rx_reg_value);
	} else {
		rx_reg_value = devmem_read(tx_delay_reg);
		rx_reg_value &= ~(RX_DELAY_MASK << RX_DELAY_SHIFT);
		rx_reg_value |= (rx_delay & RX_DELAY_MASK) << RX_DELAY_SHIFT;
		devmem_write(tx_delay_reg, rx_reg_value);
	}
	return 0;
}

int main(void)
{
	int ret = -1;
	int status = 1;	
	ret = reg_init();
	if(ret) {
		printf("reg init err!\n");
		return -1;
	}
	ret = log_init();
	if(ret) {
		printf("log init err!\n");
		goto log_init_err;
		return -1;
	}

	uint32_t i = TX_DELAY_START;
	uint32_t j = RX_DELAY_START;
	
	for (;i <= TX_DELAY_END; i++) {
		write_log("TX(%2x): ", i);
		for (;j <= RX_DELAY_END; j++) {
			ret = set_delay(i, j);
			if (ret) {
				printf("set_delay faile");
				goto set_delay_err;
			}
			status = ping_server(PING_SERVER_IP, PING_INET, PING_WAITTIME, PING_COUNT);
			write_log("%x", status);
		}
		write_log("\n");
	}

	log_deinit();
	reg_deinit();

	return 0;

set_delay_err:
	log_deinit();
log_init_err:
	reg_deinit();

	return -1;
}
