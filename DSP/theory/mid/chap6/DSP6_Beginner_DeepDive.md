# DSP Chapter 6: Sampling and Reconstruction — Beginner's Deep Dive

## Table of Contents
1. [Why Sampling Matters](#why-sampling-matters)
2. [The Sampling Theorem (Intuitive)](#the-sampling-theorem-intuitive)
3. [Aliasing Explained](#aliasing-explained)
4. [Bandlimited Signals](#bandlimited-signals)
5. [Anti-Aliasing Filters](#anti-aliasing-filters)
6. [Ideal Sampling Method](#ideal-sampling-method)
7. [Natural Sampling (PAM)](#natural-sampling-pam)
8. [Sample-and-Hold (ZOH)](#sample-and-hold-zoh)
9. [Reconstruction & Interpolation](#reconstruction--interpolation)
10. [Summary & Key Takeaways](#summary--key-takeaways)

---

## Why Sampling Matters

### The Real-World Problem

In the physical world, signals are **continuous**—they have values at every instant in time.

**Example:** A microphone capturing sound produces a continuous voltage signal:
```
Microphone voltage V(t):
      ▲ V(t)
      │     ╱╲      ╱╲
      │    ╱  ╲    ╱  ╲
      │   ╱    ╲  ╱    ╲
      │  ╱      ╲╱      ╲
      │ ╱                ╲___
    0 │────────────────────────► t (time)
      │ (continuous)
```

**The Challenge:** Computers work with **digital numbers** (discrete). They can't store infinite data.

To use computers for signal processing:
- Convert continuous signal to a sequence of numbers (samples)
- Each number represents the signal value at specific time instants
- Process these numbers digitally
- Convert back to continuous signal (if needed)

**This process is called A/D conversion** (Analog-to-Digital).

### What Sampling Looks Like

Taking samples means picking specific time points and recording the signal value there:

```
Original continuous signal x(t):
      ▲ x(t)
      │     ╱╲      ╱╲
      │    ╱  ╲    ╱  ╲
      │   ╱    ╲  ╱    ╲    ← Smooth curve
      │  ╱      ╲╱      ╲
    0 │────────────────────────► t


Sample at times t = 0, T_s, 2T_s, 3T_s, ...
      ▲ x(t)
      │     •           •       ← Sample points
      │    ╱╲  │      ╱╲  │
      │   ╱  ╲ │     ╱  ╲ │
      │  ╱    ╲│    ╱    ╲│
    0 │────────────────────────► t
      0  T_s  2T_s 3T_s
          ▲
       spacing between samples

Digital sequence x[n]:
      x[0], x[1], x[2], x[3], ...
      = x(0), x(T_s), x(2T_s), x(3T_s), ...
```

**Key question:** How fast must we sample to not lose information?

Answer: **The Sampling Theorem**

---

## The Sampling Theorem (Intuitive)

### Simple Version

> **If a signal contains frequencies up to $B$ Hz, you must sample at least $2B$ times per second to capture all the information.**

**Why the factor of 2?**

Think of a sine wave:
```
One complete cycle has:
  - Peak (max value)
  - Trough (min value)
  
You need AT LEAST 2 samples per cycle to capture both the peak and trough!

Sampling at exactly 2 points per cycle (at peak and trough):
      ▲ x(t)
      │     •                    ← Peak
      │    ╱ ╲                   You see:
      │   ╱   ╲                  "goes up, comes down"
      │  ╱     ╲                 ✓ Captured the oscillation
      │ ╱       ╲
    0 ├─────────•─────────────► t
                ▲
              Trough

If you sample slower (only once per cycle):
      ▲ x(t)
      │     •                    ← Only one point
      │    ╱ ╲                   per cycle
      │   ╱   ╲                  ✗ Miss the shape!
      │  ╱     ╲ 
      │ ╱       ╲ •              Could think it's
    0 ├─────────────────────────► t  just noise or DC
```

### Formal Definition

**Nyquist–Shannon Sampling Theorem:**

For a signal $x(t)$ with all frequency components in range $|F| \leq B$ Hz:
- **Minimum sampling rate:** $F_s \geq 2B$ Hz
- **Called "Nyquist rate":** $F_N = 2B$
- **Nyquist frequency:** $F_N/2 = B$ (highest frequency in signal)

**Example:**
- Speech frequencies: 0–3.5 kHz
- Minimum sampling rate: $F_s \geq 2 \times 3.5 = 7$ kHz
- In practice, pick $F_s = 8$ kHz (common for phone)

**CD audio:**
- Human hearing: 0–20 kHz (roughly)
- Minimum sampling: $F_s \geq 40$ kHz
- CD standard: $F_s = 44.1$ kHz (slight safety margin)

### What Happens if We Sample Too Slowly?

This leads to **aliasing** — the next crucial topic.

---

## Aliasing Explained

### What is Aliasing?

Aliasing happens when **you sample too slowly** and **high-frequency information disguises itself as low-frequency information**.

### Visual Example: The Wagon Wheel Effect

Ever notice in movies when a wagon wheel rotates slowly, then speeds up, and suddenly starts rotating *backwards*? That's aliasing!

```
Wheel rotating slowly (sampled fast enough):
    Frame 1:  Frame 2:  Frame 3:  Frame 4:
      |         /         —         \
      •         •         •         •     ← Looks like smooth rotation
     Rotation direction: clockwise ✓

Now wheel rotating very fast (sampled slowly):
    Frame 1:  Frame 2:  Frame 3:  Frame 4:
      |         \         —         /
      •         •         •         •     ← Looks like COUNTER-clockwise!
                          ▲ 
                    Aliasing effect!
```

The fast rotation creates an **illusion** of slow backward rotation because we're not sampling often enough.

### Aliasing in Signals

Same thing happens with frequency:

```
High-frequency tone at 12 kHz, sampled at 10 kHz (too slow):

Actual signal:     (oscillates rapidly 12 times per second)
      ▲
      │ ╱╲╱╲╱╲╱╲╱╲╱╲╱╲╱╱╲╱╲╱╲╱╲
      │╱  ╲  ╱  ╲  ╱  ╲  ╱  ╲  ╱  ╲
    0 ├─────────────────────────────► t
      │

Sample at 10 kHz (every 0.1 ms):
      ▲        Sampled points show...
      │     •           •              A SLOWER 
      │    • •         • •             oscillation!
      │ •       •   •       •
    0 ├─────────────────────────► t

The 12 kHz signal APPEARS to be 2 kHz because we didn't sample fast enough!
```

### Why Does This Happen? (Frequency Domain View)

When we sample, the **frequency spectrum repeats**:

```
Original signal spectrum X(F):
        ▲ |X(F)|
          │      ___
          │     /   \
          │____/     \____
              -12   0  +12 kHz
              
              (12 kHz tone)

After sampling at 10 kHz (SLOW - causes aliasing):
        ▲ |X_p(F)|
          │           replicas at ±10 kHz
          │    ___    ___    ___    ___
          │   /   \  /   \  /   \  /   \
        ──┼──/─────\/─────\/─────\/──────
          │       ▲
       -10 0  +10 kHz
       Replicas overlap! → Signal "folds back"
       12 kHz component appears as 2 kHz
       
       (because 12 - 10 = 2 kHz)
```

### The Aliasing Formula

When a tone at frequency $f_0$ is sampled below the Nyquist rate, it appears as:

$$f_{\text{alias}} = |f_0 - \text{round}(f_0/F_s) \cdot F_s|$$

**Example:**
- Tone: 12 kHz
- Sampling rate: 10 kHz
- $f_{\text{alias}} = |12 - \text{round}(12/10) \cdot 10| = |12 - 1 \times 10| = |12 - 10| = 2$ kHz

### Why Is This Bad?

Once aliasing happens, **it's impossible to undo**! You've lost information permanently.

```
Original:     12 kHz tone
                ↓ (sample at 10 kHz)
    Aliased:  2 kHz tone (appears to reconstruction)
                ↓ (reconstruct)
Result:       2 kHz sine wave (NOT the original 12 kHz!)
              Information lost forever ✗
```

---

## Bandlimited Signals

### What Does "Bandlimited" Mean?

A signal is **bandlimited to $B$ Hz** if it contains **no frequency components above $B$ Hz**.

Think of it like a frequency range:

```
Bandlimited to B = 4 kHz:

Frequency spectrum |X(F)|:
        ▲
        │      ___
        │     /   \
        │    /     \
        │   /       \
        │__/         \___
         -4   0   +4 kHz
        
        ✓ All signal energy within ±4 kHz
        ✓ Nothing at 5 kHz, 10 kHz, etc.

NOT bandlimited:
        ▲
        │    ___    ___
        │   /   \  /   \   ← Components spread out
        │  /     \/     \
        │_/              \___
         -∞              +∞ kHz
        ✗ Energy everywhere
```

### Real Signals Are Almost Never Naturally Bandlimited

**Real-world example:** Microphone + surroundings
- Speech: 0–3.5 kHz (main energy)
- Furniture creaking: 5–10 kHz
- Air conditioning hum: 100 Hz + harmonics
- Electronic noise: scattered across spectrum
- Etc.

The signal has energy everywhere, so it's **not** strictly bandlimited.

**Solution:** Use an **anti-aliasing filter** before sampling.

---

## Anti-Aliasing Filters

### The Core Idea

Before sampling, **filter out high frequencies** to make the signal bandlimited. Then you can safely sample.

```
Real signal (bandlimited to ∞):
        ▲
        │    ___    ___    ___     All frequencies
        │   /   \  /   \  /   \
        │  /     \/     \/     \
        │_/                      \___
         -∞                      +∞

        ↓ Pass through anti-aliasing filter (low-pass)

Filtered signal (bandlimited to B):
        ▲
        │      ___
        │     /   \
        │    /     \  (high frequencies removed)
        │   /       \
        │__/         \___
         -B     0    +B

        ↓ Now safe to sample at F_s ≥ 2B
```

### What is a Low-Pass Filter?

A **low-pass filter** allows low frequencies to pass through and **blocks high frequencies**.

Think of it like a speaker mesh:
```
Speaker mesh (low-pass filter):
      ▲ Amplitude
        │
      1 ├─────────────────  ← Low frequencies pass
        │                  (speaker cone vibrates)
        │     ╱╲          ╲
        │    ╱  ╲          ╲
      0 ├───╱────╲──────────╲─► F
        0    |     |__________| 
           F_p   F_s/2
        Pass   Cut
        
Low frequencies (bass): Pass through mesh → you hear them
High frequencies (treble): Blocked by mesh → you don't hear them
```

### Anti-Aliasing Filter Spec

For a signal you want to sample at $F_s$:

**Filter requirements:**
1. **Passband edge** $F_p$: frequencies below this pass through unchanged
2. **Stopband edge** $F_s$: frequencies above this are strongly attenuated
3. **Transition band**: smooth rolloff between passband and stopband

**Practical rule:**
- Choose $F_p \leq B$ (or slightly less) where $B$ is your desired signal bandwidth
- Choose $F_s/2$ as a rough guide for blocking replicas
- Leave a **guard band** (margin) of 10–20% to account for imperfect filters

**Example:**
```
Speech signal, want to sample at 8 kHz:

Nyquist frequency = 8/2 = 4 kHz

AA filter design:
  Passband:  0 – 3.5 kHz (keep speech)
  Transition: 3.5 – 4.5 kHz (smooth rolloff)
  Stopband:  4.5 – ∞ kHz (remove noise, high frequencies)

Spectrum before AA filter:
        ▲ |X(F)|
          │    ___    ___
          │   /   \  /   \
          │__/─────X─────X── (noise, hum, etc.)
          0 3.5 4.5    kHz

Spectrum after AA filter:
        ▲ |X_AA(F)|
          │      ___
          │     /   \
          │____/     \___
          0 3.5 4.5      (clean!)
```

### Why Not Just Use a Brick-Wall Filter?

Ideal filters have infinite order and are **non-causal** (require knowledge of future samples). Impractical.

**Practical filters** (Butterworth, Chebyshev, Elliptic):
- Finite order (realizable with resistors, capacitors, op-amps)
- Trade-off: smooth transition vs. steepness
- Butterworth: smoothest, slowest rolloff
- Chebyshev: faster rolloff, but ripple in passband
- Elliptic: fastest rolloff, but more ripples

---

## Ideal Sampling Method

### What is "Ideal" Sampling?

In **ideal sampling**, you imagine multiplying the signal by a train of **infinitely narrow impulses** (delta functions).

```
Time-domain view:

Continuous signal x(t):
      ▲ x(t)
      │     ╱╲      ╱╲
      │    ╱  ╲    ╱  ╲
      │   ╱    ╲  ╱    ╲
    0 ├────────────────────► t

Impulse train p(t) (sampling function):
      ▲ p(t)
      │ |   |   |   |   |   |   (infinitely tall, zero width)
      │ |   |   |   |   |   |   
      │ |   |   |   |   |   |   
    0 ├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─► t
      0  T_s 2T_s 3T_s

Sampled signal x_p(t) = x(t) × p(t):
      ▲ x_p(t)
      │ •   •   •   •   •   •   (impulses with weights = samples)
      │ |   |   |   |   |   |   
      │ |   |   |   |   |   |   
    0 ├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─► t
      0  T_s 2T_s 3T_s
      
      Weight of each impulse = original signal value at that time
```

### Mathematically

**Impulse train:**
$$p(t) = \sum_{n=-\infty}^{\infty} \delta(t - nT_s)$$

where $T_s$ = sampling period = $1/F_s$

**Sampled signal:**
$$x_p(t) = x(t) \cdot p(t) = \sum_{n=-\infty}^{\infty} x(nT_s) \delta(t - nT_s)$$

This represents a sequence of weighted impulses: the weight at time $nT_s$ is the signal value $x(nT_s)$.

### Frequency Domain View (Most Important!)

When you sample in time, **copies of the spectrum appear** at multiples of the sampling rate.

**Impulse train spectrum:**
$$P(F) = \frac{1}{T_s} \sum_{k=-\infty}^{\infty} \delta(F - kF_s)$$

In words: impulses in frequency domain at every multiple of $F_s$, each with "height" $1/T_s$.

**Sampled signal spectrum** (convolution in frequency):
$$X_p(F) = X(F) * P(F) = \frac{1}{T_s} \sum_{k=-\infty}^{\infty} X(F - kF_s)$$

**Visual explanation:**
```
Original signal spectrum X(F):
        ▲ |X(F)|         (one "hump")
        │      ___
        │     /   \
        │____/     \____
           -B   0    B  F


After ideal sampling at F_s = 5B (slow, for visualization):
        ▲ |X_p(F)|       (replicas at ±5B, ±10B, ...)
        │    ___      ___       ___
        │   /   \    /   \     /   \
        │__/─────\──/─────\───/─────\__
         -5B 0 B 5B      10B
            k=0       k=1

Key insight: 
  - k=0: original spectrum, centered at F=0
  - k=±1: copies centered at F=±F_s
  - k=±2: copies centered at F=±2F_s
  - etc.

Spacing between replicas = F_s (sampling rate)
Width of each replica = 2B (signal bandwidth)

SAFE if no overlap: F_s ≥ 2B ✓
ALIASING if overlap:  F_s < 2B ✗
```

### Why This Matters

The sampled spectrum shows why **Nyquist theorem works**:

- If replicas don't overlap, you can **isolate one replica** with a low-pass filter
- That replica is an exact copy of the original spectrum
- Inverting the low-pass filter output recovers the original signal perfectly
- But if replicas overlap, they mix → information permanently lost

---

## Natural Sampling (PAM)

### What is Natural Sampling?

Instead of infinitely narrow impulses (impractical), use **short rectangular pulses**:

```
Time-domain:

Pulse shape s(t) (width T_p, amplitude A):
      ▲ s(t)
      │    _____
      │   |     |
      A ├───|     |────
      │   |     |
      │   |_____|
    0 ├────────────────► t
      0  T_p
      

Pulse train p(t) = repeated pulses every T_s:
      ▲ p(t)
      │    ___    ___    ___
      │   |   |  |   |  |   |
      │   |   |  |   |  |   |
    0 ├───|___|──|___|──|___|───► t
      0  T_s  2T_s 3T_s

Duty cycle D = T_p / T_s = (pulse width) / (period)
```

**Sampled signal:**
$$x_p(t) = x(t) \cdot p(t)$$

The signal is **modulated by the pulse train** (not just weighted impulses).

### Frequency Domain

The pulse spectrum is:
$$S(F) = A T_p \cdot \text{sinc}(F T_p)$$

Remember: $\text{sinc}(x) = \frac{\sin(\pi x)}{\pi x}$

The pulse train spectrum becomes:
$$X_p(F) = \frac{1}{T_s} \sum_{k=-\infty}^{\infty} S(kF_s) \cdot X(F - kF_s)$$

**Key difference from ideal sampling:** Each replica is weighted by $S(kF_s)$, not constant!

```
Frequency response of pulse S(F):
        ▲ |S(F)|         sinc envelope
        │      /\
        │     /  \
        │    /    \___
        │   /           \___
        │__/                 \___
           0   F_s  2F_s  3F_s   F

Replica k is scaled by S(kF_s):
  k=0 (baseband):  S(0) = AT_p (maximum)
  k=±1:            S(±F_s) = AT_p · sinc(D)
  k=±2:            S(±2F_s) = AT_p · sinc(2D)
  
As k increases, sinc envelope decays, so higher replicas are automatically attenuated!
```

### Practical Advantage

Natural sampling **automatically suppresses higher replicas** due to the sinc envelope.

This makes reconstruction easier:
- Don't need a perfect brick-wall filter
- Wider transition band acceptable
- Practical filters (Butterworth, Chebyshev) work fine

### Comparison: Ideal vs. Natural Sampling

```
Ideal sampling:        Natural sampling (PAM):

|X_p(F)|               |X_p(F)|
  ▲                      ▲
  │ ___   ___   ___      │ ___  /\  /\   /\
  │/   \ /   \ /   \     │/   \/  \/  \ /  \
  │\___/ \___/ \___/     │ \___    ____/    \___
  │                      │
  └─────────────────► F  └─────────────────► F

All replicas          Replicas weighted by
equal height          sinc envelope (decays)
```

---

## Sample-and-Hold (ZOH)

### What is ZOH (Zero-Order Hold)?

In real A/D converters, you can't generate perfect impulses or perfect pulses. Instead, the circuit **holds the sample value constant** until the next sample arrives:

```
Time-domain:

Samples taken at t = 0, T_s, 2T_s, ...:
      ▲ x(t)
      │ • •      •       •
      │ | |      |       |     ← Sample times
      │ | |      |       |
    0 ├─┼─┼──────┼───────┼─────► t

ZOH output (sample-and-hold):
      ▲ x_ZOH(t)
      │ ___    ___    ___      ← Staircase!
      │|   |  |   |  |   |
      │|___|  |___|  |___|
    0 ├─────────────────────────► t
           T_s    2T_s

      Each sample "held" for duration T_s
```

### ZOH Impulse Response

The ZOH operation is equivalent to filtering with a rectangular pulse:

$$h_0(t) = u(t) - u(t - T_s)$$

where $u(t)$ is the step function.

In the frequency domain:
$$H_0(F) = T_s \cdot \text{sinc}(F T_s) \cdot e^{-j\pi F T_s}$$

The magnitude:
$$|H_0(F)| = T_s \cdot |\text{sinc}(F T_s)|$$

### ZOH Causes Amplitude Droop

The sinc envelope **attenuates high frequencies**:

```
Frequency response magnitude |H_0(F)|:
        ▲ |H_0(F)|
        │
      1 ├──────────  (DC response)
        │    ╱│╲
        │   ╱ │ ╲
      0.9├──╱  │  ╲   ← At F/F_s = 0.3, droop to ~90%
        │ ╱    │   ╲
        │╱     │    ╲___
    0 ├──────────────────► F/F_s
        0    0.3   0.5 (Nyquist)

Problem: 
  High frequencies are attenuated relative to low frequencies
  This distorts the reconstructed signal
  (loses "brightness", more "muddy")
```

**Example:** $F_s = 10$ kHz
- At F = 3 kHz: $|H_0(3)| / |H_0(0)| = \text{sinc}(0.3) \approx 0.859$ (14% loss)
- At F = 4 kHz: $|H_0(4)| / |H_0(0)| = \text{sinc}(0.4) \approx 0.757$ (24% loss)

### Why This Matters

ZOH is cheap and easy to implement in hardware, but introduces high-frequency distortion.

**Compensation techniques:**
1. Use a **reconstruction filter** with slight **high-frequency boost** (equalizer)
2. Use **pre-emphasis** before sampling (boost high frequencies)
3. Use more advanced hold circuits (not just ZOH)

---

## Reconstruction & Interpolation

### The Problem

After sampling, we have a sequence of numbers: $x[0], x[1], x[2], \ldots$

We want to get back a continuous signal $x_r(t)$ that matches the original $x(t)$.

**How do we "fill in" between samples?**

### Ideal Reconstruction (Sinc Interpolation)

**The perfect answer:** Use sinc functions!

**Formula:**
$$x_r(t) = \sum_{n=-\infty}^{\infty} x[n] \cdot \text{sinc}\left(\frac{t - nT_s}{T_s}\right)$$

where $\text{sinc}(x) = \frac{\sin(\pi x)}{\pi x}$

**What does this mean?**

For each sample $x[n]$, create a sinc function centered at time $nT_s$. Sum all these sincs:

```
Individual sinc functions:
        ▲                        ▲                        ▲
        │         sinc           │         sinc           │         sinc
        │        centered         │        centered        │        centered
        │        at n=0           │        at n=1          │        at n=2
        │           •             │           •            │           •
        │          ╱ ╲            │          ╱ ╲           │          ╱ ╲
    ────┼─────────╱───╲──────  ───┼──────────╱───╲────── ───┼──────────╱───╲────
      n=0 T_s     2T_s          n=1 T_s    2T_s          n=2 T_s   2T_s
        │          n=0            │         n=1           │         n=2
        │ ← Passes through        │ ← Passes through     │ ← Passes through
        │   sample n=0            │   sample n=1         │   sample n=2


Sum them all (superposition):
        ▲ x_r(t)
        │     ╱╲╱╲╱╲╱╲        ← Smooth curve
        │    ╱  ╲  ╱  ╲
    ────┼───╱────╲╱────╲───────
        │  •      •      •
        │  n=0    n=1    n=2
        │  (passes through all samples!)
```

**Magic property of sinc:**
$$\text{sinc}(k) = \begin{cases} 1 & k = 0 \\ 0 & k = \text{any other integer} \end{cases}$$

This means:
- At sample times $t = nT_s$, only the $n$-th sinc contributes (equals 1), all others vanish (equal 0)
- The reconstructed signal **passes exactly through all sample points**!

### Why Sinc Is Perfect

The sinc interpolation exactly reconstructs any **bandlimited signal** that was sampled at or above the Nyquist rate.

**Proof idea (frequency domain):**
- In frequency: each sampled replica can be isolated with a brick-wall low-pass filter
- The inverse FT of a brick-wall filter is exactly a sinc function in time
- So sinc interpolation recovers the original spectrum perfectly

### Practical Issue: Sinc is Infinite

The sinc function extends to $t = \pm \infty$, so **exact sinc reconstruction is impossible**:

```
sinc function:
        ▲
        │        ╱\
        │       ╱  \
        │      ╱    \___
    ────┼─────╱──────────\───────────
        │    ╱  ╲   ╱ ╲  ╱ ╲  ╱    ╲
        │   ╱    \ ╱   \/   \/      ╲
        └──────────────────────────────► t
         -3T_s -2T_s -T_s 0 T_s 2T_s 3T_s

        ↑ Tails extend forever!
        Real computers can't handle this.
```

**Practical solutions:**

1. **Zero-Order Hold (ZOH):**
   - Cheapest, easiest
   - Causes aliasing/distortion (amplitude droop)
   - Used in simple D/A converters

2. **First-Order Hold (Linear Interpolation):**
   - Triangular impulse response
   - Smoother than ZOH
   - Still not perfect

3. **Higher-Order Holds:**
   - Cubic spline, etc.
   - Approach sinc as order increases
   - More computation

### Reconstruction Filter (Frequency Domain)

After D/A conversion (sampling in reverse), use a low-pass filter to isolate the baseband:

```
Sampled-and-reconstructed spectrum (before filtering):
        ▲ |X_DAC(F)|
        │   replicas at ±F_s, ±2F_s, ...
        │    ___     ___     ___
        │   /   \   /   \   /   \
        │__/─────\─/─────\─/─────\__
         -F_s 0 F_s 2F_s
        
        ↓ Pass through reconstruction filter H_r(F) (low-pass)
        
After filtering |X_out(F)|:
        ▲
        │      ___
        │     /   \
        │____/     \____
           -B   0    B

        Replicas removed, only baseband remains ✓
```

**Filter design:**
- **Passband:** Covers original signal bandwidth (0 to B Hz)
- **Stopband:** Blocks first replica (starts around $F_s/2$ to $F_s$)
- **Gain:** $T_s$ to compensate for $1/T_s$ scaling during sampling

---

## Summary & Key Takeaways

### Quick Mental Model

1. **Sampling:** Convert continuous signal to numbers by taking snapshots
2. **Nyquist Theorem:** To avoid aliasing, sample at least twice per highest frequency
3. **Aliasing:** If you sample too slowly, high frequencies masquerade as low frequencies (irreversible!)
4. **Anti-Aliasing Filter:** Pre-filter to make signal bandlimited before sampling
5. **Three Sampling Methods:**
   - Ideal (impulses): theoretical, all replicas equal
   - Natural (pulses): practical, replicas weighted by sinc
   - ZOH (staircase): cheapest, causes high-freq distortion
6. **Reconstruction:** Interpolate samples back to continuous signal
7. **Ideal: Sinc interpolation** passes through sample points perfectly (but infinite-length)
8. **Practical:** Use ZOH, linear, or higher-order holds, combined with low-pass filtering

### The Big Picture

```
Continuous world          Digital world           Continuous world
(Analog)                  (Numbers)               (Analog)

x(t)                     x[n]                    x_r(t)
  │                        │                       ▲
  │                        │                       │
  ├→ Anti-aliasing Filter  │                       │
  │    ↓                    │                       │
  ├→ Sampler (A/D)────────→│                       │
  │    ↓                    │                       │
  │   Spectrum repeats      ├→ Reconstruction Filter
  │   (replicas)            │    ↓
  │                         └→ D/A Converter ────→ ├→ (hopefully ≈ x(t))
  │
  ← Nyquist criterion: F_s ≥ 2B ensures no aliasing
```

### Exam Checklist

Before the exam, make sure you can:

- [ ] State the Nyquist theorem: $F_s \geq 2B$
- [ ] Explain aliasing and why it's irreversible
- [ ] Design an anti-aliasing filter (specify passband, transition, stopband)
- [ ] Write the sampled spectrum formula: $X_p(F) = \frac{1}{T_s}\sum_k X(F-kF_s)$
- [ ] Sketch spectra (original, sampled, after reconstruction)
- [ ] Explain ZOH droop and why it happens
- [ ] Describe sinc interpolation and its ideal properties
- [ ] Compare ideal vs. natural vs. ZOH sampling
- [ ] Calculate alias frequencies
- [ ] Design reconstruction filters

---

## Visual Summary: The Sampling Process

```
STEP 1: Original Analog Signal
        ▲ x(t)
        │     ╱╲      ╱╲
        │    ╱  ╲    ╱  ╲
        │   ╱    ╲  ╱    ╲
    ────┼──────────────────► t
        │

STEP 2: Anti-Aliasing Filter
        Signal → [Low-Pass Filter] → Bandlimited signal
        (remove high-freq noise)

STEP 3: Sampling (A/D Converter)
        ▲ x[n] sequence
        │ •   •   •   •   •   •
        │ |   |   |   |   |   |
    ────┼─┼───┼───┼───┼───┼───┼──► n
        0 1   2   3   4   5   6

STEP 4: Digital Processing
        (if needed: filtering, analysis, etc.)

STEP 5: Reconstruction (D/A Converter)
        ▲ x_r(t)
        │ ___    ___    ___    (ZOH example)
        │|   |  |   |  |   |
        │|___|  |___|  |___|
    ────┼───────────────────► t

STEP 6: Reconstruction Filter
        Signal → [Low-Pass Filter] → Smooth analog output
        (remove replicas/artifacts)

STEP 7: Reconstructed Signal
        ▲ x_out(t)
        │     ╱╲      ╱╲       (hopefully ≈ original!)
        │    ╱  ╲    ╱  ╲
        │   ╱    ╲  ╱    ╲
    ────┼──────────────────► t
```

---

**Good luck with your studies! Master these concepts, and the math formulas will make sense.**
