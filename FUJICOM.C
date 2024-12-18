/**
 * #FUJINET Low Level Routines
 */

#include "comport.h"
#include "fujicom.h"

void fujicom_init(unsigned char p)
{
	comport_init(p);
}

unsigned char fujicom_cksum(unsigned char *buf, unsigned short len)
{
	unsigned int chk = 0;
	int i=0;

	for (i=0;i<len;i++)
		chk = ((chk+buf[i]) >> 8) + ((chk + buf[i]) & 0xFF);

	return (unsigned char)chk;
}

/**
 * @brief Internal function, send command, get response.
 *
 * @param c ptr to command frame to send
 * @return 'A'ck, or 'N'ak.
 */
char _fujicom_send_command(cmdFrame_t *c)
{
	int i;
	unsigned char *cc = (unsigned char *)c;

	/* Calculate checksum and place in frame */
	c->dcksum = fujicom_cksum(cc,4);

	/* Assert DTR to indicate start of command frame */
	comport_dtr(1);

	/* Write command frame */
	comport_write_buffer(cc);

	/* Wait for frame to complete */
	while (comport_outque_pos())
	{
	}

	delay(1);

	/* Desert DTR to indicate end of command frame */
	comport_dtr(0);

	return comport_read_sync();
}

char fujicom_command(cmdFrame_t *c)
{
	_fujicom_send_command(c);

	return comport_read_sync();
}

char fujicom_command_read(cmdFrame_t *c, unsigned char *buf, unsigned short len)
{
	unsigned char r; /* response */
	int i;

	r = _fujicom_send_command(c);

	printf("Command %02x sent, r: %c\n",c->dcomnd,r);

	if (r == 'N')
		return r; /* Return NAK */

	/* Get COMPLETE/ERROR */

	r = comport_read_sync();

	if (r == 'C')
	{
		/* Complete, get payload */

		for (i=0;i<len;i++)
			buf[i]=comport_read();

		/* Get Checksum byte, we don't use it. */
		comport_read_sync();
	}

	/* Otherwise, we got an error, return it. */
	return r;
}

char fujicom_command_write(cmdFrame_t *c, unsigned char *buf, unsigned short len)
{
	unsigned char r; /* response */
	int i;
	unsigned char ck;

	r = _fujicom_send_command(c);

	if (r == 'N')
		return r;

	/* Write the payload */
	comport_write_buffer(buf,len);

	/* Write the checksum */
	ck=fujicom_cksum(buf,len);
	comport_write_buffer(&ck,1);

	/* Wait for ACK/NACK */
	r = comport_read_sync();
	
	if (r == 'N')
		return r;

	/* Wait for COMPLETE/ERROR */
	return comport_read_sync();	
}

void fujicom_done(void)
{
	comport_done();
}
