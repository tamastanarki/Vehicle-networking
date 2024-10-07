tile-add    0 40000000 10000000 128k 32 16
tile-add    1 40000000 10000000 128k 32 16
tile-add    2 40000000 10000000 128k 32 16
tile-tdm-os 0 2000
tile-tdm-os 1 2000
tile-tdm-os 2 2000
tile-mem-os 0 32k
tile-mem-os 1 32k
tile-mem-os 2 32k
tile-mem-add  0 1 32k 0x8000
tile-mem-data 0 1 vep_1/partition_0_1/out.hex
tile-mmu-add  0 1 0 0x80FC0000 0x80FC0000 0xFFFEFFF8
tile-mmu-add  0 1 1 0x81000000 0x81000000 0xff000000
tile-mmu-add  0 1 2 0x800C0000 0x800C1000 0xFFFFFC00
tile-mmu-add  0 1 3 0x80020000 0x80020000 0xFFFFFC00
tile-mmu-add  0 1 4 0x80030000 0x80030000 0xFFFFFC00
tile-tdm-add  0 0 5000
tile-tdm-add  0 1 10000
tile-mem-add  1 1 16k 0x8000
tile-mem-data 1 1 vep_1/partition_1_1/out.hex
tile-mmu-add  1 1 0 0x80FC0000 0x80FC0000 0xFFFEFFF8
tile-mmu-add  1 1 1 0x81000000 0x81000000 0xff000000
tile-mmu-add  1 1 2 0x800C0000 0x800C1000 0xFFFFFC00
tile-mmu-add  1 1 3 0x80020000 0x80020000 0xFFFFFC00
tile-mem-add  1 2 16k 0xC000
tile-mem-data 1 2 vep_1/partition_1_2/out.hex
tile-mmu-add  1 2 0 0x80FC0000 0x80FC0000 0xFFFEFFF8
tile-mmu-add  1 2 1 0x81000000 0x81000000 0xff000000
tile-mmu-add  1 2 2 0x800C0000 0x800C1400 0xFFFFFC00
tile-mmu-add  1 2 3 0x80020000 0x80020000 0xFFFFFC00
tile-mem-add  1 3 16k 0x10000
tile-mem-data 1 3 vep_1/partition_1_3/out.hex
tile-mmu-add  1 3 0 0x80FC0000 0x80FC0000 0xFFFEFFF8
tile-mmu-add  1 3 1 0x81000000 0x81000000 0xff000000
tile-mmu-add  1 3 2 0x800C0000 0x800C1800 0xFFFFFC00
tile-mmu-add  1 3 3 0x80020000 0x80020000 0xFFFFFC00
tile-mem-add  1 4 16k 0x14000
tile-mem-data 1 4 vep_1/partition_1_4/out.hex
tile-mmu-add  1 4 0 0x80FC0000 0x80FC0000 0xFFFEFFF8
tile-mmu-add  1 4 1 0x81000000 0x81000000 0xff000000
tile-mmu-add  1 4 2 0x800C0000 0x800C1C00 0xFFFFFC00
tile-mmu-add  1 4 3 0x80020000 0x80020000 0xFFFFFC00
tile-tdm-add  1 0 5000
tile-tdm-add  1 1 50000
tile-tdm-add  1 2 450000
tile-tdm-add  1 1 50000
tile-tdm-add  1 3 450000
tile-tdm-add  1 1 50000
tile-tdm-add  1 4 450000
tile-tdm-add  1 1 50000
tile-mem-add  2 1 16k 0x8000
tile-mem-data 2 1 vep_1/partition_2_1/out.hex
tile-mmu-add  2 1 0 0x80FC0000 0x80FC0000 0xFFFEFFF8
tile-mmu-add  2 1 1 0x81000000 0x81000000 0xff000000
tile-mmu-add  2 1 2 0x800C0000 0x800C1000 0xFFFFFC00
tile-mmu-add  2 1 3 0x80020000 0x80020000 0xFFFFFC00
tile-mem-add  2 2 16k 0xC000
tile-mem-data 2 2 vep_1/partition_2_2/out.hex
tile-mmu-add  2 2 0 0x80FC0000 0x80FC0000 0xFFFEFFF8
tile-mmu-add  2 2 1 0x81000000 0x81000000 0xff000000
tile-mmu-add  2 2 2 0x800C0000 0x800C1400 0xFFFFFC00
tile-mmu-add  2 2 3 0x80020000 0x80020000 0xFFFFFC00
tile-mem-add  2 3 16k 0x10000
tile-mem-data 2 3 vep_1/partition_2_3/out.hex
tile-mmu-add  2 3 0 0x80FC0000 0x80FC0000 0xFFFEFFF8
tile-mmu-add  2 3 1 0x81000000 0x81000000 0xff000000
tile-mmu-add  2 3 2 0x800C0000 0x800C1800 0xFFFFFC00
tile-mmu-add  2 3 3 0x80020000 0x80020000 0xFFFFFC00
tile-mem-add  2 4 16k 0x14000
tile-mem-data 2 4 vep_1/partition_2_4/out.hex
tile-mmu-add  2 4 0 0x80FC0000 0x80FC0000 0xFFFEFFF8
tile-mmu-add  2 4 1 0x81000000 0x81000000 0xff000000
tile-mmu-add  2 4 2 0x800C0000 0x800C1C00 0xFFFFFC00
tile-mmu-add  2 4 3 0x80020000 0x80020000 0xFFFFFC00
tile-tdm-add  2 0 5000
tile-tdm-add  2 1 50000
tile-tdm-add  2 2 450000
tile-tdm-add  2 1 50000
tile-tdm-add  2 3 450000
tile-tdm-add  2 1 50000
tile-tdm-add  2 4 450000
tile-tdm-add  2 1 50000
