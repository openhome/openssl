/* BSP dependent options file */
/* automatically generated -- DO NOT EDIT!! */

#ifndef __BSP_OPTIONS_H
#define __BSP_OPTIONS_H

/* include/bspopts.tmp.  Generated from bspopts.h.in by configure.  */
/* include/bspopts.h.in.  Generated from configure.ac by autoheader.  */






/* If defined, then the PowerPC specific code in RTEMS will use some of the
   special purpose registers to slightly optimize interrupt response time. The
   use of these registers can conflict with other tools like debuggers. */
#define PPC_USE_SPRG 1

/* This defines the base address of the exception table. NOTE: Vectors are
   actually at 0xFFF00000 but file starts at offset. */
#define PPC_VECTOR_FILE_BASE 0x0100

/* This defines the location of the hardware specific "xparameters.h" file. in
   the file system. Specify an absolute path. Don't forget the double quotes
   */
#define RTEMS_XPARAMETERS_H <xparameters_dflt.h>

/* Defines path to Xilinx XPS PPC libraries. */
#define RTEMS_XPPC_BASE .

#endif /* __BSP_OPTIONS_H */
