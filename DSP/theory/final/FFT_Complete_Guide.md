# FFT Complete Study Guide: DTFT vs DFT → Radix-2 FFT

---

## **PART 1: DTFT vs DFT Comparison**

### **1.1 Comparison Table**

| **Aspect** | **DTFT** | **DFT** |
|---|---|---|
| **Time Domain** | Aperiodic, Infinite ($n = -\infty$ to $+\infty$) | Finite, periodic ($n = 0$ to $N-1$) |
| **Frequency Domain** | Continuous ($\omega$) | Discrete ($k$) |
| **Periodicity** | $2\pi$ | $N$ samples |
| **Formula** | $X(e^{j\omega}) = \sum_{n=-\infty}^{\infty} x[n]e^{-j\omega n}$ | $X[k] = \sum_{n=0}^{N-1} x[n]e^{-j2\pi kn/N}$ |
| **Practical?** | No (infinite, continuous) | **Yes** (finite, discrete) |
| **Computational** | Cannot compute in computer | Can compute |

---

## **PART 2: Solved Problems Using Both Definitions**

### **Problem 1: Calculate DTFT and DFT for Same Sequence**

**Given:** $x[n] = \{1, 2\}$ (finite sequence, length $L=2$)

#### **Step A: Calculate DTFT**

Using the definition:
$$X(e^{j\omega}) = \sum_{n=-\infty}^{\infty} x[n]e^{-j\omega n}$$

Since $x[n] = 0$ for $n < 0$ and $n \geq 2$:
$$X(e^{j\omega}) = x[0]e^{j\omega \cdot 0} + x[1]e^{-j\omega \cdot 1}$$
$$X(e^{j\omega}) = 1 + 2e^{-j\omega}$$

**Converting to magnitude and phase:**
$$|X(e^{j\omega})| = |1 + 2e^{-j\omega}| = |1 + 2\cos(\omega) - j2\sin(\omega)|$$
$$= \sqrt{(1 + 2\cos\omega)^2 + (2\sin\omega)^2}$$
$$= \sqrt{1 + 4\cos\omega + 4\cos^2\omega + 4\sin^2\omega}$$
$$= \sqrt{1 + 4\cos\omega + 4} = \sqrt{5 + 4\cos\omega}$$

**Phase:**
$$\angle X(e^{j\omega}) = -\arctan\left(\frac{2\sin\omega}{1+2\cos\omega}\right)$$

**Key insight:** DTFT is a **continuous function** of $\omega$ for all $\omega \in [0, 2\pi]$.

#### **Step B: Calculate 4-point DFT**

Zero-pad the sequence to $N=4$: $x[n] = \{1, 2, 0, 0\}$

Using the definition:
$$X[k] = \sum_{n=0}^{3} x[n]W_4^{kn} \quad \text{where} \quad W_4 = e^{-j2\pi/4} = e^{-j\pi/2} = -j$$

**For k=0:**
$$X[0] = x[0] \cdot 1 + x[1] \cdot 1 + x[2] \cdot 1 + x[3] \cdot 1 = 1 + 2 = \boxed{3}$$

**For k=1:**
$$X[1] = x[0]W_4^0 + x[1]W_4^1 = 1(1) + 2(-j) = \boxed{1 - 2j}$$

**For k=2:**
$$X[2] = x[0]W_4^0 + x[1]W_4^2 = 1(1) + 2(-1) = \boxed{-1}$$

**For k=3:**
$$X[3] = x[0]W_4^0 + x[1]W_4^3 = 1(1) + 2(+j) = \boxed{1 + 2j}$$

**Result:** $X[k] = \{3, 1-2j, -1, 1+2j\}$ (4 discrete values)

**Key insight:** DFT gives **4 discrete samples** at frequencies $\omega_k = \frac{2\pi k}{N} = \{0, \frac{\pi}{2}, \pi, \frac{3\pi}{2}\}$.

These are **samples of the DTFT** at those specific frequencies!

---

### **Problem 2: 8-point DFT by Hand (Definition)**

**Given:** $x[n] = \{1, 1, 1, 1, 0, 0, 0, 0\}$

Using $W_8 = e^{-j2\pi/8} = e^{-j\pi/4} = \cos(45°) - j\sin(45°) = \frac{1}{\sqrt{2}}(1-j)$

| k | Calculation | Result |
|---|---|---|
| **0** | $1+1+1+1 = 4$ | $\boxed{4}$ |
| **1** | $1 + W_8 + W_8^2 + W_8^3$ | $1 + \frac{1-j}{\sqrt{2}} + (-j) + \frac{-1-j}{\sqrt{2}} = \boxed{1.414 - j2.414}$ |
| **2** | $1 + W_8^2 + W_8^4 + W_8^6$ | $1 + (-j) + (-1) + j = \boxed{0}$ |
| **3** | $1 + W_8^3 + W_8^6 + W_8^9$ | $\boxed{0.414 - j0.414}$ |
| **4** | $1 + W_8^4 + W_8^8 + W_8^{12}$ | $1 + (-1) + 1 + (-1) = \boxed{0}$ |
| **5** | Conjugate of X[3] | $\boxed{0.414 + j0.414}$ |
| **6** | Conjugate of X[2] | $\boxed{0}$ |
| **7** | Conjugate of X[1] | $\boxed{1.414 + j2.414}$ |

---

## **PART 3: Drawbacks of Direct DFT Method**

### **3.1 Computational Complexity**

**Direct method (by definition):**
- Each output $X[k]$ requires computing $N$ complex multiplications and $N-1$ additions
- For all $N$ outputs: **$N^2$ multiplications**, **$N(N-1)$ additions**
- **Total complexity: $O(N^2)$**

**Example comparison:**

| N | Direct DFT | FFT | Speedup |
|---|---|---|---|
| 64 | 4,096 ops | 384 ops | **10.7x** |
| 256 | 65,536 ops | 2,048 ops | **32x** |
| 1024 | 1,048,576 ops | 10,240 ops | **102x** |
| 4096 | 16.8M ops | 49,152 ops | **342x** |

**Problem:** For real-time audio (48 kHz sampling), processing even one second of audio requires billions of operations!

### **3.2 Twiddle Factor Redundancy**

The direct method calculates every $W_N^{kn}$ independently, ignoring key properties:

**Property 1 - Periodicity:**
$$W_N^{k+N} = W_N^k \quad \text{(repeats every N values)}$$

**Property 2 - Symmetry:**
$$W_N^{k + N/2} = e^{-j2\pi(k+N/2)/N} = e^{-j2\pi k/N} \cdot e^{-j\pi} = -W_N^k$$

**Example (N=8):**
- Direct: Calculate $W_8^0, W_8^1, W_8^2, \dots, W_8^7$ separately
- Efficient: Note that $W_8^5 = -W_8^1$, $W_8^6 = -W_8^2$, etc. → **Use only 4 unique values!**

### **3.3 Memory and Speed Issues**

1. **Pre-computing twiddle factors:** Storing all $N$ values requires $O(N)$ memory.
2. **Repeated calculations:** Same twiddle factors calculated multiple times.
3. **Cache misses:** Large arrays don't fit in CPU cache efficiently.
4. **Slow for large N:** A 1024-point DFT takes ~1 second on old systems.

---

## **PART 4: Why FFT?**

### **4.1 The Insight: Divide and Conquer**

Instead of computing all $N$ outputs from scratch, FFT:
1. **Splits** the input into even and odd samples
2. **Recursively** computes smaller DFTs
3. **Combines** results using twiddle factors and symmetry

### **4.2 Complexity Reduction**

**Radix-2 FFT:**
- Two $N/2$-point DFTs: $2 \cdot (N/2)^2 = N^2/2$ operations
- Combining via butterflies: $N$ operations
- **Total: $N^2/2 + N = O(N^2/2)$** (only 50% improvement)

**But apply recursively:**
- Level 0: $N$ operations
- Level 1: $2 \times (N/2) = N$ operations
- Level 2: $4 \times (N/4) = N$ operations
- ...
- Level $\log_2 N$: $N$ operations

**Total: $N \log_2 N$ operations** ✓

### **4.3 Speed Comparison**

For real-time audio processing (1 MHz CPU):

| N | Direct DFT | FFT | Direct Time | FFT Time |
|---|---|---|---|---|
| 128 | 16,384 ops | 896 ops | 16.4 μs | 0.9 μs |
| 1024 | 1.05M ops | 10.2k ops | **1.05 ms** | **10 μs** |
| 4096 | 16.8M ops | 49.2k ops | **16.8 ms** | **49 μs** |

**For real-time audio (48 kHz = 48 samples/ms):**
- Direct DFT: Cannot process in real-time
- FFT: **100+ channels simultaneously possible**

---

## **PART 5: Radix-2 FFT Algorithm**

### **5.1 Algorithm Overview**

**Decimation-In-Time (DIT) Radix-2:**

```
Given: x[n], n = 0, 1, ..., N-1  (N = 2^m)

Step 1: Bit-Reversal
  Reorder input using bit-reversed indices

Step 2: Butterfly Stages (m = log₂ N stages)
  Stage s = 1, 2, ..., m
    Butterfly spacing: 2^(s-1)
    Twiddle increment: N/2^s
    For each butterfly pair:
      temp = W * B
      X_new = A + temp
      B_new = A - temp

Result: X[k] = FFT output (in bit-reversed order initially, then sorted)
```

### **5.2 Butterfly Structure**

Single butterfly operation:
```
    a ──┐
        ├─(+)─ A + W*B ──→ X₀
    b ─W┤
        ├─(-)─ A - W*B ──→ X₁
```

Where:
- $a$, $b$ are input values
- $W = W_N^k = e^{-j2\pi k/N}$ is the twiddle factor
- $X_0$, $X_1$ are outputs

### **5.3 Twiddle Factors to Memorize**

```
N=4:  W₄⁰=1,     W₄¹=-j,     W₄²=-1,    W₄³=j
N=8:  W₈⁰=1,     W₈¹=0.707-j0.707,  W₈²=-j,  W₈³=-0.707-j0.707
```

---

## **PART 6: Solved Problems - Radix-2 FFT**

### **Problem 1: 4-point FFT**

**Given:** $x[n] = \{1, 2, 3, 4\}$

#### **Step 1: Bit Reversal**

| Decimal | Binary | Reversed | New Index |
|---|---|---|---|
| 0 | 00 | 00 | 0 |
| 1 | 01 | 10 | 2 |
| 2 | 10 | 01 | 1 |
| 3 | 11 | 11 | 3 |

**Reordered:** $x'[n] = \{x[0], x[2], x[1], x[3]\} = \{1, 3, 2, 4\}$

#### **Step 2: Stage 1 (2-point DFTs)**

Twiddle factor: $W_2 = e^{-j\pi} = -1$ (but we use $W_2^0 = 1$)

**Butterfly pair 1:** $(1, 3)$ with $W_2^0 = 1$
$$y[0] = 1 + 1 \cdot 3 = 4$$
$$y[1] = 1 - 1 \cdot 3 = -2$$

**Butterfly pair 2:** $(2, 4)$ with $W_2^0 = 1$
$$y[2] = 2 + 1 \cdot 4 = 6$$
$$y[3] = 2 - 1 \cdot 4 = -2$$

**Stage 1 output:** $\{4, -2, 6, -2\}$

#### **Step 3: Stage 2 (4-point combination)**

Twiddle factors: $W_4^0 = 1$, $W_4^1 = -j$

**Butterfly 1:** $(y[0]=4, y[2]=6)$ with $W_4^0 = 1$
$$X[0] = 4 + 1 \cdot 6 = \boxed{10}$$
$$X[2] = 4 - 1 \cdot 6 = \boxed{-2}$$

**Butterfly 2:** $(y[1]=-2, y[3]=-2)$ with $W_4^1 = -j$
$$X[1] = -2 + (-j) \cdot (-2) = -2 + 2j = \boxed{-2 + 2j}$$
$$X[3] = -2 - (-j) \cdot (-2) = -2 - 2j = \boxed{-2 - 2j}$$

**Final Result:** $\boxed{X[k] = \{10, -2+2j, -2, -2-2j\}}$

✅ **Verify:** Compare with direct DFT method—same result!

---

### **Problem 2: 8-point FFT**

**Given:** $x[n] = \{2, 1, 2, 1, 0, 0, 0, 0\}$

#### **Step 1: Bit Reversal**

Binary reversal of indices (3-bit):

| Decimal | Binary | Reversed | Value |
|---|---|---|---|
| 0 | 000 | 000 | $x[0]=2$ |
| 1 | 001 | 100 | $x[4]=0$ |
| 2 | 010 | 010 | $x[2]=2$ |
| 3 | 011 | 110 | $x[6]=0$ |
| 4 | 100 | 001 | $x[1]=1$ |
| 5 | 101 | 101 | $x[5]=0$ |
| 6 | 110 | 011 | $x[3]=1$ |
| 7 | 111 | 111 | $x[7]=0$ |

**Reordered:** $\{2, 0, 2, 0, 1, 0, 1, 0\}$

#### **Step 2: Stage 1 (2-point DFTs, 4 pairs)**

*Butterfly pair 1:* $(2, 0)$ → $\{2, 2\}$
*Butterfly pair 2:* $(2, 0)$ → $\{2, 2\}$
*Butterfly pair 3:* $(1, 0)$ → $\{1, 1\}$
*Butterfly pair 4:* $(1, 0)$ → $\{1, 1\}$

**Stage 1 output:** $\{2, 2, 2, 2, 1, 1, 1, 1\}$

#### **Step 3: Stage 2 (4-point DFTs)**

Twiddles: $W_4^0=1, W_4^1=-j, W_4^2=-1, W_4^3=j$

*Group 1 (indices 0-3): inputs $\{2, 2, 2, 2\}$*
- Butterfly 1 (with $W_4^0=1$): $(2, 2)$ → $\{4, 0\}$
- Butterfly 2 (with $W_4^1=-j$): $(2, 2)$ → $\{2-2j, 2+2j\}$
- Output: $\{4, 2-2j, 0, 2+2j\}$

*Group 2 (indices 4-7): inputs $\{1, 1, 1, 1\}$*
- Butterfly 1 (with $W_4^0=1$): $(1, 1)$ → $\{2, 0\}$
- Butterfly 2 (with $W_4^1=-j$): $(1, 1)$ → $\{1-j, 1+j\}$
- Output: $\{2, 1-j, 0, 1+j\}$

**Stage 2 output:** $\{4, 2-2j, 0, 2+2j, 2, 1-j, 0, 1+j\}$

#### **Step 4: Stage 3 (8-point combination)**

Twiddles: $W_8^0=1, W_8^1=\frac{1}{\sqrt{2}}(1-j), W_8^2=-j, W_8^3=-\frac{1}{\sqrt{2}}(1+j)$

Top group (indices 0-3): $\{4, 2-2j, 0, 2+2j\}$
Bottom group (indices 4-7): $\{2, 1-j, 0, 1+j\}$

| k | Top | Bot | Twiddle | X[k] | X[k+4] |
|---|---|---|---|---|---|
| **0** | 4 | 2 | $W_8^0=1$ | $4+2=\boxed{6}$ | $4-2=\boxed{2}$ |
| **1** | $2-2j$ | $1-j$ | $W_8^1≈0.707-j0.707$ | $(2-2j)+W(1-j)=\boxed{2.414-j2.414}$ | $\boxed{1.586-j1.586}$ |
| **2** | $0$ | $0$ | $W_8^2=-j$ | $\boxed{0}$ | $\boxed{0}$ |
| **3** | $2+2j$ | $1+j$ | $W_8^3≈-0.707-j0.707$ | $\boxed{2.414+j0.414}$ | $\boxed{1.586+j3.414}$ |

**Final Result:**
$$\boxed{X[k] = \{6, 2.414-j2.414, 0, 2.414+j0.414, 2, 1.586-j1.586, 0, 1.586+j3.414\}}$$

---

### **Problem 3: 8-point FFT (Sparse Input)**

**Given:** $x[n] = \{1, 0, 1, 0, 1, 0, 1, 0\}$ (alternating pattern)

#### **Step 1: Bit Reversal**

Reordered: $\{1, 1, 1, 1, 0, 0, 0, 0\}$

#### **Step 2: Stage 1 (2-point DFTs)**

All pairs are either $(1, 1) \to \{2, 0\}$ or $(0, 0) \to \{0, 0\}$

**Stage 1 output:** $\{2, 0, 2, 0, 2, 0, 2, 0\}$

#### **Step 3: Stage 2 (4-point DFTs)**

*Group 1 (inputs $\{2, 0, 2, 0\}$):*
- Butterfly 1: $(2, 2)$ → $\{4, 0\}$
- Butterfly 2: $(0, 0)$ → $\{0, 0\}$
- Output: $\{4, 0, 0, 0\}$

*Group 2 (inputs $\{2, 0, 2, 0\}$):*
- Same → Output: $\{4, 0, 0, 0\}$

**Stage 2 output:** $\{4, 0, 0, 0, 4, 0, 0, 0\}$

#### **Step 4: Stage 3 (8-point combination)**

Top: $\{4, 0, 0, 0\}$, Bottom: $\{4, 0, 0, 0\}$

| k | Calculation | X[k] |
|---|---|---|
| **0** | $4 + 1 \cdot 4 = 8$ | $\boxed{8}$ |
| **1** | $0 + W_8^1 \cdot 0 = 0$ | $\boxed{0}$ |
| **2** | $0 + (-j) \cdot 0 = 0$ | $\boxed{0}$ |
| **3** | $0 + W_8^3 \cdot 0 = 0$ | $\boxed{0}$ |
| **4** | $4 - 1 \cdot 4 = 0$ | $\boxed{0}$ |
| **5** | $0 - W_8^1 \cdot 0 = 0$ | $\boxed{0}$ |
| **6** | $0 - (-j) \cdot 0 = 0$ | $\boxed{0}$ |
| **7** | $0 - W_8^3 \cdot 0 = 0$ | $\boxed{0}$ |

**Final Result:** $\boxed{X[k] = \{8, 0, 0, 0, 0, 0, 0, 0\}}$

**Interpretation:** Alternating $\{1, 0, 1, 0, 1, 0, 1, 0\}$ is a Nyquist frequency component (DC + highest frequency), so only bin 0 has energy.

---

## **Summary & Quick Checklist**

✅ **DTFT vs DFT:** Understand the difference (continuous vs discrete)
✅ **Direct DFT complexity:** $O(N^2)$ — too slow for large N
✅ **Twiddle redundancy:** Exploit symmetry to reduce operations
✅ **FFT speedup:** $O(N \log N)$ → 100x+ faster
✅ **Bit reversal:** Always reorder input first
✅ **Butterflies:** Master the $(+)$ and $(-)$ operation
✅ **Twiddles:** Know $W_4, W_8$ by heart
✅ **Practice:** Solve 4-pt, 8-pt by hand until automatic

---
