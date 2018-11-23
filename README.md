This repository has been repurposed from the original [QSPI flash core
repository](https://opencores.org/project/qspiflash).  Instead of two large
and monolithic QSPI flash cores for two different types of flash, this
repository now contains three cores: a [SPI flash core](rtl/spixpress.v),
a [Dual SPI flash core](rtl/dualflexpress.v), and a
[Quad SPI flash core](rtl/qflexpress.v) which should be usable across a wider
range of SPI flash cores.  Even better, these new controllers use the
DDR primitive for the SCK line, so they should be able to run twice as fast
as the older cores.

The normal [SPI flash core](rtl/spixpress.v) has been
[blogged about](http://zipcpu.com/blog/2018/08/16/spiflash.html)
on [ZipCPU.com](http://zipcpu.com).

- Each of these cores has been formally verified, though not all of them
  have seen hardware (yet).  SymbiYosys scripts for verification may be found
  in the [bench/formal](bench/formal) directory, together with GTKwave save
  files for viewing any resulting traces.

  This includes cover traces, in case you'd like to get a glimpse of how the
  core is supposed to operate.

- A [flash simulator](bench/cpp/flashsim.cpp) has been placed into the
  [bench/cpp](bench/cpp) directory.  You may find this useful when simulating
  any of these flash cores using Verilator.

- A [software flash driver](sw/flashdrvr.cpp) can be found in the [sw](sw)
  directory.  You may find this useful for writing values to any of these
  flash drivers.

- [AutoFPGA](autodata/) scripts have been created for each flash device, though
  not yet tested.

## Status

Although this project has been around for some time, it is currently in the
process of getting a massive rewrite.  As of today, the RTL code is complete
although it still needs to see hardware.  The simulation software is also
full featured, and has been used to simulate many flash devices.  Work remains
integrating the flash controllers into their various designs using AutoFPGA,
as well as testing the various flash controllers in hardware once integrated.
The software driver code will be used for this test, and will need to be
full featured by then for that purpose.

In some, the following are left to do:

- Write a simulation script to demonstrate each of the respective flash
  controllers.  This would replace the [old script](bench/cpp/qspiflash_tb.cpp)
  from before the rewrite.

- Update the [AutoFPGA](https://github.com/ZipCPU/autofpga) scripts to make
  certain they work with both Xilinx and iCE40 parts.  (Intel parts remain in
  the distance)

- Update the [software flash driver](sw/flashdrvr.cpp) so that one driver
  can apply to any controller

- The last remains of the [older driver](rtl/wbqspiflash.v), and [its
  cousin](rtl/eqspiflash.v) need to be removed from the repository.

- The specification needs some formatting work and editing.

## License

These three cores, together with their supporting infrasstructure, have
been released under the [LGPL license](doc/lgpl-3.0.pdf).  You are welcome
to use these cores under that license.
