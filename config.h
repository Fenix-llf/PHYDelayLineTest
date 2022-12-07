#ifndef __CONFIG_H__
#define __CONFIG_H__

#define TX_DELAY_REG   0x100000e4
#define TX_DELAY_MASK  0x7f
#define TX_DELAY_SHIFT 12
#define TX_DELAY_START 0
#define TX_DELAY_END   0x7f

#define RX_DELAY_REG   0x100000e4
#define RX_DELAY_MASK  0x7f
#define RX_DELAY_SHIFT 4
#define RX_DELAY_START 0
#define RX_DELAY_END   0x7f

#define SERVER_IP "192.168.1.135"
#define PING_COUNT    3
#define PING_WAITTIME 3

#endif // !__CONFIG_H__
