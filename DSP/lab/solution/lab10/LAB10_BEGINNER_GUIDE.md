# LAB 10: IIR FILTER DESIGN — BEGINNER'S GUIDE
## Everything You Need to Know (Simple Language, No Mess)

---

## PREREQUISITES CHECKLIST
### Before Starting This Lab, Make Sure You Know...

**CRITICAL (Must Understand):**
- ✔ **Continuous vs Discrete signals** — analog (t) vs digital (n)
- ✔ **Frequency response** — magnitude and phase behavior
- ✔ **Four filter types** — LP, HP, BP, BS
- ✔ **Passband/Stopband concepts** — what frequencies pass vs block
- ✔ **Passband ripple (Rp) and Stopband attenuation (Rs)** — dB specifications
- ✔ **Filter order** — higher order = sharper transition
- ✔ **Bilinear transformation** — converts analog (s) to digital (z)
- ✔ **MATLAB filter commands** — butter(), cheby1(), cheby2(), ellip()

**IMPORTANT (Should Understand):**
- ✔ **Frequency normalization** — converting Hz → rad/s → rad/sample
- ✔ **Analog filter types** — Butterworth (flat), Chebyshev-I (passband ripple), Chebyshev-II (stopband ripple), Elliptic (both ripples)
- ✔ **Frequency transformations** — LP → BP, LP → HP, LP → BS
- ✔ **IIR filter basics** — feedback, infinite impulse response
- ✔ **MATLAB visualization** — fvtool(), freqz(), zplane()

**NICE TO HAVE (Good background):**
- Pole-zero concepts in s-plane and z-plane
- Complex numbers and logarithms (for dB)
- Impulse response h(t) and h[n]

**Missing Any of These?** Don't worry — Part 1 explains everything you need in simple terms. Start reading now and you'll be ready.

---

## DETAILED PREREQUISITES GUIDE
### Complete Reference for All 8 Prerequisite Topics

---

### **TOPIC 1: Basic Signals & Systems Concepts**

#### What You Must Know:

**1.1 — Continuous-Time (CT) vs Discrete-Time (DT) Signals**

**Continuous-Time Signal:** 
- Exists at **every moment in time** (smooth, unbroken)
- Represented by variable **t** (time in seconds)
- Example: Your voice in the air, analog voltage
- Notation: $x(t)$, $f(t)$

```
Continuous signal:
    |     ╱╲      ╱╲
    |    ╱  ╲    ╱  ╲
    |___╱____╲__╱____╲___
    0        t
    (smooth curve, defined everywhere)
```

**Discrete-Time Signal:**
- Exists only at **specific time points** (sampled)
- Represented by variable **n** (sample number: 0, 1, 2, 3...)
- Example: MP3 audio file (44,100 samples per second), digital sensor readings
- Notation: $x[n]$, $f[n]$

```
Discrete signal:
    |  •        •        •
    |  •        •        •
    |__•___•____•____•___•___
    0  1  2  3  4  5  6
    (points only, no values between)
```

**Why Lab 10 Cares:**
- **Analog filters** work with CT signals (your prototype)
- **Digital filters** work with DT signals (your implementation)
- **Bilinear transform** connects them

**Sampling Frequency ($f_s$):**
- How many samples per second you collect
- Example: $f_s = 8000$ Hz means 8000 samples per second
- Related to discrete-time by: $T_s = 1/f_s$ (sampling period)

---

**1.2 — Time-Domain vs Frequency-Domain Behavior**

**Time-Domain:**
- Shows **how the signal changes over time**
- Asks: "What is the value at time t = 2 seconds?"
- Plot: x-axis = time, y-axis = amplitude
- Easy to visualize, hard to understand filtering

Example:
```
x(t) = sin(2πt) + noise
```

**Frequency-Domain:**
- Shows **what frequencies are present** in the signal
- Asks: "How much of 100 Hz is in the signal? How much of 500 Hz?"
- Plot: x-axis = frequency (Hz), y-axis = magnitude
- Hard to visualize naturally, easy to understand filtering

Example:
```
X(jω) = strong at 100 Hz, weak at 500 Hz
```

**Key Connection (Fourier Transform):**
- Transform converts from time → frequency
- Formula: $X(j\omega) = \int_{-\infty}^{\infty} x(t) e^{-j\omega t} dt$
- **You don't need to calculate this** — just understand concept

**Why Lab 10 Cares:**
- Filters are **defined in frequency domain**
- "Keep 800-2000 Hz" = frequency specification
- You'll use MATLAB to plot frequency response

---

**1.3 — Impulse Response and Frequency Response**

**Impulse Response $h(t)$ or $h[n]$:**
- **Output when input is a single spike** (impulse)
- Completely describes a linear system
- Tells you how system "rings" when poked once

Example:
```
Input: spike at t=0
    |
    |*
    |
    |___________
    0           t

Output (impulse response):
    |    ╱╲
    |   ╱  ╲
    |__╱____╲____ (rings, then dies down)
    0           t
```

**Frequency Response $H(j\omega)$ or $H(e^{j\omega})$:**
- **Output magnitude and phase for each frequency**
- Shows how system treats different frequencies
- Format: $H(j\omega) = |H(j\omega)| \angle \theta(\omega)$
  - Magnitude: $|H(j\omega)|$ — how much is output amplified/reduced
  - Phase: $\theta(\omega)$ — how much signal is delayed

Example for low-pass filter:
```
Magnitude Response:
    |
  1 |____
    |    \
    |     \___
    |         \___
  0 |_____________\___
    0        cutoff    ω

Phase Response:
    |
  0 |____
    |    \
    |-90°|  \____
    |         \___
-180°|____________\__
    0        cutoff    ω
```

**Why Lab 10 Cares:**
- You'll design filters based on frequency response
- MATLAB shows this with `freqz()` and `fvtool()`
- Filter specifications use magnitude response

---

### **TOPIC 2: Analog Filter Theory (VERY IMPORTANT)**

#### What You Must Know:

**2.1 — The Four Standard Analog Filter Types**

**Butterworth Filter:**
- **Characteristic:** Maximally flat passband
- **Ripple:** None (perfectly flat in passband)
- **Attenuation:** Gradual in stopband
- **Use:** When you want smooth, ripple-free output
- **Visual:**
```
Magnitude
    |
  1 |____         Butterworth
    |    \___     (smooth, no ripples)
  0 |        \___
    |
    Frequency
```

**Chebyshev Type-I (Chebyshev-I) Filter:**
- **Characteristic:** Allows ripple in passband
- **Ripple:** Yes, small controlled ripples
- **Attenuation:** Sharper in stopband (better than Butterworth)
- **Use:** When stopband rejection is more important than smooth passband
- **Visual:**
```
Magnitude
    |
  1 |_/\_/\_     Chebyshev-I
    |  /\  \__   (ripples in passband)
  0 |_________\__
    |
    Frequency
```

**Chebyshev Type-II (Chebyshev-II) Filter:**
- **Characteristic:** Allows ripple in stopband (flat passband)
- **Ripple:** None in passband, ripples in stopband
- **Attenuation:** Very sharp
- **Use:** When you need flat passband but don't care about stopband ripple
- **Visual:**
```
Magnitude
    |
  1 |____         Chebyshev-II
    |    \        (flat passband,
  0 | /\/\/\__   ripples in stopband)
    |/
    Frequency
```

**Elliptic Filter (Cauer Filter):**
- **Characteristic:** Ripples in both passband and stopband
- **Ripple:** Yes, in both bands
- **Attenuation:** Sharpest transition (most selective)
- **Use:** When you want the sharpest transition possible
- **Visual:**
```
Magnitude
    |
  1 |_/\_/\_     Elliptic
    | /\  /\_    (ripples everywhere,
  0 |_  \/  \__ sharpest transition)
    |
    Frequency
```

**Quick Comparison Table:**

| Filter | Passband Ripple | Stopband Ripple | Sharpness | Filter Order |
|--------|-----------------|-----------------|-----------|--------------|
| **Butterworth** | No | No | Gradual | Highest |
| **Cheby-I** | Yes | No | Good | Medium |
| **Cheby-II** | No | Yes | Better | Medium |
| **Elliptic** | Yes | Yes | Sharpest | Lowest |

---

**2.2 — Filter Performance Parameters**

**Passband Ripple (Rp) — in dB:**
- Measurement of how flat the passband is
- Ranges typically: 0.1 dB to 3 dB
- **Lower is better** (flatter passband)
- Example: 0.5 dB means passband magnitude wiggles by ±0.5 dB
- Formula: $Rp = 20 \log_{10}(1 + \delta_p)$ where $\delta_p$ is ripple amplitude

**Stopband Attenuation (As) — in dB:**
- Measurement of how much stopband is suppressed
- Ranges typically: 20 dB to 100 dB
- **Higher is better** (more suppression)
- Example: 40 dB attenuation means stopband signal is 100× smaller than passband
- Formula: $As = -20 \log_{10}(\delta_s)$ where $\delta_s$ is stopband error

**Cutoff Frequency (ωc) — in rad/s or Hz:**
- **Boundary** between passband and stopband
- For low-pass: all frequencies below $\omega_c$ = passband
- For band-pass: middle region = passband

**Transition Band:**
- Region between passband and stopband
- Wider = gentler transition, lower filter order
- Narrower = sharper transition, higher filter order

---

**2.3 — Magnitude Squared Response |H(jω)|²**

**Why It Matters:**
- Analog filter tables and design formulas use this form
- Easier mathematically than just |H(jω)|
- Definition: $|H(j\omega)|^2 = H(j\omega) \cdot H(-j\omega)$

**Example (Butterworth):**
$$|H(j\omega)|^2 = \frac{\omega_c^{2n}}{(\omega^2 + \omega_c^2)^n}$$

- $\omega_c$ = cutoff frequency
- $n$ = filter order
- At $\omega = 0$: $|H|^2 = 1$ (full response)
- At $\omega = \omega_c$: $|H|^2 = 0.5$ (half power = -3 dB)

**You don't need to memorize this, but know it exists.**

---

### **TOPIC 3: Analog → Digital Filter Conversion**

#### What You Must Know:

**3.1 — Bilinear Transformation (BLT) — THE MOST IMPORTANT**

**Definition:**
$$s = \frac{2}{T} \frac{1 - z^{-1}}{1 + z^{-1}} = \frac{2f_s(1 - z^{-1})}{1 + z^{-1}}$$

Where:
- $s$ = analog variable (Laplace)
- $z^{-1}$ = digital variable (delay operator)
- $T = 1/f_s$ = sampling period
- $f_s$ = sampling frequency (Hz)

**What It Does:**
- **Maps** analog filter coefficients → digital filter coefficients
- **Converts** analog transfer function $H(s)$ → digital transfer function $H(z)$
- **Preserves** passband and stopband specifications (approximately)

**Key Property — Frequency Mapping:**
$$\omega_d = 2 \arctan\left(\frac{\omega_a}{2f_s}\right)$$

- $\omega_a$ = analog frequency (rad/s)
- $\omega_d$ = digital frequency (rad/sample)
- **Important:** This is **nonlinear** — high analog frequencies "compress" more

**Visual Example:**
```
Analog filter response:
    |
  1 |____       (smooth, designed for specific frequencies)
    |    \___
  0 |        \___
    0   ωc    ω_a

BLT applies...

Digital filter response:
    |
  1 |____       (similar shape, but frequencies compressed)
    |    \___   due to frequency warping
  0 |        \___
    0 Ω_c    Ω_d (rad/sample)
```

**Pre-Warping (Advanced Concept):**
- Because of frequency mapping compression, you must **pre-warp** critical frequencies
- Before designing analog filter, adjust specifications:
$$\omega_{a,p} = 2f_s \tan\left(\frac{\omega_{d,p}}{2}\right)$$
- **Don't worry:** MATLAB does this automatically with `bilinear()`

**Why This Lab Uses BLT:**
- Simple, reliable, widely used
- Preserves analog filter characteristics
- Stable (analog poles on left-plane → digital poles inside unit circle)

---

**3.2 — Impulse Invariant Transformation**

**Definition:**
- Maps analog impulse response $h_a(t)$ → digital impulse response $h_d[n]$
- Formula: $h_d[n] = T \cdot h_a(nT)$ where $T = 1/f_s$

**Advantage:** 
- Preserves exact analog impulse response

**Disadvantage:**
- Can cause **aliasing** if analog filter has high frequencies
- Less common today

**When Used:**
- Mostly in older textbooks
- **Lab 10 uses BLT, not impulse invariant**

---

### **TOPIC 4: Frequency Band Transformations**

#### What You Must Know:

**4.1 — Analog Transformations (s → s')**

**Key Idea:**
- Start with **analog low-pass prototype**
- Apply algebraic substitution to convert → HP, BP, BS

**Transformation Formulas:**

**Low-Pass to High-Pass:**
$$s \rightarrow \frac{\omega_c}{s}$$
- Swaps passband and stopband
- Visual: LP [0, $\omega_c$] → HP [$\omega_c$, ∞]

**Low-Pass to Band-Pass:**
$$s \rightarrow \frac{s^2 + \omega_0^2}{Bs}$$
Where:
- $\omega_0 = \sqrt{\omega_p \cdot \omega_s}$ (center frequency)
- $B = \omega_s - \omega_p$ (bandwidth)
- Creates passband between $\omega_p$ and $\omega_s$

**Low-Pass to Band-Stop:**
$$s \rightarrow \frac{Bs}{s^2 + \omega_0^2}$$
- Inverse of BP transformation
- Creates stopband between $\omega_p$ and $\omega_s$

**Visual Example (LP → BP):**
```
Low-pass prototype:
    |
  1 |____
    |    \___
  0 |        \___
    0   ωc       ω_a
    (0 to ωc: pass, ωc+: stop)

Apply LP→BP transformation:
    |
  1 |      ____
    |     |    |
  0 |_____|    |_____
    0  ωp  ωc  ωs    ω_a
    (passband: ωp to ωs)
```

**MATLAB Functions:**
```matlab
[num_hp, den_hp] = lp2hp(num_lp, den_lp, wc);    % LP → HP
[num_bp, den_bp] = lp2bp(num_lp, den_lp, wo, Bw); % LP → BP
[num_bs, den_bs] = lp2bs(num_lp, den_lp, wo, Bw); % LP → BS
```

**Lab 10 Sequence:**
1. Design analog **LP** prototype with `butter()`, `cheby2()`, etc.
2. Apply `lp2bp()` or `lp2hp()` to transform
3. Apply `bilinear()` to convert to digital

---

**4.2 — Digital Transformations (z → z')**

**Alternative Approach (Not Used in Standard Lab 10):**
- Apply transformations in **digital domain** instead of analog
- More complex, less common
- **Skip this for Lab 10**

---

### **TOPIC 5: Frequency Normalization and De-normalization**

#### What You Must Know:

**5.1 — Three Forms of Frequency**

**Hertz (Hz) — Cycles Per Second:**
- Most intuitive for real-world signals
- Example: Audio at 1000 Hz
- Notation: $f$ (lowercase)
- Range: 0 Hz to $\infty$

**Radians Per Second (rad/s) — Analog:**
- Mathematical form for analog filters
- Relationship: $\omega_a = 2\pi f$
- Example: 1000 Hz = $2\pi \times 1000 = 6283$ rad/s
- Notation: $\omega_a$ (with subscript 'a' for analog)
- Range: 0 to $\infty$

**Radians Per Sample (rad/sample) — Digital:**
- Mathematical form for digital filters
- Relationship: $\omega_d = \frac{2\pi f}{f_s}$ or $\omega_d = \Omega T_s$ 
- Example: If $f = 1000$ Hz and $f_s = 8000$ Hz, then $\omega_d = 2\pi \times 1000/8000 = 0.785$ rad/sample
- Notation: $\omega_d$ (with subscript 'd' for digital) or $\Omega$
- Range: 0 to $\pi$ (due to Nyquist limit)

**Normalized Analog Frequency:**
- Divide analog frequency by passband edge: $\Omega = \omega / \omega_p$
- Dimensionless (just a ratio)
- Used for designing **low-pass prototype** (standardized to have $\omega_p = 1$ rad/s)

---

**5.2 — Conversion Formulas**

**Hz ↔ rad/s:**
$$\omega_a = 2\pi f$$
$$f = \frac{\omega_a}{2\pi}$$

**Hz ↔ rad/sample (requires sampling frequency $f_s$):**
$$\omega_d = \frac{2\pi f}{f_s}$$
$$f = \frac{f_s \cdot \omega_d}{2\pi}$$

**rad/s ↔ rad/sample:**
$$\omega_d = \frac{\omega_a}{f_s}$$ (approximately, for small frequencies)
$$\omega_d = 2\arctan\left(\frac{\omega_a}{2f_s}\right)$$ (exact, BLT formula)

---

**5.3 — Practical Example**

**Scenario:** Design a band-pass filter for 800-2000 Hz, sampling at 8000 Hz.

**Step 1: Convert to rad/s (for analog design)**
- $f_p = 800$ Hz → $\omega_p = 2\pi \times 800 = 5027$ rad/s
- $f_s = 2000$ Hz → $\omega_s = 2\pi \times 2000 = 12566$ rad/s

**Step 2: Design analog low-pass prototype**
- Use specifications on these frequencies

**Step 3: Apply band-pass transformation**
- MATLAB handles this internally

**Step 4: Convert to digital with BLT**
- Input: analog coefficients from Step 2
- `bilinear()` converts using $f_s = 8000$ Hz
- Output: digital filter ready to use

---

### **TOPIC 6: MATLAB Basics for Filters**

#### What You Must Know:

**6.1 — Filter Order Determination**

**What is "Order"?**
- Determines sharpness of transition
- Higher order = more computation, sharper transition
- Automatic calculation based on specifications

**MATLAB Functions (Butterworth example):**

```matlab
% Butterworth
[N, Wn] = buttord(Wp, Ws, Rp, Rs);

% Inputs:
% Wp = passband frequency (rad/sample), range [0, 1]
% Ws = stopband frequency (rad/sample), range [0, 1]
% Rp = passband ripple (dB), e.g., 0.5
% Rs = stopband attenuation (dB), e.g., 40
% Outputs:
% N = minimum order needed
% Wn = optimal cutoff frequency

% Example:
[N, Wn] = buttord(0.2, 0.3, 0.5, 40);
% Returns: N = 5 (order 5 filter needed)
```

**For Other Filter Types:**
```matlab
[N, Wn] = cheb1ord(Wp, Ws, Rp, Rs);  % Chebyshev-I
[N, Wn] = cheb2ord(Wp, Ws, Rp, Rs);  % Chebyshev-II
[N, Wn] = ellipord(Wp, Ws, Rp, Rs);  % Elliptic
```

**Key Point:**
- MATLAB calculates optimal order for your specifications
- **You don't calculate by hand** — just use the function

---

**6.2 — Filter Design Functions**

**Butterworth Design:**
```matlab
[b, a] = butter(N, Wn);
% b, a = numerator and denominator coefficients
% N = order (from buttord)
% Wn = cutoff frequency (from buttord)
```

**Chebyshev Type-I:**
```matlab
[b, a] = cheby1(N, Rp, Wn);
% Rp = passband ripple (dB)
```

**Chebyshev Type-II:**
```matlab
[b, a] = cheby2(N, Rs, Wn);
% Rs = stopband attenuation (dB)
```

**Elliptic:**
```matlab
[b, a] = ellip(N, Rp, Rs, Wn);
% Both Rp and Rs specified
```

**Understanding Outputs:**
- `b` = numerator coefficients [b₀ b₁ b₂ ...]
- `a` = denominator coefficients [a₀ a₁ a₂ ...]
- These represent the **analog** transfer function initially
- Form: $H(s) = \frac{b_0 + b_1 s + b_2 s^2 + ...}{a_0 + a_1 s + a_2 s^2 + ...}$

---

**6.3 — Bilinear Transformation**

**Convert analog → digital:**
```matlab
[bt, at] = bilinear(b, a, fs);
% b, a = analog filter coefficients
% fs = sampling frequency (Hz)
% bt, at = digital filter coefficients
```

**Example:**
```matlab
fs = 8000;  % sampling frequency
[b, a] = butter(5, 0.5);  % analog filter
[bt, at] = bilinear(b, a, fs);  % convert to digital
```

---

**6.4 — Frequency Band Transformations**

**Low-Pass to High-Pass:**
```matlab
[num_hp, den_hp] = lp2hp(num_lp, den_lp, wc);
% wc = high-pass cutoff frequency (rad/s)
```

**Low-Pass to Band-Pass:**
```matlab
[num_bp, den_bp] = lp2bp(num_lp, den_lp, wo, Bw);
% wo = band-pass center frequency (rad/s)
% Bw = bandwidth = ws - wp (rad/s)
```

**Low-Pass to Band-Stop:**
```matlab
[num_bs, den_bs] = lp2bs(num_lp, den_lp, wo, Bw);
```

---

**6.5 — Filter Analysis and Visualization**

**Frequency Response Plot:**
```matlab
freqz(b, a);  % Digital filter frequency response
% Shows magnitude and phase vs normalized frequency
```

**Pole-Zero Plot:**
```matlab
zplane(b, a);  % Shows poles (×) and zeros (○)
% Poles inside unit circle = stable filter
```

**Filter Visualization Tool:**
```matlab
fvtool(b, a);  % Opens interactive filter design tool
% Shows magnitude, phase, step response, pole-zero plot
```

---

### **TOPIC 7: Understanding IIR Filter Behavior**

#### What You Must Know:

**7.1 — Feedback (Recursive Structure)**

**IIR Formula:**
$$y[n] = b_0 x[n] + b_1 x[n-1] + b_2 x[n-2] + ... - a_1 y[n-1] - a_2 y[n-2] - ...$$

Components:
- **Feed-forward:** $b_0 x[n] + b_1 x[n-1] + ...$ (depends on input)
- **Feedback:** $-a_1 y[n-1] - a_2 y[n-2] - ...$ (depends on **previous outputs**)

**Visual Block Diagram:**
```
Input x[n] ──→ [b coefficients] ──→ (+) ──→ Output y[n]
                                    ↑
                    Delay → [a coefficients] ←
                           (feedback loop)
```

**Key Difference from FIR:**
- FIR: $y[n] = b_0 x[n] + b_1 x[n-1] + ...$ (no feedback)
- IIR: $y[n] = b_0 x[n] + b_1 x[n-1] + ... - a_1 y[n-1] - ...$ (has feedback)

---

**7.2 — Infinite Impulse Response**

**Definition:**
- When you give the filter a single spike (impulse) input, the output continues **forever**
- Oscillations gradually die down but theoretically never reach exactly zero

**Example:**
```
Input (impulse at n=0):
|*
|_______

Output (IIR):
| /\  /\  /\
|/  \/  \/  \______ (continues forever, decays)
```

**Why "Infinite"?**
- Feedback loop allows signal to bounce around forever
- Mathematically: $\sum_{n=0}^{\infty} |h[n]| < \infty$ for stable filters
- **Contrast:** FIR has finite response length

---

**7.3 — Not Linear Phase**

**Linear Phase Means:**
- All frequencies delayed by **same amount**
- No phase distortion
- Important for music, images where distortion is harmful

**IIR Reality:**
- Different frequencies have **different delays**
- Some frequencies delayed more than others
- Can cause audible phase distortion

**Why IIR Can't Be Linear Phase:**
- Feedback structure inherently creates nonlinear phase
- Tradeoff: IIR is efficient but has phase distortion
- FIR can be linear phase but less efficient

**When Does It Matter?**
- Audio: Can hear the distortion
- Images: Visible blur/ringing
- Telecommunications: May degrade signal
- **Lab 10:** Not focused on this, but good to know

---

**7.4 — Approximates Analog Filters**

**Key Advantage:**
- IIR digital filters **mimic** analog filter behavior
- Useful when you have analog filter specifications
- Easy to design: analog → digital conversion

**Process:**
1. Design analog filter (well-studied, lots of tables)
2. Apply bilinear transformation
3. Get digital IIR filter with similar specs

**Accuracy:**
- Passband/stopband match well
- Phase response **may not** match (due to nonlinear phase)
- Group delay (frequency-dependent delay) differs

---

### **TOPIC 8: Math Skills Required**

#### What You Must Know:

**8.1 — Complex Numbers**

**Form:** $a + jb$ where $j = \sqrt{-1}$

**Operations:**
- Addition: $(a + jb) + (c + jd) = (a+c) + j(b+d)$
- Multiplication: $(a + jb)(c + jd) = (ac - bd) + j(ad + bc)$
- Conjugate: $(a + jb)^* = a - jb$

**Polar Form:** $r e^{j\theta} = r(\cos\theta + j\sin\theta)$
- $r = |a + jb| = \sqrt{a^2 + b^2}$ (magnitude)
- $\theta = \tan^{-1}(b/a)$ (phase angle)

**Why Needed:**
- Transfer functions use complex notation: $H(s)$, $H(z)$
- Frequency response is complex: $H(j\omega) = |H| \angle \theta$

---

**8.2 — Poles and Zeros**

**Poles:**
- Values of $s$ (or $z$) where denominator = 0
- System "blows up" at poles
- Represent the system's natural frequencies

**Zeros:**
- Values of $s$ (or $z$) where numerator = 0
- System output = 0 at these frequencies

**Stability (CRITICAL for IIR):**
- **Analog:** Poles must be on left half of s-plane (negative real part)
- **Digital:** Poles must be **inside unit circle** in z-plane (magnitude < 1)
- If poles violate this → **filter goes unstable** (output blows up)

**MATLAB Check:**
```matlab
[z, p, k] = tf2zpk(b, a);  % Extract zeros and poles
% z = zeros, p = poles
% Check: abs(p) < 1 for stability
```

**Visual:**
```
Pole-Zero Plot (Digital):
Unit Circle (|z| = 1)
        |
      × | ○      × = pole, ○ = zero
        | | × 
     ---|---| ---
        | | 
        | ×

Stable: all poles (×) inside circle
Unstable: any pole outside circle
```

---

**8.3 — s-Plane to z-Plane Mapping**

**Bilinear Transform Maps:**
$$s \leftrightarrow z$$

**Mapping Properties:**
- Left-half s-plane → inside unit circle in z-plane
- Imaginary axis (s = jω) → unit circle boundary
- Stability preserved: stable analog → stable digital

**Visual:**
```
s-plane                    z-plane
(analog)                   (digital)

jω |←—— LHS ——→|            |
   |                       unit circle
———+——— σ                 ———+———
   |←——— RHS ——→|           |

Stable region → Stable region
```

---

**8.4 — Factorization**

**Why Needed:**
- Transfer functions factored as products of simpler terms
- Example: $H(s) = \frac{K(s-z_1)(s-z_2)...}{(s-p_1)(s-p_2)...}$
- Each factor represents a pole or zero

**Quadratic Factorization:**
- Second-order sections (SOS): $H(z) = H_1(z) \cdot H_2(z) \cdot ...$
- Better for numerical stability in high-order filters

```matlab
% Convert to second-order sections
[sos, g] = tf2sos(b, a);
% sos = matrix of SOS coefficients
% g = gain factor
```

---

**8.5 — Logarithms and dB (Decibels)**

**Decibel Definition:**
$$\text{Magnitude (dB)} = 20 \log_{10}|H(j\omega)|$$
$$\text{Power (dB)} = 10 \log_{10}|H(j\omega)|^2$$

**Common Values:**
- 0 dB = magnitude = 1 (no change)
- -3 dB = magnitude ≈ 0.707 (half power)
- -6 dB = magnitude = 0.5
- 20 dB = magnitude = 10
- 40 dB = magnitude = 100

**Why dB?**
- Logarithmic scale shows details for small and large values
- Intuitive for human perception (hearing, vision)
- Standard in filter specifications

**Inverse:**
$$|H(j\omega)| = 10^{\text{dB}/20}$$

---

## SUMMARY OF DETAILED PREREQUISITES

**Now you have complete explanations for:**
1. ✅ Continuous vs discrete signals and time/frequency domains
2. ✅ Analog filter types (Butterworth, Chebyshev-I/II, Elliptic)
3. ✅ Bilinear transformation and frequency warping
4. ✅ Frequency band transformations (LP→BP, LP→HP, LP→BS)
5. ✅ Frequency normalization (Hz, rad/s, rad/sample)
6. ✅ MATLAB filter design and analysis functions
7. ✅ IIR filter behavior (feedback, infinite response, phase)
8. ✅ Math skills (complex numbers, poles/zeros, logarithms)

**Next Steps:**
- If any topic feels unclear, re-read that section
- Try MATLAB examples alongside reading
- Move to **PART 1** for how these topics apply to Lab 10

---

## PART 1: THE 8 ESSENTIAL CONCEPTS

### 1. What is a Filter? (Basic Idea Only)

A **filter** is a system that:
- **Allows some frequencies to pass** through
- **Blocks other frequencies**

Think of it like a coffee filter — allows coffee (signal you want) but blocks grounds (noise you don't want).

**4 Basic Types:**

| Type | What it does |
|---|---|
| **Low-pass (LP)** | Keeps low frequencies, removes high frequencies |
| **High-pass (HP)** | Keeps high frequencies, removes low frequencies |
| **Band-pass (BP)** | Keeps a middle range of frequencies, removes rest |
| **Band-stop (BS)** | Removes a middle range, keeps low and high |

**Visual Example:**
```
Original signal: Has both low and high frequency components
        ↓
Low-pass filter
        ↓
Output: Only low frequencies remain
```

**That's it — no equations, just the concept.**

---

### 2. Analog vs Digital Filters

**Analog Filter:**
- Works with **continuous signals** (like sound waves in air)
- Uses components like resistors, capacitors
- Described with variable **t** (time)

**Digital Filter:**
- Works with **discrete samples** (numbers on a computer)
- Implemented in software/code
- Described with variable **n** (sample number: 0, 1, 2, 3...)

**Key Point:** This lab converts **analog filters → digital filters** so we can use them in computers.

---

### 3. What is an IIR Filter?

**IIR = Infinite Impulse Response**

Simple definition:
- Uses **feedback** (output depends on previous outputs)
- Current output = function of (current input + previous outputs)
- Response continues forever (infinite)
- Fast and efficient

**Simple Equation (Don't memorize, just understand):**
```
y[n] = input[n] + a₁×y[n-1] + a₂×y[n-2] + ...
         ↑                    ↑
    current input      feedback from past
```

**Why use it?**
- Needs fewer calculations than FIR filters
- Good for real-time applications
- But: Cannot have linear phase (doesn't preserve all phase information)

---

### 4. Passband and Stopband

These are **frequency ranges:**

**Passband:**
- Frequencies that the filter **allows to pass through**
- The filter should have **minimal loss** here
- Example: 800-2000 Hz in Lab Task 1

**Stopband:**
- Frequencies that the filter **blocks/removes**
- The filter should **heavily attenuate** here
- Example: Below 800 Hz or above 2000 Hz

**Two Important Terms:**

**Ripple (dB):**
- Small up-and-down variations in passband
- Should be **small** (less ripple = better passband)
- Example: 0.5 dB ripple means passband wiggles up/down by 0.5 dB
- **Lower is better**

**Attenuation (dB):**
- How much the stopband is suppressed
- Example: 30 dB attenuation = stopband is 1000× smaller than passband
- **Higher is better**

**Visual:**
```
Magnitude
    |
1   |  ← Passband (signal allowed)
    | /‾‾‾‾‾‾‾\ (with ripple)
    |/         \
0   |___________\_____ ← Stopband (signal blocked)
    |
    0Hz         Cutoff        Nyquist
```

---

### 5. What Are Cutoff Frequencies?

**Cutoff frequency = the boundary between passband and stopband**

Simple example:
- Low-pass filter with cutoff at 1000 Hz
- 0-1000 Hz: **Passband** (allowed)
- 1000+ Hz: **Stopband** (blocked)

**Different Forms:**

| Form | Example | Used When |
|---|---|---|
| **Hz (Hertz)** | 800 Hz | Real-world frequencies |
| **rad/s (rad/sec)** | 2π × 800 = 5027 rad/s | Analog filters |
| **rad/sample** | 0.2π rad/sample | Digital filters |

**Conversion:**
```
Ω (rad/sample) = 2π × f(Hz) / fs(Hz)

Example: f = 800 Hz, fs = 6000 Hz
Ω = 2π × 800 / 6000 = 0.838 rad/sample
```

---

### 6. What Does Filter Order Mean?

**Order = How sharp is the filter's transition?**

Simple rule:
- **Higher order** → **sharper transition** (faster drop in stopband)
- **Lower order** → **gentler transition** (slower drop in stopband)

**Visual:**
```
Higher Order (Order = 20):    Lower Order (Order = 2):
Magnitude                      Magnitude
   |                              |
 1 |____                        1 |____
   |    \                         |     \_
   |     \___                     |       \___
   |         \___                 |           \___
 0 |_____________\               0 |_______________\
   Frequency                       Frequency
```

**Why care?**
- Higher order = better stopband suppression, but more computation
- Lower order = simpler, but worse stopband suppression

**In Lab 10:** MATLAB calculates the order for you — you just need to know it exists.

---

### 7. Very Basic Understanding of Analog Prototype

**This is the KEY IDEA for the lab:**

**Process:**
1. **Design an analog low-pass filter** (using Butterworth, Chebyshev, or Elliptic)
2. **Convert to digital** using **bilinear transform**
3. **Apply frequency transformation** (LP → BP, LP → HP, etc.)

**Why this approach?**
- Analog filter design is well-studied (formulas, tables exist)
- Bilinear transform is a standard mathematical tool
- Easy to apply transformations

**That's it — no poles, no s-plane, no complex math needed for viva.**

---

### 8. Minimum MATLAB Skills Needed

You only need to **run** these commands (don't memorize how they work):

```matlab
% Design filters
[b,a] = butter(N, Wn);        % Butterworth filter
[b,a] = cheby1(N, Rp, Wn);    % Chebyshev Type-I
[b,a] = cheby2(N, Rs, Wn);    % Chebyshev Type-II
[b,a] = ellip(N, Rp, Rs, Wn); % Elliptic filter

% Find required filter order
[N, Wn] = buttord(Wp, Ws, Rp, Rs);   % for Butterworth
[N, Wn] = ellipord(Wp, Ws, Rp, Rs);  % for Elliptic

% Conversions
[bt,at] = bilinear(b, a, fs);        % Analog → Digital
[num,den] = lp2bp(b, a, wo, Bw);     % LP → BP transform

% Visualization
fvtool(b, a);                        % Plot filter response
freqz(b, a);                         % Frequency response
zplane(b, a);                        % Pole-zero plot
```

**You don't need to know internals — just plug in numbers and run.**

---

## PART 2: SUPER-SIMPLE SUMMARY (Memorize This)

Learn these 10 lines and you pass the viva:

1. **Analog → Digital conversion** uses **bilinear transform**
2. **Always start** with a **low-pass analog filter**
3. **Then convert** to BP/HP/BS using **frequency transformations**
4. **IIR filters** have **feedback** and **infinite impulse response**
5. **MATLAB** calculates filter order and coefficients (we don't calculate by hand)
6. **Passband** = frequencies the filter **allows**
7. **Stopband** = frequencies the filter **blocks**
8. **Ripple (dB)** = how uneven the passband is (lower is better)
9. **Attenuation (dB)** = how much we suppress stopband (higher is better)
10. **Higher filter order** = sharper transition, more computation

---

## PART 3: LAB TASKS EXPLAINED SIMPLY

### PRE-LAB TASK A: Butterworth Low-Pass Prototype

**What you do:**
1. You're given: ωp, ωs, Rp (dB), Rs (dB)
2. MATLAB calculates: Filter order N
3. MATLAB calculates: Cutoff frequency ωc
4. MATLAB generates: Transfer function H(s)

**Don't worry about:**
- The formulas for calculating N
- The math behind cutoff frequency
- The transfer function coefficients

**Just understand:**
- We're designing an **analog low-pass filter**
- It will be **converted to digital later**

**MATLAB commands:**
```matlab
[N, Wc] = buttord(wp, ws, Rp, Rs);     % Find order
[num, den] = butter(N, Wc, 's');       % Design filter
```

---

### LAB TASK 1: Chebyshev Type-II Band-Pass Filter

**Given:** Order=20, passband=800Hz, stopband=2000Hz, fs=6000Hz

**What you do:**
1. **Normalize frequencies** to Nyquist (divide by fs/2)
2. **Design Chebyshev Type-II analog LP** filter
3. **Convert to digital** using bilinear transform
4. **Apply LP → BP** transformation
5. **Plot** the response and verify it meets specs

**Simple Steps:**

```matlab
% Step 1: Normalize frequencies
Wn_pass = 800 / (6000/2);      % = 0.2667
Wn_stop = 2000 / (6000/2);     % = 0.6667

% Step 2: Design analog filter
[b_analog, a_analog] = cheby2(20, 60, Wn_pass);

% Step 3: Convert to digital
[b_digital, a_digital] = bilinear(b_analog, a_analog, 6000);

% Step 4: Transform LP → BP
[b_bp, a_bp] = lp2bp(b_digital, a_digital, center_freq, bandwidth);

% Step 5: Plot
fvtool(b_bp, a_bp, 'Fs', 6000);
```

**That's it — just run these commands.**

---

### LAB TASK 2: Digital Band-Pass IIR Filter (Custom Specs)

**Given:** 
- Passband: (N-5) to (N+5) MHz
- Stopband: (N-5.5) to (N+5.5) MHz
- Rp = 0.5 dB, Rs = 30 dB
- fs = 8N MHz
- N = from your registration number

**What you do:**

1. **Choose filter type:** Elliptic (sharpest transition)
2. **Normalize frequencies** using Ω = 2πf/fs
3. **Calculate filter order** using ellipord()
4. **Design filter** using ellip()
5. **Plot** response and verify

**Why Elliptic?**
- Tight specification (±0.5 dB ripple, 30 dB attenuation)
- Need low order
- Elliptic gives sharpest transition with lowest order

**Simple Code:**
```matlab
% Step 1: Normalized frequencies
Wp = [2*pi*fp_low/(2*fs), 2*pi*fp_high/(2*fs)];
Ws = [2*pi*fs_low/(2*fs), 2*pi*fs_high/(2*fs)];

% Step 2: Find order
[N, Wn] = ellipord(Wp, Ws, 0.5, 30);

% Step 3: Design
[b, a] = ellip(N, 0.5, 30, Wn);

% Step 4: Plot
fvtool(b, a);
zplane(b, a);
```

---

## PART 4: WHAT THE PROFESSOR WILL ASK IN VIVA

### Easy Questions (Definitely Will Ask)

**Q1: What is the difference between analog and digital filters?**  
A: Analog works on continuous signals (t), digital works on discrete samples (n). This lab converts analog to digital.

**Q2: What is IIR filter?**  
A: A filter with feedback that has infinite impulse response. Output depends on current input + previous outputs.

**Q3: What is passband and stopband?**  
A: Passband is frequencies the filter allows. Stopband is frequencies the filter blocks.

**Q4: What does bilinear transform do?**  
A: Converts an analog filter to a digital filter using the formula: s = 2*fs*(z-1)/(z+1)

**Q5: Why start with low-pass analog filter?**  
A: Because design formulas and tables exist for low-pass. Other types (BP, HP, BS) are obtained by transformation.

**Q6: What is filter order?**  
A: Higher order = sharper transition. Lower order = gentler transition.

### Medium Questions

**Q7: What is frequency normalization? Why do we do it?**  
A: Converting frequencies to a standard range (0 to π or 0 to 1) so MATLAB functions work correctly.

**Q8: What does ripple mean?**  
A: Small up-down variations in passband. Lower ripple = better passband flatness. Measured in dB.

**Q9: What does attenuation mean?**  
A: How much we suppress the stopband. Higher attenuation = better stopband suppression. Measured in dB.

**Q10: Why use Elliptic for Task 2?**  
A: Because it has sharpest transition and lowest order for given ripple/attenuation specs. Meets tight requirements.

### Harder Questions

**Q11: Explain the process: Analog → Digital → BP**  
A: (1) Design analog LP, (2) Convert to digital using bilinear, (3) Transform digital LP to BP using lp2bp().

**Q12: What is Chebyshev Type-II?**  
A: Filter with ripple in stopband (not passband). Good when you want flat passband but can accept stopband ripple.

**Q13: What is the advantage of IIR over FIR?**  
A: IIR is faster and uses fewer coefficients. Disadvantage: Cannot have linear phase.

---

## PART 5: KEY FORMULAS YOU MUST KNOW FOR VIVA

You don't need to derive or calculate — just know they exist:

**1. Frequency Normalization:**
$$\Omega = \frac{2\pi f}{f_s}$$
(Converts Hz to rad/sample)

**2. Bilinear Transform:**
$$s = \frac{2f_s(z-1)}{z+1}$$
(Maps analog s-plane to digital z-plane)

**3. Butterworth Order Formula:**
$$N \geq \frac{\log_{10}\left(\frac{10^{Rs/10}-1}{10^{Rp/10}-1}\right)}{2\log_{10}(\omega_s/\omega_p)}$$
(Calculates minimum order for Butterworth)

**IMPORTANT:** You don't calculate these by hand — MATLAB does it. Just know what they represent.

---

## PART 6: QUICK REFERENCE TABLE FOR VIVA

| Concept | Definition | Used For |
|---|---|---|
| **Passband** | Frequencies filter allows | Specification |
| **Stopband** | Frequencies filter blocks | Specification |
| **Ripple (dB)** | Passband variation | Spec constraint |
| **Attenuation (dB)** | Stopband suppression | Spec constraint |
| **Bilinear Transform** | Analog → Digital | Conversion |
| **lp2bp()** | LP → BP transformation | Getting BP from LP |
| **Butterworth** | Flat passband | Simple design |
| **Chebyshev-I** | Ripple in passband | Sharper than Butterworth |
| **Chebyshev-II** | Ripple in stopband | Flat passband |
| **Elliptic** | Ripple both sides | Sharpest, lowest order |
| **Filter Order** | Sharpness of transition | Design parameter |
| **fvtool()** | Plot filter response | Verification |

---

## PART 7: WHAT NOT TO WORRY ABOUT

❌ **Don't memorize:**
- Pole-zero locations
- Exact transfer function coefficients
- Complex mathematical derivations
- s-plane vs z-plane mappings (just know they exist)
- How MATLAB calculates filter internally

✅ **Do understand:**
- What each function does
- Why we choose a particular filter type
- How to interpret magnitude/phase plots
- What passband/stopband/ripple/attenuation mean

---

## PART 8: LAST-MINUTE CHECKLIST FOR VIVA

Print this and check before your viva:

```
☐ I know what a filter does (allows some frequencies, blocks others)
☐ I understand passband = frequencies allowed, stopband = frequencies blocked
☐ I know IIR has feedback and infinite impulse response
☐ I understand Analog ≠ Digital (continuous vs discrete)
☐ I know bilinear transform converts Analog → Digital
☐ I know we start with LP filter then transform to BP/HP/BS
☐ I understand higher order = sharper transition
☐ I know Elliptic has sharpest transition (lowest order)
☐ I can explain why we use Chebyshev Type-II in Task 1
☐ I understand ripple (dB) and attenuation (dB) are specs
☐ I can identify the 4 MATLAB function types (design, order, transform, plot)
☐ I know fvtool() shows magnitude and phase response
☐ I understand zplane() shows poles and zeros
☐ I know I don't need to calculate by hand — MATLAB does it
☐ I can explain Approach 1: Analog → Analog → Digital
```

If you can check all these boxes, you're ready for the viva!

---

## PART 9: EXAMPLE VIVA ANSWER (How to Answer Simply)

**Viva Q:** Explain Lab Task 1 in one sentence.

**Good Answer:** "We design a Chebyshev Type-II analog low-pass filter, convert it to digital using bilinear transform, then apply LP-to-BP transformation to get a band-pass filter that passes 800-2000 Hz."

**Don't say:** (Complex math about s-plane, z-plane, contour integration, etc.)

---

**That's everything you need. Good luck with your viva!** 🎯

