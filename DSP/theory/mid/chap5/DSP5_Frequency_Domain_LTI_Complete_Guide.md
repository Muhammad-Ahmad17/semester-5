# DSP Chapter 5: Frequency Domain & LTI Systems — Complete Learning Guide

## Table of Contents
1. [What is an LTI System?](#what-is-an-lti-system)
2. [Time Domain vs Frequency Domain](#time-domain-vs-frequency-domain)
3. [The Frequency Response H(ω)](#the-frequency-response-hω)
4. [DTFT and Magnitude-Phase Form](#dtft-and-magnitude-phase-form)
5. [Computing Frequency Response](#computing-frequency-response)
6. [Real-Valued Impulse Response Properties](#real-valued-impulse-response-properties)
7. [Filtering in Frequency Domain](#filtering-in-frequency-domain)
8. [System Function and Poles/Zeros](#system-function-and-poleszeros)
9. [Complete Worked Examples](#complete-worked-examples)
10. [Problem-Solving Strategy](#problem-solving-strategy)

---

## What is an LTI System?

### Understanding LTI

**LTI = Linear Time-Invariant**

Two key properties:

#### 1. **Linear**
If input $x_1(n)$ produces output $y_1(n)$ and input $x_2(n)$ produces output $y_2(n)$, then:
$$ax_1(n) + bx_2(n) \rightarrow ay_1(n) + by_2(n)$$

**Example:**
```
System: y(n) = 2x(n)

If:  x1(n) = [1, 2, 3]  → y1(n) = [2, 4, 6]
And: x2(n) = [4, 5, 6]  → y2(n) = [8, 10, 12]

Then: 3x1(n) + x2(n) = 3[1,2,3] + [4,5,6] = [7, 11, 15]
      → y(n) = [14, 22, 30] = 3y1(n) + y2(n) ✓
```

**NOT linear example:**
- $y(n) = x^2(n)$ (nonlinear)
- $y(n) = x(n) + 5$ (has offset)

#### 2. **Time-Invariant**
If input $x(n)$ produces output $y(n)$, then input $x(n - n_0)$ produces output $y(n - n_0)$.

**Example:**
```
System: y(n) = 2x(n)

If:  x(n) = [1, 2, 3, 4, 5]  → y(n) = [2, 4, 6, 8, 10]

Then: x(n-2) = [_, _, 1, 2, 3, 4, 5] (shifted)
      → y(n) = [_, _, 2, 4, 6, 8, 10] (same shift) ✓
```

**NOT time-invariant example:**
- $y(n) = n \cdot x(n)$ (coefficient depends on time)

### Why Care About LTI Systems?

**Advantages:**
1. Can be fully described by **single response** (impulse response $h(n)$)
2. Output can be computed by **convolution** with input
3. Can be analyzed in **frequency domain**
4. Easy to predict and control

**Real examples:**
- Audio filters (equalizer in your phone)
- Image blur/sharpen
- Communication channels
- Control systems

### Two Descriptions of LTI Systems

Every LTI system can be described in two ways:

#### Time Domain
**Impulse response:** $h(n)$ = output when input is $\delta(n)$ (unit impulse)

Output computed by convolution:
$$y(n) = x(n) * h(n) = \sum_{k=-\infty}^{\infty} x(k) h(n-k)$$

**Advantage:** Direct physical interpretation

#### Frequency Domain
**Frequency response:** $H(\omega)$ = DTFT of $h(n)$

Output computed by multiplication:
$$Y(e^{j\omega}) = X(e^{j\omega}) \cdot H(e^{j\omega})$$

**Advantage:** Easier to understand filtering effects

---

## Time Domain vs Frequency Domain

### The Duality

```
TIME DOMAIN                  FREQUENCY DOMAIN
───────────────             ───────────────

h(n) ←────DTFT────→ H(ω)
x(n) ←────DTFT────→ X(ω)
y(n) ←────DTFT────→ Y(ω)

Convolution in time = Multiplication in frequency

y(n) = x(n) * h(n) ←──→ Y(ω) = X(ω) · H(ω)

INVERSE DTFT
←──────────

Why this matters:
  Convolution is hard (many operations)
  Multiplication is easy (one operation per frequency)
```

### Conceptual Difference

**Time domain:** "What happens step by step in time"
- Shows exact values at each time instant
- Good for simulation
- Hard to see overall effects

**Frequency domain:** "What frequencies are amplified/reduced"
- Shows which frequencies pass through system
- Good for understanding filtering
- Natural for frequency analysis

---

## The Frequency Response H(ω)

### Definition

**Frequency response** $H(\omega)$ describes how the system responds to different input frequencies.

$$H(\omega) = \text{DTFT}\{h(n)\}$$

### Physical Interpretation

If you apply a **cosine input** at frequency $\omega_0$:
$$x(n) = \cos(\omega_0 n)$$

The output is **cosine at same frequency, but scaled and phase-shifted**:
$$y(n) = |H(\omega_0)| \cos(\omega_0 n + \angle H(\omega_0))$$

**Three pieces of information:**
1. **Magnitude** $|H(\omega)|$: How much is the signal amplified/attenuated
2. **Phase** $\angle H(\omega)$: Time shift applied to signal
3. **Frequency** $\omega$: Measured in rad/sample

### Example: Simple Averaging Filter

**Impulse response:** $h(n) = \frac{1}{3}[\delta(n+1) + \delta(n) + \delta(n-1)]$

This averages three consecutive samples.

**Frequency response:**
$$H(\omega) = \frac{1}{3}[e^{j\omega} + 1 + e^{-j\omega}] = \frac{1}{3}[1 + 2\cos(\omega)]$$

**Magnitude:**
$$|H(\omega)| = \frac{1}{3}|1 + 2\cos(\omega)|$$

**Let's evaluate at specific frequencies:**

At $\omega = 0$ (DC, very low frequency):
$$|H(0)| = \frac{1}{3}(1 + 2 \cdot 1) = 1 \quad \text{✓ Passed unchanged}$$

At $\omega = \pi/3$:
$$|H(\pi/3)| = \frac{1}{3}(1 + 2\cos(\pi/3)) = \frac{1}{3}(1 + 2 \cdot 0.5) = \frac{2}{3}$$

At $\omega = \pi$ (highest frequency in discrete domain):
$$|H(\pi)| = \frac{1}{3}(1 + 2 \cdot (-1)) = 0 \quad \text{✗ Completely blocked}$$

**Interpretation:**
- Low frequencies pass (averaging smooths)
- High frequencies blocked (averaging removes rapid changes)
- This is a **low-pass filter**!

```
|H(ω)|
  1.0 │  ___
      │ /   \
 0.67 │/     \___
      │          \___
  0   └─────────────────► ω
      0    π/3   π      (rad/sample)
```

---

## DTFT and Magnitude-Phase Form

### General DTFT Form

The DTFT of a real signal can always be written as:
$$H(\omega) = |H(\omega)| e^{j\angle H(\omega)}$$

**Two representations:**

#### 1. Cartesian Form
$$H(\omega) = \text{Re}\{H(\omega)\} + j \cdot \text{Im}\{H(\omega)\}$$

**Example:**
$$H(\omega) = \frac{1}{3}(1 + 2\cos\omega) + j \cdot 0$$

(Imaginary part is zero for this symmetric filter)

#### 2. Magnitude-Phase Form
$$H(\omega) = |H(\omega)| e^{j\phi(\omega)}$$

where:
- $|H(\omega)| = \sqrt{[\text{Re}]^2 + [\text{Im}]^2}$ = magnitude
- $\phi(\omega) = \arctan\left(\frac{\text{Im}}{\text{Re}}\right)$ = phase

**Example (averaging filter):**
$$|H(\omega)| = \left|\frac{1}{3}(1 + 2\cos\omega)\right|$$
$$\phi(\omega) = \begin{cases} 0 & \text{if } 1 + 2\cos\omega \geq 0 \\ \pi & \text{if } 1 + 2\cos\omega < 0 \end{cases}$$

### Phase and Delay

**Phase shift indicates time delay:**
$$\phi(\omega) = -\omega \cdot n_d \quad \Rightarrow \quad n_d = -\frac{\phi(\omega)}{\omega}$$

where $n_d$ is the delay in samples.

**Example:** If $\phi(\omega) = -2\omega$, then delay = 2 samples

**Key insight:** Different frequencies can have different delays!

---

## Computing Frequency Response

### Method 1: From Difference Equation

**Given:** Difference equation
$$y(n) = a_0 x(n) + a_1 x(n-1) + ... + b_1 y(n-1) + b_2 y(n-2) + ...$$

**Step 1:** Replace $x(n) \to X(\omega)$, $y(n) \to Y(\omega)$, $n \to e^{j\omega n}$

**Step 2:** Solve for $H(\omega) = Y(\omega)/X(\omega)$

**Example:**
$$y(n) = \frac{1}{3}[x(n+1) + x(n) + x(n-1)]$$

Multiply by $e^{-j\omega n}$ and sum:
$$Y(\omega) = \frac{1}{3}X(\omega)[e^{j\omega} + 1 + e^{-j\omega}]$$

$$H(\omega) = \frac{Y(\omega)}{X(\omega)} = \frac{1}{3}[e^{j\omega} + 1 + e^{-j\omega}] = \frac{1}{3}[1 + 2\cos\omega]$$

### Method 2: From System Function H(z)

**Given:** System function (z-transform)
$$H(z) = \frac{b_0 + b_1 z^{-1} + b_2 z^{-2} + ...}{1 + a_1 z^{-1} + a_2 z^{-2} + ...}$$

**To get frequency response:** Substitute $z = e^{j\omega}$

$$H(\omega) = H(z)|_{z=e^{j\omega}} = \frac{b_0 + b_1 e^{-j\omega} + b_2 e^{-2j\omega} + ...}{1 + a_1 e^{-j\omega} + a_2 e^{-2j\omega} + ...}$$

**Why this works:**
- Unit circle in z-plane ($|z| = 1$) corresponds to frequencies
- Evaluating $H(z)$ on unit circle gives frequency response

### Method 3: From Impulse Response h(n)

**Direct DTFT:**
$$H(\omega) = \sum_{n=-\infty}^{\infty} h(n) e^{-j\omega n}$$

For finite impulse response (FIR):
$$H(\omega) = h(0) + h(1)e^{-j\omega} + h(2)e^{-2j\omega} + ...$$

**Example:** $h(n) = \{1, 2, 3\}$ (length 3)
$$H(\omega) = 1 + 2e^{-j\omega} + 3e^{-2j\omega}$$

At $\omega = 0$:
$$H(0) = 1 + 2 + 3 = 6$$

At $\omega = \pi/2$:
$$H(\pi/2) = 1 + 2e^{-j\pi/2} + 3e^{-j\pi} = 1 + 2(-j) + 3(-1) = -2 - 2j$$
$$|H(\pi/2)| = \sqrt{4+4} = 2\sqrt{2}$$

---

## Real-Valued Impulse Response Properties

### Key Symmetry Properties

If impulse response $h(n)$ is **real-valued**, the frequency response has special properties:

**Property 1: Conjugate Symmetry**
$$H(\omega) = H^*(-\omega)$$

In other words:
$$|H(\omega)| = |H(-\omega)| \quad \text{(magnitude is even)}$$
$$\angle H(\omega) = -\angle H(-\omega) \quad \text{(phase is odd)}$$

**Property 2: Only need to know $H(\omega)$ for $0 \leq \omega \leq \pi$**

Negative frequencies mirror positive frequencies.

**Property 3: For real h(n), imaginary part is zero or simple**

$$\text{Im}\{H(\omega)\} = -\sum_{n=1}^{\infty} h(n) \sin(\omega n)$$
$$\text{Re}\{H(\omega)\} = h(0) + \sum_{n=1}^{\infty} 2h(n) \cos(\omega n)$$

### Visualization

```
Magnitude response |H(ω)| for real h(n):

|H(ω)|
  ▲
  │     Symmetric around ω = 0
  │   ╱╲              ╱╲
  │  ╱  ╲────────────╱  ╲
  │ ╱                    ╲
  └──────┼─────────┼─────────► ω
       -π        0         π
        │  Mirror │  Mirror │
        │←─ image ─│─image ─→│
        
Phase response ∠H(ω):

∠H(ω)
  ▲
  │       Odd symmetry around ω = 0
  │  ╱
  │ ╱ ────────
  │╱
  ├─────────────────────────► ω
  │\
  │ ╲ ────────
  │  ╲
  │   ╲
      -π        0         π
```

---

## Filtering in Frequency Domain

### Basic Filtering Operation

**Concept:** Multiply input spectrum by filter response

$$Y(\omega) = X(\omega) \cdot H(\omega)$$

**In terms of Fourier series (if periodic signal):**
$$|Y_k| = |X_k| \cdot |H(k\omega_0)|$$
$$\angle Y_k = \angle X_k + \angle H(k\omega_0)$$

### Example: Filtering Periodic Signal

**Input:** Periodic signal with Fourier series components
$$x(n) = A_1 \cos(\omega_1 n) + A_2 \cos(\omega_2 n)$$

**Filter:** Frequency response $H(\omega)$

**Output at frequency $\omega_1$:**
$$y_1(n) = |H(\omega_1)| A_1 \cos(\omega_1 n + \angle H(\omega_1))$$

**Output at frequency $\omega_2$:**
$$y_2(n) = |H(\omega_2)| A_2 \cos(\omega_2 n + \angle H(\omega_2))$$

**Total output:**
$$y(n) = y_1(n) + y_2(n)$$

### Practical Filtering Types

#### Low-Pass Filter
Passes low frequencies, blocks high frequencies
$$|H(\omega)| = \begin{cases} 1 & \omega \text{ small} \\ 0 & \omega \text{ large} \end{cases}$$

**Application:** Noise removal, smoothing

#### High-Pass Filter
Blocks low frequencies, passes high frequencies
$$|H(\omega)| = \begin{cases} 0 & \omega \text{ small} \\ 1 & \omega \text{ large} \end{cases}$$

**Application:** Edge detection, removing DC

#### Band-Pass Filter
Passes middle-range frequencies
$$|H(\omega)| = \begin{cases} 0 & \omega < \omega_1 \\ 1 & \omega_1 \leq \omega \leq \omega_2 \\ 0 & \omega > \omega_2 \end{cases}$$

**Application:** Selecting one frequency component

#### Band-Stop (Notch) Filter
Blocks middle-range frequencies
$$|H(\omega)| = \begin{cases} 1 & \omega < \omega_1 \text{ or } \omega > \omega_2 \\ 0 & \omega_1 \leq \omega \leq \omega_2 \end{cases}$$

**Application:** Removing 50/60 Hz hum

---

## System Function and Poles/Zeros

### What is System Function H(z)?

The **z-transform** of impulse response $h(n)$:
$$H(z) = \sum_{n=-\infty}^{\infty} h(n) z^{-n}$$

For causal finite-order systems:
$$H(z) = \frac{b_0 + b_1 z^{-1} + ... + b_M z^{-M}}{1 + a_1 z^{-1} + ... + a_N z^{-N}}$$

### Poles and Zeros

**Zeros:** Values of $z$ where numerator = 0
**Poles:** Values of $z$ where denominator = 0

**Example:**
$$H(z) = \frac{z + 1}{(z - 0.5)(z - 0.8)}$$

- Zeros: $z = -1$
- Poles: $z = 0.5, 0.8$

### Pole-Zero Diagram

```
Pole-Zero plot (complex z-plane):

      Imaginary
        ▲
        │   ○ (zero)
      1 ├───●────────  (pole)
        │   │
        │   │
      0 ├───┼───●─────── Real
        │   │ 0.8
       -1 ├───────────
        
  ○ = zero
  ● = pole
  
Stability requirement: ALL POLES inside unit circle (|pole| < 1)
```

### Frequency Response from Poles/Zeros

**On unit circle** ($z = e^{j\omega}$), frequency response has:
- **Magnitude peaks** near poles
- **Magnitude zeros** at zero locations
- **Phase variation** determined by pole/zero distances

---

## Complete Worked Examples

### Example 1: Simple Moving Average Filter

**Problem:**
A filter has impulse response: $h(n) = \frac{1}{3}[\delta(n) + \delta(n-1) + \delta(n-2)]$

Find:
1. Frequency response $H(\omega)$
2. Magnitude at $\omega = 0, \pi/2, \pi$
3. Filter type (LP/HP/BP/BS)

**Solution:**

**Step 1: Frequency response from impulse response**
$$H(\omega) = \sum_{n=0}^{2} \frac{1}{3} e^{-j\omega n} = \frac{1}{3}[1 + e^{-j\omega} + e^{-2j\omega}]$$

Factor out $e^{-j\omega}$:
$$H(\omega) = \frac{1}{3} e^{-j\omega}[e^{j\omega} + 1 + e^{-j\omega}] = \frac{1}{3} e^{-j\omega}[1 + 2\cos\omega]$$

**Step 2: Separate magnitude and phase**

Magnitude:
$$|H(\omega)| = \frac{1}{3}|1 + 2\cos\omega|$$

Phase:
$$\angle H(\omega) = -\omega + \begin{cases} 0 & \text{if } 1+2\cos\omega \geq 0 \\ \pi & \text{if } 1+2\cos\omega < 0 \end{cases}$$

**Step 3: Evaluate at specific frequencies**

At $\omega = 0$:
$$|H(0)| = \frac{1}{3}(1 + 2 \cdot 1) = 1$$

At $\omega = \pi/2$:
$$|H(\pi/2)| = \frac{1}{3}(1 + 2 \cdot 0) = \frac{1}{3}$$

At $\omega = \pi$:
$$|H(\pi)| = \frac{1}{3}|1 + 2(-1)| = 0$$

**Step 4: Determine filter type**

Magnitude response:
- High at low frequencies ($\omega = 0$): $|H(0)| = 1$
- Low at high frequencies ($\omega = \pi$): $|H(\pi)| = 0$
- This is a **LOW-PASS filter** ✓

**Visualization:**
```
|H(ω)| vs ω:

|H(ω)|
  1.0 │  ●
      │ /╲
 1/3  │● ╲ ●
      │ \ ╲ \
  0   │  ╲ ╲ ●
      └───────────► ω
      0  π/2  π

Filter Effect:
  Input DC (ω=0): amplitude = 1 → PASSES ✓
  Input π rad/sample (ω=π): amplitude = 0 → BLOCKED ✗
```

---

### Example 2: Difference Equation to Frequency Response

**Problem:**
A digital filter is described by:
$$y(n) = x(n) - 0.5 y(n-1)$$

Find:
1. System function $H(z)$
2. Frequency response $H(\omega)$
3. Magnitude and phase at $\omega = 0$

**Solution:**

**Step 1: Take z-transform of difference equation**
$$Y(z) = X(z) - 0.5 z^{-1} Y(z)$$

Rearrange:
$$Y(z) + 0.5 z^{-1} Y(z) = X(z)$$
$$Y(z)[1 + 0.5 z^{-1}] = X(z)$$

**Step 2: Find system function**
$$H(z) = \frac{Y(z)}{X(z)} = \frac{1}{1 + 0.5 z^{-1}} = \frac{z}{z + 0.5}$$

**Step 3: Get frequency response (substitute $z = e^{j\omega}$)**
$$H(\omega) = \frac{1}{1 + 0.5 e^{-j\omega}}$$

**Step 4: Evaluate at $\omega = 0$**

At $\omega = 0$: $e^{-j\omega} = 1$
$$H(0) = \frac{1}{1 + 0.5} = \frac{1}{1.5} = \frac{2}{3}$$

Magnitude: $|H(0)| = \frac{2}{3} = 0.667$
Phase: $\angle H(0) = 0$ (real and positive)

**Step 5: Evaluate at $\omega = \pi$**

At $\omega = \pi$: $e^{-j\pi} = -1$
$$H(\pi) = \frac{1}{1 + 0.5(-1)} = \frac{1}{1 - 0.5} = \frac{1}{0.5} = 2$$

Magnitude: $|H(\pi)| = 2$
Phase: $\angle H(\pi) = 0$

**Interpretation:**
- DC (low frequency) is slightly attenuated (0.667)
- High frequency is amplified (2.0)
- This is a **HIGH-PASS filter** ✓

---

### Example 3: Finding System from Poles and Zeros

**Problem:**
A causal LTI system has:
- Zero at $z = -1$
- Pole at $z = 0.5$

Gain at DC is 4. Find:
1. System function $H(z)$
2. Difference equation
3. Impulse response $h(n)$

**Solution:**

**Step 1: Write system function**
$$H(z) = K \frac{z + 1}{z - 0.5}$$

where $K$ is gain constant.

**Step 2: Find K from DC gain condition**

At DC: $\omega = 0 \Rightarrow z = 1$
$$H(1) = K \frac{1 + 1}{1 - 0.5} = K \frac{2}{0.5} = 4K = 4$$

Therefore: $K = 1$

**Step 3: Final system function**
$$H(z) = \frac{z + 1}{z - 0.5} = \frac{z+1}{z-0.5}$$

Multiply top and bottom by $z^{-1}$:
$$H(z) = \frac{1 + z^{-1}}{1 - 0.5z^{-1}}$$

**Step 4: Find difference equation**

$$Y(z)[1 - 0.5z^{-1}] = X(z)[1 + z^{-1}]$$

Take inverse z-transform:
$$y(n) - 0.5 y(n-1) = x(n) + x(n-1)$$

**Rearrange:**
$$y(n) = x(n) + x(n-1) + 0.5 y(n-1)$$

**Step 5: Find impulse response**

Apply $x(n) = \delta(n)$:

$$y(n) = \delta(n) + \delta(n-1) + 0.5 y(n-1)$$

**Calculate term by term:**

$n = 0$: $y(0) = 1 + 0 + 0 = 1$
$n = 1$: $y(1) = 0 + 1 + 0.5(1) = 1.5$
$n = 2$: $y(2) = 0 + 0 + 0.5(1.5) = 0.75$
$n = 3$: $y(3) = 0 + 0 + 0.5(0.75) = 0.375$

**Pattern:** For $n \geq 2$: $y(n) = 0.5^{n-1}$

**Impulse response:**
$$h(n) = \begin{cases} 1 & n=0 \\ 1.5 & n=1 \\ 0.5^{n-1} & n \geq 2 \end{cases}$$

Or more compactly:
$$h(n) = \delta(n) + 1.5 \delta(n-1) + 0.5^{n-1} u(n-2)$$

---

## Problem-Solving Strategy

### Strategy for Any LTI System Problem

**STEP 1: Identify what you're given**
- Impulse response $h(n)$?
- Difference equation?
- System function $H(z)$?
- Magnitude/phase specs?
- Poles and zeros?

**STEP 2: Choose the right domain**
- **Time domain:** For convolution, impulse response calculations
- **Frequency domain:** For filtering, magnitude/phase analysis
- **z-domain:** For poles/zeros, stability, system description

**STEP 3: Apply the appropriate transformation**

| From | To | Use |
|------|---|---|
| $h(n)$ | $H(\omega)$ | $H(\omega) = \sum h(n)e^{-j\omega n}$ |
| $H(\omega)$ | $h(n)$ | IDTFT (table lookup or numerical) |
| Diff. eq. | $H(\omega)$ | Substitute $e^{j\omega}$ for exponentials |
| $H(z)$ | $H(\omega)$ | Substitute $z=e^{j\omega}$ |
| $H(\omega)$ | Magnitude/Phase | $\|H\| = \sqrt{\text{Re}^2 + \text{Im}^2}$, $\phi = \arctan(\text{Im}/\text{Re})$ |

**STEP 4: Evaluate at key frequencies**
- $\omega = 0$ (DC)
- $\omega = \pi/2$ (quarter Nyquist)
- $\omega = \pi$ (Nyquist, highest frequency)

**STEP 5: Interpret the result**
- Filter type (LP/HP/BP/BS)?
- Stability (poles inside unit circle)?
- System behavior (stable, causal, linear)?

### Common Problem Types & Solutions

#### Type 1: Given h(n), find H(ω)
**Solution:** Use DTFT formula directly
$$H(\omega) = \sum_{n=-\infty}^{\infty} h(n) e^{-j\omega n}$$

#### Type 2: Given difference equation, find frequency response
**Solution:** Replace each term with frequency domain equivalent
- $x(n) \to X(\omega)$
- $y(n) \to Y(\omega)$
- $e^{j\omega n}$ for exponentials

#### Type 3: Given poles/zeros, find H(z)
**Solution:** 
$$H(z) = K \frac{\prod(z - z_i)}{\prod(z - p_i)}$$
Use boundary condition to find $K$.

#### Type 4: Analyze filtering of periodic signal
**Solution:**
1. Find Fourier series of input
2. Apply $|Y_k| = |X_k| \cdot |H(k\omega_0)|$ for each component
3. Add filtered components

---

## Quick Reference: Key Formulas

### Frequency Response
$$H(\omega) = \text{DTFT}\{h(n)\} = \sum_{n=-\infty}^{\infty} h(n) e^{-j\omega n}$$

### Magnitude-Phase Decomposition
$$H(\omega) = |H(\omega)| e^{j\angle H(\omega)}$$

$$|H(\omega)| = \sqrt{[\text{Re}\{H\}]^2 + [\text{Im}\{H\}]^2}$$

$$\angle H(\omega) = \arctan\left(\frac{\text{Im}\{H\}}{\text{Re}\{H\}}\right)$$

### Filtering in Frequency Domain
$$Y(\omega) = X(\omega) \cdot H(\omega)$$

### System Function to Frequency Response
$$H(\omega) = H(z)|_{z=e^{j\omega}}$$

### Power Spectral Density
$$|H(\omega)|^2 = H(\omega) \cdot H^*(\omega)$$

### For Real h(n), Real Part
$$\text{Re}\{H(\omega)\} = h(0) + 2\sum_{n=1}^{\infty} h(n)\cos(\omega n)$$

### Group Delay (Phase Delay)
$$\tau_g(\omega) = -\frac{d\angle H(\omega)}{d\omega}$$

---

**Now you understand DSP Chapter 5! Practice solving problems using the strategy above, and you'll master frequency domain analysis.**
