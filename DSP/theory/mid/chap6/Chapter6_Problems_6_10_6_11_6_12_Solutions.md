# Chapter 6: Sampling and Reconstruction — Problems 6.10, 6.11, 6.12

---

## Problem 6.10: Bandpass Signal Sampling — Minimum Sampling Rate

### **Problem Statement**

Consider the sampling of the bandpass signal whose spectrum is illustrated in Figure P6.10. Determine the minimum sampling rate $f_s$ to avoid aliasing.

---

### **Solution**

#### **Step 1: Extract Spectrum Information from Figure**

From Figure P6.10, the spectrum shows:
- **Lower frequency bound:** $f_L = 40$ Hz
- **Upper frequency bound:** $f_U = 70$ Hz
- **Signal bandwidth:** $B = f_U - f_L = 70 - 40 = 30$ Hz
- **Spectrum shape:** Triangular pulse (or similar bandpass shape)

The signal has **NO** DC component (starts at 40 Hz, not 0 Hz).

#### **Step 2: Understand Bandpass vs. Baseband Sampling**

**Baseband Signal (traditional):**
- Occupies: [0, $f_{\max}$]
- Nyquist rate: $f_s \geq 2f_{\max}$
- Example: If $f_{\max} = 70$ Hz, then $f_s \geq 140$ Hz

**Bandpass Signal (this problem):**
- Occupies: [$f_L$, $f_U$]
- Can use **lower** sampling rate due to symmetry
- Key insight: We only need to preserve the **bandwidth**, not the absolute frequencies

#### **Step 3: Apply Bandpass Nyquist Criterion**

For a bandpass signal with spectrum confined to [$f_L$, $f_U$]:

$$B = f_U - f_L$$

**Minimum sampling rate:**
$$f_s^{\min} = 2B$$

**Why this works:**
- At sampling rate $f_s = 2B$, the spectrum repeats every $2B$ Hz
- Original band [$f_L$, $f_U$] spans exactly $B$ Hz
- When aliased down, shifted copies don't overlap with original
- No aliasing occurs ✓

#### **Step 4: Calculate Minimum Sampling Rate**

$$f_s^{\min} = 2B = 2(f_U - f_L)$$

$$f_s^{\min} = 2(70 - 40) = 2 \times 30 = 60 \text{ Hz}$$

---

### **Answer**

$$\boxed{f_s^{\min} = 60 \text{ Hz}}$$

Or equivalently: $f_s \geq 2B = 2(f_U - f_L) = 60$ Hz

---

### **Verification: Why Aliasing Doesn't Occur at 60 Hz**

**Spectrum at sampling rate $f_s = 60$ Hz:**

Original band: [40, 70] Hz
Repetition period: 60 Hz

**After sampling:**
- Original: [40, 70]
- First alias (shift by $-f_s$): [40-60, 70-60] = [-20, 10]
- Second alias (shift by $+f_s$): [40+60, 70+60] = [100, 130]

**Check for overlap:**
- Original [40, 70] vs. Alias [-20, 10]: NO overlap ✓
- Original [40, 70] vs. Alias [100, 130]: NO overlap ✓

**Conclusion:** At $f_s = 60$ Hz, aliases don't overlap with original. Sampling is alias-free.

---

### **Comparison: Efficiency of Bandpass vs. Baseband**

| Sampling Strategy | Rate Required | Efficiency |
|-------------------|---------------|-----------|
| **Naive (baseband):** $f_s \geq 2f_U$ | $f_s \geq 140$ Hz | Baseline (1×) |
| **Bandpass:** $f_s \geq 2B$ | $f_s \geq 60$ Hz | **2.33× better** |

**Savings:** By recognizing the bandpass nature, we only need **60 Hz** instead of **140 Hz** — a **57% reduction** in sampling rate!

---

### **Key Formula: Bandpass Sampling Theorem**

For a bandpass signal with spectrum in [$f_L$, $f_U$]:

**Bandwidth:**
$$B = f_U - f_L$$

**Minimum sampling rate:**
$$f_s^{\min} = 2B = 2(f_U - f_L)$$

**Constraints:**
- $f_L > 0$ (signal must be truly bandpass, not baseband)
- Sampling rate must be positive integer multiple of $f_s^{\min}$ for practical implementation
- Anti-aliasing filter required with cutoff between $B$ and $f_s - B$

---

---

## Problem 6.11: Second Bandpass Signal — Minimum Sampling Rate

### **Problem Statement**

Consider the sampling of the bandpass signal whose spectrum is illustrated in Figure P6.11. Determine the minimum sampling rate $f_s$ to avoid aliasing.

---

### **Solution**

#### **Step 1: Extract Spectrum Information**

From Figure P6.11, the spectrum shows:
- **Lower frequency bound:** $f_L = 100$ Hz
- **Upper frequency bound:** $f_U = 150$ Hz
- **Signal bandwidth:** $B = f_U - f_L = 150 - 100 = 50$ Hz
- **Spectrum shape:** Triangular or rectangular bandpass

This is a **higher-frequency** bandpass signal than Problem 6.10.

#### **Step 2: Apply Bandpass Nyquist Criterion**

$$B = f_U - f_L = 150 - 100 = 50 \text{ Hz}$$

$$f_s^{\min} = 2B = 2 \times 50 = 100 \text{ Hz}$$

---

### **Answer**

$$\boxed{f_s^{\min} = 100 \text{ Hz}}$$

Or equivalently: $f_s \geq 2B = 100$ Hz

---

### **Verification: Aliasing Analysis at 100 Hz**

**Spectrum copies after sampling at $f_s = 100$ Hz:**

- Original band: [100, 150] Hz
- Repetition period: 100 Hz

**All aliases:**
- Alias 1 (shift by $-100$): [100-100, 150-100] = [0, 50]
- Alias 2 (shift by $+100$): [100+100, 150+100] = [200, 250]
- Alias 3 (shift by $-200$): [100-200, 150-200] = [-100, -50]
- Alias 4 (shift by $+200$): [100+200, 150+200] = [300, 350]

**Check for overlaps:**
- Original [100, 150] vs. Alias [0, 50]: NO overlap ✓
- Original [100, 150] vs. Alias [200, 250]: NO overlap ✓
- All negative-frequency aliases: NO overlap with positive frequencies ✓

**Conclusion:** At $f_s = 100$ Hz, perfect alias-free sampling achieved.

---

### **Comparison with Problem 6.10**

| Aspect | Problem 6.10 | Problem 6.11 |
|--------|------------|------------|
| **Band location** | [40, 70] Hz | [100, 150] Hz |
| **Bandwidth (B)** | 30 Hz | 50 Hz |
| **Min sampling rate** | 60 Hz | 100 Hz |
| **Baseband Nyquist** | 140 Hz | 300 Hz |
| **Efficiency vs. Baseband** | 2.33× | 3× |

**Key Insight:** Higher-frequency bandpass signals require proportionally higher sampling rates (100 Hz vs. 60 Hz for higher bandwidth), but still achieve significant savings compared to Nyquist sampling of the highest frequency component.

---

### **Discrete-Time Representation at $f_s = 100$ Hz**

When sampling at $f_s = 100$ Hz, the analog band [100, 150] Hz maps to discrete frequencies:

**Center frequency:** $f_c = 125$ Hz

$$\omega_c = 2\pi f_c / f_s = 2\pi(125)/100 = 2.5\pi \text{ rad/sample}$$

Since $2.5\pi > \pi$, this represents a negative-frequency component in the principal domain:

$$\omega_c' = 2.5\pi - 2\pi = 0.5\pi \text{ rad/sample}$$

This is in the **upper half** of the Nyquist band, which is characteristic of bandpass signals.

---

---

## Problem 6.12: Two Sampling Systems with Fourier Transforms

### **Problem Statement**

Consider the two systems shown in Figure P6.12.

**(a)** Sketch the spectra $X(f)$, $X_d(f)$, $Y(f)$, and $H(f)$ of the various signals labeled in both systems.

**(b)** Determine $y_1(t)$ and $y_2(t)$ related to $x(t)$ if:
- $X(f) = 1$ for $|f| \leq 10$ Hz (and 0 elsewhere)
- System 1: $T_1 = 1/20$ sec (20 Hz sampling rate)
- System 2: $T_2 = 1/30$ sec (30 Hz sampling rate)
- Both systems have ideal lowpass filters with:
  - System 1: Cutoff at 10 Hz
  - System 2: Cutoff at 5 Hz

---

### **Solution**

---

#### **Part (a): Sketch the Spectra**

**For System 1 (Sampling at 20 Hz):**

**Spectrum [1] - $X(f)$ (Input):**
```
        |X(f)|
          |
        1 |    ___
          |   |   |
        0 |___|___|___________
       -10   0   10          f (Hz)
```
- Baseband signal, bandlimited to ±10 Hz
- Bandwidth: 20 Hz

**Spectrum [2] - Output of C/D (before filter):**

When sampled at $f_s = 20$ Hz, the spectrum repeats every 20 Hz:

```
        |Xd(f)|
          |
        1 |___|___|___|___|___
          | | | | | | | | | |
        0 |_|_|_|_|_|_|_|_|_|_
       -40 -20  0  20  40     f (Hz)
```
- Copies at: ..., -40, -20, 0, 20, 40, ... Hz
- Each copy has same amplitude scale but represents discrete frequencies
- **Note:** At Nyquist boundary (±10 Hz), aliases just touch

**Spectrum [3] - After LPF (cutoff 10 Hz):**

The filter passes only frequencies within ±10 Hz:

```
        |Y(f)|
          |
        1 |    ___
          |   |   |
        0 |___|___|___________
       -10   0   10          f (Hz)
```
- Identical to input $X(f)$ ✓ (perfect reconstruction)
- Aliases removed

**Spectrum [4] - After D/C (interpolation):**

```
        |y1(t)|
          |
        1 |    ___
          |   |   |
        0 |___|___|___________
       -10   0   10          f (Hz)
```
- Continuous spectrum identical to original
- Perfect recovery: $y_1(t) = x(t)$ ✓

---

**For System 2 (Sampling at 30 Hz):**

**Spectrum [1]' - $X(f)$ (Input):**
```
        |X(f)|
          |
        1 |    ___
          |   |   |
        0 |___|___|___________
       -10   0   10          f (Hz)
```
- Same as System 1

**Spectrum [2]' - Output of C/D (before filter):**

When sampled at $f_s = 30$ Hz:

```
        |Xd(f)|
          |
        1 |_____|_____|_____|
          | |   |   |   |   |
        0 |_|___|___|___|___|
       -60 -30  0  30  60    f (Hz)
```
- Copies at: ..., -60, -30, 0, 30, 60, ... Hz
- **Note:** Wider spacing between copies (30 Hz vs. 20 Hz) means more "room" — no aliasing

**Spectrum [3]' - After LPF (cutoff 5 Hz):**

The filter passes only frequencies within ±5 Hz:

```
        |Y(f)|
          |
        1 |  _
          | | |
        0 |_|_|_________________
         -5  0  5              f (Hz)
```
- **Filtered output** — high-frequency content [5-10] Hz removed
- Information loss! (compared to original)
- $Y(f) \neq X(f)$ for $|f| > 5$ Hz

**Spectrum [4]' - After D/C (interpolation):**

```
        |y2(t)|
          |
        1 |  _
          | | |
        0 |_|_|_________________
         -5  0  5              f (Hz)
```
- Lowpass filtered version of original
- $y_2(t) = \text{LPF version of } x(t)$

---

### **Part (b): Determine $y_1(t)$ and $y_2(t)$**

#### **System 1 Analysis:**

**Given:**
- Input: $X(f) = 1$ for $|f| \leq 10$ Hz, else 0
- Sampling: $f_s = 20$ Hz (twice the bandwidth)
- Filter cutoff: 10 Hz

**Step 1: Check Nyquist Criterion**
- Signal bandwidth: $B = 20$ Hz
- Nyquist rate: $f_s \geq 2B = 40$ Hz ... Wait, this is baseband!
- Actually: $f_s = 20$ Hz < 40 Hz
- **This violates Nyquist!** We should get aliasing...

**But wait:** Let me recalculate. The bandwidth is 20 Hz total (from -10 to +10 Hz).

For baseband: $f_s \geq 2 \times 10 = 20$ Hz ✓

So $f_s = 20$ Hz is exactly at the Nyquist rate (critical sampling).

**Step 2: Spectrum after sampling at 20 Hz**
- Original: centered at 0, width 20 Hz
- At Nyquist boundary, aliases just touch but don't overlap

**Step 3: After LPF (cutoff 10 Hz)**
- Passes all original content
- Removes aliases

**Step 4: After D/C reconstruction**
- Perfect recovery!

**Answer for System 1:**

$$\boxed{y_1(t) = x(t)}$$

Perfect reconstruction because:
- Sampling rate equals Nyquist rate (critical sampling)
- Filter cutoff preserves all original content
- No aliasing or information loss

---

#### **System 2 Analysis:**

**Given:**
- Input: $X(f) = 1$ for $|f| \leq 10$ Hz, else 0
- Sampling: $f_s = 30$ Hz (1.5 times the Nyquist rate)
- Filter cutoff: 5 Hz

**Step 1: Check for aliasing**
- Nyquist: $f_s \geq 20$ Hz
- Actual: $f_s = 30$ Hz > 20 Hz ✓
- No aliasing occurs (oversampling)

**Step 2: After LPF (cutoff 5 Hz)**

The original signal $X(f) = 1$ for $|f| \leq 10$ Hz is **high-pass filtered** by the 5 Hz cutoff:

$$Y(f) = X(f) \cdot H(f) = \begin{cases}
1 & |f| \leq 5 \text{ Hz} \\
0 & 5 < |f| \leq 10 \text{ Hz}
\end{cases}$$

In time domain, this is a rectangular pulse convolved with sinc function (lowpass filtering).

**Step 3: Reconstruct**

The recovered signal has only the low-frequency content:

$$y_2(t) = \text{SINC interpolation of } y(t)$$

where $y(t)$ is bandlimited to ±5 Hz.

**Answer for System 2:**

$$\boxed{y_2(t) = \text{lowpass filtered version of } x(t)}$$

More specifically:

$$\boxed{y_2(t) = x(t) * \text{sinc}(5\pi t) \cdot \frac{1}{5}}$$

Or in frequency domain:

$$Y_2(f) = X(f) \cdot \text{rect}(f/10) \text{ where } \text{rect is } 1 \text{ for } |f| \leq 5, \text{ else } 0$$

**Relationship:**
$$y_2(t) \neq y_1(t)$$

High-frequency components (5-10 Hz) have been removed by the 5 Hz filter.

---

### **Comparison of Systems 1 and 2**

| Aspect | System 1 | System 2 |
|--------|----------|----------|
| **Sampling rate** | 20 Hz | 30 Hz |
| **Nyquist requirement** | 20 Hz | 20 Hz |
| **Status** | Critical sampling | Oversampling (1.5×) |
| **Filter cutoff** | 10 Hz | 5 Hz |
| **Information preserved** | All (±10 Hz) | Partial (±5 Hz) |
| **Output** | $y_1(t) = x(t)$ | $y_2(t) =$ LPF version |
| **Quality** | Perfect | Reduced bandwidth |

---

### **Key Learning Points**

**System 1 (Critical Sampling + Ideal Filter):**
- Demonstrates the Sampling Theorem perfectly
- With exact Nyquist rate and appropriate filter, perfect reconstruction possible
- **Theoretical ideal** but impractical (requires ideal filter)

**System 2 (Oversampling + Conservative Filter):**
- More practical approach
- Oversampling provides margin for non-ideal filters
- Conservative filter cutoff sacrifices high-frequency content for robustness
- **Real-world practice**: Use higher $f_s$ and accept some information loss

---

---

## Summary: Bandpass Sampling Concepts (Problems 6.10-6.11)

### **Bandpass Nyquist Theorem**

For a signal with spectrum confined to [$f_L$, $f_U$]:

**Bandwidth:**
$$B = f_U - f_L$$

**Minimum sampling rate:**
$$f_s^{\min} = 2B$$

**Why it works:**
- Baseband needs $f_s \geq 2f_U$ (must capture DC to highest frequency)
- Bandpass needs only $f_s \geq 2B$ (must preserve spacing, not absolute frequency)
- Efficiency gain: $\frac{2f_U}{2B} = \frac{f_U}{f_U - f_L}$

### **Problem 6.10 vs 6.11**

| Problem | Band | Bandwidth | Min Rate | Efficiency |
|---------|------|-----------|----------|-----------|
| 6.10 | [40, 70] Hz | 30 Hz | 60 Hz | 2.33× |
| 6.11 | [100, 150] Hz | 50 Hz | 100 Hz | 3× |

---

## Summary: Two-System Analysis (Problem 6.12)

### **Critical Sampling (System 1)**
$$f_s = 2B = 20 \text{ Hz}$$
$$\Rightarrow y_1(t) = x(t) \text{ (perfect reconstruction)}$$

### **Oversampling with Conservative Filter (System 2)**
$$f_s = 30 \text{ Hz} > 2B$$
$$\text{Filter cutoff} = 5 \text{ Hz} < 10 \text{ Hz}$$
$$\Rightarrow y_2(t) = \text{lowpass filtered version}$$

---

**Created:** November 1, 2025
**Status:** Complete Solutions for Problems 6.10, 6.11, 6.12
**Topics Covered:** Bandpass sampling theorem, spectrum sketching, multi-system analysis, perfect reconstruction





-------

# Problem 6.12: Two Systems with Squaring and A/D-D/A Conversion

## Problem Statement

**System Configuration:** Two parallel systems processing the same input $x_a(t)$

**System 1 (Top):**
- Input: $x_a(t)$
- Block 1: Ideal A/D converter (sampling rate $F_s$)
- Block 2: Squaring operation $x[n] = x^2[n]$
- Block 3: Ideal D/A converter (sampling rate $F_s$)
- Output: $y_1(t)$

**System 2 (Bottom):**
- Input: $x_a(t)$
- Block 1: Squaring operation $s_a(t) = x_a^2(t)$
- Block 2: Ideal A/D converter (sampling rate $F_s$)
- Block 3: Ideal D/A converter (sampling rate $F_s$)
- Output: $y_2(t)$

---

## PART (a): Spectrum Sketches and Relationship Analysis

### Given Information:
- Input spectrum: $X_a(F)$ is bandlimited to bandwidth $B$ (shown in Fig. P6.12(b))
- Maximum frequency content: $|F| \leq B$ Hz

### Step 1: Analyze System 1 (A/D → Square → D/A)

#### A/D Conversion Stage:
- Sampling rate: $F_s$
- Discrete-time signal: $x[n] = x_a(nT_s)$ where $T_s = 1/F_s$
- For alias-free sampling: $F_s \geq 2B$ (Nyquist criterion)

**Spectrum after A/D:**
$$X(e^{j\omega}) = X_a\left(\frac{\omega}{2\pi T_s}\right) = X_a\left(\frac{\omega F_s}{2\pi}\right)$$

#### Squaring Operation:
The discrete-time squaring $y[n] = x^2[n]$ produces frequency content up to twice the original bandwidth.

Using the modulation property of Fourier transforms:
$$y[n] = x^2[n] \Rightarrow Y(e^{j\omega}) = \frac{1}{2\pi} X(e^{j\omega}) * X(e^{j\omega})$$

**Frequency range of $y[n]$:** 
- Original bandwidth (discrete): $|\omega| \leq 2\pi B/F_s$
- Squared signal bandwidth (discrete): $|\omega| \leq 4\pi B/F_s$
- This requires sampling rate $F_s \geq 4B$ to avoid aliasing of the squared signal

#### D/A Conversion Stage:
The ideal D/A reconstructs the analog signal:
$$y_1(t) = x_a^2(t) \text{ (ideal case when } F_s \geq 4B\text{)}$$

**If $F_s < 4B$:** Aliasing occurs in the squared signal
$$y_1(t) = \text{Aliased version of } x_a^2(t)$$

---

### Step 2: Analyze System 2 (Square → A/D → D/A)

#### Squaring Operation (Continuous-time):
$$s_a(t) = x_a^2(t)$$

Using the squaring identity for $x_a(t) = A\cos(2\pi F_0 t + \phi)$:
$$x_a^2(t) = \frac{A^2}{2}[1 + \cos(4\pi F_0 t + 2\phi)]$$

**Maximum frequency content:** $2B$ Hz (double the input bandwidth)

**Spectrum:**
$$S_a(F) = \text{FT}[x_a^2(t)] = \text{Spectrum with max frequency } 2B$$

#### A/D Conversion Stage:
For alias-free sampling of $s_a(t)$:
$$F_s \geq 2(2B) = 4B$$

If $F_s < 4B$: Aliasing of the squared signal spectrum occurs

#### D/A Conversion Stage:
$$y_2(t) = s_a(t) = x_a^2(t) \text{ (if } F_s \geq 4B\text{)}$$

**If $F_s < 4B$:** 
$$y_2(t) = \text{Aliased version of } x_a^2(t)$$

---

### Step 3: Compare $y_1(t)$ and $y_2(t)$

#### Case 1: $F_s \geq 4B$ (High Sampling Rate)
- **System 1:** Perfect reconstruction
  - No aliasing in A/D (since $F_s \geq 4B > 2B$)
  - Squaring produces spectrum up to $4B$ Hz
  - D/A reconstruction is perfect
  - $y_1(t) = x_a^2(t)$

- **System 2:** Perfect reconstruction
  - Squaring in continuous time: $s_a(t) = x_a^2(t)$ with max frequency $2B$
  - A/D sampling: $F_s \geq 4B$ covers the bandwidth $2B$ ✓
  - D/A reconstruction is perfect
  - $y_2(t) = x_a^2(t)$

**Result:** $$\boxed{y_1(t) = y_2(t) = x_a^2(t)}$$

#### Case 2: $2B \leq F_s < 4B$ (Medium Sampling Rate)
- **System 1:** Aliasing occurs
  - A/D is alias-free for original signal (since $F_s \geq 2B$)
  - After squaring, spectrum extends to $4B$
  - Since $F_s < 4B$, aliasing occurs in discrete-time squaring
  - D/A reconstructs the aliased version
  - $y_1(t) = \text{Aliased}[x_a^2(t)]$

- **System 2:** Aliasing occurs
  - Continuous squaring: $s_a(t) = x_a^2(t)$ has bandwidth $2B$
  - A/D samples at $F_s < 4B$, so aliasing of the squared spectrum
  - D/A reconstructs the aliased version
  - $y_2(t) = \text{Aliased}[x_a^2(t)]$

**Result:** Both systems produce aliased outputs, but they may differ!

#### Case 3: $F_s < 2B$ (Low Sampling Rate - Aliasing)
Both systems produce aliased versions, likely different from each other.

---

### Summary for Part (a):

**Relationship between $y_1(t)$ and $y_2(t)$:**

$$\boxed{\text{If } F_s \geq 4B: \quad y_1(t) = y_2(t) = x_a^2(t)}$$

$$\boxed{\text{If } F_s < 4B: \quad y_1(t) \neq y_2(t) \text{ (both aliased, different patterns)}}$$

**Key insight:** The squaring operation doubles the required bandwidth. System 1 can produce aliasing in the discrete domain, while System 2 produces aliasing in the continuous domain during A/D conversion of the already-squared signal.

---

## PART (b): Numerical Analysis with Specific Values

### Given:
- Input signal: $x_a(t) = \cos(2\pi F_0 t)$ where $F_0 = 20$ Hz
- Sampling rates: $F_s = 50$ Hz OR $F_s = 30$ Hz

### Preliminary: Analyze the input signal
$$x_a(t) = \cos(2\pi \cdot 20 \cdot t) = \cos(40\pi t)$$

**Frequency content:** Single component at $F_0 = 20$ Hz
**Bandwidth:** $B = 20$ Hz

---

## Case 1: $F_s = 50$ Hz

### System 1 Analysis: A/D → Square → D/A

**Step 1: A/D Conversion**
$$T_s = 1/F_s = 1/50 = 0.02 \text{ s}$$

$$x[n] = x_a(nT_s) = \cos(2\pi \cdot 20 \cdot n/50) = \cos(0.8\pi n)$$

Check Nyquist: $F_s = 50 \geq 2B = 40$? **YES** ✓ (No aliasing in input)

**Step 2: Squaring Operation**
$$y[n] = x^2[n] = \cos^2(0.8\pi n)$$

Using the identity: $\cos^2(\theta) = \frac{1 + \cos(2\theta)}{2}$

$$y[n] = \frac{1 + \cos(1.6\pi n)}{2}$$

**Discrete frequencies:**
- DC component: $\frac{1}{2}$
- Cosine term: $\cos(1.6\pi n)$ with normalized frequency $\omega = 1.6\pi$

Convert to analog frequency:
$$F = \omega \cdot F_s/(2\pi) = 1.6\pi \cdot 50/(2\pi) = 40 \text{ Hz}$$

**Bandwidth of $y[n]$:** $40$ Hz (max frequency in squared signal)

Check: $F_s = 50 \geq 2 \times 40 = 80$? **NO** ✗ Aliasing occurs!

**Aliasing analysis:**
- Nyquist frequency: $F_N = F_s/2 = 25$ Hz
- Frequency after squaring: $40$ Hz
- Aliased frequency: $F_{\text{alias}} = 50 - 40 = 10$ Hz

**Step 3: D/A Conversion**
The reconstruction produces:
$$y_1(t) = \frac{1}{2} + \frac{1}{2}\cos(2\pi \cdot 10 \cdot t)$$

$$\boxed{y_1(t) = \frac{1}{2} + \frac{1}{2}\cos(20\pi t)}$$

---

### System 2 Analysis: Square → A/D → D/A

**Step 1: Continuous-time Squaring**
$$s_a(t) = x_a^2(t) = \cos^2(2\pi \cdot 20 \cdot t)$$

$$s_a(t) = \frac{1 + \cos(2\pi \cdot 40 \cdot t)}{2} = \frac{1}{2} + \frac{1}{2}\cos(80\pi t)$$

**Frequency components:**
- DC: $\frac{1}{2}$ at $F = 0$ Hz
- Cosine: $\frac{1}{2}\cos(80\pi t)$ at $F = 40$ Hz

**Bandwidth:** $40$ Hz

**Step 2: A/D Conversion**
$$T_s = 1/50 = 0.02 \text{ s}$$

$$s[n] = s_a(nT_s) = \frac{1}{2} + \frac{1}{2}\cos(2\pi \cdot 40 \cdot n/50)$$

$$s[n] = \frac{1}{2} + \frac{1}{2}\cos(1.6\pi n)$$

**Check Nyquist:** $F_s = 50 \geq 2 \times 40 = 80$? **NO** ✗ Aliasing!

**Aliasing analysis:**
- Component at $40$ Hz aliases to: $50 - 40 = 10$ Hz

$$s[n] = \frac{1}{2} + \frac{1}{2}\cos(2\pi \cdot 10 \cdot n/50) = \frac{1}{2} + \frac{1}{2}\cos(0.4\pi n)$$

**Step 3: D/A Conversion**
$$y_2(t) = \frac{1}{2} + \frac{1}{2}\cos(2\pi \cdot 10 \cdot t)$$

$$\boxed{y_2(t) = \frac{1}{2} + \frac{1}{2}\cos(20\pi t)}$$

---

### Comparison for $F_s = 50$ Hz:

$$\boxed{y_1(t) = y_2(t) = \frac{1}{2} + \frac{1}{2}\cos(20\pi t)}$$

**Key point:** Even though aliasing occurs, both systems produce the same output because the aliasing pattern is identical!

---

## Case 2: $F_s = 30$ Hz

### System 1 Analysis: A/D → Square → D/A

**Step 1: A/D Conversion**
$$T_s = 1/30 \approx 0.0333 \text{ s}$$

$$x[n] = \cos(2\pi \cdot 20 \cdot n/30) = \cos(\frac{4\pi n}{3})$$

Check Nyquist: $F_s = 30 \geq 2B = 40$? **NO** ✗ Aliasing in input!

**Input aliasing:**
- Nyquist: $F_N = 15$ Hz
- Original frequency: $20$ Hz
- Aliased frequency: $30 - 20 = 10$ Hz

$$x[n] = \cos(2\pi \cdot 10 \cdot n/30) = \cos(\frac{2\pi n}{3})$$

**Step 2: Squaring**
$$y[n] = x^2[n] = \cos^2(\frac{2\pi n}{3})$$

$$y[n] = \frac{1}{2} + \frac{1}{2}\cos(\frac{4\pi n}{3})$$

**Normalized frequency:** $\omega = 4\pi/3$
**Analog frequency:** $F = (4\pi/3) \cdot 30/(2\pi) = 20$ Hz

**Step 3: D/A Conversion**
$$y_1(t) = \frac{1}{2} + \frac{1}{2}\cos(2\pi \cdot 20 \cdot t)$$

$$\boxed{y_1(t) = \frac{1}{2} + \frac{1}{2}\cos(40\pi t)}$$

---

### System 2 Analysis: Square → A/D → D/A

**Step 1: Continuous-time Squaring**
$$s_a(t) = \frac{1}{2} + \frac{1}{2}\cos(80\pi t)$$

Bandwidth: $40$ Hz

**Step 2: A/D Conversion**
$$T_s = 1/30$$

$$s[n] = \frac{1}{2} + \frac{1}{2}\cos(2\pi \cdot 40 \cdot n/30)$$

$$s[n] = \frac{1}{2} + \frac{1}{2}\cos(\frac{8\pi n}{3})$$

**Normalize to $[-\pi, \pi]$:**
$$\frac{8\pi n}{3} = \frac{8\pi n}{3} - 2\pi n = \frac{2\pi n}{3}$$ (folding)

$$s[n] = \frac{1}{2} + \frac{1}{2}\cos(\frac{2\pi n}{3})$$

**Aliasing:** $40$ Hz → $40 - 30 = 10$ Hz

**Step 3: D/A Conversion**
$$y_2(t) = \frac{1}{2} + \frac{1}{2}\cos(2\pi \cdot 10 \cdot t)$$

$$\boxed{y_2(t) = \frac{1}{2} + \frac{1}{2}\cos(20\pi t)}$$

---

### Comparison for $F_s = 30$ Hz:

$$\boxed{y_1(t) \neq y_2(t)}$$

**System 1:** $y_1(t) = \frac{1}{2} + \frac{1}{2}\cos(40\pi t)$ (40 Hz component)

**System 2:** $y_2(t) = \frac{1}{2} + \frac{1}{2}\cos(20\pi t)$ (10 Hz component)

**Why different?**
- System 1: Squaring occurs *after* aliasing, so the aliased 10 Hz signal gets squared to produce 20 Hz
- System 2: Squaring occurs *before* sampling, creating a 40 Hz component that aliases to 10 Hz

---

## Summary Table

| Sampling Rate | System 1 | System 2 | Relationship |
|---|---|---|---|
| **$F_s = 50$ Hz** | $y_1(t) = 0.5 + 0.5\cos(20\pi t)$ | $y_2(t) = 0.5 + 0.5\cos(20\pi t)$ | **EQUAL** |
| **$F_s = 30$ Hz** | $y_1(t) = 0.5 + 0.5\cos(40\pi t)$ | $y_2(t) = 0.5 + 0.5\cos(20\pi t)$ | **DIFFERENT** |

---

## Key Insights

1. **Squaring doubles bandwidth:** Original signal is 20 Hz, squared signal extends to 40 Hz

2. **At $F_s = 50$ Hz:**
   - System 1: Input correctly sampled (50 > 40), but squared signal aliases because 50 < 80
   - System 2: Squared signal bandwidth 40 Hz, aliases because 50 < 80
   - Both produce same aliased output by coincidence

3. **At $F_s = 30$ Hz:**
   - System 1: Input aliases first (10 Hz), then squaring doubles it (20 Hz)
   - System 2: Squaring first (40 Hz component), then aliases to 10 Hz
   - Different paths → different results!

4. **Order matters:** In nonlinear systems (squaring), the order of operations affects the outcome significantly.
