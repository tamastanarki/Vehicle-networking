#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <libuserchannel-arm.h>
#include <signal.h>

#define FREQ  40000 // 40k since we use ms in $timescale

FILE* vcd;

void handle_sig()
{
  fclose(vcd);
  on_arm_cleanup(1, 1);
  on_arm_cleanup(2, 1);
  exit(0);
}


uint64_t rcv_time_stamp (int tileid, int pid)
{
  unsigned char b0 = on_arm_rcv_from_riscv_to_arm (tileid, pid);
  unsigned char b1 = on_arm_rcv_from_riscv_to_arm (tileid, pid);
  unsigned char b2 = on_arm_rcv_from_riscv_to_arm (tileid, pid);
  unsigned char b3 = on_arm_rcv_from_riscv_to_arm (tileid, pid);
  unsigned char b4 = on_arm_rcv_from_riscv_to_arm (tileid, pid);
  unsigned char b5 = on_arm_rcv_from_riscv_to_arm (tileid, pid);
  unsigned char b6 = on_arm_rcv_from_riscv_to_arm (tileid, pid);
  unsigned char b7 = on_arm_rcv_from_riscv_to_arm (tileid, pid);

  return ((uint64_t)b7 << 56) | ((uint64_t)b6 << 48) | ((uint64_t)b5 << 40) | ((uint64_t)b4 << 32) | ((uint64_t)b3 << 24) | ((uint64_t)b2 << 16) | ((uint64_t)b1 << 8) | (uint64_t)b0;
}

void write_vcd_header(FILE* f)
{
  fprintf(f, "$timescale 1ms $end\n");
  fprintf(f, "$scope module CAN $end\n");
  fprintf(f, "$var wire 1 C bus $end\n");
  fprintf(f, "$var wire 1 ( bus_clk $end\n");
  fprintf(f, "$var wire 1 ! client_1_2 $end\n");
  fprintf(f, "$var wire 1 @ client_1_3 $end\n");
  fprintf(f, "$var wire 1 # client_1_4 $end\n");
  fprintf(f, "$var wire 1 $ client_2_2 $end\n");
  fprintf(f, "$var wire 1 %% client_2_3 $end\n");
  fprintf(f, "$var wire 1 ^ client_2_4 $end\n");
  fprintf(f, "$upscope $end\n");
  fprintf(f, "$enddefinitions $end\n");
  fprintf(f, "$dumpvars\n");
  fprintf(f, "xC\nx(\nx!\nx@\nx#\nx$\nx%%\nx^\n$end\n");
  fprintf(f, "#0\nxC\n0(\nx!\nx@\nx#\nx$\nx%%\nx^\n");
}

void print_written_value(int tile_id, int partition_id, int value)
{
  printf("\tclient_%d_%d: ", tile_id, partition_id);
  if (value == 0) printf("D\n");
  else if (value == 1) printf("R\n");
  else printf("U\n");
}

int main()
{

  // open output file
  char vcd_name[200] = "./bus.vcd";
  vcd = fopen (vcd_name, "wb");
  if (vcd == NULL)
  {
    fprintf(stderr, "Cannot open %s\n",vcd_name);
    return EXIT_FAILURE;
  }
  else
  {
    write_vcd_header(vcd);
  }

  if (on_arm_initialise (1, 1)) return 1;
  if (on_arm_initialise (2, 1)) return 1;

  signal(SIGINT, handle_sig);

  printf("Receiving bus values from RISC-V\n");

  uint64_t time_stamp, t0 = 0;
  int clk = 0;
  int bus_value, client_1_2_value, client_1_3_value, client_1_4_value, client_2_2_value, client_2_3_value, client_2_4_value;
  while(1)
  {
    time_stamp = rcv_time_stamp(1, 1);
    if (t0 == 0)
      t0 = time_stamp;

    // read client symbols
    printf("time_stamp: %" PRIu64 ", written_values:\n", (time_stamp - t0)/FREQ);
    fprintf(vcd, "#%" PRIu64 "\n", (time_stamp - t0)/FREQ);
    clk = !clk;
    fprintf(vcd, "%d(\n", clk);

    client_1_2_value = on_arm_rcv_from_riscv_to_arm(1, 1);
    print_written_value(1, 2, client_1_2_value);
    if (client_1_2_value == 2) fprintf(vcd, "x!\n");
    else fprintf(vcd, "%d!\n", client_1_2_value);

    client_1_3_value = on_arm_rcv_from_riscv_to_arm(1, 1);
    print_written_value(1, 3, client_1_3_value);
    if (client_1_3_value == 2) fprintf(vcd, "x@\n");
    else fprintf(vcd, "%d@\n", client_1_3_value);

    client_1_4_value = on_arm_rcv_from_riscv_to_arm(1, 1);
    print_written_value(1, 4, client_1_4_value);
    if (client_1_4_value == 2) fprintf(vcd, "x#\n");
    else fprintf(vcd, "%d#\n", client_1_4_value);

    rcv_time_stamp(2, 1);
    client_2_2_value = on_arm_rcv_from_riscv_to_arm(2, 1);
    print_written_value(2, 2, client_2_2_value);
    if (client_2_2_value == 2) fprintf(vcd, "x$\n");
    else fprintf(vcd, "%d$\n", client_2_2_value);

    client_2_3_value = on_arm_rcv_from_riscv_to_arm(2, 1);
    print_written_value(2, 3, client_2_3_value);
    if (client_2_3_value == 2) fprintf(vcd, "x%%\n");
    else fprintf(vcd, "%d%%\n", client_2_3_value);

    client_2_4_value = on_arm_rcv_from_riscv_to_arm(2, 1);
    print_written_value(2, 4, client_2_4_value);
    if (client_2_4_value == 2) fprintf(vcd, "x^\n");
    else fprintf(vcd, "%d^\n", client_2_4_value);
    printf("\n");

    // read bus symbol
    time_stamp = rcv_time_stamp(1, 1);
    bus_value = on_arm_rcv_from_riscv_to_arm(1, 1);
    rcv_time_stamp(2, 1);
    on_arm_rcv_from_riscv_to_arm(2, 1);
    if (bus_value == 0) {
      printf("time_stamp: %" PRIu64 ", bus_value: D\n\n", (time_stamp - t0)/FREQ);
    }
    else {
      printf("time_stamp: %" PRIu64 ", bus_value: R\n\n", (time_stamp - t0)/FREQ);
    }

    fprintf(vcd, "#%" PRIu64 "\n", (time_stamp - t0)/FREQ);
    fprintf(vcd, "%dC\n", bus_value);

  }
  return 0;
}
