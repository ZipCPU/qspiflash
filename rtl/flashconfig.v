////////////////////////////////////////////////////////////////////////////////
//
// Filename: 	flashconfig.v
//
// Project:	A Set of Wishbone Controlled SPI Flash Controllers
//
// Purpose:	A configuration file, separated from the controller file, so
//		that multiple files can use the same wishbone Quad Spi Flash
//	controller, while each having a separate configuration.  Currently,
//	the configuration only includes whether the flash is read only or not.
//	Other configuration options may be added later.
//
// Creator:	Dan Gisselquist, Ph.D.
//		Gisselquist Technology, LLC
//
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2015-2018, Gisselquist Technology, LLC
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
`ifndef	FLASH_CONFIG_V
`define	FLASH_CONFIG_V
//
// `define	READ_ONLY
//
`endif
//
