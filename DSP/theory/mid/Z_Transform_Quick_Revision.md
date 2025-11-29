# Z-Transform: Quick Revision Guide

## 1. DEFINITION

The Z-transform of a discrete-time sequence x(n) is:

$$X(z) = \sum_{n=-\infty}^{\infty} x(n)z^{-n}$$

**Region of Convergence (ROC):** The set of z values for which the sum converges.

---

## 2. KEY PROPERTIES

| Property | Time Domain | Z-Domain | ROC |
|----------|------------|----------|-----|
| **Linearity** | $ax_1(n) + bx_2(n)$ | $aX_1(z) + bX_2(z)$ | At least $R_1 \cap R_2$ |
| **Time Shift** | $x(n-n_0)$ | $z^{-n_0}X(z)$ | Same as $X(z)$ |
| **Scaling** | $a^n x(n)$ | $X(a^{-1}z)$ | $\|a\|R$ |
| **Time Reversal** | $x(-n)$ | $X(z^{-1})$ | $1/R$ (reversed) |
| **Differentiation** | $nx(n)$ | $-z\frac{dX(z)}{dz}$ | $R$ |
| **Convolution** | $x_1(n) * x_2(n)$ | $X_1(z)X_2(z)$ | At least $R_1 \cap R_2$ |

---

## 3. COMMON Z-TRANSFORM PAIRS

### **Causal Sequences (ROC: $\|z\| > \|a\|$)**

| Time Domain | Z-Transform | ROC |
|------------|-------------|-----|
| $\delta(n)$ | $1$ | All $z$ |
| $u(n)$ | $\frac{z}{z-1}$ or $\frac{1}{1-z^{-1}}$ | $\|z\| > 1$ |
| $nu(n)$ | $\frac{z}{(z-1)^2}$ | $\|z\| > 1$ |
| $a^n u(n)$ | $\frac{z}{z-a}$ or $\frac{1}{1-az^{-1}}$ | $\|z\| > \|a\|$ |
| $na^n u(n)$ | $\frac{az}{(z-a)^2}$ | $\|z\| > \|a\|$ |
| $\cos(\omega_0 n)u(n)$ | $\frac{z^2 - z\cos(\omega_0)}{z^2 - 2z\cos(\omega_0) + 1}$ | $\|z\| > 1$ |
| $\sin(\omega_0 n)u(n)$ | $\frac{z\sin(\omega_0)}{z^2 - 2z\cos(\omega_0) + 1}$ | $\|z\| > 1$ |

### **Anti-causal Sequences (ROC: $\|z\| < \|a\|$)**

| Time Domain | Z-Transform |
|------------|-------------|
| $-a^n u(-n-1)$ | $\frac{1}{1-az^{-1}}$ |
| $-na^n u(-n-1)$ | $\frac{az^{-1}}{(1-az^{-1})^2}$ |

---

## 4. INVERSE Z-TRANSFORM METHODS

### **Method 1: Partial Fraction Expansion**

For rational $X(z) = \frac{P(z)}{Q(z)}$:

**If pole at z = a:**
$$X(z) = \frac{A}{z-a} + \text{(other terms)} \Rightarrow x(n) = Aa^n u(n)$$

**If repeated pole (multiplicity 2):**
$$X(z) = \frac{A_1}{z-a} + \frac{A_2}{(z-a)^2} \Rightarrow x(n) = (A_1 + A_2 n)a^n u(n)$$

### **Method 2: Power Series Expansion**

$$X(z) = \sum_{n=0}^{\infty} x(n)z^{-n}$$

Read coefficients directly from series expansion.

### **Method 3: Contour Integration (Residue Method)**

$$x(n) = \frac{1}{2\pi j} \oint X(z)z^{n-1} dz$$

$$x(n) = \sum \text{Residues of } X(z)z^{n-1} \text{ at poles inside ROC}$$

---

## 5. SOLVING DIFFERENCE EQUATIONS

### **General Form:**
$$\sum_{k=0}^{N} a_k y(n-k) = \sum_{m=0}^{M} b_m x(n-m)$$

### **Solution Steps:**

1. **Take Z-transform** of both sides using linearity + time shift property
2. **Solve for Y(z):**
   $$Y(z) = \frac{\sum_{m=0}^{M} b_m z^{-m}}{\sum_{k=0}^{N} a_k z^{-k}} X(z)$$

3. **Identify transfer function:**
   $$H(z) = \frac{Y(z)}{X(z)} = \frac{\sum_{m=0}^{M} b_m z^{-m}}{\sum_{k=0}^{N} a_k z^{-k}}$$

4. **Apply inverse Z-transform** to get y(n)

### **Example:**
```
Difference equation: y(n) - 0.5y(n-1) = x(n)
                     with y(-1) = 0 (initial condition)

Z-transform: Y(z) - 0.5z^(-1)Y(z) = X(z)
            Y(z)[1 - 0.5z^(-1)] = X(z)
            H(z) = Y(z)/X(z) = 1/(1 - 0.5z^(-1))
            
            = z/(z - 0.5)

Pole: z = 0.5
ROC: |z| > 0.5

h(n) = (0.5)^n u(n)
```

---

## 6. FREQUENCY RESPONSE FROM Z-TRANSFORM

### **Concept:**
Replace $z = e^{j\omega}$ where $\omega$ is the normalized frequency:
- $\omega = 0$: DC (direct current)
- $\omega = \pi$: Nyquist frequency
- Period: $2\pi$ (discrete-time digital frequency)

### **Magnitude and Phase:**
$$H(e^{j\omega}) = \text{Re} + j\cdot\text{Im}$$

$$|H(e^{j\omega})| = \sqrt{\text{Re}^2 + \text{Im}^2}$$

$$\angle H(e^{j\omega}) = \arctan\left(\frac{\text{Im}}{\text{Re}}\right)$$

### **Example:**
```
H(z) = 1/(1 - 0.5z^(-1))

H(e^(jω)) = 1/(1 - 0.5e^(-jω))
          = 1/(1 - 0.5[cos(ω) - j·sin(ω)])
          = 1/[(1 - 0.5cos(ω)) + j·0.5sin(ω)]

Magnitude:
|H(e^(jω))| = 1/√[(1-0.5cos(ω))² + (0.5sin(ω))²]
            = 1/√[1 - cos(ω) + 0.25]
            = 1/√[1.25 - cos(ω)]

At ω=0 (DC):    |H(1)| = 1/0.5 = 2
At ω=π (Nyquist): |H(-1)| = 1/1.5 ≈ 0.67
```

---

## 7. POLE-ZERO DIAGRAMS

### **Stability Criterion:**
- **Causal & Stable:** All poles inside unit circle ($|z| < 1$)
- **Unstable:** At least one pole on or outside unit circle
- **Marginally Stable:** Poles on unit circle (oscillatory)

### **Interpreting Pole-Zero Plots:**
```
X-axis: Real axis
Y-axis: Imaginary axis
Circle: Unit circle (|z| = 1)

Poles (×) inside = Stable (exponential decay)
Poles (×) on circle = Marginally stable (constant amplitude)
Poles (×) outside = Unstable (exponential growth)

Zeros (○) placement affects magnitude response
```

### **Example: Lowpass Filter**
```
H(z) = (1-a)/(1-az^(-1)) with |a| < 1

Pole: z = a (inside unit circle → stable)
Zero: z = 0 (at origin)

At ω=0: High response (passes DC)
At ω=π: Low response (blocks high frequencies)
→ Lowpass behavior
```

---

## 8. COMMON OPERATIONS QUICK REFERENCE

### **Time Shift (Delay):**
$$x(n-1) \Rightarrow z^{-1}X(z)$$

### **Multiplication by Exponential:**
$$a^n x(n) \Rightarrow X(a^{-1}z)$$

### **Convolution in Time Domain:**
$$y(n) = x(n) * h(n) \Rightarrow Y(z) = X(z)H(z)$$

### **Derivative in Frequency Domain:**
$$n \cdot x(n) \Rightarrow -z\frac{dX(z)}{dz}$$

### **Initial Value Theorem:**
$$x(0) = \lim_{z \to \infty} X(z)$$

### **Final Value Theorem (if limit exists):**
$$x(\infty) = \lim_{z \to 1} (z-1)X(z)$$

---

## 9. STANDARD FORMS

### **Factored Form:**
$$H(z) = K\frac{(z-z_1)(z-z_2)\cdots(z-z_M)}{(z-p_1)(z-p_2)\cdots(z-p_N)}$$

Where $z_i$ = zeros, $p_i$ = poles, $K$ = gain

### **Standard Biquad (2nd Order):**
$$H(z) = \frac{b_0 + b_1z^{-1} + b_2z^{-2}}{1 + a_1z^{-1} + a_2z^{-2}}$$

Can be factored as:
$$H(z) = \frac{b_0(1 - r_1e^{j\theta_1}z^{-1})(1 - r_1e^{-j\theta_1}z^{-1})}{(1 - p_1e^{j\phi_1}z^{-1})(1 - p_1e^{-j\phi_1}z^{-1})}$$

---

## 10. EXAM QUICK CHECKLIST

- [ ] Identify if sequence is causal/anti-causal (determines ROC)
- [ ] Use linearity for complex expressions
- [ ] Time shift property for difference equations
- [ ] Partial fractions for inverse transform
- [ ] Frequency response: substitute $z = e^{j\omega}$
- [ ] Check pole locations for stability
- [ ] Verify ROC doesn't overlap inconsistently
- [ ] Remember: ROC affects validity of inverse transform
- [ ] For repeated poles, use $(z-a)^{-k}$ terms carefully
- [ ] Final value theorem only works if pole at $z=1$ is simple

---

## 11. WORKED EXAMPLE: COMPLETE SOLUTION

**Problem:** Find impulse response of $y(n) - 0.6y(n-1) + 0.08y(n-2) = x(n)$

**Solution:**

1. Z-transform both sides:
   $$Y(z) - 0.6z^{-1}Y(z) + 0.08z^{-2}Y(z) = X(z)$$

2. Factor out Y(z):
   $$Y(z)[1 - 0.6z^{-1} + 0.08z^{-2}] = X(z)$$

3. For impulse response, $X(z) = 1$ (impulse input):
   $$H(z) = \frac{1}{1 - 0.6z^{-1} + 0.08z^{-2}}$$

4. Multiply numerator/denominator by $z^2$:
   $$H(z) = \frac{z^2}{z^2 - 0.6z + 0.08}$$

5. Find poles using quadratic formula:
   $$z = \frac{0.6 \pm \sqrt{0.36 - 0.32}}{2} = \frac{0.6 \pm 0.2}{2}$$
   
   Poles: $z_1 = 0.4$, $z_2 = 0.2$

6. Partial fraction expansion:
   $$H(z) = \frac{A}{z - 0.4} + \frac{B}{z - 0.2}$$
   
   $$A = \frac{z^2}{z-0.2}\bigg|_{z=0.4} = \frac{0.16}{0.2} = 0.8$$
   
   $$B = \frac{z^2}{z-0.4}\bigg|_{z=0.2} = \frac{0.04}{-0.2} = -0.2$$

7. Inverse transform:
   $$h(n) = 0.8(0.4)^n u(n) - 0.2(0.2)^n u(n)$$
   
   $$h(n) = [0.8(0.4)^n - 0.2(0.2)^n]u(n)$$

---

## Summary Table: Transform Pairs

| $x(n)$ | $X(z)$ |
|--------|--------|
| $\delta(n)$ | $1$ |
| $u(n)$ | $z/(z-1)$ |
| $(1/2)^n u(n)$ | $z/(z-1/2)$ |
| $(1/3)^n u(n)$ | $z/(z-1/3)$ |
| $n(1/2)^n u(n)$ | $z/2(z-1/2)^2$ |
| $[\cos(\omega_0 n)]u(n)$ | $[z^2-z\cos\omega_0]/[z^2-2z\cos\omega_0+1]$ |

---

**Document Version:** 1.0  
**Last Updated:** 2025-11-04  
**Best For:** Quick exam revision, formula lookup, worked examples
