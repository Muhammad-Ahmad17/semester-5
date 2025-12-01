# Paging Study Guide

## 1. Why Paging?
Paging allows the operating system to map a process's virtual address space onto physical memory frames of equal size. It eliminates external fragmentation, simplifies memory allocation, and enables demand paging, copy-on-write, and shared code segments.

## 2. Key Terms At A Glance
| Term | Meaning |
| --- | --- |
| Page | Fixed-size block of a process's virtual address space. |
| Frame | Fixed-size block of physical memory; same size as a page. |
| Page Table | Maps virtual page numbers (VPN) to physical frame numbers (PFN). |
| PTBR | Page Table Base Register; points to the first entry of the active page table. |
| TLB | Translation Lookaside Buffer; small cache holding recent VPN→PFN translations. |

## 3. Formula Reference From `Paging_3.pptx`
All equations below follow the exact order of the slide deck so you can cross-check quickly.

### 3.1 Physical Memory Math (Slide 4)
- `Physical address space = size of main memory = (# of frames) × (page size)`.
- `Frame size = page size` by definition, so you can swap the terms freely.
- If main memory holds `2^X` frames, then the frame number field needs `X` bits.
- If the page size is `2^Y` bytes, then the page offset consumes `Y` bits.
- Consequently, a physical address that points anywhere inside main memory must carry `X + Y` bits.

### 3.2 Virtual / Process Space (Slide 5)
- `Virtual address space = process size` (for a fully allocated process).
- `# of pages in a process = process size / page size`.
- If the process size equals `2^X` bytes, then its virtual addresses require `X` bits. Those bits divide into the virtual page number (VPN) and the offset, matching the frame/offset split above.

### 3.3 Page Table Sizing (Slide 6)
- `Page table size = (# of entries) × (page table entry size)`.
- `# of entries = # of pages in the process = process size / page size`.
- Each entry must encode at least the physical frame number, so a minimum entry width is `bits_in_frame_number`. Add more bits if the architecture stores valid/dirty/reference bits inline.

### 3.4 Address-Capacity Rule of Thumb (Slide 7)
- With `n` address bits you can form `2^n` distinct addresses.
- **Byte-addressable:** memory size = `2^n × 1 byte`.
- **Word-addressable (word = m bytes):** memory size = `2^n × m` bytes.
- This slide underpins Problems 1 and 2: you either solve for `n` given capacity or solve for capacity given `n`.

### 3.5 Putting The Bits Together
- **Physical address layout:** `| frame number (X bits) | page offset (Y bits) |`.
- **Virtual address layout:** `| VPN (X' bits) | page offset (Y bits) |`. The offset must match because pages and frames are equally sized.
- `Y = log2(page size in bytes)`.
- `X' = total virtual bits − Y`, while `X = total physical bits − Y`.

### 3.6 Multi-Level Paging Conditions (Slides 26–35)
- Multi-level paging is needed when a single-level table would exceed the frame size.
- Let `inner_table_size = (# of process pages) × entry_size`. If this exceeds page size, split it into `inner_table_size / page_size` pages (inner tables).
- `Entries per inner-table page = page_size / entry_size`. The number of bits required to index within that page is `log2(entries_per_page)`.
- The outer table then stores PFNs for each inner-table page: `outer_table_size = (# of inner pages) × entry_size`.
- Repeat the splitting until `table_size ≤ frame_size`. The number of splits equals the number of paging levels.

### 3.7 Quick Reference Equations
```
# of pages in process = process size / page size
# of frames in memory = memory size / page size
VPN bits = log2(# of pages in process)
PFN bits = log2(# of frames in memory)
offset bits = log2(page size)
page table size = (# of pages) × (entry size)
```

## 4. Single-Level Paging Workflow
1. Split the logical address into VPN and offset.
2. Use VPN to index the page table and fetch PFN (plus protection bits, dirty bit, etc.).
3. Concatenate PFN with offset to form the physical address.
4. On a TLB hit, steps 1–2 collapse to a single lookup.

## 5. Worked Problems

### Problem 1 — Memory Size from Address Bits
**Prompt:** A memory is 2-byte addressable and uses 22-bit addresses. What is the memory size?

**Solution:**
- Distinct locations: `2^22`.
- Size per location: 2 bytes → total size `2^22 × 2 = 2^23` bytes `= 8 MB`.

### Problem 2 — Bits Required for 16 GB
**Prompt:** Find the number of address bits for a 16 GB, 4-byte-addressable memory.

**Solution:**
1. Let `n` be the number of bits. Size = `2^n × 4` bytes.
2. Set `2^n × 4 = 16 GB = 2^34 bytes` → `2^(n+2) = 2^34`.
3. `n + 2 = 34` → `n = 32` bits.

### Problem 3 — Page Table Size (32-bit System)
**Prompt:** 32-bit logical address space, 4 KB page size, and 4-byte page table entries (PTE). Find the page table size.

**Solution:**
- Process size: `2^32 = 4 GB`.
- Pages per process: `4 GB / 4 KB = 2^20` pages.
- Page table size: `2^20 entries × 4 bytes = 4 MB`.

### Problem 4 — Fitting Page Table in One Page
**Prompt:** Virtual space = 4 MB, page size = 4 KB. What is the largest PTE size if the whole page table must fit in one page?

**Solution:**
- Pages per process: `4 MB / 4 KB = 2^10`.
- Let entry size = `B` bytes. Table size = `2^10 × B`.
- Requirement: `2^10 × B ≤ 4 KB = 2^12` → `B ≤ 4` bytes.

### Problem 5 — Minimum Page Size (Table Fits in One Page)
**Prompt:** Virtual space = 256 MB, PTE size = 4 bytes. What is the smallest page size so the page table fits into one page?

**Solution:**
- Let page size = `2^k` bytes.
- Pages per process = `256 MB / 2^k = 2^28 / 2^k = 2^(28−k)`.
- Page table size = `2^(28−k) × 4 = 2^(30−k)` bytes.
- Need `2^(30−k) ≤ 2^k` → `30 − k ≤ k` → `k ≥ 15`.
- Minimum page size = `2^15 bytes = 32 KB`.

### Problem 6 — Segmented Paging Minimum Page Size
**Prompt:** Virtual and physical spaces are `2^16` bytes. The virtual space has 8 equal segments; PTEs are 2 bytes. Page tables must fit into a single page. Find the minimum page size and VA breakdown.

**Solution:**
- Segment size: `2^16 / 8 = 2^13 bytes = 8 KB`.
- Let page size = `2^k`. Pages per segment = `2^13 / 2^k = 2^(13−k)`.
- Page table size per segment = `2^(13−k) × 2 = 2^(14−k)` bytes.
- Require `2^(14−k) ≤ 2^k` → `14 − k ≤ k` → `k ≥ 7`.
- Minimum page size = `2^7 = 128 bytes`.
- VA bits: 3 bits segment number, 6 bits page number (`2^(13−7)` pages), 7-bit offset.

### Problem 7 — Two-Level Paging Example
**Scenario:**
- Logical address space = 4 GB.
- Physical address space = 16 TB.
- Page size = 4 KB.

**Key Steps:**
1. Physical memory bits = `log2(16 TB) = 44` bits.
2. Offset bits = `log2(4 KB) = 12` bits; PFN bits = `44 − 12 = 32`.
3. Pages per process = `4 GB / 4 KB = 2^20`.
4. Inner page table size = `2^20 entries × 4-byte PFN = 4 MB`, which is > 4 KB frame, so split into `4 MB / 4 KB = 2^10` pages.
5. Outer page table tracks those `2^10` inner pages: size `2^10 × 4 bytes = 4 KB`, fitting one frame. Result: two-level paging with 10-bit outer index, 10-bit inner index, and 12-bit offset.

## 6. Extra Practice Problems
1. **Offset Bits:** A system uses 4 MB pages and 48-bit virtual addresses. How many bits belong to the page offset and to the virtual page number?
2. **Page Table Memory Footprint:** Physical memory is 512 MB, virtual space is 8 GB, page size is 8 KB, and each PTE is 8 bytes (includes metadata). How much memory is needed for the full page table of a single process? Could it fit entirely in RAM if only 64 MB is free?
3. **TLB Reach:** A TLB stores 128 entries and the page size is 16 KB. What range of virtual memory can be “covered” by the TLB? How does doubling the page size change that range?
4. **Inverse Page Table Thought Experiment:** A machine has an inverse page table with one entry per frame. If the physical memory has 2^18 frames, what is the minimum number of bits to store the virtual page number for 64-bit processes plus a process identifier? What overheads arise compared to traditional per-process tables?
5. **Hybrid Segmentation + Paging:** Design the virtual address format for a system with 16 segments, 1 GB virtual space, 64 KB pages, and 2-byte PTEs. What constraints ensure each segment's page table fits in 256 KB?

*Suggested approach:* try to solve each problem before checking with a peer or instructor. For Exercises 1–3, the answers are numerical (bits or bytes). For Exercises 4–5, focus on reasoning and stating assumptions clearly.

## 7. Tips & Common Pitfalls
- Always keep units consistent (bytes vs KB vs MB).
- Remember that page size equals frame size.
- Verify whether the memory is byte- or word-addressable before computing total size.
- When forcing a page table to fit into a page, set `table_size ≤ page_size` and solve the inequality.
- For multi-level paging, break large tables recursively until each level fits in one frame.

Happy studying! Let me know if you want worked solutions for the practice problems or another topic summarized.
