# LAB 11: FIR FILTER DESIGN WITH WINDOWS — BEGINNER'S GUIDE
## Everything You Need to Know (Simple Language, No Mess)

---

## PREREQUISITES CHECKLIST
### Before Starting This Lab, Make Sure You Know...

**CRITICAL (Must Understand):**
- ✔ **FIR vs IIR filters** — Non-recursive vs recursive structures
- ✔ **Impulse response h[n]** — Filter's output to unit impulse
- ✔ **Ideal filters** — What we want but can't have (infinite duration)
- ✔ **Frequency response** — Magnitude and phase from DTFT
- ✔ **Passband/Stopband** — Pass frequencies vs block frequencies
- ✔ **Windowing concept** — How to truncate infinite responses
- ✔ **Window functions** — Rectangular, Hanning, Hamming, Blackman
- ✔ **Linear phase** — Constant delay across frequencies (no distortion)

**IMPORTANT (Should Understand):**
- ✔ **DTFT (Discrete-Time Fourier Transform)** — Frequency representation of DT signals
- ✔ **Sinc function** — Mathematical basis of ideal filter impulse response
- ✔ **Trade-offs** — Transition width vs stopband attenuation
- ✔ **Windowing design** — How windows affect filter characteristics
- ✔ **MATLAB commands** — fir1(), window functions, freqz(), fvtool()

**NICE TO HAVE (Good background):**
- Fourier series and Fourier transform concepts
- Complex exponentials and phasors
- Decibel calculations and logarithmic scales
- Symmetry properties of signals

---

## DETAILED PREREQUISITES GUIDE
### Complete Reference for All Topics

---

### **TOPIC 1: FIR Filter Basics**

#### **1.1 — What is an FIR Filter?**

**FIR = Finite Impulse Response**

A filter whose impulse response **h[n]** is:
- **Finite duration** — Non-zero only for $0 \leq n \leq M-1$
- **No feedback** — Output depends only on current and past inputs
- **Non-recursive** — $y[n] = \sum_{k=0}^{M-1} h[k] \cdot x[n-k]$

```
FIR Filter Structure:
    x[n] ──→ h[0] ──┐
              ↓     │
             z^-1   │
              ↓     ├──→ Σ ──→ y[n]
             h[1]   │
              ↓     │
             z^-1   │
              ↓     │
             h[2]  ─┘
            ...
             
(Each tap multiplied by h coefficient and summed)
```

**Why FIR?**
- ✅ **Always stable** — No poles, only zeros
- ✅ **Linear phase possible** — Symmetric coefficients = constant delay
- ✅ **Design is straightforward** — Can use windowing method
- ❌ **Higher order needed** — Compared to IIR for same specs
- ❌ **More computation** — More multiply-add operations

---

#### **1.2 — FIR vs IIR at a Glance**

| **Property** | **FIR** | **IIR** |
|---|---|---|
| **Impulse response** | Finite duration | Infinite duration |
| **Feedback** | No | Yes (recursive) |
| **Stability** | Always stable | Can be unstable |
| **Linear phase** | Easy (symmetric h[n]) | Difficult |
| **Filter order** | Higher | Lower |
| **Implementation** | Simple (convolution) | More complex (poles/zeros) |
| **Design method** | Window, Remez, etc. | Bilinear transform, etc. |

---

### **TOPIC 2: Ideal Filters (The Problem)**

#### **2.1 — What is an Ideal Filter?**

An **ideal filter** has:
- **Perfect passband** — All frequencies pass unchanged ($H = 1$)
- **Perfect stopband** — All other frequencies blocked completely ($H = 0$)
- **Sharp transition** — No gradual roll-off

```
Ideal Low-Pass Filter Frequency Response:
    |H(f)|
    1 |     ┌─────────────┐
      |     │             │
  0.5 |     │             │
      |     │             │
    0 |_____┘             └___
      0     fc     f
      
   Perfect brick wall
   (reality: impossible!)
```

#### **2.2 — The Problem: Infinite Duration**

When you take the **inverse Fourier transform** of ideal filter, you get:

$$H_d(e^{j\omega}) = \begin{cases} e^{-j\omega\alpha} & |\omega| \leq \omega_c \\ 0 & \omega_c < |\omega| \leq \pi \end{cases}$$

The impulse response is:

$$h_d[n] = \frac{\sin[\omega_c(n-\alpha)]}{\pi(n-\alpha)}$$

**Key observations:**
- **Extends to infinity** — Non-zero for all $n$ (past and future)
- **Not causal** — Depends on future values
- **Symmetric** — Good news for linear phase!
- **Sinc function** — Classic infinite sinc pulse

```
Ideal Low-Pass Impulse Response:
     |
   h[n]
     |      ╱╲
     |     ╱  ╲     ╱╲
   α |────●────╲   ╱  ╲─
     |         ╲ ╱      ╲
     |_________╱ ╲_______╲_
    -4 -3 -2 -1 0 1 2 3 4  n
    
    (Never actually stops!)
```

---

### **TOPIC 3: The Solution: Windowing**

#### **3.1 — What is Windowing?**

**Windowing** = Multiplying the infinite impulse response by a finite window function:

$$h[n] = h_d[n] \cdot w[n]$$

Where:
- $h_d[n]$ = Ideal (infinite) impulse response
- $w[n]$ = Window function (zero outside $[0, M-1]$)
- $h[n]$ = Practical (finite) impulse response

**Why it works:**
- ✅ Makes response causal and finite
- ✅ Preserves overall shape
- ✅ Simple to implement
- ❌ Creates ripple (Gibbs phenomenon)
- ❌ Trade-off: Wider transition or more stopband ripple

```
Windowing Process:
    
    h_d[n] (infinite)
    |      ╱╲
    |     ╱  ╲
    |    ╱    ╲
    |___╱______╲___
    
    × 
    
    w[n] (window)
    |  ╱‾‾‾‾‾‾╲
    | ╱        ╲
    |╱          ╲
    |_____________
    
    =
    
    h[n] (practical)
    |    ╱╲
    |   ╱  ╲
    |  ╱    ╲
    |_╱______╲__
```

---

#### **3.2 — Types of Windows**

Each window makes a **trade-off** between:
- **Transition width** ($\Delta\omega$) — How fast frequency response falls
- **Stopband attenuation** ($A_s$) — How much higher frequencies are blocked
- **Passband ripple** — Oscillations near cutoff

| **Window** | **Transition Width** | **Stopband Atten.** | **Ripple** | **Use Case** |
|---|---|---|---|---|
| **Rectangular** | Narrowest (4π/M) | Lowest (21 dB) | None | Simple, narrow transition needed |
| **Bartlett** | 8π/M | 25 dB | Low | Basic smoothing |
| **Hanning** | 8π/M | 44 dB | Medium | Good general purpose |
| **Hamming** | 8π/M | 53 dB | Medium | Better ripple than Hanning |
| **Blackman** | Widest (12π/M) | Highest (74 dB) | Low | Excellent stopband, wide transition |

**Visual comparison:**
```
Frequency Response Magnitude (dB):
    0 |         ┌────┐    Passband (should be flat)
   -20|         │    └─┐
   -40|  Rect   │      └─┐  ← Transition (steep = good)
   -60|         │        ├─┐
   -80|_________|________|_├──
     0    fc           fs
     
    0 |      ┌─────────┐
   -20|      │         └─┐
   -40|Hamm  │          ├─┐  ← Stopband ripple (should be flat/low)
   -60|      │          │ ├─┐
   -80|______|__________|_|_├──
```

---

#### **3.3 — Key Window Properties**

**Rectangular:**
- **Formula:** $w[n] = 1$ for all $n$ in $[0, M-1]$
- **Best at:** Narrow transition width
- **Worst at:** High stopband ripple (Gibbs phenomenon)

**Hamming:**
- **Formula:** $w[n] = 0.54 - 0.46\cos(2\pi n/(M-1))$
- **Best at:** Balancing transition and ripple
- **Most popular** for general use

**Blackman:**
- **Formula:** $w[n] = 0.42 - 0.5\cos(2\pi n/(M-1)) + 0.08\cos(4\pi n/(M-1))$
- **Best at:** Excellent stopband attenuation
- **Cost:** Wider transition width

---

### **TOPIC 4: Design Specifications & Equations**

#### **4.1 — Given Specifications**

You'll be given:
- $\omega_p$ — Passband edge frequency (rad/sample)
- $\omega_s$ — Stopband edge frequency (rad/sample)
- $A_s$ — Required stopband attenuation (dB)
- Sometimes $R_p$ — Passband ripple (dB)

#### **4.2 — Calculate Filter Order**

From your lab manual, the **normalized transition width** is:

$$\Delta f = \frac{\omega_s - \omega_p}{2\pi}$$

The **filter order** (length M) is:

$$M \approx \frac{A_s - 7.95}{14.36 \Delta f} + 1$$

**Example:**
- $\omega_p = 0.2\pi$, $\omega_s = 0.3\pi$, $A_s = 50$ dB
- $\Delta f = (0.3\pi - 0.2\pi)/(2\pi) = 0.1\pi/(2\pi) = 0.05$
- $M = (50 - 7.95)/(14.36 \times 0.05) + 1 = 42.05/0.718 + 1 \approx 59$

#### **4.3 — Choosing the Right Window**

1. **Calculate required $M$ from $A_s$**
2. **Check table 11.1 for window with enough attenuation**
3. **If $M$ is too large**, use better window (Hamming → Blackman)
4. **If narrow transition required**, use Rectangular (accept low $A_s$)

---

### **TOPIC 5: Linear Phase Property**

#### **5.1 — What is Linear Phase?**

A filter has **linear phase** if:

$$\angle H(e^{j\omega}) = -\alpha \omega$$

Where $\alpha$ is a **constant delay**.

**Why important?**
- ✅ No signal distortion — Just delayed, not distorted
- ✅ Preserves wave shape
- ❌ Impossible for IIR (in general)
- ✅ Easy for FIR — Use symmetric coefficients!

**Linear phase visualization:**
```
Input signal:
    |╱╲╱╲
    |  ╲╱╲    (undistorted)
    |
    
Output of linear phase filter:
    |  ╱╲╱╲   (same shape,
    |╱╲╱  ╲   just delayed)
    |
    
vs.
    
Output of non-linear phase:
    |╱╱╱
    |╲╲╲╲╲   (DISTORTED!)
    |
```

#### **5.2 — Symmetric h[n] → Linear Phase**

For FIR, if $h[n]$ is **symmetric**, then phase is **linear**:

$$h[n] = h[M-1-n]$$

This is why windowing designs automatically give linear phase (when window is symmetric).

---

### **TOPIC 6: MATLAB Implementation**

#### **6.1 — Key MATLAB Commands**

**Design FIR with window:**
```matlab
% Method 1: Using fir1
b = fir1(M-1, wn, 'low', window(hamming(M)));
% M-1 is the order, wn is normalized frequency

% Method 2: Direct windowing
h_ideal = ... % computed from sinc
w = hamming(M);
h = h_ideal .* w;

% Method 3: Using fircls (frequency response masking)
b = fircls(M, wn, [As, Rp]);
```

**Frequency response:**
```matlab
freqz(b, 1, 1024, fs);  % Plot frequency response
fvtool(b);              % Interactive visualization
```

**Window functions:**
```matlab
w = rectwin(M);     % Rectangular
w = hamming(M);     % Hamming
w = hanning(M);     % Hanning
w = blackman(M);    % Blackman
```

---

## QUICK DESIGN RECIPE

### **5-Step Window Design Process**

**Step 1: Identify specs**
- Passband edge: $\omega_p$
- Stopband edge: $\omega_s$
- Stopband attenuation: $A_s$ dB

**Step 2: Calculate transition width**
$$\Delta f = \frac{\omega_s - \omega_p}{2\pi}$$

**Step 3: Calculate minimum order**
$$M = \left\lceil\frac{A_s - 7.95}{14.36 \Delta f}\right\rceil + 1$$

**Step 4: Choose window from Table 11.1**
- Need 50 dB? → Use Hamming
- Need 74 dB? → Use Blackman

**Step 5: Design in MATLAB**
```matlab
M = 59;  % calculated order
wn = (wp + ws) / (2*pi);  % normalized cutoff
w = hamming(M);
b = fir1(M-1, wn, 'low', w);
freqz(b, 1, 1024);
```

---

## COMMON MISTAKES TO AVOID

❌ **Forgetting to normalize frequencies** — $\omega$ is in rad/sample, but MATLAB expects $[0, 1]$  
✅ **Convert:** $\omega_{norm} = \omega/(2\pi \cdot f_s)$

❌ **Confusing filter order N with length M** — They differ by 1!  
✅ **Remember:** $M = N + 1$

❌ **Choosing window without checking stopband attenuation**  
✅ **Always check:** Does window provide enough $A_s$?

❌ **Not verifying linear phase**  
✅ **Check:** Is $h[n]$ symmetric? Plot phase response.

❌ **Ignoring passband ripple from windowing**  
✅ **Remember:** Windows cause Gibbs ripple near edges

---

## PRACTICE PROBLEM ROADMAP

1. **Simple:** Design rectangular window LP filter (understand math first)
2. **Medium:** Design Hamming LP filter (apply selection rules)
3. **Challenge:** Design BP filter with specific specs (use lp2bp transformation in design)
4. **Viva:** Explain trade-offs between different windows

---

## KEY FORMULAS TO MEMORIZE

$$h_d[n] = \frac{\sin[\omega_c(n-\alpha)]}{\pi(n-\alpha)}$$

$$h[n] = h_d[n] \cdot w[n]$$

$$M \approx \frac{A_s - 7.95}{14.36 \Delta f} + 1$$

$$\Delta f = \frac{\omega_s - \omega_p}{2\pi}$$

---

**Ready? Let's design some filters!**

Next → See `lab11_complete_solution_summary.md` for task-by-task walkthrough.

---

*Abu bakar Talha* | IEEE 324 | Digital Signal Processing Lab Manual | Part 1: Theory & Background
