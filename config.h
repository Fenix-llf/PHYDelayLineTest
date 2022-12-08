#ifndef __CONFIG_H__
#define __CONFIG_H__

#define TX_DELAY_REG_BASE   0x10000000
#define TX_DELAY_REG_OFFSET 0xe8
#define TX_DELAY_MASK  0x7f
#define TX_DELAY_SHIFT 12
#define TX_DELAY_START 0
#define TX_DELAY_END   0x7f

#define RX_DELAY_REG_BASE   0x10000000
#define RX_DELAY_REG_OFFSET 0xe8
#define RX_DELAY_MASK  0x7f
#define RX_DELAY_SHIFT 4
#define RX_DELAY_START 0
#define RX_DELAY_END   0x7f

#define TX_DELAY_MIN 0
#define RX_DELAY_MIN 0
#define TX_DELAY_MAX 0x7f
#define RX_DELAY_MAX 0x7f

#define PING_SERVER_IP "192.168.1.135"
#define PING_INET     "eth1"
#define PING_COUNT    3
#define PING_WAITTIME 3

#define LOG_PATH "/tmp/delayline.log"

#if __SIZEOF_POINTER__ == 4
#define reg_t uint32_t
#else 
#define reg_t uint64_t
#endif

#define TX_DELAY_REG  (TX_DELAY_REG_BASE + TX_DELAY_REG_OFFSET)
#define RX_DELAY_REG  (RX_DELAY_REG_BASE + RX_DELAY_REG_OFFSET)

#define SAME_REG_FLAG (TX_DELAY_REG == RX_DELAY_REG)

#endif // !__CONFIG_H__
