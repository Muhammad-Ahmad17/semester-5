# DSP Chapter 5: Quick Reference & Exam Checklist

## One-Page Formulas

### Core Definitions

| Concept | Formula | Use |
|---------|---------|-----|
| **Frequency Response** | $H(\omega) = \text{DTFT}\{h(n)\}$ | Response at each frequency |
| **DTFT of h(n)** | $H(\omega) = \sum_{n=-\infty}^{\infty} h(n)e^{-j\omega n}$ | Calculate from impulse response |
| **Magnitude** | $\|H(\omega)\| = \sqrt{\text{Re}^2 + \text{Im}^2}$ | Amplification factor |
| **Phase** | $\angle H(\omega) = \arctan(\text{Im}/\text{Re})$ | Time delay indicator |
| **Magnitude-Phase** | $H(\omega) = \|H(\omega)\|e^{j\angle H(\omega)}$ | Polar form |
| **Output** | $Y(\omega) = X(\omega) \cdot H(\omega)$ | Filtering effect |
| **System Function** | $H(z) = \sum h(n)z^{-n}$ | z-transform of h(n) |
| **Freq. from z-domain** | $H(\omega) = H(z)\|_{z=e^{j\omega}}$ | Substitute z = e^jω |

### Key Properties for Real h(n)

| Property | Formula | Meaning |
|----------|---------|---------|
| **Conjugate Symmetry** | $H(\omega) = H^*(-\omega)$ | Magnitude even, phase odd |
| **Only need** | $0 \leq \omega \leq \pi$ | Negative freq. mirror positive |
| **Real Part** | $\text{Re} = h(0) + 2\sum_{n=1} h(n)\cos(\omega n)$ | Even symmetry |
| **Imaginary Part** | $\text{Im} = -\sum_{n=1} h(n)\sin(\omega n)$ | Odd symmetry |

### Standard Forms

#### Common Time-Domain Signals → Frequency Response

| h(n) | H(ω) |
|------|------|
| $\delta(n)$ | $1$ |
| $\delta(n-n_0)$ | $e^{-j\omega n_0}$ |
| $u(n)$ | $\frac{1}{1-e^{-j\omega}} = \frac{e^{-j\omega/2}}{2j\sin(\omega/2)}$ |
| $a^n u(n)$ | $\frac{1}{1-ae^{-j\omega}}$ |
| $(n+1)a^n u(n)$ | $\frac{1}{(1-ae^{-j\omega})^2}$ |
| $\cos(\omega_0 n)u(n)$ | $\frac{1-\cos(\omega_0)e^{-j\omega}}{1-2\cos(\omega_0)e^{-j\omega}+e^{-2j\omega}}$ |

#### Common z-Domain Forms → Difference Equation

| H(z) | Difference Equation |
|------|---------------------|
| $\frac{b_0}{1-az^{-1}}$ | $y(n) = ay(n-1) + b_0 x(n)$ |
| $\frac{1+z^{-1}}{1-az^{-1}}$ | $y(n) = ay(n-1) + x(n) + x(n-1)$ |
| $1 + az^{-1}$ | $y(n) = x(n) + ax(n-1)$ |
| $1 - az^{-1}$ | $y(n) = x(n) - ax(n-1)$ |

---

## Filter Classification Quick Table

| Filter Type | Magnitude at DC | Magnitude at Nyquist | Typical Use |
|------------|-----------------|-------------------|-----------|
| **Low-Pass** | High | Low | Smoothing, noise removal |
| **High-Pass** | Low | High | Differencing, DC removal |
| **Band-Pass** | Low/Zero | Low/Zero | Selecting one frequency |
| **Band-Stop** | High | High | Removing one frequency (notch) |
| **All-Pass** | 1.0 | 1.0 | Phase modification only |

---

## How to Identify Filter Type

```
Plot |H(ω)| vs ω from 0 to π:

If high at ω=0 and low at ω=π        → LOW-PASS
If low at ω=0 and high at ω=π        → HIGH-PASS
If low at ω=0, high in middle, low at ω=π   → BAND-PASS
If high at ω=0, low in middle, high at ω=π  → BAND-STOP (NOTCH)
If constant at all ω                  → ALL-PASS
```

---

## Phase Interpretation

### Phase and Group Delay

**Phase delay:**
$$\tau_p(\omega) = -\frac{\angle H(\omega)}{\omega}$$
Time delay of phase component

**Group delay:**
$$\tau_g(\omega) = -\frac{d\angle H(\omega)}{d\omega}$$
Time delay of modulated signal (envelope)

### Phase Transitions

```
If ∠H(ω) transitions:
  From 0 → -π: System has zero (blocks that frequency)
  From 0 → +π: Usually not seen in causal systems
  Smooth change: Linear phase characteristic
```

---

## Problem-Solving Flowchart

```
START: Given a problem involving LTI system

What are you given?
│
├─ Impulse response h(n)?
│  └─ Use DTFT: H(ω) = Σ h(n)e^(-jωn)
│
├─ Difference equation?
│  └─ Replace x(n) with X(ω), y(n) with Y(ω)
│     Solve for H(ω) = Y(ω)/X(ω)
│
├─ System function H(z)?
│  ├─ For frequency response: H(ω) = H(z)|_(z=e^(jω))
│  ├─ For impulse response: Use inverse z-transform
│  └─ For poles/zeros: Factor numerator and denominator
│
└─ Magnitude/Phase specs?
   └─ Use inverse DTFT (tables or numerical methods)

What are you asked to find?

├─ Frequency response?
│  └─ Calculate H(ω) as above
│
├─ Filter type?
│  └─ Plot |H(ω)| and check DC/Nyquist
│
├─ Output for input signal?
│  ├─ Find frequency components in input
│  ├─ Find |H(ω)| at each frequency
│  ├─ Scale input magnitudes by |H(ω)|
│  └─ Add scaled components
│
├─ Stability?
│  └─ Check: All poles have |z| < 1
│
├─ Difference equation from H(z)?
│  └─ Cross multiply and inverse z-transform
│
└─ Poles/Zeros?
   └─ Factor H(z) and find zeros of num, poles of denom
```

---

## Common Exam Question Patterns

### Pattern 1: Calculate Frequency Response
**Question:** "Given h(n) = {a, b, c}, find H(ω)"

**Solution Template:**
1. Write DTFT: $H(\omega) = ae^{0} + be^{-j\omega} + ce^{-2j\omega}$
2. Factor phase: $H(\omega) = e^{-j\omega}[ae^{j\omega} + b + ce^{-j\omega}]$
3. Simplify using $e^{j\omega} + e^{-j\omega} = 2\cos(\omega)$
4. Extract magnitude and phase

### Pattern 2: Classify the Filter
**Question:** "Is this low-pass, high-pass, band-pass, or band-stop?"

**Solution Template:**
1. Evaluate $|H(0)|$ (DC response)
2. Evaluate $|H(\pi)|$ (Nyquist response)
3. Check if peak exists at intermediate frequency
4. Match to classification table

### Pattern 3: Find Output for Sinusoid
**Question:** "Input is $x(n) = A\cos(\omega_0 n)$, find $y(n)$"

**Solution Template:**
1. Calculate $H(\omega_0)$ at input frequency
2. Find magnitude: $|H(\omega_0)|$
3. Find phase: $\angle H(\omega_0)$
4. Output: $y(n) = A|H(\omega_0)|\cos(\omega_0 n + \angle H(\omega_0))$

### Pattern 4: Difference Equation ↔ System Function
**Question:** "Given difference equation, find H(z)" or vice versa

**Solution Template:**
- Forward: Replace each term, solve for H(z) = Y(z)/X(z)
- Backward: Cross multiply H(z), inverse z-transform each term

### Pattern 5: Stability Analysis
**Question:** "Is this system stable?"

**Solution Template:**
1. Find poles (roots of denominator of H(z))
2. Check condition: $|$pole$| < 1$ for all poles
3. If all poles inside unit circle → STABLE ✓
4. If any pole outside → UNSTABLE ✗

---

## Exam Checklist

Before the exam, make sure you can:

### Understanding (Conceptual)
- [ ] Explain what "frequency response" means physically
- [ ] Describe how magnitude and phase affect the output
- [ ] Distinguish between time-domain and frequency-domain views
- [ ] Interpret pole-zero diagrams and stability
- [ ] Classify filters by their magnitude response
- [ ] Explain why phase matters in filter design

### Computation (Mathematical)
- [ ] Calculate DTFT from impulse response
- [ ] Find frequency response from difference equation
- [ ] Evaluate H(ω) at DC (ω=0) and Nyquist (ω=π)
- [ ] Extract magnitude and phase from complex H(ω)
- [ ] Convert between H(z) and difference equation
- [ ] Find poles and zeros from H(z)
- [ ] Determine stability from pole positions

### Applications (Problem-Solving)
- [ ] Filter a periodic signal by component
- [ ] Design or verify an inverse system
- [ ] Analyze cascaded systems: $H_{total} = H_1 \cdot H_2$
- [ ] Compare different filter designs
- [ ] Predict output for various inputs

### Common Mistakes to Avoid
- [ ] Forgetting the $e^{-j\omega n}$ term (negative exponent!)
- [ ] Confusing magnitude with power (power = |H|²)
- [ ] Mixing up poles (denominator) with zeros (numerator)
- [ ] Assuming linear phase (only for symmetric h(n))
- [ ] Forgetting stability condition: all |poles| < 1
- [ ] Getting sign wrong on phase: $\phi = -\omega d$, not $+\omega d$

---

## Numerical Quick Reference

### Key Frequency Values
- **DC:** $\omega = 0$ rad/sample
- **Quarter Nyquist:** $\omega = \pi/4$ rad/sample
- **Half Nyquist:** $\omega = \pi/2$ rad/sample
- **Three-quarters Nyquist:** $\omega = 3\pi/4$ rad/sample
- **Nyquist (max):** $\omega = \pi$ rad/sample

### Special Values
| ω | cos(ω) | sin(ω) | exp(-jω) |
|---|--------|--------|----------|
| 0 | 1 | 0 | 1 |
| π/4 | 0.707 | 0.707 | 0.707 - 0.707j |
| π/2 | 0 | 1 | -j |
| 3π/4 | -0.707 | 0.707 | -0.707 - 0.707j |
| π | -1 | 0 | -1 |

### Useful Identities
$$e^{j\omega} + e^{-j\omega} = 2\cos(\omega)$$
$$e^{j\omega} - e^{-j\omega} = 2j\sin(\omega)$$
$$|a + bj| = \sqrt{a^2 + b^2}$$
$$\arg(a + bj) = \arctan(b/a)$$

---

## Final Tips for Exam Success

1. **Always sketch magnitude response** before classifying filter
2. **Check DC and Nyquist** first to narrow down filter type
3. **Use conjugate to convert division** to multiplication form
4. **Factor out phase** to simplify magnitude calculation
5. **Verify stability first** before analyzing frequency response
6. **For periodic signals**, filter each Fourier component independently
7. **Remember:** Phase shift corresponds to time delay
8. **Group delay:** Frequency-dependent delay for modulated signals
9. **All-pass filters:** $|H| = 1$ everywhere, only phase changes
10. **Cascaded systems:** Multiply individual frequency responses

---

## Practice Problem Summary

### What You Should Be Able to Do:

1. **Basic:** Given h(n), find H(ω) at specific frequencies ✓
2. **Intermediate:** Classify filter from magnitude response ✓
3. **Advanced:** Design inverse system or cascaded filter ✓
4. **Challenge:** Analyze system with complex pole/zero ✓

### Difficulty Progression:

- **Easy:** Simple FIR filters (short impulse response)
- **Medium:** First-order IIR filters (one pole/zero)
- **Hard:** Higher-order filters (multiple poles/zeros)
- **Expert:** All-pass or inverse systems

---

## Additional Resources

### Related Topics to Review:
- DFT (Chapter 3-4): Discrete Fourier Transform
- Z-Transform (if needed): Poles/zeros analysis
- Filter Design (Chapter 6): Practical filter creation

### Key Insights:
- **Filtering = Multiplication in frequency domain**
- **Phase = Time delay indicator**
- **Poles determine frequency response shape**
- **Stability = All poles inside unit circle**

---

**Good luck on the exam! You've got this. Master these formulas and patterns, and you can solve any frequency domain LTI problem.**
