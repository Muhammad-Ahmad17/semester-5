# ASSIGNMENT NO 2 - OPERATING SYSTEMS (CSC323)
**System:** 64-bit, Page size: 8 KB, PD & PT: 4096 entries each  
**Virtual Address:** 0x03A_205_1F4 | **Max Marks:** 20

---

## SOLUTION

### a) Address Breakdown (Bit counts)

**Page Size = 8 KB = 2^13 → Offset = 13 bits**  
**PD entries = 4096 = 2^12 → PD Index = 12 bits**  
**PT entries = 4096 = 2^12 → PT Index = 12 bits**

**Virtual Address: 0x03A_205_1F4**
- **PD Index:** 0x03A (12 bits)
- **PT Index:** 0x205 (12 bits)
- **Offset:** 0x1F4 (13 bits)
- **Total:** 12 + 12 + 13 = 37 bits ✓

---

### b) Step-by-Step Translation

**Step 1:** PD[0x03A] → PT @ frame 0x1F20 ✓ (Valid)

**Step 2:** PT[0x205] in table @ 0x1F20 → frame 0x0ACD ✓ (Present & Dirty)

**Step 3:** Offset = 0x1F4 (13-bit offset within 8KB page)

**Step 4:** Physical Address = (0x0ACD << 13) + 0x1F4 = **0x56874**

---

### c) Lazy Allocation Behavior

**Lazy allocation means:** Page table created on first access, not upfront.

From dump: PD[0x07C] → "lazy allocated"

**When accessed:**
1. System detects lazy flag
2. Allocates physical frame for PT (e.g., 0x2000)
3. Updates PD[0x07C] with frame address
4. Creates PT with entries marked "not present"
5. Subsequent PT[n] lookups proceed normally

**Benefits:** Saves memory for sparse address spaces  
**Cost:** First access latency for allocation

---

### d) Dirty Bit Eviction Process

**Initial:** PT[0x205] → frame 0x0ACD, Dirty bit = 1 (modified)

**Eviction Steps:**
1. **Check Dirty Bit:** Reads PT[0x205], finds Dirty = 1
2. **Write-Back:** Copies 8 KB page from frame 0x0ACD → Disk/Swap
3. **Update Table:** 
   - Present bit = 0 (no longer in memory)
   - Dirty bit = 0 (successfully written)
   - Stores disk location pointer
4. **Free Frame:** 0x0ACD available for reuse

**If Re-accessed:** Page fault → Load from disk → New frame allocated

**Critical:** Without dirty bit check, modified data would be lost!

---

### e) TLB Flush Requirement

**Answer: NO TLB flush needed** for this translation

**Justification:**
- This is **read-only** translation (no page state changes)
- PT[0x205] remains: Present, Frame 0x0ACD, Permissions unchanged
- No eviction occurred → mapping still valid
- No context switch → same address space
- TLB cached entry remains consistent with page table

**TLB flush WOULD be needed if:**
- Page evicted (frame changes)
- Permissions modified (access rights change)
- Context switch (different process)
- Page table remapped to different frame
