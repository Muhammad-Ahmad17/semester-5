# DSP Chapter 6: Design Outcomes — Complete Exam Guide

## Table of Contents
1. [Anti-Aliasing Filter Design](#anti-aliasing-filter-design)
2. [Spectrum Analysis: Before and After Sampling](#spectrum-analysis-before-and-after-sampling)
3. [Sampling Function Spectrum](#sampling-function-spectrum)
4. [Reconstruction Filter Design](#reconstruction-filter-design)
5. [Signal Spectrum After Reconstruction](#signal-spectrum-after-reconstruction)
6. [Complete Worked Examples](#complete-worked-examples)
7. [Quick Reference & Checklist](#quick-reference--checklist)

---

## Anti-Aliasing Filter Design

### Outcome 1: Design AA Filter Given Sampling Rate

**What you need to find:**
1. Cutoff frequencies (passband edge, stopband edge)
2. Filter order/type
3. Impulse response

### Step 1: Understand the Requirements

**Given:**
- Original signal bandwidth: $B$ Hz (or approximate range of frequencies)
- Desired sampling rate: $F_s$ Hz

**Goal:**
- Limit signal to ensure no aliasing when sampled at $F_s$

**Nyquist constraint:**
$$F_s \geq 2B \quad \Rightarrow \quad B \leq \frac{F_s}{2}$$

### Step 2: Determine Filter Cutoff Frequencies

**Filter must pass the signal and reject replicas.**

#### Passband Edge ($F_p$)

The passband should include all signal frequencies of interest.

**Rules:**
- For natural signals: $F_p \approx B$ (signal bandwidth)
- For robustness: $F_p \leq B - \text{margin}$
- Typical margin: 10–20% safety factor

**Example:** If speech is 3.5 kHz, set $F_p = 3.5$ kHz (or 3.0 kHz for safety)

$$F_p = B \quad \text{or} \quad F_p = 0.9B \text{ (safe)}$$

#### Stopband Edge ($F_s$)

The stopband should start before the first replica (avoid aliasing).

First replica starts at: $F_s/2$ (Nyquist frequency)

**Rules:**
- Start transition near Nyquist: $F_s \approx F_s/2$
- Leave guard band for practical filter rolloff: $F_s \geq F_s/2 + \text{margin}$
- Typical margin: 10–20% of Nyquist

**Example:** If sampling at 8 kHz, Nyquist = 4 kHz
- Stopband edge: $F_s = 4 + 0.5 = 4.5$ kHz (0.5 kHz margin)

$$F_s \approx \frac{F_s}{2} + \text{guard band}$$

**Common mistake:** Don't confuse $F_s$ (stopband edge) with $F_s$ (sampling rate). They're different!

#### Transition Band

The filter transitions smoothly between passband and stopband.

$$\text{Transition width} = F_s - F_p$$

**Rule of thumb:**
$$\text{Transition width} \approx 0.2 \times F_p \text{ to } 0.5 \times F_p$$

### Step 3: Complete Filter Specification

**Typical AA filter spec:**

| Parameter | Value |
|-----------|-------|
| Passband frequency | $F_p$ (Hz) |
| Passband ripple | $\leq 0.5$ dB (typical) |
| Transition band | $F_p$ to $F_s$ (Hz) |
| Stopband frequency | $F_s$ (Hz) |
| Stopband attenuation | $\geq 60$ dB (typical for audio) |
| Filter type | Butterworth / Chebyshev / Elliptic |

### Step 4: Impulse Response

The impulse response depends on filter type and order.

#### Butterworth Low-Pass Filter

**General properties:**
- Maximally flat passband (best frequency response)
- Slow rolloff (order-dependent)
- Most "natural" sounding (for audio)

**Magnitude response:**
$$|H(F)| = \frac{1}{\sqrt{1 + (F/F_c)^{2n}}}$$

where:
- $F_c$ = cutoff frequency (usually = $F_p$ or midpoint)
- $n$ = filter order

**Impulse response:** Complex (involves partial fraction expansion)
- Higher order → longer impulse response
- Non-causal part is negligible for design purposes

**Transfer function (2nd-order example):**
$$H(s) = \frac{\omega_c^2}{s^2 + \sqrt{2}\omega_c s + \omega_c^2}$$

where $\omega_c = 2\pi F_c$

#### Chebyshev Low-Pass Filter

**Properties:**
- Sharper rolloff than Butterworth (for same order)
- Ripple in passband (trade-off for sharpness)
- Useful when sharp cutoff is critical

**Passband ripple:** Typical 0.5–3 dB

#### Magnitude Response Comparison

```
Butterworth vs Chebyshev:

|H(F)| (dB)
  0  ├─────────────────  ← Butterworth (flat)
     │     ╱╲         ╲
 -20 │    ╱  ╲         ╲__  ← Chebyshev (ripple, faster rolloff)
     │   ╱    ╲    ╱╲╱
 -40 │  ╱      ╲__╱
     │
 -60 └─────┬─────────────────► F
       F_p       F_s
```

### Step 5: Detailed AA Filter Design Example

**Problem:**
Audio signal with frequency content 0–4 kHz. Design an AA filter for sampling at 8 kHz.

**Solution:**

**Step 1: Verify Nyquist**
$$F_s = 8 \text{ kHz}, \quad B = 4 \text{ kHz}$$
$$F_s = 8 \geq 2B = 8 \quad \checkmark$$

Just meets Nyquist (tight!). Use safety margin.

**Step 2: Cutoff frequencies**

- Passband edge: $F_p = 3.5$ kHz (allow some margin below 4 kHz)
- Stopband edge: $F_s = 4.5$ kHz (1 kHz margin before Nyquist at 4 kHz)
- Transition: 3.5–4.5 kHz (1 kHz wide)

**Step 3: Filter type & order**

For audio (smooth sound preferred):
- Type: **Butterworth** (maximally flat)
- Order: **4th order** (sufficient for 60+ dB stopband attenuation)

**Step 4: Specifications**

| Spec | Value |
|------|-------|
| Passband edge | 3.5 kHz |
| Passband ripple | 0 dB (Butterworth is flat) |
| Transition | 3.5–4.5 kHz |
| Stopband edge | 4.5 kHz |
| Stopband attenuation | > 60 dB |
| Order | 4 |
| Type | Butterworth |

**Step 5: Transfer function (4th-order Butterworth)**

Normalized to $\omega_c = 1$, then scale:

$$H(s) = \frac{\omega_c^4}{(s^2 + 1.848\omega_c s + \omega_c^2)(s^2 + 0.765\omega_c s + \omega_c^2)}$$

With $\omega_c = 2\pi \times 3500$ rad/s, substitute and simplify.

**Step 6: Impulse response**

For 4th-order Butterworth with $F_c = 3.5$ kHz:

$$h(t) = A e^{-\alpha t} \left[\cos(\beta t) + \frac{\alpha}{\beta}\sin(\beta t)\right]$$

where $\alpha$, $\beta$ depend on the pole locations (typically computed numerically).

**Key property:**
- Exponential decay: $h(t) \to 0$ as $t \to \infty$
- Reasonable approximation for practical implementation

---

## Spectrum Analysis: Before and After Sampling

### Outcome 2: Plot Signal Spectra Before & After Sampling

### Part A: Spectrum Before Sampling

**Before AA filter:**

Original signal $x(t)$ may have energy across wide frequency range.

**Example spectrum:**
```
|X(F)| — Original analog signal:

      ▲
      │      ___
      │     /   \
      │    /     \      (main energy)
      │___/       \___
      │       •
      │     0 kHz
      │        ← Some high-freq components (noise)
      │  |
      │  |  hum/noise
      └──┼──────────────► F (kHz)
      0  10  20  30  40
      
Non-bandlimited: Energy scattered across spectrum
```

**After AA filter (bandlimited to B = 4 kHz):**

AA filter removes components above 4.5 kHz.

```
|X_AA(F)| — After anti-aliasing filter:

      ▲
      │      ___
      │     /   \
      │    /     \      (main energy preserved)
      │___/       \___
      └──────────────► F (kHz)
      0  4.5  9
      
Bandlimited to ~4.5 kHz: Safe to sample at 8+ kHz
```

### Part B: Spectrum After Sampling

**After ideal sampling at $F_s = 8$ kHz:**

The spectrum **repeats at multiples of $F_s$**.

**Sampled spectrum formula:**
$$X_p(F) = \frac{1}{T_s} \sum_{k=-\infty}^{\infty} X(F - kF_s)$$

**Visualization:**
```
|X_p(F)| — After ideal sampling:

Each replica is a scaled copy of X(F)
Spacing = F_s = 8 kHz
Scaling factor = 1/T_s = F_s = 8

        ▲
        │    ___      ___      ___
        │   /   \    /   \    /   \
        │__/─────\__/─────\__/─────\__
    ────┼────────────────────────────────► F (kHz)
        │-8  -4  0  4  8  12 16 20  24
        │k=-1   k=0    k=1    k=2
        
Center of each replica at:
  k=0: F = 0 kHz (baseband)
  k=1: F = 8 kHz
  k=2: F = 16 kHz
  k=-1: F = -8 kHz
  
Spacing between replicas = 8 kHz
Width of each replica = 2B = 9 kHz (roughly)

Since 2B = 9 < F_s = 8... wait, that's wrong!
Actually: 2B = 8 ≈ F_s, so we're at the edge!
```

**Zoom in on baseband and first replica:**
```
Baseband region:       First positive replica:

|X_p(F)|                |X_p(F)|
   ▲                       ▲
   │                       │
   │   ___                 │   ___
   │  /   \                │  /   \
   │ /     \               │ /     \
 ──┼─────────────────   ──┼─────────────────► F
 -4.5 0  4.5              3.5 8 12.5
   │                       │
   ← Can isolate with      ← Remove with filter
     low-pass filter
```

---

## Sampling Function Spectrum

### Outcome 3: Spectrum of Sampling Function

### Part A: Ideal Impulse Train Sampling

**Sampling function (time domain):**
$$p(t) = \sum_{n=-\infty}^{\infty} \delta(t - nT_s)$$

Train of Dirac delta functions spaced $T_s$ apart.

**Fourier transform (frequency domain):**
$$P(F) = \frac{1}{T_s} \sum_{k=-\infty}^{\infty} \delta(F - kF_s)$$

**Interpretation:**
- Impulses in frequency domain
- Spaced $F_s$ apart
- Each impulse has "strength" $1/T_s$

**Visual:**
```
Impulse train p(t):          Spectrum P(F):
   ▲                            ▲
   │ |    |    |    |           │ |  |  |  |  |
   │ |    |    |    |           │ |  |  |  |  |  ← Impulses at multiples of F_s
   │ |    |    |    |           │ |  |  |  |  |
 0 ├─┼────┼────┼────┼───► t     │ |  |  |  |  |
   │ 0   T_s  2T_s 3T_s        0 F_s 2F_s
   │                            
   │                        Height of each impulse = 1/T_s
```

### Part B: Natural Sampling (Rectangular Pulses)

**Sampling function (pulse train):**
$$p(t) = \sum_{n=-\infty}^{\infty} s(t - nT_s)$$

where $s(t)$ is a rectangular pulse of width $T_p$.

**Single pulse spectrum:**
$$S(F) = A T_p \cdot \text{sinc}(F T_p) = A T_p \frac{\sin(\pi F T_p)}{\pi F T_p}$$

**Pulse train spectrum:**
$$P(F) = S(F) \cdot \frac{1}{T_s} \sum_{k=-\infty}^{\infty} \delta(F - kF_s)$$

**Result: Weighted impulse train**
$$P(F) = \frac{1}{T_s} \sum_{k=-\infty}^{\infty} S(kF_s) \delta(F - kF_s)$$

**Visual:**
```
Single pulse s(t):           Pulse spectrum S(F):
    ▲ s(t)                      ▲ |S(F)|
    │   ___                     │     /\
    │  |   |                    │    /  \___
    │  |   |                    │   /       \___
  0 ├──────────► t            0 ├────────────────► F
    0  T_p                     0  F_s  2F_s

Pulse train P(F):
    ▲ |P(F)|
    │ |  |  |      ← Impulses scaled by sinc
    │ |/\|/\|\    ← Decreasing heights
    │ |  |  |  \
  0 ├────────────────► F
    0  F_s  2F_s  3F_s

Key difference: Heights modulated by sinc envelope!
```

**Duty cycle effect:**
$$D = \frac{T_p}{T_s} = \text{(pulse width)} / \text{(period)}$$

Weight of replica $k$:
$$|S(kF_s)| = |A T_p \cdot \text{sinc}(k D)|$$

**Example with D = 0.1 (10% duty cycle):**
```
|P(F)|:
   ▲
   │  1.0                sinc envelope
   │   •                    /\
   │  •/•                  /  \___
   │ •/ \•                /       \___
   └──────•──────────────────────────► F
     0.98 0.94 0.87  0.75
        ↑    ↑    ↑     ↑
       S(0) S(F_s) S(2F_s) S(3F_s)

Relative weights: 1.0, 0.98, 0.94, 0.87, ...
Higher replicas naturally attenuated!
```

### Part C: Sample-and-Hold (ZOH) Spectrum

**ZOH impulse response (time domain):**
$$h_0(t) = u(t) - u(t - T_s) = \begin{cases} 1 & 0 \leq t < T_s \\ 0 & \text{otherwise} \end{cases}$$

**ZOH frequency response:**
$$H_0(F) = T_s \cdot \text{sinc}(F T_s) \cdot e^{-j\pi F T_s}$$

**Magnitude only:**
$$|H_0(F)| = T_s \left| \text{sinc}(F T_s) \right| = T_s \left| \frac{\sin(\pi F T_s)}{\pi F T_s} \right|$$

**Spectrum of ZOH sampled signal:**
$$X_{ZOH}(F) = H_0(F) \cdot X_p(F)$$

All replicas are distorted by the sinc envelope.

**Visual:**
```
|X_p(F)| — Ideal sampling:       |X_ZOH(F)| — After ZOH:

   ▲ ___    ___    ___             ▲ ___    ___    ___
   │/   \  /   \  /   \           │/   \  /   \  /   \
   │\___/  \___/  \___/           │\__/   \___/  \___/
   └───────────────────► F        └───────────────────► F
   
   All equal height            Heights modulated by sinc
                               (decreases at higher F)
                               
|H_0(F)| envelope:
   ▲
   │  1.0 ─────
   │    ╱│╲    ╲
   │   ╱ │ ╲    ╲___
   │  ╱  │  ╲___
   │ ╱   │       ╲___
   └─────────────────────► F/F_s
     0  0.25  0.5
       ↑ Zero crossings at F = k·F_s
```

---

## Reconstruction Filter Design

### Outcome 4: Design Reconstruction Filter

### Part A: Frequency-Domain Design

**Goal:** Isolate the baseband replica from $X_p(F)$ to recover original signal.

**Ideal reconstruction filter:**
$$H_r(F) = \begin{cases}
T_s & |F| \leq B \\
0 & |F| > F_s/2
\end{cases}$$

This is a **brick-wall low-pass filter**:
- Passband: 0 to $B$ Hz
- Cutoff: $B$ Hz
- Stopband: $> F_s/2$ Hz
- Gain: $T_s$ (to undo $1/T_s$ scaling from sampling)

**Why gain $T_s$?**

The sampled signal has amplitude $1/T_s$ times the original:
$$X_p(F) = \frac{1}{T_s} X(F) + \text{(replicas)}$$

Multiplying by $T_s$ restores unity gain:
$$X_r(F) = X_p(F) \cdot H_r(F) = \frac{1}{T_s} X(F) \cdot T_s = X(F)$$

### Part B: Magnitude Response Specification

**Practical reconstruction filter:**
```
Brick-wall ideal:           Practical Butterworth:

|H_r(F)|                    |H_r(F)|
   ▲                           ▲
  T_s├─────────────            │
   │ │              │         T_s├──────╲
   │ │              │           │       ╲___
   0 ├──────────────┴────→ F    │         ╲____
      B    F_s/2                │
                              0 ├──────────────── ─► F
                                   B  F_s/2
                                   
Perfect cutoff            Smooth transition
(non-causal, ideal)       (realizable)
```

**Practical specifications:**

| Parameter | Value |
|-----------|-------|
| Passband frequency | $0$ to $B$ Hz |
| Passband ripple | $\leq 0.5$ dB |
| Transition band | $B$ to $F_s/2$ Hz |
| Stopband frequency | $\geq F_s/2$ Hz |
| Stopband attenuation | $\geq 60$ dB |
| Filter type | Butterworth / Chebyshev |
| Order | 4th to 8th order typical |

### Part C: Impulse Response

**Ideal brick-wall filter (non-causal, theoretical):**
$$h_r(t) = 2B \cdot \text{sinc}(2Bt)$$

**Properties:**
- Infinitely long tails (non-causal)
- Passes through zero at $t = \pm n/(2B)$
- Optimal for bandlimited signals

**Practical Butterworth (4th order example):**
$$h_r(t) = \text{(exponential decay with oscillation)}$$

Computed from poles in s-plane, generally:
$$h_r(t) = A_1 e^{-\alpha_1 t} \cos(\beta_1 t + \theta_1) + A_2 e^{-\alpha_2 t} \cos(\beta_2 t + \theta_2)$$

**Comparison:**
```
Ideal sinc:                 Practical Butterworth:

|h_r(t)|                    |h_r(t)|
   ▲                           ▲
   │         ╱│╲               │      ╱│╲
   │        ╱ │ ╲              │     ╱ │ ╲
   │   ____╱  │  ╲____         │    /  │  \___
   │  ╱       │       ╲        │  ╱   │    ╲___
───┼───────────────────────► t  ├─────────────────► t
   │ ← Infinite tails!         │ ← Finite decay!
   
Oscillates forever         Exponential envelope,
(ideal but impossible)     causal (practical)
```

### Part D: Worked Example: Reconstruction Filter Design

**Problem:**
Signal sampled at $F_s = 10$ kHz with original bandwidth $B = 4$ kHz.
Design a reconstruction filter.

**Solution:**

**Step 1: Verify Nyquist**
$$2B = 8 \text{ kHz} < F_s = 10 \text{ kHz} \quad \checkmark$$

No aliasing, so reconstruction possible.

**Step 2: Ideal filter spec**
- Passband: 0 to 4 kHz (pass original signal)
- Stopband: 5+ kHz (block replica starting at $F_s/2 = 5$ kHz)
- Gain: $T_s = 1/F_s = 100$ μs

**Step 3: Practical filter (Butterworth)**
- Type: **Butterworth** (smooth, minimal distortion)
- Order: **6th order** (sharp enough to block replicas)
- Passband edge: $F_p = 4$ kHz
- Stopband edge: $F_s = 5$ kHz
- Stopband attenuation: > 80 dB

**Step 4: Specifications table**

| Parameter | Value |
|-----------|-------|
| Passband | 0–4 kHz |
| Passband ripple | 0 dB (Butterworth) |
| Transition | 4–5 kHz |
| Stopband | 5+ kHz |
| Attenuation @ 5 kHz | > 80 dB |
| Order | 6 |
| Type | Butterworth |
| DC gain | $T_s = 100$ μs |

**Step 5: Transfer function (6th-order Butterworth)**

Normalized cutoff, then scale:
$$H_r(s) = \frac{\omega_c^6}{\text{(6 poles in left half-plane)}}$$

$\omega_c = 2\pi \times 4000$ rad/s

---

## Signal Spectrum After Reconstruction

### Outcome 5: Spectrum After Reconstruction

### Part A: Step-by-Step Spectrum Evolution

**Stage 1: Original bandlimited signal**
```
|X(F)|:

    ▲
    │      ___
    │     /   \
    │____/     \____
    └─────────────────► F
      -B   0    B
```

**Stage 2: After AA filter (before sampling)**
```
|X_AA(F)|: (essentially same if AA was ideal)

    ▲
    │      ___
    │     /   \
    │____/     \____
    └─────────────────► F
      -B   0    B
```

**Stage 3: After sampling (replicas appear)**
```
|X_p(F)|:

    ▲    ___     ___      ___
    │   /   \   /   \    /   \
    │__/─────\─/─────\──/─────\__
    └────────────────────────────────► F
     -F_s 0 F_s 2F_s
       k=-1 k=0 k=1
```

**Stage 4: After D/A conversion (e.g., ZOH hold)**
```
|X_ZOH(F)|: (amplitude modulated by |sinc(F T_s)|)

    ▲    ___     ___      ___
    │   /   \   /   \/    /   \/
    │__/─────\─/────────\/─────\___
    └────────────────────────────────► F
     -F_s 0 F_s 2F_s
       k=-1 k=0 k=1
       
Note: Replicas attenuated, baseband distorted by sinc
```

**Stage 5: After reconstruction filter**
```
|X_out(F)|: (only baseband remains, scaled appropriately)

    ▲
    │      ___
    │     /   \
    │____/     \____
    └─────────────────► F (kHz)
      0    B
      
✓ Original signal recovered!
✓ Replicas removed
✓ No additional components
```

### Part B: Visualization with Numbers

**Complete example: $F_s = 10$ kHz, $B = 4$ kHz**

```
|X(F)| — Original:
     ▲
     │  Max amplitude = 1.0
     │      ___
     │     /   \
     │____/     \____
     └─────────────────► F (kHz)
      -4   0    4


|X_p(F)| — After ideal sampling (1/T_s = 10 scaling):
     ▲
     │  Scaled by 1/T_s
     │    ___     ___      ___
     │   /   \   /   \    /   \
     │__/─────\─/─────\──/─────\__
     └────────────────────────────────► F (kHz)
      -14 -10 -6 -4 0 4 6 10 14
       k=-1   k=0   k=1


After ZOH (h_0(t) multiplied):
     ▲
     │  |sinc(F/F_s)| envelope applied
     │ ╱ ___   ╲    ___  ╱   ___  ╲
     │╱ /   \ ╲ │  /   \ │  /   \ │╲
     │\/─────\ │ ─/─────\─ ─/─────\─ \
     └────────────────────────────────────► F (kHz)
        ↑     ↑        ↑      ↑
        |     |        |      |
     Baseband still good, but replicas now visible


After reconstruction filter (low-pass at 4–5 kHz):
     ▲
     │  Passband: 0–4 kHz
     │      ___
     │     /   \
     │____/     \____
     └─────────────────► F (kHz)
      -4   0    4

✓ Replicas completely removed
✓ Baseband spectrum recovered
✓ Overall gain restored to unity
```

### Part C: Key Properties of Reconstructed Spectrum

**Perfect reconstruction requires:**

1. **No aliasing:** $F_s \geq 2B$ ✓
2. **No overlap of replicas:** Guaranteed if $F_s \geq 2B$
3. **Reconstruction filter passband:** Includes entire original bandwidth $[0, B]$
4. **Reconstruction filter stopband:** Starts at or before first replica ($F_s/2$)
5. **Correct gain:** $H_r(0) = T_s$ (or normalized to unity)

**If all conditions met:**
$$X_r(F) = X(F) \quad \text{for} |F| \leq B$$

$$x_r(t) = x(t) \quad \text{(perfect reconstruction)}$$

---

## Complete Worked Examples

### Example 1: Audio CD System

**Specifications:**
- Human hearing: 0–20 kHz
- Desired sampling rate: 44.1 kHz (CD standard)

**Part 1: AA Filter Design**

**Input:** Original audio (potentially up to kHz and noise)

**Step 1: Nyquist requirement**
$$F_s = 44.1 \text{ kHz} \Rightarrow \text{Nyquist} = 22.05 \text{ kHz}$$

Since human hearing is 20 kHz, we have margin.

**Step 2: AA filter spec**
- Passband: 0–20 kHz (human hearing range)
- Stopband: 22+ kHz (block first replica starting at $F_s/2 = 22.05$ kHz)
- Transition: 20–22 kHz (2 kHz wide)

**Step 3: Filter type**
- **Butterworth 8th order** (smooth, natural sound)
- Passband edge: 20 kHz
- Stopband edge: 22.05 kHz
- Stopband attenuation: > 100 dB

**Specifications:**

| Item | Value |
|------|-------|
| Filter type | Butterworth |
| Order | 8 |
| Passband | 0–20 kHz, < 0.1 dB ripple |
| Transition | 20–22 kHz |
| Stopband | > 22 kHz, > 100 dB atten. |

**Part 2: Spectrum Analysis**

```
Original audio (before AA):
     ▲
     │        ___
     │       /   \
     │      /     \
     │_____/       \__  ← Noise, hum,
     │                 hiss scattered
     └──────────────────► F (kHz)
     0  20  44.1

After AA filter:
     ▲
     │        ___
     │       /   \
     │      /     \
     │_____/       \____
     └──────────────────► F (kHz)
     0  20  22.05

After sampling (replicas at ±44.1, ±88.2 kHz...):
     ▲   ___       ___        ___
     │  /   \     /   \      /   \
     │ /     \   /     \    /     \
     │/───────\_/───────\__/───────\
     └──────────────────────────────────────► F (kHz)
     0 22.05 44.1 66.15 88.2

After reconstruction filter (0–20 kHz):
     ▲
     │        ___
     │       /   \
     │      /     \
     │_____/       \____
     └──────────────────► F (kHz)
     0  20  22
     
✓ Perfect reconstruction of original audio!
```

### Example 2: Telephone System (8 kHz)

**Specifications:**
- Speech bandwidth: 0–3.5 kHz
- Sampling rate: 8 kHz (ITU standard)

**Part 1: AA Filter Design**

**Step 1: Verify Nyquist**
$$2B = 7 \text{ kHz} < F_s = 8 \text{ kHz} \quad \checkmark$$

Just meets Nyquist, use safety margin.

**Step 2: AA filter spec**
- Passband: 0–3.5 kHz (speech)
- Stopband: 4.0+ kHz (block first replica at $F_s/2 = 4$ kHz)
- Transition: 3.5–4.0 kHz (0.5 kHz)

**Step 3: Filter type**
- **Chebyshev Type I, 6th order** (sharper rolloff needed for narrow transition)
- Passband edge: 3.5 kHz
- Passband ripple: 0.5 dB (acceptable for telephony)
- Stopband edge: 4.0 kHz
- Stopband attenuation: > 60 dB

**Specifications:**

| Item | Value |
|------|-------|
| Filter type | Chebyshev I |
| Order | 6 |
| Passband | 0–3.5 kHz, 0.5 dB ripple |
| Transition | 3.5–4.0 kHz |
| Stopband | > 4 kHz, > 60 dB atten. |

**Part 2: Spectrum Analysis**

```
Before AA:
     ▲
     │    ___
     │   /   \
     │  /     \  ← Hum at 50/60 Hz, noise
     │_/       \__
     └──────────────────► F (kHz)
     0 3.5 4.0  8

After AA filter:
     ▲
     │    ___
     │   /   \
     │  /     \  ← Clean!
     │_/       \___
     └──────────────────► F (kHz)
     0 3.5 4.0

After sampling (replicas at ±8, ±16 kHz...):
     ▲  ___    ___     ___
     │ /   \  /   \   /   \
     │/─────\/─────\─/─────\
     └───────────────────────────► F (kHz)
     0 4 8 12 16

After reconstruction filter:
     ▲
     │    ___
     │   /   \
     │  /     \
     │_/       \___
     └──────────────────► F (kHz)
     0 3.5 4.0

✓ Speech perfectly reconstructed!
```

### Example 3: Zero-Order Hold (ZOH) Distortion

**Problem:**
Signal sampled at 10 kHz with ZOH. Compare amplitude response at DC vs. 2 kHz.

**Solution:**

**ZOH frequency response:**
$$|H_0(F)| = T_s |\text{sinc}(FT_s)| = \frac{1}{F_s} |\text{sinc}(F/F_s)|$$

**At DC (F = 0):**
$$|H_0(0)| = \frac{1}{F_s} |\text{sinc}(0)| = \frac{1}{10000} \times 1 = 100 \text{ μV/V}$$

(normalized: 1.0)

**At 2 kHz:**
$$|H_0(2000)| = \frac{1}{10000} |\text{sinc}(2000/10000)| = |\text{sinc}(0.2)|$$

$$\text{sinc}(0.2) = \frac{\sin(0.2\pi)}{0.2\pi} = \frac{\sin(36°)}{36°} \approx \frac{0.588}{0.628} \approx 0.936$$

**Relative amplitude:**
$$\frac{|H_0(2000)|}{|H_0(0)|} = 0.936 = 93.6\%$$

**Attenuation:**
$$\text{Attenuation} = 1 - 0.936 = 6.4\%$$
$$\text{In dB:} 20\log_{10}(0.936) \approx -0.58 \text{ dB}$$

**Interpretation:**
A 2 kHz tone will be 6.4% quieter due to ZOH (small but noticeable).

**At 4 kHz:**
$$|H_0(4000)| / |H_0(0)| = |\text{sinc}(0.4)| = \frac{\sin(0.4\pi)}{0.4\pi} \approx 0.757$$

$$\text{Attenuation} = 24.3\% \approx -2.41 \text{ dB}$$

**Spectrum comparison:**

```
Ideal flat response:     ZOH response:
|H_0(F)|                |H_0(F)|
    ▲                       ▲
  1 ├─────────────            │
    │                       1 ├─────
  0 ├─────────────────► F    0 ├──╱─╲──────────────► F
    0    4    8 kHz          0 4  8 kHz
    
Perfect (impossible)    Amplitude droop at high freq
```

---

## Quick Reference & Checklist

### Design Checklist: AA Filter

- [ ] Verify Nyquist theorem: $F_s \geq 2B$ ✓
- [ ] Set passband edge: $F_p \leq B$ (with safety margin)
- [ ] Set stopband edge: $F_s \approx F_s/2 + \text{margin}$
- [ ] Choose filter type (Butterworth/Chebyshev/Elliptic)
- [ ] Choose filter order (typically 4th–8th)
- [ ] Verify passband ripple spec
- [ ] Verify stopband attenuation spec
- [ ] Specify transition bandwidth
- [ ] Compute/simulate frequency response
- [ ] Verify impulse response is realizable

### Spectrum Analysis Checklist

- [ ] Sketch original signal spectrum $|X(F)|$
- [ ] Identify bandwidth and peak frequencies
- [ ] After AA filter: sketch bandlimited spectrum
- [ ] After sampling: show replicas at $k \cdot F_s$
- [ ] Mark baseband (k=0) and first replicas (k=±1)
- [ ] Check spacing: $F_s \geq 2B$ (no overlap)
- [ ] After D/A: show amplitude modulation (if ZOH)
- [ ] After reconstruction: show isolated baseband
- [ ] Verify passband matches original
- [ ] Verify replicas are attenuated > 60 dB

### Reconstruction Filter Checklist

- [ ] Verify $F_s \geq 2B$ (no aliasing) ✓
- [ ] Passband: 0 to $B$ Hz
- [ ] Stopband: $F_s/2$ to $\infty$ Hz
- [ ] Transition band: $B$ to $F_s/2$ Hz
- [ ] DC gain: $H_r(0) = T_s$ (or unity if normalized)
- [ ] Filter type (Butterworth for smooth)
- [ ] Filter order (sufficient for stopband spec)
- [ ] Impulse response approximately finite
- [ ] Magnitude response smooth (minimal ripple)
- [ ] Stopband attenuation > 60 dB

### Key Formulas Summary

| Formula | Meaning |
|---------|---------|
| $F_s \geq 2B$ | Nyquist criterion |
| $T_s = 1/F_s$ | Sampling period |
| $X_p(F) = \frac{1}{T_s}\sum_k X(F-kF_s)$ | Ideal sampling spectrum |
| $P(F) = \frac{1}{T_s}\sum_k \delta(F-kF_s)$ | Impulse train spectrum |
| $S(F) = AT_p \cdot \text{sinc}(FT_p)$ | Pulse spectrum (natural sampling) |
| $H_0(F) = T_s \cdot \text{sinc}(FT_s) \cdot e^{-j\pi FT_s}$ | ZOH frequency response |
| $H_r(F) = \begin{cases} T_s & \|F\| \leq B \\ 0 & \text{else} \end{cases}$ | Ideal reconstruction filter |
| $h_r(t) = 2B \cdot \text{sinc}(2Bt)$ | Ideal reconstruction impulse response |
| $x_r(t) = \sum_n x[n] \cdot \text{sinc}\left(\frac{t-nT_s}{T_s}\right)$ | Sinc interpolation formula |

---

## Exam Tips

1. **Always sketch spectra first** before doing calculations
2. **Mark replica centers** and verify they don't overlap
3. **Show all intermediate steps** (AA filter → sampling → reconstruction)
4. **Specify cutoff frequencies clearly** (passband edge vs. stopband edge)
5. **Justify filter type choice** (why Butterworth vs. Chebyshev?)
6. **Verify Nyquist** before any other calculations
7. **Use normalized frequency** $u = F/F_s$ for cleaner math
8. **Remember:** AA filter prevents aliasing; reconstruction filter removes replicas
9. **Common mistake:** Confusing sampling period $T_s$ with filter transition width
10. **DC gain matters:** Reconstruction filter must have gain $T_s$ to restore amplitude

---

**You're ready! These are the exact topics that appear on exams. Master these examples and you'll ace the design questions.**
