# DSP Chapter 6: Sampling and Reconstruction — Complete Study Guide

## Table of Contents
1. [Core Concepts](#core-concepts)
2. [Problem 1: AA Filter Design](#problem-1-aa-filter-design)
3. [Problem 2: Aliasing Check & Replica Centers](#problem-2-aliasing-check--replica-centers)
4. [Problem 3: Tone Aliasing & Discrete Frequency](#problem-3-tone-aliasing--discrete-time-frequency)
5. [Problem 4: Ideal Impulse Sampling Spectrum](#problem-4-spectrum-of-ideal-impulse-sampling)
6. [Problem 5: ZOH Amplitude Droop](#problem-5-zoh-hold-distortion-amplitude-droop)
7. [Problem 6: Natural Sampling Weighting](#problem-6-natural-sampling-pam-replica-weighting)
8. [Problem 7: Ideal Reconstruction (Time-Domain)](#problem-7-ideal-reconstruction-from-samples-time-domain)
9. [Problem 8: Reconstruction Filter Design](#problem-8-reconstruction-filter-design-frequency-domain)
10. [Self-Check Drill Explained](#self-check-drill-explained)

---

## Core Concepts

### Nyquist–Shannon Sampling Theorem
**Statement:** A bandlimited signal with bandwidth $B$ Hz can be perfectly reconstructed from its samples if sampled at rate $F_s \geq 2B$ Hz.

- **Nyquist rate:** $F_N = 2B$ (minimum sampling rate for alias-free sampling)
- **Nyquist frequency:** $F_N/2 = B$ (highest frequency in baseband)
- **If $F_s < 2B$:** spectral copies overlap → **aliasing** → irreversible information loss

### Key Definitions

| Term | Symbol | Definition |
|------|--------|-----------|
| Sampling period | $T_s$ | $T_s = 1/F_s$ (seconds between samples) |
| Sampling rate | $F_s$ | Samples per second (Hz) |
| Bandwidth | $B$ | Highest frequency component of signal |
| Sinc function | $\text{sinc}(x)$ | $\text{sinc}(x) = \frac{\sin(\pi x)}{\pi x}$ |
| Normalized freq | $u = F/F_s$ | Frequency relative to sampling rate |

### Three Sampling Methods (Frequency Domain)

#### 1. **Ideal Sampling (Impulse Train)**
$$p(t) = \sum_{n=-\infty}^{\infty} \delta(t - nT_s)$$
$$P(F) = \frac{1}{T_s}\sum_{k=-\infty}^{\infty} \delta(F - kF_s)$$
$$X_p(F) = X(F) * P(F) = \frac{1}{T_s}\sum_{k=-\infty}^{\infty} X(F - kF_s)$$

**Key:** All replicas scaled equally by $1/T_s$.

#### 2. **Natural Sampling (PAM)**
Uses rectangular pulse train: $s(t) = $ rectangular pulse of width $T_p$, amplitude $A$
$$S(F) = AT_p \cdot \text{sinc}(F T_p)$$
$$X_p(F) = \frac{1}{T_s} \sum_{k=-\infty}^{\infty} S(kF_s) \cdot X(F - kF_s)$$

**Key:** Replica $k$ is weighted by $S(kF_s) = AT_p \cdot \text{sinc}(kF_s T_p)$ (frequency-dependent weighting).

#### 3. **Sample-and-Hold (ZOH)**
Holds last sample value until next sample arrives.
$$h_0(t) = u(t) - u(t - T_s) \quad \text{(rectangular pulse of width } T_s\text{)}$$
$$H_0(F) = T_s \cdot \text{sinc}(F T_s) \cdot e^{-j\pi F T_s}$$

**Key:** Baseband distorted by sinc envelope; higher replicas more heavily attenuated.

---

## Problem 1: AA Filter Design

### Problem Statement
A real analog signal is bandlimited to $B = 3.5$ kHz. You want alias-free sampling with a practical anti-aliasing (AA) filter that has a $1$ kHz transition band. Pick $F_s$ and specify AA filter passband/stopband edges.

### Step-by-Step Solution

**Step 1: Apply Nyquist criterion**
$$F_s \geq 2B = 2 \times 3.5 = 7 \text{ kHz}$$

Choose a practical value: $F_s = 8$ kHz or $F_s = 10$ kHz.

**Step 2: Design AA filter for $F_s = 8$ kHz**

The AA filter must limit the signal bandwidth to at most $F_s/2 = 4$ kHz to prevent aliasing.

- **Passband edge:** $F_p \leq 3.0$ kHz (leave 1 kHz margin below Nyquist)
- **Stopband edge:** $F_s/2 + 1 = 5$ kHz (or keep replicas from folding in)
- **Transition band:** 3.0–5.0 kHz (exactly 2 kHz, practical for analog implementation)

| Specification | Value |
|---|---|
| Passband edge $F_p$ | 3.0 kHz |
| Stopband edge $F_s$ | 5.0 kHz |
| Transition width | 2.0 kHz |
| Stopband attenuation | ≥ 60 dB (typical requirement) |

**Step 3: Why this works**

```
Original signal:     |X(F)|
                     ___
                    /   \
    _______________/     \_______________
                -3.5          0          3.5 kHz

After AA filter:     |X_AA(F)|
                      ___
                     /   \
    __________________/     \__________________
                -3.0          0          3.0 kHz

After sampling:      |X_p(F)|
    Replica at -8 kHz    Baseband        Replica at +8 kHz
          |               |___|               |
    ______|_______________|   |_______________|_____
    -8  -5 -3  0  3    5   8           kHz
```

Since all signal energy is now in $[-3, +3]$ kHz and first replica starts at $F_s = 8$ kHz, there's a gap $[3, 5]$ kHz with no signal. Perfect reconstruction is possible.

---

## Problem 2: Aliasing Check & Replica Centers

### Problem Statement
Let $X(F)$ be zero for $|F| > B$, with $B = 4$ kHz. If $F_s = 10$ kHz, will the sampled spectrum replicas overlap? Where are their centers?

### Step-by-Step Solution

**Step 1: Check Nyquist condition**
$$2B = 2 \times 4 = 8 \text{ kHz} \quad \text{vs} \quad F_s = 10 \text{ kHz}$$

Since $2B = 8 < F_s = 10$: **NO ALIASING** ✓

**Step 2: Identify replica centers**

For ideal sampling, replicas appear at multiples of $F_s$:
$$\text{Replica centers: } F = k \cdot F_s \quad \text{for } k = 0, \pm1, \pm2, \ldots$$

$$F = \ldots, -20, -10, 0, +10, +20, \ldots \text{ kHz}$$

**Step 3: Sketch the spectrum**

```
|X_p(F)| — Sampled spectrum (ideal sampling)

k=-1             k=0 (baseband)      k=+1
  |                  |                 |
  |___              |___|             |___
 -14  -6            -4  0  +4        +6  +14 kHz
 <-------- 8 kHz gap -------> <------- 8 kHz gap -------->

Each replica has width 2B = 8 kHz
All centered at multiples of F_s = 10 kHz
No overlap since 8 < 10 ✓
```

**Step 4: Why no aliasing?**

Replica spacing = $F_s = 10$ kHz
Replica width = $2B = 8$ kHz
Gap between replicas = $F_s - 2B = 10 - 8 = 2$ kHz

The replicas fit without overlapping because there's a 2 kHz guard band between each pair.

---

## Problem 3: Tone Aliasing & Discrete-Time Frequency

### Problem Statement
$x(t) = \cos(2\pi \cdot 12 \text{ kHz} \cdot t)$. Sample at $F_s = 18$ kHz. Find:
- (a) Does it alias?
- (b) The aliased baseband frequency (Hz)
- (c) The discrete-time radian frequency $\omega$ (rad/sample)

### Step-by-Step Solution

**Step 1: Check Nyquist condition**

Original tone frequency: $f_0 = 12$ kHz
Nyquist frequency: $F_N = F_s/2 = 18/2 = 9$ kHz

Since $f_0 = 12$ kHz $> F_N = 9$ kHz: **YES, IT ALIASES** ⚠️

**Step 2: Find aliased frequency (fold into baseband)**

When a tone is above Nyquist, it "folds back" into the baseband. Use:
$$f_{\text{alias}} = |f_0 - \text{round}(f_0/F_s) \cdot F_s|$$

Calculate:
- $f_0/F_s = 12/18 = 0.667$
- $\text{round}(0.667) = 1$
- $f_{\text{alias}} = |12 - 1 \times 18| = |12 - 18| = 6$ kHz

**Interpretation:**
```
Analog tone at 12 kHz
        │
        │ Sample at 18 kHz
        ▼
        Sampled signal appears as
        6 kHz tone in baseband!
```

Think of it as: the 12 kHz tone "looks like" a 6 kHz tone when sampled at 18 kHz.

**Step 3: Find discrete-time frequency**

Once we know the aliased baseband frequency is 6 kHz, the discrete-time (angular) frequency is:
$$\omega = \frac{2\pi f_{\text{alias}}}{F_s} = \frac{2\pi \times 6}{18} = \frac{12\pi}{18} = \frac{2\pi}{3} \text{ rad/sample}$$

Or in normalized form:
$$\hat{f} = \frac{f_{\text{alias}}}{F_s} = \frac{6}{18} = \frac{1}{3}$$

**Summary Table:**

| Parameter | Value |
|-----------|-------|
| Original analog frequency | 12 kHz |
| Sampling rate | 18 kHz |
| Nyquist frequency | 9 kHz |
| Aliased baseband frequency | 6 kHz |
| Discrete-time frequency $\omega$ | $\frac{2\pi}{3}$ rad/sample |
| Normalized frequency $\hat{f}$ | 1/3 |

---

## Problem 4: Spectrum of Ideal Impulse Sampling

### Problem Statement
Given ideal sampling with $p(t) = \sum_n \delta(t - nT_s)$, show that $X_p(F) = \frac{1}{T_s}\sum_k X(F - kF_s)$. Explain when reconstruction can be perfect with an ideal LPF.

### Step-by-Step Solution

**Step 1: Find Fourier transform of impulse train**

The impulse train $p(t)$ is periodic with period $T_s$. Using the Fourier series:
$$p(t) = \sum_{n=-\infty}^{\infty} \delta(t - nT_s)$$

The Fourier transform (using the impulse train property):
$$P(F) = \frac{1}{T_s} \sum_{k=-\infty}^{\infty} \delta(F - kF_s)$$

where $F_s = 1/T_s$.

**Interpretation:** The impulse train in time produces evenly spaced impulses in frequency, spaced $F_s$ apart, each with amplitude $1/T_s$.

**Step 2: Find spectrum of sampled signal**

The sampled signal is:
$$x_p(t) = x(t) \cdot p(t)$$

Multiplication in time = convolution in frequency:
$$X_p(F) = X(F) * P(F)$$

Substituting $P(F)$:
$$X_p(F) = X(F) * \left[\frac{1}{T_s}\sum_{k=-\infty}^{\infty} \delta(F - kF_s)\right]$$

Using the sifting property of the delta function ($f(t)*\delta(t-a) = f(t-a)$):
$$X_p(F) = \frac{1}{T_s} \sum_{k=-\infty}^{\infty} X(F - kF_s)$$

**Step 3: Visualize the spectrum**

```
Original spectrum |X(F)|:
                   _____
                  /     \
    _____________/       \____________
                -B   0    B     F

After ideal sampling |X_p(F)|:
                           1/Ts scaling
                            ↓
    .... ___ ... ___ ... ___ ... ___ ... ___
   k=-1 / \  k=0/ \  k=+1/ \  k=+2/ \
       /   \_    /   \_    /   \_    /   \___
    -Fs  0   Fs 2Fs 3Fs  ...

Each replica is a scaled copy of X(F), centered at kF_s
```

**Step 4: Conditions for perfect reconstruction**

For perfect reconstruction, we need:
1. **No aliasing:** The baseband replica (k=0) must not overlap with k=±1 replicas
   - This requires: $B \leq F_s/2$ or equivalently $F_s \geq 2B$ (Nyquist!)

2. **Ideal LPF design:** Use a low-pass filter with:
   - **Passband:** $|F| \leq B$ with gain $H_r(F) = T_s$
   - **Stopband:** $|F| > F_s/2$ with gain $H_r(F) = 0$

3. **Why gain $T_s$?** The sampled spectrum is scaled by $1/T_s$, so we multiply back by $T_s$ to restore unity gain:
$$X_r(F) = X_p(F) \cdot H_r(F) = \frac{1}{T_s}\sum_k X(F-kF_s) \cdot T_s = X(F) \quad \text{(for baseband k=0)}$$

Result: $x_r(t) = x(t)$ (perfect reconstruction!)

---

## Problem 5: ZOH Amplitude Droop

### Problem Statement
A sample-and-hold (ZOH) has $h_0(t) = u(t) - u(t - T_s)$. Its frequency response magnitude is $|H_0(F)| = T_s \cdot |\text{sinc}(F T_s)|$. For $F_s = 10$ kHz, what is the relative amplitude at 3 kHz compared to DC?

### Step-by-Step Solution

**Step 1: Understand ZOH (staircase hold)**

A ZOH system receives impulse samples and outputs a rectangular pulse (holds value constant):
```
Samples:     │     │     │           Continuous ZOH output:
             │     │     │              ___     ___     ___
    ──────────────────────  ────►    ___|   |___|   |___|   |___
             T_s             (time)
```

Impulse response: $h_0(t) = u(t) - u(t - T_s)$ (rectangular pulse of duration $T_s$)

**Step 2: Frequency response of ZOH**

Taking the Fourier transform of $h_0(t)$:
$$H_0(F) = \int_0^{T_s} e^{-j2\pi F t} \, dt = \frac{e^{-j\pi F T_s} \sin(\pi F T_s)}{\pi F T_s}$$

Magnitude (dropping phase):
$$|H_0(F)| = T_s \left| \frac{\sin(\pi F T_s)}{\pi F T_s} \right| = T_s \cdot |\text{sinc}(F T_s)|$$

**Step 3: Normalized frequency response (droop factor)**

To see the distortion in the baseband, normalize by the DC response:
$$\frac{|H_0(F)|}{|H_0(0)|} = |\text{sinc}(F T_s)| = \left|\frac{\sin(\pi F T_s)}{\pi F T_s}\right|$$

Or using normalized frequency $u = F/F_s$:
$$\frac{|H_0(F)|}{|H_0(0)|} = |\text{sinc}(\pi u)|$$

**Step 4: Calculate droop at 3 kHz**

Given: $F_s = 10$ kHz, $F = 3$ kHz
$$u = \frac{F}{F_s} = \frac{3}{10} = 0.3$$

$$\text{Droop factor} = |\text{sinc}(0.3)| = \left| \frac{\sin(0.3\pi)}{0.3\pi} \right|$$

Calculate:
- $0.3\pi \approx 0.9425$ radians
- $\sin(0.9425) \approx 0.8090$
- Droop = $0.8090 / 0.9425 \approx 0.8586$

**Step 5: Interpret the result**

**Droop factor = 0.8586 = 85.86%**

This means:
- A 3 kHz component is attenuated to **85.86%** of its original amplitude by ZOH
- Attenuation = $1 - 0.8586 = 0.1414 = 14.14\%$
- In dB: $20\log_{10}(0.8586) \approx -1.33$ dB

**Why this happens:**
```
Frequency response of ZOH:
|H_0(F)|
   ▲
   │     (DC=1.0)
 1 │    ╱│╲
   │   ╱ │ ╲
0.9│  ╱  │  ╲  ←── At F/Fs = 0.3, we're at ~0.86 of DC
   │ ╱   │   ╲
   │╱____│____╲___
   └─────────────────► F/Fs
   0    0.3   0.5
        3kHz (at 0.3)
```

**Practical implication:**
- High frequencies are more attenuated than low frequencies
- This is why ZOH introduces **high-frequency roll-off** (undesired!)
- Real systems often compensate this with an equalizer

---

## Problem 6: Natural Sampling (PAM) Replica Weighting

### Problem Statement
A PAM sampler uses rectangular pulses of width $T_p$ with period $T_s$ (duty cycle $D = T_p/T_s$) and amplitude $A$. Show how replica $k$ is weighted and evaluate the ratio for $D = 0.1$ at $k = 1$.

### Step-by-Step Solution

**Step 1: Single pulse spectrum**

A single rectangular pulse $s(t)$ of width $T_p$ and amplitude $A$:
$$s(t) = \begin{cases} A & 0 \leq t \leq T_p \\ 0 & \text{otherwise} \end{cases}$$

Fourier transform:
$$S(F) = A T_p \cdot \text{sinc}(F T_p) = A T_p \frac{\sin(\pi F T_p)}{\pi F T_p}$$

Or using duty cycle $D = T_p / T_s$:
$$S(F) = A T_s D \cdot \text{sinc}(F T_s D) = A T_s D \cdot \text{sinc}(\pi F T_s D)$$

**Step 2: Pulse train (natural sampling function)**

Periodic train of pulses, repetition $T_s$:
$$p(t) = s(t) * \sum_{n=-\infty}^{\infty} \delta(t - nT_s)$$

Frequency domain:
$$P(F) = S(F) \cdot \frac{1}{T_s} \sum_{k=-\infty}^{\infty} \delta(F - kF_s)$$

The convolution with the delta train produces:
$$P(F) = \frac{1}{T_s} \sum_{k=-\infty}^{\infty} S(kF_s) \delta(F - kF_s)$$

**Step 3: Sampled signal spectrum**

$$x_p(t) = x(t) \cdot p(t)$$
$$X_p(F) = X(F) * P(F) = \frac{1}{T_s} \sum_{k=-\infty}^{\infty} S(kF_s) \cdot X(F - kF_s)$$

**Key insight:** The $k$-th replica is weighted by $S(kF_s)$ (not constant as in ideal sampling!)

**Step 4: Weight of replica k**

$$\text{Weight of replica } k = S(kF_s) = A T_p \cdot \text{sinc}(k F_s T_p) = A T_s D \cdot \text{sinc}(k D)$$

Compare replicas:
- Replica 0: $S(0) = A T_p$ (maximum)
- Replica ±1: $S(\pm F_s) = A T_p \cdot \text{sinc}(D)$
- Replica ±2: $S(\pm 2F_s) = A T_p \cdot \text{sinc}(2D)$
- etc.

**Step 5: Evaluate for $D = 0.1$, $k = 1$**

$$\text{Ratio} = \frac{|S(F_s)|}{|S(0)|} = |\text{sinc}(D)| = |\text{sinc}(0.1)|$$

Calculate:
$$\text{sinc}(0.1) = \frac{\sin(0.1\pi)}{0.1\pi} = \frac{\sin(0.31416)}{0.31416} \approx \frac{0.30902}{0.31416} \approx 0.9836$$

**Result:** First replicas are **98.36%** of the baseband power! Nearly unchanged.

**Step 6: Visualize the effect**

```
Duty cycle D = 0.1 (10% pulse, 90% gap)

Pulse spectrum S(F):
     ▲
   A │      sinc envelope (falls slowly)
     │    ╱╲      ╱╲      ╱╲
     │   ╱  ╲    ╱  ╲    ╱  ╲
     │  ╱    ╲  ╱    ╱  ╱    ╲
     └─────────────────────────► F
     0  F_s  2F_s  3F_s

     Weights:  S(0) ≈ 1.0
               S(F_s) ≈ 0.9836  ← Only 1.6% reduction!
               S(2F_s) ≈ 0.942
               S(3F_s) ≈ 0.866


Comparison: IDEAL vs NATURAL sampling
             |X_p(F)| ideal          |X_p(F)| natural (D=0.1)
                    ___                     ___
                   /   \                   /   \
     ______      /       \     ______    /       \    ______
           ╲    /         ╲  /      ╲  /         ╲  /
    Fs     ╲___/           \/        ╲_/           ╲_   Fs
           ↑                          ↑ Nearly same!
           Equal scaling             Weighted by sinc
```

**Practical implication:**
- Natural sampling ≈ ideal sampling when duty cycle is small (D ≪ 1)
- With D = 0.1, first replica barely attenuated, so reconstruction must have sharp cutoff
- As D increases (wider pulses), higher replicas are progressively attenuated, making reconstruction easier

---

## Problem 7: Ideal Reconstruction from Samples (Time-Domain)

### Problem Statement
Let $F_s = 1$ kHz ($T_s = 1$ ms). Suppose samples $x[-1] = 0.5$, $x[0] = 1.0$, $x[1] = 0.5$ and others negligible. Compute $x_r(t)$ at $t = 0.5 T_s$ using ideal sinc interpolation:
$$x_r(t) = \sum_{n=-\infty}^{\infty} x[n] \cdot \text{sinc}\left(\frac{t - nT_s}{T_s}\right)$$

### Step-by-Step Solution

**Step 1: Set up the problem**

- Sampling period: $T_s = 1$ ms = 0.001 s
- Target time: $t = 0.5 T_s = 0.5$ ms
- Normalized time: $\tau = t / T_s = 0.5$

**Step 2: Write the interpolation formula**

$$x_r(t) = \sum_{n=-\infty}^{\infty} x[n] \cdot \text{sinc}(\tau - n)$$

where $\tau = t/T_s$.

**Step 3: Calculate each term**

The formula involves a sum over all $n$, but we're told other samples are negligible, so include only $n = -1, 0, 1$:

**Term for $n = -1$:**
$$x[-1] \cdot \text{sinc}(0.5 - (-1)) = 0.5 \cdot \text{sinc}(1.5)$$

Calculate sinc:
$$\text{sinc}(1.5) = \frac{\sin(1.5\pi)}{1.5\pi} = \frac{-1}{1.5\pi} \approx \frac{-1}{4.712} \approx -0.2122$$

Term value: $0.5 \times (-0.2122) \approx -0.1061$

**Term for $n = 0$:**
$$x[0] \cdot \text{sinc}(0.5 - 0) = 1.0 \cdot \text{sinc}(0.5)$$

Calculate sinc:
$$\text{sinc}(0.5) = \frac{\sin(0.5\pi)}{0.5\pi} = \frac{1}{0.5\pi} = \frac{2}{\pi} \approx 0.6366$$

Term value: $1.0 \times 0.6366 \approx 0.6366$

**Term for $n = 1$:**
$$x[1] \cdot \text{sinc}(0.5 - 1) = 0.5 \cdot \text{sinc}(-0.5)$$

Calculate sinc (note: sinc is even, so $\text{sinc}(-x) = \text{sinc}(x)$):
$$\text{sinc}(-0.5) = \text{sinc}(0.5) \approx 0.6366$$

Term value: $0.5 \times 0.6366 \approx 0.3183$

**Step 4: Sum all terms**

$$x_r(0.5 T_s) = -0.1061 + 0.6366 + 0.3183 = 0.8488$$

### Verification and Interpretation

**Why these values?**

The sample at $n=0$ contributes most (central sample, largest sinc weight at its own time).
The sample at $n=1$ also contributes significantly (nearby sample).
The sample at $n=-1$ contributes negatively (negative sinc tail).

**Graphical view:**
```
x_r(t) is sum of scaled sinc functions:

       sinc(-0.5) = 0.637     sinc(0.5) = 0.637      sinc(1.5) = -0.212
              ▼                    ▼                        ▼
              │        
            0.5├─ ╱╲         
        x[-1]=│ ╱  ╲     
             ├╱      ╲
    ─────────┼───────── ═══════════ ─────────
             │     x[0]=1.0
             │
          1.0├────╱╲
             │   ╱  ╲
           0.5├ ╱    ╲  x[1]=0.5
             │╱       ╲
    ─────────┼─────────╲═════════─────────
             │         ╲    ╱╲
             ├──────────╲──╱  ╲
             │           ╲     ╲
    -1      0       0.5    1       2      n
            (t/Ts = 0.5)
```

At $t = 0.5 T_s$ (midpoint between samples 0 and 1), the interpolated value is 0.8488.

**Key property of sinc interpolation:**
$$x_r(nT_s) = \sum_{m=-\infty}^{\infty} x[m] \cdot \text{sinc}(n - m) = x[n]$$

because $\text{sinc}(0) = 1$ and $\text{sinc}(k) = 0$ for integer $k \neq 0$. This ensures the reconstruction **passes exactly through all sample points**.

---

## Problem 8: Reconstruction Filter Design (Frequency-Domain)

### Problem Statement
You have ideal sampling with $F_s = 12$ kHz and a signal bandlimit $B = 4.5$ kHz. Design a reconstruction filter $H_r(F)$ for perfect recovery. What gain ensures unity overall?

### Step-by-Step Solution

**Step 1: Check for aliasing**

$$2B = 2 \times 4.5 = 9 \text{ kHz} \quad \text{vs} \quad F_s = 12 \text{ kHz}$$

Since $2B = 9 < F_s = 12$: **No aliasing** ✓

Nyquist frequency (Fs/2) = 6 kHz

**Step 2: Identify replica positions in $X_p(F)$**

After ideal sampling, replicas appear at:
- $k=0$ (baseband): centered at 0 Hz, width ±4.5 kHz, so spans [−4.5, +4.5] kHz
- $k=±1$: centered at ±12 kHz, spans [7.5, 16.5] kHz and [−16.5, −7.5] kHz
- $k=±2$: centered at ±24 kHz, etc.

**Step 3: Design ideal reconstruction filter**

The goal: pass the baseband replica, reject all others.

**Ideal filter spec:**
- **Passband:** $|F| \leq 4.5$ kHz (to pass entire signal, no distortion)
- **Guard band:** (4.5, 6.0] kHz (optional, for margin)
- **Transition band:** (6.0, 6.0) kHz (ideally zero width; practically 1-2 kHz)
- **Stopband:** $|F| \geq 6.0$ kHz (reject first replica at 12 kHz)

**Magnitude response:**
$$H_r(F) = \begin{cases}
T_s & |F| \leq 4.5 \text{ kHz} \\
0 & |F| \geq 6 \text{ kHz} \\
\text{(smooth transition)} & 4.5 < |F| < 6 \text{ kHz}
\end{cases}$$

**Step 4: Why gain $H_r = T_s$?**

Recall the sampled spectrum has a scaling factor of $1/T_s$:
$$X_p(F) = \frac{1}{T_s} \sum_k X(F - kF_s)$$

For the baseband replica:
$$X_p(F)|_{\text{baseband}} = \frac{1}{T_s} X(F) \quad \text{(for } |F| \leq B\text{)}$$

After passing through the reconstruction filter:
$$X_r(F) = X_p(F) \cdot H_r(F) = \frac{1}{T_s} X(F) \cdot T_s = X(F)$$

This restores **unity gain** so $x_r(t) = x(t)$.

**Step 5: Determine $T_s$ numerically**

$$T_s = \frac{1}{F_s} = \frac{1}{12 \text{ kHz}} \approx 83.33 \text{ μs}$$

Filter gain: $H_r = 83.33$ μs (in time-domain terms, this is unity gain after normalization)

**Step 6: Frequency-domain illustration**

```
Original signal X(F):
        ▲
        │      ___
        │     /   \
    ────┼────/     \────
        │          
        │   -4.5   0   +4.5 kHz

After sampling X_p(F):
        ▲
        │  ×1/Ts    (scaling)
        │   ___      ___      ___
        │  /   \    /   \    /   \
    ────┼─/─────\──/─────\──/─────\──
        │       
       -12  -4.5 0  4.5  12  20 kHz
         k=-1    k=0    k=+1

After reconstruction filter (ideal):
        ▲
        │      ___
        │     /   \
    ────┼────/     \────  × H_r(F) = T_s
        │          
        -4.5   0   +4.5 kHz
        
Final X_r(F) = X(F) ✓
```

**Step 7: Practical filter design**

Since ideal filters are non-causal and impossible to implement, use a realizable low-pass filter:

Options:
- **Butterworth LPF:** smooth, monotonic rolloff, maximally flat passband
- **Chebyshev LPF:** sharper transition, passband ripple
- **Elliptic LPF:** sharpest transition, but both ripples

Example (Butterworth, 4th order):
- Passband: 0–4.5 kHz, ripple < 0.5 dB
- Transition: 4.5–6 kHz
- Stopband: > 6 kHz, attenuation > 50 dB

---

## Self-Check Drill Explained

### Drill Problem 1: Alias Frequency

**Problem:** Compute alias frequency for $f_0 = 7.2$ kHz at $F_s = 10$ kHz.

**Solution:**

Nyquist = $F_s/2 = 5$ kHz. Since $7.2 > 5$, aliasing occurs.

$$f_{\text{alias}} = |f_0 - \text{round}(f_0/F_s) \cdot F_s|$$

- $f_0/F_s = 7.2/10 = 0.72$
- $\text{round}(0.72) = 1$
- $f_{\text{alias}} = |7.2 - 1 \times 10| = |-2.8| = 2.8$ kHz

**Discrete-time frequency:**
$$\omega = 2\pi \frac{f_{\text{alias}}}{F_s} = 2\pi \frac{2.8}{10} = 2\pi \times 0.28 = 0.56\pi \text{ rad/sample}$$

Or: $\omega = 1.76$ rad/sample

**Answer:** Aliased to **2.8 kHz**; $\omega = 0.56\pi$ rad/sample.

---

### Drill Problem 2: ZOH Droop

**Problem:** For ZOH at $F_s = 8$ kHz, what's the droop at 2 kHz?

**Solution:**

Normalized frequency: $u = F/F_s = 2/8 = 0.25$

Droop factor:
$$|\text{sinc}(u)| = |\text{sinc}(0.25)| = \left| \frac{\sin(0.25\pi)}{0.25\pi} \right|$$

Calculate:
- $0.25\pi \approx 0.7854$ rad
- $\sin(0.7854) \approx 0.7071$
- Droop = $0.7071 / 0.7854 \approx 0.9003$

**In dB:** $20 \log_{10}(0.9003) \approx -0.91$ dB

**Answer:** Droop factor **0.9003 (−0.91 dB)**. The 2 kHz component is attenuated to 90% of its original value.

---

### Drill Problem 3: Aliasing and Replicas

**Problem:** Write $X_p(F)$ for ideal sampling and mark replica centers for $B = 2.5$ kHz, $F_s = 6$ kHz. Is there aliasing?

**Solution:**

**Check Nyquist:**
$$2B = 5 \text{ kHz} < F_s = 6 \text{ kHz} \quad \Rightarrow \text{NO ALIASING}$$ ✓

**Formula:**
$$X_p(F) = \frac{1}{T_s} \sum_{k=-\infty}^{\infty} X(F - kF_s) = \frac{1}{6 \text{ μs}} \sum_k X(F - 6k \text{ kHz})$$

**Replica centers:**
$$F = k \cdot 6 \text{ kHz} = \ldots, -12, -6, 0, +6, +12, +18, \ldots \text{ kHz}$$

Each replica spans width $2B = 5$ kHz (±2.5 kHz around each center).

**Sketch:**
```
|X_p(F)|
         1/(6 μs) scaling

k=-1          k=0          k=+1
  |            |            |
  |___        |___        |___
 -8.5 -3.5   -2.5 2.5   3.5 8.5  kHz
       gap=1 kHz           gap=1 kHz
```

**Answer:** 
- Replica centers: $\ldots, -12, -6, 0, +6, +12, \ldots$ kHz
- Each centered at $k \cdot 6$ kHz
- **No aliasing** (gap of 1 kHz between adjacent replicas)

---

### Drill Problem 4: Natural Sampling Weighting

**Problem:** Given $D = 0.2$ (natural sampling), compare $|S(0)|$ and $|S(F_s)|$.

**Solution:**

**Duty cycle:** $D = T_p/T_s = 0.2 = 20\%$ (pulse width is 20% of period)

**Weights:**
- Baseband ($k=0$): $S(0) = A T_p$ (maximum, normalized to 1.0)
- First replica ($k=1$): $S(F_s) = A T_p \cdot \text{sinc}(D) = A T_p \cdot \text{sinc}(0.2)$

**Calculate sinc(0.2):**
$$\text{sinc}(0.2) = \frac{\sin(0.2\pi)}{0.2\pi} = \frac{\sin(0.6283)}{0.6283} \approx \frac{0.5878}{0.6283} \approx 0.9355$$

**Ratio:**
$$\frac{|S(F_s)|}{|S(0)|} = |\text{sinc}(0.2)| \approx 0.9355 = 93.55\%$$

**Interpretation:** First replicas are at 93.55% of baseband power. They're significantly attenuated (6.45% reduction), so reconstruction filter can have gentler rolloff compared to ideal sampling.

**Answer:** Ratio ≈ **0.9355 or 93.55%**. First replicas are ~6.45% smaller than baseband.

---

## Summary Table: Quick Reference

| Concept | Formula | Notes |
|---------|---------|-------|
| **Nyquist Criterion** | $F_s \geq 2B$ | Necessary condition to avoid aliasing |
| **Ideal Sampling Spectrum** | $X_p(F) = \frac{1}{T_s}\sum_k X(F-kF_s)$ | Replicas equally scaled |
| **Natural Sampling Weight** | $S(kF_s) = AT_p \cdot \text{sinc}(kD)$ | Frequency-dependent scaling |
| **ZOH Frequency Response** | $H_0(F) = T_s \cdot \text{sinc}(FT_s) \cdot e^{-j\pi FT_s}$ | Causes amplitude droop at high freq |
| **ZOH Droop Factor** | $\frac{\|H_0(F)\|}{\|H_0(0)\|} = \|\text{sinc}(F/F_s)\|$ | Normalized magnitude |
| **Ideal Reconstruction (time)** | $x_r(t) = \sum_n x[n] \cdot \text{sinc}((t-nT_s)/T_s)$ | Bandlimited interpolation |
| **Reconstruction Filter Gain** | $H_r(F) = T_s$ for $\|F\| \leq B$ | Compensates $1/T_s$ sampling scale |
| **Alias Frequency** | $f_{\text{alias}} = \|f_0 - \text{round}(f_0/F_s) F_s\|$ | Folds tones above Nyquist to baseband |

---

## Study Tips for Exam

1. **Draw spectra:** Always sketch $X(F)$, $X_p(F)$, and $X_r(F)$ to visualize replicas.
2. **Remember sinc:** $\text{sinc}(0)=1$, $\text{sinc}(\pm n)=0$ for integer $n \neq 0$, $\text{sinc}(0.5) \approx 0.6366$.
3. **Nyquist is critical:** Check $F_s \geq 2B$ first on every sampling problem.
4. **Filter design:** For reconstruction, always choose a low-pass with cutoff between $B$ and $F_s/2$.
5. **Practical filters:** Butterworth, Chebyshev, Elliptic are the main types; know the tradeoffs (ripple vs. steepness).
6. **Multiple choice trap:** Aliasing happens when $F_s < 2B$, not just when it's small.

---

**Good luck with your exam!**
