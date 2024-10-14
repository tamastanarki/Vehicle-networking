/* DO NOT EDIT
 * this file was automatically generated from ../../tools/generate-json for
 * VEP_ID=1 TILE_ID=1 PARTITION_ID=1
 */
MEMORY {
mem : ORIGIN = 0x0, LENGTH = 0x00004000
publicmem : ORIGIN = 0x82000000, LENGTH = 0x00080000 /* vep=1 tile=1 public regions of all veps */
mem01 : ORIGIN = 0x80020000, LENGTH = 0x00000400 /* vep=1 tile=1 partition=1 private memory */
}
_STACK_SIZE = 0x00001000;
OUTPUT_ARCH( "riscv" )
ENTRY( _start )
_STACK_SIZE = DEFINED(_STACK_SIZE) ? _STACK_SIZE : 0x1000;
SECTIONS
{
  . = 0x00000000;
  .text.init : { *(.text.init) } > mem
  .text : { *(.text) } > mem
  .data : { *(.data) } > mem
  .bss.align :
   {
           . = ALIGN(4);
           _bss = .;
   } > mem
   .bss.start :
   {
          _bss_lma = LOADADDR(.bss.start);
   } >mem
   .bss :
   {
           *(.sbss*)
           *(.gnu.linkonce.sb.*)
           *(.bss .bss.*)
           *(.gnu.linkonce.b.*)
           *(COMMON)
        . = ALIGN(4);
        _ebss = .;
   } >mem
  .mem01 : { *(.mem01) } > mem01
  _end = .;
  .stack (NOLOAD) : {
          _stack_end = .;
          . += _STACK_SIZE;
          . = ALIGN(4);
          _stack_start = .;
  } > mem
  PROVIDE (end = .);
}
