////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	qspiflash_tb.cpp
//
// Project:	Wishbone Controlled Quad SPI Flash Controller
//
// Purpose:	To determine whether or not the qspiflash module works.  Run
//		this with no arguments, and check whether or not the last line
//	contains "SUCCESS" or not.  If it does contain "SUCCESS", then the
//	module passes all tests found within here.
//
// Creator:	Dan Gisselquist, Ph.D.
//		Gisselquist Technology, LLC
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015-2016, Gisselquist Technology, LLC
//
// This program is free software (firmware): you can redistribute it and/or
// modify it under the terms of  the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program.  (It's in the $(ROOT)/doc directory.  Run make with no
// target there if the PDF file isn't present.)  If not, see
// <http://www.gnu.org/licenses/> for a copy.
//
// License:	GPL, v3, as defined and found on www.gnu.org,
//		http://www.gnu.org/licenses/gpl.html
//
//
////////////////////////////////////////////////////////////////////////////////
//
//
#include "verilated.h"
#include "Vwbqspiflash.h"
#include "qspiflashsim.h"
#include "wbflash_tb.h"

#define	QSPIFLASH	0x0400000
#define	PARENT	WBFLASH_TB<Vwbqspiflash>

class	QSPIFLASH_TB : public PARENT {
	QSPIFLASHSIM	*m_flash;
	bool		m_bomb;
public:

	QSPIFLASH_TB(void) {
		m_core = new Vwbqspiflash;
		m_flash= new QSPIFLASHSIM;
		m_flash->debug(true);
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
		m_core->i_qspi_dat = (*m_flash)(m_core->o_qspi_cs_n,
			m_core->o_qspi_sck, m_core->o_qspi_dat);


		if (writeout) {
			printf("%08lx-WB: %s %s/%s %s %s",
				m_tickcount,
				(m_core->i_wb_cyc)?"CYC":"   ",
				(m_core->i_wb_data_stb)?"DSTB":"    ",
				(m_core->i_wb_ctrl_stb)?"CSTB":"    ",
				(m_core->o_wb_stall)?"STALL":"     ",
				(m_core->o_wb_ack)?"ACK":"   ");
			printf(" %s@0x%08x[%08x/%08x]",
				(m_core->i_wb_we)?"W":"R",
				(m_core->i_wb_addr), (m_core->i_wb_data),
				(m_core->o_wb_data));
			printf(" QSPI:%x:%x/%02x/%02x/%2d",
				m_core->i_qspi_dat, m_core->o_qspi_mod,
				m_core->v__DOT__state,
				m_core->v__DOT__lldriver__DOT__state,
				m_core->v__DOT__lldriver__DOT__spi_len);
			printf(" %08x/%08x", m_core->v__DOT__spi_in,
				m_core->v__DOT__lldriver__DOT__r_input);
			printf(" %d,%d,%d/%d,%08x%c", 
				m_core->v__DOT__spi_busy,
				m_core->v__DOT__spi_valid,
				m_core->v__DOT__spi_wr,
				m_core->v__DOT__spi_len,
				m_core->v__DOT__spi_out,
				(m_core->v__DOT__write_in_progress)?'W':' ');
	
			printf("\n");
		}

		PARENT::tick();
	}

	bool	bombed(void) const { return m_bomb; }

};

#define ERASEFLAG	0x80000000
#define DISABLEWP	0x10000000
#define ENABLEWP	0x00000000
#define NPAGES		256
#define SZPAGEB		256
#define SZPAGEW		(SZPAGEB>>2)
#define SECTORSZW	(NPAGES * SZPAGEW)
#define SECTORSZB	(NPAGES * SZPAGEB)
#define	RDBUFSZ		(NPAGES * SZPAGEW)

int main(int  argc, char **argv) {
	Verilated::commandArgs(argc, argv);
	QSPIFLASH_TB	*tb = new QSPIFLASH_TB;
	const char	*DEV_RANDOM = "/dev/urandom";
	unsigned	rdv;
	unsigned	*rdbuf;

	// tb->opentrace("qspi.vcd");

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
		tblv = (*tb)[(i<<2)];
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
	tb->wb_read(1000, 1000, rdbuf);
	if (tb->bombed())
		goto	test_failure;
	for(int i=0; i<1000; i++) {
		if ((*tb)[(i<<2)+1000] != rdbuf[i]) {
			printf("BOMB: V-READ[%08x] %08x, EXPECTED %08x\n", 1000+i, rdv, (*tb)[i+1000]);
			goto	test_failure;
		}
	} if (tb->bombed())
		goto test_failure;
	printf("VECTOR TEST PASSES!\n");

	// Read the status register
/*
	printf("ID[%2d]-RG = %08x\n", 0, rdv = tb->wb_read(8+0));
	if (rdv != 0x20ba1810) {
		printf("BOMB: ID[%2d]-RG = %08x != %08x\n", 0, rdv,
			0x20ba1810);
		goto test_failure;
	}

	for(int i=1; i<5; i++)
		printf("ID[%2d]-RG = %02x\n", i, tb->wb_read(8+i));
	if (tb->bombed())
		goto test_failure;
*/


	printf("Attempting to switch in Quad mode\n");
	// tb->wb_write(4, (tb->wb_read(4)&0x07f)); // Adjust EVconfig

	for(int i=0; (i<1000)&&(!tb->bombed()); i++) {
		unsigned	tblv;
		tblv = (*tb)[(i<<2)];
		rdv = tb->wb_read((i<<2));

		if(tblv != rdv) {
			printf("BOMB: Q-READ/SINGLE %08x, EXPECTED %08x\n", rdv, tblv);
			goto test_failure;
			break;
		} else printf("MATCH: %08x == %08x\n", rdv, tblv);
	} tb->wb_read(1000, 1000, rdbuf);
	if (tb->bombed())
		goto	test_failure;
	for(int i=0; i<1000; i++) {
		if ((*tb)[(i<<2)+1000] != rdbuf[i]) {
			printf("BOMB: Q-READ/VECTOR %08x, EXPECTED %08x\n", rdv, (*tb)[i+1000]);
			goto	test_failure;
		}
	} printf("VECTOR TEST PASSES! (QUAD)\n");

	printf("Attempting to switch to Quad mode with XIP\n");
	tb->wb_write(3, tb->wb_read(3)|0x08);
	// tb->wb_write(0, 0x22000000);

	printf("Attempting to read in Quad mode, using XIP mode\n");
	for(int i=0; (i<1000)&&(!tb->bombed()); i++) {
		unsigned	tblv;
		tblv = (*tb)[(i<<2)];
		rdv = tb->wb_read((i<<2));

		if(tblv != rdv) {
			printf("BOMB: Q-READ/XIP %08x, EXPECTED %08x\n", rdv, tblv);
			goto test_failure;
			break;
		} else printf("MATCH: %08x == %08x\n", rdv, tblv);
	}

	// Try a vector read
	tb->wb_read(1000, 1000, rdbuf);
	if (tb->bombed())
		goto	test_failure;
	for(int i=0; i<1000; i++) {
		if ((*tb)[(i<<2)+1000] != rdbuf[i]) {
			printf("BOMB: Q-READ/XIP/VECTOR %08x, EXPECTED %08x\n", rdv, (*tb)[i+1000]);
			goto	test_failure;
		}
	} printf("VECTOR TEST PASSES! (QUAD+XIP)\n");

	rdbuf[0] = tb->wb_read(1023);
	rdbuf[1] = tb->wb_read(2048);


	// Make sure, for testing purposes, that the words preceeding the
	// sector we are going to erase and following it don't look like they've
	// already been erased.
	if ((*tb)[SECTORSZW-1] == 0xffffffff)
		tb->set(SECTORSZW, 0);
	if ((*tb)[2*SECTORSZW] == 0xffffffff)
		tb->set(2*SECTORSZW, 0);

	printf("Turning off write-protect, calling WEL\n");
	tb->wb_ctrl_write(0, DISABLEWP);

	/*
	if (tb->write_protect()) {
		printf("WRITE PROTECT ISN\'T OFF YET, EVEN THOUGH WEL ISSUED\n");
		goto test_failure;
	} */

	printf("Attempting to erase subsector 1\n");
	tb->wb_ctrl_write(0, ERASEFLAG | (1*SECTORSZW));

	/*
	if (!tb->write_in_progress()) {
		printf("BOMB: Write in progress is false!\n");
		goto test_failure;
	}
	*/

	while (tb->wb_ctrl_read(0)&ERASEFLAG)
		;

	/*
	if (tb->write_in_progress()) {
		printf("BOMB: No write in progress\n");
		goto test_failure;
	}
	*/

	printf("Checking that the erase was successful\n");
	for(int i=SECTORSZB; i<SECTORSZB*2; i+=4) {
		if ((*tb)[i] != 0xffffffff) {
			printf("BOMB: Erase of [%08x] was unsuccessful, FLASH[%08x] = %08x\n", i, i, (*tb)[i]);
			goto test_failure;
		}
	}

	// Make sure we didn't erase anything else
	if ((*tb)[SECTORSZB-4] == 0xffffffff) {
		printf("BOMB: Post write check #2, the prior address changed\n");
		goto test_failure;
	} if ((*tb)[2*SECTORSZB] == 0xffffffff) {
		printf("BOMB: Post write check #2, the next address changed\n");
		goto test_failure;
	}

	if (tb->wb_read(SECTORSZB-4) != (*tb)[SECTORSZB-4]) {
		printf("BOMB: Post write check #2, the prior address changed\n");
		goto test_failure;
	} if (tb->wb_read(2*SECTORSZB) != (*tb)[2*SECTORSZB]) {
		printf("BOMB: Post write check #2, the next address changed\n");
		goto test_failure;
	}



	printf("Test: Trying a single word write\n");

	// Try to execute a single write
	tb->wb_ctrl_write(0,DISABLEWP);
	tb->wb_write(SECTORSZB, 0x12345678);

	while (tb->wb_ctrl_read(0)&ERASEFLAG)
		;

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
		rdbuf[0] = 0x12345678;
	}

	// Now, let's try writing this sector ... one page at a time.
	for(int p=0; p<NPAGES; p++) {

		printf("Writing page %d\n", p);
		tb->wb_ctrl_write(0, DISABLEWP);
		// if (tb->write_protect()) goto	test_failure;
		tb->wb_write(SECTORSZB+p*SZPAGEB, SZPAGEW, &rdbuf[p*SZPAGEW]);

		while (tb->wb_ctrl_read(0)&ERASEFLAG)
			;

		printf("Checking page %d\n", p);
		for(int i=0; i<SZPAGEW; i++) {
			if (rdbuf[p*SZPAGEW+i] != (*tb)[SECTORSZB+p*SZPAGEB+(i<<2)]) {
				printf("BOMB: Write check, Addr[%08x]\n", SECTORSZB+p*SZPAGEB+(i<<2));
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
