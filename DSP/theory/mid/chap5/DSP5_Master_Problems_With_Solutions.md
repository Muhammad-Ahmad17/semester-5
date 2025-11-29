# DSP Chapter 5: Master Problems — Complete Solutions

## Table of Contents
1. [Basic Frequency Response Problems](#basic-frequency-response-problems)
2. [Magnitude and Phase Analysis](#magnitude-and-phase-analysis)
3. [Filter Type Identification](#filter-type-identification)
4. [System Function and Poles/Zeros](#system-function-and-poleszeros)
5. [Filtering of Periodic Signals](#filtering-of-periodic-signals)
6. [Inverse Systems and Equalization](#inverse-systems-and-equalization)
7. [Advanced Problems](#advanced-problems)

---

## Basic Frequency Response Problems

### Problem 1.1: Simple FIR Filter

**Given:** $h(n) = \{1, 2, 1\}$

**Find:** $H(\omega)$ and evaluate at $\omega = 0, \pi/4, \pi$

**Solution:**

$$H(\omega) = \sum_{n=-\infty}^{\infty} h(n) e^{-j\omega n} = 1 + 2e^{-j\omega} + e^{-2j\omega}$$

Factor out $e^{-j\omega}$:
$$H(\omega) = e^{-j\omega}[e^{j\omega} + 2 + e^{-j\omega}] = e^{-j\omega}[2 + 2\cos\omega]$$

**Magnitude and Phase:**
$$|H(\omega)| = 2|1 + \cos\omega|$$
$$\angle H(\omega) = -\omega \text{ (when } 1 + \cos\omega > 0\text{)}$$

**Evaluate:**

At $\omega = 0$:
$$H(0) = 1 + 2 + 1 = 4$$
$$|H(0)| = 4, \quad \angle H(0) = 0$$

At $\omega = \pi/4$:
$$H(\pi/4) = 1 + 2e^{-j\pi/4} + e^{-j\pi/2}$$
$$= 1 + 2(\cos\pi/4 - j\sin\pi/4) + (-j)$$
$$= 1 + 2 \cdot 0.707 - j(2 \cdot 0.707 + 1)$$
$$= 2.414 - j2.414$$
$$|H(\pi/4)| = \sqrt{(2.414)^2 + (2.414)^2} = 3.41$$
$$\angle H(\pi/4) = -\arctan(2.414/2.414) = -\pi/4$$

At $\omega = \pi$:
$$H(\pi) = 1 + 2e^{-j\pi} + e^{-j2\pi} = 1 + 2(-1) + 1 = 0$$
$$|H(\pi)| = 0$$

**Answer:** Low-pass filter (passes DC, blocks highest frequency)

---

### Problem 1.2: Exponential Decay System

**Given:** $h(n) = (0.8)^n u(n)$ (causal exponential)

**Find:** $H(\omega)$

**Solution:**

$$H(\omega) = \sum_{n=0}^{\infty} 0.8^n e^{-j\omega n} = \sum_{n=0}^{\infty} (0.8 e^{-j\omega})^n$$

This is geometric series with ratio $r = 0.8 e^{-j\omega}$:
$$H(\omega) = \frac{1}{1 - 0.8e^{-j\omega}}$$

**Magnitude:**
$$|H(\omega)| = \frac{1}{|1 - 0.8e^{-j\omega}|} = \frac{1}{\sqrt{(1-0.8\cos\omega)^2 + (0.8\sin\omega)^2}}$$

$$= \frac{1}{\sqrt{1 - 1.6\cos\omega + 0.64\cos^2\omega + 0.64\sin^2\omega}}$$

$$= \frac{1}{\sqrt{1.64 - 1.6\cos\omega}}$$

**At specific frequencies:**

$\omega = 0$: $|H(0)| = \frac{1}{\sqrt{1.64-1.6}} = \frac{1}{\sqrt{0.04}} = 5$

$\omega = \pi$: $|H(\pi)| = \frac{1}{\sqrt{1.64+1.6}} = \frac{1}{\sqrt{3.24}} = 0.556$

**Answer:** Low-pass filter characteristic (higher gain at low frequency)

---

### Problem 1.3: Moving Average of Different Lengths

**Given:** $h(n) = \frac{1}{M}[1, 1, ..., 1]$ (M ones)

**Find:** $H(\omega)$ and explain the effect

**Solution:**

$$H(\omega) = \frac{1}{M} \sum_{n=0}^{M-1} e^{-j\omega n} = \frac{1}{M} \frac{1 - e^{-j\omega M}}{1 - e^{-j\omega}}$$

Using Euler's formula:
$$H(\omega) = \frac{1}{M} \frac{\sin(\omega M/2)}{\sin(\omega/2)} e^{-j\omega(M-1)/2}$$

**Magnitude:**
$$|H(\omega)| = \frac{1}{M}\left|\frac{\sin(\omega M/2)}{\sin(\omega/2)}\right|$$

**Key property:** This has zeros at $\omega = \frac{2\pi k}{M}$ for $k = 1, 2, ..., M-1$

**For M=3 (example given in lecture):**
$$H(\omega) = \frac{1}{3}\frac{\sin(3\omega/2)}{\sin(\omega/2)}$$

Zeros at: $\omega = \frac{2\pi}{3}, \frac{4\pi}{3}$ (equiv. to $\omega = -\frac{2\pi}{3}$)

**For different M:**
- M=2: First zero at $\omega = \pi$
- M=3: First zero at $\omega = 2\pi/3$
- M=10: Zeros more densely packed
- Larger M → narrower mainlobe → more selective low-pass filter

---

## Magnitude and Phase Analysis

### Problem 2.1: Extract Magnitude and Phase

**Given:** $H(\omega) = \frac{1 + 0.5e^{-j\omega}}{1 + 0.5e^{j\omega}}$

**Find:** Magnitude and phase (simplify first!)

**Solution:**

**Step 1: Simplify by multiplying numerator and denominator by $e^{j\omega/2}$:**

$$H(\omega) = \frac{e^{j\omega/2}(1 + 0.5e^{-j\omega})}{e^{j\omega/2}(1 + 0.5e^{j\omega})} = \frac{e^{j\omega/2} + 0.5e^{-j\omega/2}}{e^{j\omega/2} + 0.5e^{j3\omega/2}}$$

Actually, let me use conjugate directly:

**Step 2: Direct approach**

$$H(\omega) = \frac{1 + 0.5e^{-j\omega}}{1 + 0.5e^{j\omega}} = \frac{1 + 0.5(\cos\omega - j\sin\omega)}{1 + 0.5(\cos\omega + j\sin\omega)}$$

$$= \frac{(1+0.5\cos\omega) - j(0.5\sin\omega)}{(1+0.5\cos\omega) + j(0.5\sin\omega)}$$

**Numerator:** Real part = $1+0.5\cos\omega$, Imag part = $-0.5\sin\omega$
**Denominator:** Real part = $1+0.5\cos\omega$, Imag part = $0.5\sin\omega$

**Magnitude:**
$$|H(\omega)| = \frac{\sqrt{(1+0.5\cos\omega)^2 + (0.5\sin\omega)^2}}{\sqrt{(1+0.5\cos\omega)^2 + (0.5\sin\omega)^2}} = 1$$

**Result:** Magnitude is 1 everywhere! (all-pass filter)

**Phase:**
$$\angle H(\omega) = \arctan\left(\frac{-0.5\sin\omega}{1+0.5\cos\omega}\right) - \arctan\left(\frac{0.5\sin\omega}{1+0.5\cos\omega}\right)$$

$$= -2\arctan\left(\frac{0.5\sin\omega}{1+0.5\cos\omega}\right)$$

**Answer:** All-pass filter with frequency-dependent phase shift

---

### Problem 2.2: Phase Delay vs Group Delay

**Given:** $H(\omega) = e^{-j3\omega}$ (pure delay of 3 samples)

**Find:** Phase and group delay

**Solution:**

**Phase:**
$$\angle H(\omega) = -3\omega$$

**Phase delay (group delay):**
$$\tau_g(\omega) = -\frac{d\angle H(\omega)}{d\omega} = -\frac{d(-3\omega)}{d\omega} = 3 \text{ samples}$$

**Answer:** Both indicate delay of 3 samples (constant delay for all frequencies)

---

### Problem 2.3: Calculate Phase at Multiple Frequencies

**Given:** $H(z) = \frac{1+z^{-1}}{1-0.5z^{-1}}$

**Find:** Phase $\angle H(\omega)$ at $\omega = 0, \pi/2, \pi$

**Solution:**

$$H(\omega) = \frac{1+e^{-j\omega}}{1-0.5e^{-j\omega}}$$

$$= \frac{(1+\cos\omega - j\sin\omega)}{(1-0.5\cos\omega + j0.5\sin\omega)}$$

**At $\omega = 0$:**
$$H(0) = \frac{1+1}{1-0.5} = 4 \quad (\text{real positive})$$
$$\angle H(0) = 0°$$

**At $\omega = \pi/2$:**
$$H(\pi/2) = \frac{1+j}{1+j0.5} = \frac{1+j}{1+0.5j}$$

Multiply by conjugate:
$$= \frac{(1+j)(1-0.5j)}{(1+0.5j)(1-0.5j)} = \frac{1 - 0.5j + j - 0.5j^2}{1 + 0.25}$$

$$= \frac{1 + 0.5j + 0.5}{1.25} = \frac{1.5 + 0.5j}{1.25} = 1.2 + 0.4j$$

$$\angle H(\pi/2) = \arctan(0.4/1.2) = \arctan(1/3) ≈ 18.4°$$

**At $\omega = \pi$:**
$$H(\pi) = \frac{1-1}{1+0.5} = 0$$

Phase is undefined (or can say 90° or -90°)

---

## Filter Type Identification

### Problem 3.1: Identify Filter Type from Magnitude Response

**Given:** Frequency response measurements:
- $|H(0)| = 1$
- $|H(\pi/4)| = 0.95$
- $|H(\pi/2)| = 0.7$
- $|H(3\pi/4)| = 0.1$
- $|H(\pi)| = 0.01$

**Classify the filter**

**Solution:**

Plot the response:
```
|H(ω)|
  1.0 │●
      │ ╲
 0.7  │  ╲●
      │    ╲
 0.1  │     ╲  ●
      │       ╲
0.01  │        ╲● 
      └──●──●─────●──●──────► ω
      0 π/4 π/2 3π/4 π
```

**Characteristics:**
- Passes low frequencies (ω near 0)
- Blocks high frequencies (ω near π)
- Smooth monotonic decrease

**Answer:** **LOW-PASS FILTER**

---

### Problem 3.2: Identify from Impulse Response

**Given:** $h(n) = \delta(n) - 0.5\delta(n-1)$

**Find:** Filter type

**Solution:**

$$H(\omega) = 1 - 0.5e^{-j\omega} = 1 - 0.5(\cos\omega - j\sin\omega)$$

$$= (1 - 0.5\cos\omega) + j(0.5\sin\omega)$$

**Magnitude:**
$$|H(\omega)| = \sqrt{(1-0.5\cos\omega)^2 + (0.5\sin\omega)^2}$$

$$= \sqrt{1 - \cos\omega + 0.25\cos^2\omega + 0.25\sin^2\omega}$$

$$= \sqrt{1.25 - \cos\omega}$$

**At key frequencies:**
- $\omega = 0$: $|H(0)| = \sqrt{1.25-1} = 0.5$
- $\omega = \pi/2$: $|H(\pi/2)| = \sqrt{1.25} = 1.118$
- $\omega = \pi$: $|H(\pi)| = \sqrt{1.25+1} = 1.5$

**Answer:** **HIGH-PASS FILTER** (blocks low frequencies, passes high)

---

### Problem 3.3: Band-Pass Filter Design Interpretation

**Given:** Magnitude response has:
- $|H(\omega)| = 0$ at $\omega = 0$
- $|H(\omega)| = \text{max}$ at $\omega = \pi/2$
- $|H(\omega)| = 0$ at $\omega = \pi$

**What type of filter?**

**Solution:**

The filter:
- Blocks DC ($\omega = 0$)
- Peaks at middle frequency ($\omega = \pi/2$)
- Blocks high frequency ($\omega = \pi$)

**Answer:** **BAND-PASS FILTER** centered at $\omega = \pi/2$

---

## System Function and Poles/Zeros

### Problem 4.1: From System Function to Difference Equation

**Given:** $H(z) = \frac{1 + 2z^{-1} + z^{-2}}{1 - 0.5z^{-1} + 0.1z^{-2}}$

**Find:** Difference equation

**Solution:**

**Step 1:** Cross multiply
$$Y(z)[1 - 0.5z^{-1} + 0.1z^{-2}] = X(z)[1 + 2z^{-1} + z^{-2}]$$

**Step 2:** Inverse z-transform
$$y(n) - 0.5y(n-1) + 0.1y(n-2) = x(n) + 2x(n-1) + x(n-2)$$

**Step 3:** Solve for y(n)
$$y(n) = x(n) + 2x(n-1) + x(n-2) + 0.5y(n-1) - 0.1y(n-2)$$

**Answer:** 
$$\boxed{y(n) = x(n) + 2x(n-1) + x(n-2) + 0.5y(n-1) - 0.1y(n-2)}$$

---

### Problem 4.2: Stability from Poles

**Given:** Poles at $z = 0.7 + 0.3j$ and $z = 0.7 - 0.3j$

**Is the system stable?**

**Solution:**

**Calculate magnitude of poles:**
$$|z| = \sqrt{(0.7)^2 + (0.3)^2} = \sqrt{0.49 + 0.09} = \sqrt{0.58} ≈ 0.762$$

**Stability criterion:** All poles must have $|z| < 1$

Since $0.762 < 1$, all poles are inside unit circle.

**Answer:** System is **STABLE** ✓

---

### Problem 4.3: Impulse Response from Poles/Zeros

**Given:** $H(z) = \frac{z}{z-0.5}$

**Find:** $h(n)$

**Solution:**

$$H(z) = \frac{z}{z-0.5} = \frac{1}{1-0.5z^{-1}}$$

**From standard form:** $\frac{1}{1-az^{-1}} \leftrightarrow a^n u(n)$

$$h(n) = (0.5)^n u(n) = \begin{cases} 0.5^n & n \geq 0 \\ 0 & n < 0 \end{cases}$$

**Verify:**
- $h(0) = 1$
- $h(1) = 0.5$
- $h(2) = 0.25$
- $h(3) = 0.125$
- ...

**Answer:** Exponential decay with rate 0.5

---

## Filtering of Periodic Signals

### Problem 5.1: Filtering a Sinusoid

**Given:**
- Input: $x(n) = 3\cos(\pi n/4)$
- Filter: $H(z) = \frac{1+z^{-1}}{1+0.5z^{-1}}$

**Find:** Output $y(n)$

**Solution:**

**Step 1:** Find frequency response at $\omega = \pi/4$

$$H(\pi/4) = \frac{1+e^{-j\pi/4}}{1+0.5e^{-j\pi/4}}$$

$$= \frac{1 + 0.707 - 0.707j}{1 + 0.354 - 0.354j}$$

$$= \frac{1.707 - 0.707j}{1.354 - 0.354j}$$

Multiply by conjugate:
$$= \frac{(1.707-0.707j)(1.354+0.354j)}{(1.354)^2 + (0.354)^2}$$

$$= \frac{2.311 + 0.604 - 0.957j + 0.240j}{1.834 + 0.125}$$

$$= \frac{2.915 - 0.717j}{1.959} = 1.487 - 0.366j$$

$$|H(\pi/4)| = \sqrt{1.487^2 + 0.366^2} = \sqrt{2.343} ≈ 1.53$$

$$\angle H(\pi/4) = \arctan(-0.366/1.487) ≈ -13.7°$$

**Step 2:** Apply to input

$$y(n) = |H(\pi/4)| \cdot 3 \cos(\pi n/4 + \angle H(\pi/4))$$

$$= 1.53 \times 3 \cos(\pi n/4 - 13.7°)$$

$$= 4.59 \cos(\pi n/4 - 13.7°)$$

**Answer:** 
$$\boxed{y(n) = 4.59 \cos(\pi n/4 - 13.7°)}$$

or in radians:
$$y(n) = 4.59 \cos(\pi n/4 - 0.239)$$

---

### Problem 5.2: Filtering a Multi-Component Signal

**Given:**
- Input: $x(n) = \cos(0.1\pi n) + 2\cos(0.5\pi n)$
- Filter: Low-pass with cutoff at $\omega = 0.3\pi$

**Find:** Output

**Solution:**

**Step 1:** Approximate magnitude response
- $\omega = 0.1\pi$ (low freq): $|H(0.1\pi)| ≈ 1$ (passes)
- $\omega = 0.5\pi$ (above cutoff): $|H(0.5\pi)| ≈ 0$ (blocked)

**Step 2:** Filter each component

Component 1 ($\omega = 0.1\pi$):
$$y_1(n) = 1 \cdot \cos(0.1\pi n) = \cos(0.1\pi n)$$

Component 2 ($\omega = 0.5\pi$):
$$y_2(n) = 0 \cdot 2\cos(0.5\pi n) = 0$$

**Step 3:** Total output
$$y(n) = \cos(0.1\pi n)$$

**Answer:** The filter **passes the low-frequency component and blocks the high-frequency component**

---

## Inverse Systems and Equalization

### Problem 6.1: Find Inverse System

**Given:** $H(z) = 1 + 0.5z^{-1}$

**Find:** Inverse system $H_{\text{inv}}(z)$ such that $H(z) \cdot H_{\text{inv}}(z) = 1$

**Solution:**

$$H_{\text{inv}}(z) = \frac{1}{H(z)} = \frac{1}{1 + 0.5z^{-1}}$$

**Check causality:**
$$H_{\text{inv}}(z) = \frac{z}{z + 0.5}$$

This has pole at $z = -0.5$, which is inside unit circle ✓

**Difference equation for inverse:**
$$H_{\text{inv}}(z) = \frac{1}{1+0.5z^{-1}}$$

$$Y(z)[1 + 0.5z^{-1}] = X(z)$$

$$y(n) + 0.5y(n-1) = x(n)$$

$$y(n) = x(n) - 0.5y(n-1)$$

**Answer:**
$$\boxed{H_{\text{inv}}(z) = \frac{1}{1+0.5z^{-1}}, \quad y(n) = x(n) - 0.5y(n-1)}$$

---

### Problem 6.2: Non-Invertible System

**Given:** $H(z) = (1 - z^{-1})^2 = 1 - 2z^{-1} + z^{-2}$

**Why can't we invert this?**

**Solution:**

**Method 1: Zeros at origin**
$$H(z) = \frac{(z-1)^2}{z^2}$$

Has zero at $z = 1$ (on unit circle - pole of inverse outside unit circle)

**Method 2: Check determinant**
Inverse would be:
$$H_{\text{inv}}(z) = \frac{1}{(1-z^{-1})^2}$$

Has pole at $z = 1$ (on unit circle - unstable!)

**Answer:** Cannot invert because **zero at z = 1 would cause poles outside unit circle, making inverse unstable**

---

## Advanced Problems

### Problem 7.1: Complete System Analysis

**Given:** Difference equation: $y(n) = 0.5x(n) + 0.5y(n-1)$

**Find:**
1. System function $H(z)$
2. Poles and zeros
3. Frequency response at $\omega = 0, \pi/2, \pi$
4. Filter classification
5. Impulse response
6. Is it stable?

**Solution:**

**1. System function:**
$$Y(z) = 0.5X(z) + 0.5z^{-1}Y(z)$$
$$Y(z)[1 - 0.5z^{-1}] = 0.5X(z)$$
$$H(z) = \frac{0.5}{1-0.5z^{-1}} = \frac{0.5z}{z-0.5}$$

**2. Poles and zeros:**
- Pole: $z = 0.5$
- Zero: $z = 0$ (at origin)

**3. Frequency response:**
$$H(\omega) = \frac{0.5e^{j\omega}}{e^{j\omega}-0.5}$$

At $\omega = 0$: $H(0) = \frac{0.5}{1-0.5} = 1$

At $\omega = \pi/2$: $|H(\pi/2)| = \frac{0.5}{|j-0.5|} = \frac{0.5}{\sqrt{0.25+1}} = 0.447$

At $\omega = \pi$: $H(\pi) = \frac{0.5(-1)}{-1-0.5} = \frac{-0.5}{-1.5} = 0.333$

**4. Filter type:** LOW-PASS

**5. Impulse response:**
$$h(n) = 0.5 \cdot (0.5)^n u(n)$$

**6. Stability:** Pole at 0.5 is inside unit circle ✓ STABLE

---

### Problem 7.2: Chebyshev vs Butterworth Conceptual

**Given:** Two filters
- Filter A: Butterworth, smooth response
- Filter B: Chebyshev, ripple in passband but sharper cutoff

**Compare for filtering a noisy signal**

**Solution:**

**For general filtering:**
- Butterworth better (no passband distortion)
- Smoother transition

**For where passband ripple acceptable:**
- Chebyshev better (sharper frequency separation)
- More selective

**For this course:** Understanding magnitude/phase response is more important than filter type details

---

## Practice Problems (With Hints)

### P1: Three-Point Centered Average
$h(n) = \frac{1}{3}[\delta(n-1) + \delta(n) + \delta(n+1)]$

Find $H(\omega)$ and explain why this is better than one-sided average.

**Hint:** Factor out the phase term

---

### P2: First-Order System
$y(n) = 0.9y(n-1) + 0.1x(n)$

Find $|H(\omega)|$ at DC and Nyquist frequency.

**Hint:** This is a low-pass filter

---

### P3: Differencing Filter
$y(n) = x(n) - x(n-1)$

Find $H(\omega)$ and classify the filter.

**Hint:** This removes DC and low frequencies

---

### P4: Cascaded Filters
If $H_1(\omega)$ and $H_2(\omega)$ are cascaded, what is $H_{\text{total}}(\omega)$?

**Hint:** $H_{\text{total}} = H_1 \cdot H_2$

---

**With these problems and solutions, you now have the complete toolkit for DSP Chapter 5. Practice by solving similar problems, and you'll master frequency domain analysis!**
