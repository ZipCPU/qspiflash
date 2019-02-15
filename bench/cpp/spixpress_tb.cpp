////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	spixpress_tb.cpp
//
// Project:	A Set of Wishbone Controlled SPI Flash Controllers
//
// Purpose:	To determine whether or not the spixpress (SPI only) module
// 		works.  Run the simulation program this with no arguments,
// 	and then check whether or not the last line contains "SUCCESS" or not.
// 	If it does contain "SUCCESS", then the module passes all tests found
// 	within here.
//
// Creator:	Dan Gisselquist, Ph.D.
//		Gisselquist Technology, LLC
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015-2016,2018-2019 Gisselquist Technology, LLC
//
// This file is part of the set of Wishbone controlled SPI flash controllers
// project
//
// The Wishbone SPI flash controller project is free software (firmware):
// you can redistribute it and/or modify it under the terms of the GNU Lesser
// General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// The Wishbone SPI flash controller project is distributed in the hope
// that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTIBILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  (It's in the $(ROOT)/doc directory.  Run make
// with no target there if the PDF file isn't present.)  If not, see
// <http://www.gnu.org/licenses/> for a copy.
//
// License:	LGPL, v3, as defined and found on www.gnu.org,
//		http://www.gnu.org/licenses/lgpl.html
//
//
////////////////////////////////////////////////////////////////////////////////
//
//
#include "verilated.h"
#include "Vspixpress.h"
#include "byteswap.h"
#include "flashsim.h"
#include "wbflash_tb.h"

#define	PARENT	WBFLASH_TB<Vspixpress>

#define	LGFLASHSZB	24

#define NPAGES		256
#define SZPAGEB		256
#define PGLENB		SZPAGEB
#define SZPAGEW		(SZPAGEB>>2)
#define PGLENW		SZPAGEW
#define SECTORSZW	(NPAGES * SZPAGEW)
#define SECTORSZB	(NPAGES * SZPAGEB)
#define	RDBUFSZ		(NPAGES * SZPAGEW)
#define	NSECTORS	((1<<LGFLASHSZB)/SECTORSZB)
#define	SECTOROF(A)	((A)&(-1<<16))
#define	SUBSECTOROF(A)	((A)&(-1<<12))
#define	PAGEOF(A)	((A)&(-1<< 8))

static const unsigned	CFG_USERMODE  = 0x1000,
	     		CFG_USER_CS_n = 0x0100;

static const unsigned	F_RESET = (CFG_USERMODE|0x0ff),
			F_EMPTY = (CFG_USERMODE|0x000),
			F_WRR   = (CFG_USERMODE|0x001),
			F_PP    = (CFG_USERMODE|0x002),
			F_QPP   = (CFG_USERMODE|0x032),
			F_READ  = (CFG_USERMODE|0x003),
			F_WRDI  = (CFG_USERMODE|0x004),
			F_RDSR1 = (CFG_USERMODE|0x005),
			F_WREN  = (CFG_USERMODE|0x006),
			F_MFRID = (CFG_USERMODE|0x09f),
			F_SE    = (CFG_USERMODE|0x0d8),
			F_END   = (CFG_USERMODE|CFG_USER_CS_n);
static const unsigned	F_READID = F_MFRID,
	     		F_RDSR   = F_RDSR1;


class	SPIXPRESS_TB : public PARENT {
	FLASHSIM	*m_flash;
	bool		m_bomb;
	int		m_flash_last_sck;
public:

	SPIXPRESS_TB(void) {
		m_core = new Vspixpress;
		m_flash= new FLASHSIM;
		m_flash->debug(true);
		m_flash_last_sck = 0;
	}

	unsigned operator[](const int index) { return (*m_flash)[index]; }
	void	setflash(unsigned addr, unsigned v) {
		m_flash->set(addr, v);
	}
	void	load(const char *fname) {
		m_flash->load(0,fname);
	}

	void	set(const unsigned addr, const unsigned val) {
		m_flash->set(addr, val);
	}

	void	tick(void) {
		bool	writeout = false;
		if (m_flash_last_sck) {
			(*m_flash)(m_core->o_spi_cs_n, 0,
				m_core->o_spi_mosi);
		} m_core->i_spi_miso = ((*m_flash)(m_core->o_spi_cs_n, 1,
				m_core->o_spi_mosi)&2)?1:0;
		m_flash_last_sck = m_core->o_spi_sck;


		if (writeout) {
			printf("%08lx-WB: %s %s/%s %s %s",
				m_tickcount,
				(m_core->i_wb_cyc)?"CYC":"   ",
				(m_core->i_wb_stb)?"DSTB":"    ",
				(m_core->i_cfg_stb)?"CSTB":"    ",
				(m_core->o_wb_stall)?"STALL":"     ",
				(m_core->o_wb_ack)?"ACK":"   ");
			printf(" %s@0x%08x[%08x/%08x]",
				(m_core->i_wb_we)?"W":"R",
				(m_core->i_wb_addr), (m_core->i_wb_data),
				(m_core->o_wb_data));

			printf("\n");
		}

		PARENT::tick();
	}

	bool	bombed(void) const { return m_bomb; }

	unsigned flreadid(void) {
		unsigned	r;

		cfg_write(F_READID);

		cfg_write(0); r = cfg_read() & 0x0ff;
		cfg_write(0); r = (r<<8) | (cfg_read() & 0x0ff);
		cfg_write(0); r = (r<<8) | (cfg_read() & 0x0ff);
		cfg_write(0); r = (r<<8) | (cfg_read() & 0x0ff);
		cfg_write(F_END);
		return r;
	}

	int	flstatus(void) {
		cfg_write(F_RDSR);
		cfg_write(0);
		cfg_write(F_END);
		return cfg_read() & 0x0ff;
	}

	void	flwait(void) {
		int	r;

		printf("Waiting for the erase/program cycle to complete\n");
		cfg_write(F_RDSR);
		do {
			cfg_write(0);
			r = cfg_read();
		} while (r & 1); // Wait while the device is busy
		cfg_write(F_END);
		printf(" ... Completed!\n");
	}

	void	flerase(unsigned sectoraddr) {
		cfg_write(F_END);
		cfg_write(F_WREN);
		cfg_write(F_END);

		cfg_write(F_SE);
		cfg_write((sectoraddr >> 16)&0x0ff);
		cfg_write((sectoraddr >>  8)&0x0ff);
		cfg_write((sectoraddr      )&0x0ff);
		cfg_write(F_END);

		flwait();
	}

	void	flpage_program(int addr, int ln, const char *buf) {
		flwait();

		printf("Page program, address = %06x, ln = %d\n", addr, ln);
		tick();

		cfg_write(F_END);
		cfg_write(F_WREN);
		cfg_write(F_END);

		tick();

		cfg_write(F_PP);
		cfg_write((addr >> 16)&0x0ff);
		cfg_write((addr >>  8)&0x0ff);
		cfg_write((addr      )&0x0ff);

		// Write the page data itself
		for(int i=0; i<ln; i++)
			cfg_write(buf[i] & 0x0ff);
		cfg_write(F_END);

		tick();

		flwait();

		tick();
	}

	void	flprogram(int addr, int ln, const char *buf) {
		int	start = addr;
		printf("PROGRAM-REQUEST!!\n");
		while(start < addr + ln) {
			int	wlen;

			if (PAGEOF(addr+ln-1)!=PAGEOF(start))
				wlen = PAGEOF(start+PGLENB)-start;
			else
				wlen = addr+ln-start;

			flpage_program(start, wlen, &buf[start-addr]);
			start = PAGEOF(start+PGLENB);
		}
		flwait();
	}
};

int main(int  argc, char **argv) {
	Verilated::commandArgs(argc, argv);
	SPIXPRESS_TB	*tb = new SPIXPRESS_TB;
	const char	*DEV_RANDOM = "/dev/urandom";
	unsigned	rdv;
	unsigned	*rdbuf;

	tb->opentrace("spixpress.vcd");

	tb->load(DEV_RANDOM);
	rdbuf = new unsigned[RDBUFSZ];
	tb->setflash(0,0);

	tb->tick();
	rdv = tb->wb_read(0);
	printf("READ[0] = %04x\n", rdv);
	if (rdv != 0)
		goto test_failure;

	tb->tick();
	if (tb->bombed())
		goto test_failure;

	for(int i=0; (i<1000)&&(!tb->bombed()); i++) {
		unsigned	tblv;
		tblv = (*tb)[i];
		rdv = tb->wb_read(i<<2);

		if(tblv != rdv) {
			printf("BOMB(INITIAL/SINGLE-READ): READ[%08x] %08x, EXPECTED %08x\n",
				(i<<2), rdv, tblv);
			goto test_failure;
			break;
		} else printf("MATCH: %08x == %08x\n", rdv, tblv);
	}

	printf("SINGLE-READ TEST PASSES\n");

	for(int i=0; i<1000; i++)
		rdbuf[i] = -1;
	tb->wb_read(1000<<2, 1000, rdbuf);
	if (tb->bombed())
		goto	test_failure;
	for(int i=0; i<1000; i++) {
		if ((*tb)[i+1000] != rdbuf[i]) {
			printf("BOMB: READ.1[%08x] %08x, EXPECTED %08x\n", 1000+i, rdbuf[i], (*tb)[i+1000]);
			goto	test_failure;
		} // else printf("MATCH: READ.1[%08x] %08x == %08x\n",
		//		i+1000, rdbuf[i], (*tb)[i+1000]);
	} if (tb->bombed())
		goto test_failure;
	printf("VECTOR TEST PASSES!\n");

	// Read the status register
	printf("Status Register = 0x%02x\n", rdv = tb->flstatus());
	assert(rdv == 0x1c);
	printf("ID     Register = 0x%08x\n", rdv = tb->flreadid());
	{	extern const unsigned DEVID;
		assert(rdv == DEVID);
	}

	// Make sure, for testing purposes, that the words preceeding the
	// sector we are going to erase and following it don't look like they've
	// already been erased.
	if ((*tb)[SECTORSZW-1] == 0xffffffff)
		tb->set(SECTORSZW-1, 0);
	if ((*tb)[2*SECTORSZW] == 0xffffffff)
		tb->set(2*SECTORSZW, 0);

	printf("Attempting to erase subsector 1\n");
	// tb->wb_ctrl_write(0, ERASEFLAG | (1*SECTORSZW));

	tb->flerase(SECTORSZB);

	printf("Checking that the erase was successful\n");
	for(int i=SECTORSZB; i<SECTORSZB*2; i+=4) {
		if ((*tb)[i>>2] != 0xffffffff) {
			printf("BOMB: Erase of [%08x] was unsuccessful, FLASH[%08x] = %08x\n", i, i, (*tb)[i]);
			goto test_failure;
		}
	}

	// Make sure we didn't erase anything else
	if ((*tb)[SECTORSZW-1] == 0xffffffff) {
		printf("BOMB: Post write check #2, the prior address changed\n");
		goto test_failure;
	} if ((*tb)[2*SECTORSZB] == 0xffffffff) {
		printf("BOMB: Post write check #2, the next address changed\n");
		goto test_failure;
	}

	if (tb->wb_read(SECTORSZB-4) != (*tb)[SECTORSZW-1]) {
		printf("BOMB: Post write check #2, the prior address changed\n");
		goto test_failure;
	} if (tb->wb_read(2*SECTORSZB) != (*tb)[2*SECTORSZW]) {
		printf("BOMB: Post write check #2, the next address changed\n");
		goto test_failure;
	}



	printf("Test: Trying a single word write\n");

	// Try to execute a single write
	{	char	buf[5];
		buf[0] = 0x12;
		buf[1] = 0x34;
		buf[2] = 0x56;
		buf[3] = 0x78;
		tb->flprogram(SECTORSZB, 4, buf);
	}

	if (tb->wb_read(SECTORSZB) != 0x12345678) {
		printf("BOMB: Single (not page) write result incorrect: %08x != 0x12345678\n", tb->wb_read(SECTORSZB));
		goto test_failure;
	}


	// Let's load up a sectors worth of random data into our buffer
	{
		FILE	*fp;
		fp = fopen(DEV_RANDOM, "r");
		assert(RDBUFSZ == fread(rdbuf, sizeof(unsigned), RDBUFSZ, fp));
		fclose(fp);
		// To keep our data consistent, make sure the first byte matches
		// what was written before
		rdbuf[0] = byteswap(0x12345678);
	}

	// Now, let's try writing this sector ... one page at a time.
	for(int p=0; p<NPAGES; p++) {

		printf("Writing page %d\n", p);
		tb->flprogram(SECTORSZB+p*SZPAGEB, SZPAGEB,
				(const char *)&rdbuf[p*SZPAGEW]);
		byteswapbuf(SZPAGEW, &rdbuf[p*SZPAGEW]);

		printf("Checking page %d\n", p);
		for(int i=0; i<SZPAGEW; i++) {
			if (rdbuf[p*SZPAGEW+i] != (*tb)[SECTORSZW+p*SZPAGEW+i]) {
				printf("BOMB: Write check, Addr[%08x], "
					"read %08x expected %08x\n",
					SECTORSZB+p*SZPAGEB+(i<<2),
					rdbuf[p*SZPAGEW+i],
					(*tb)[SECTORSZW+p*SZPAGEW+i]);
				goto test_failure;
			}
		}
	}

	printf("SUCCESS!!\n");
	exit(EXIT_SUCCESS);
test_failure:
	printf("FAIL-HERE\n");
	for(int i=0; i<8; i++)
		tb->tick();
	printf("TEST FAILED\n");
	exit(EXIT_FAILURE);
}
