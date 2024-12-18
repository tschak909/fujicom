/**
 * low level FujiNet API
 */

typedef struct		/* Command Frame */
{
	unsigned char ddev;		/* Destination Device */
	unsigned char dcomnd;		/* Command */
	unsigned char daux1;		/* Auxiliary Parameter 1 */
	unsigned char daux2;		/* Auxiliary Parameter 2 */
	unsigned char dcksum; 		/* 8-bit checksum */
} cmdFrame_t;

/**
 * @brief start fujicom
 * @param p COM port (1-4)
 */
void fujicom_init(unsigned char p);

/**
 * @brief calculate 8-bit checksum for cmdFrame_t.dcksum
 * @param buf Buffer to compute checksum against
 * @param len Length of aforementioned buffer
 */
unsigned char fujicom_cksum(unsigned char *buf, unsigned short len);

/**
 * @brief send FujiNet frame with no payload
 * @param cmdFrame Pointer to command frame
 * @return 'C'omplete, 'E'rror, or 'N'ak
 */
char fujicom_command(cmdFrame_t *c); 

/**
 * @brief send fujinet frame and read payload
 * @param cmdFrame pointer to command frame
 * @param buf Pointer to buffer to receive
 * @param len Expected buffer length
 */
char fujicom_command_read(cmdFrame_t *c, unsigned char *buf, unsigned short len);

/**
 * @brief send fujinet frame and write payload
 * @param cmdFrame pointer to command frame
 * @param buf pointer to buffer to send.
 * @param len Length of buffer to send.
 */
char fujicom_command_write(cmdFrame_t *c, unsigned char *buf, unsigned short len);

/** 
 * @brief end fujicom
 */
void fujicom_done(void);
