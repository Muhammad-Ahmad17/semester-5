# Trapezoidal Rule (n=1) — Coefficient Method Derivation

## Part 1: Polynomial Setup and Interpolation

### 1.1 Assume Linear Polynomial

For n=1 (2 nodes), we use a **linear polynomial**:
$$P(t) = at + b$$

where t is the dimensionless variable: $t = \frac{x - x_0}{h}$

**Node positions in t-space:** t = 0 (at x₀) and t = 1 (at x₁)

### 1.2 Set Up Interpolation Equations

The polynomial must pass through both nodes:

$$P(0) = a(0) + b = b = f_0$$
$$P(1) = a(1) + b = a + b = f_1$$

### 1.3 Solve for Coefficients

From equation 1:
$$\boxed{b = f_0}$$

From equation 2:
$$a = f_1 - b = f_1 - f_0$$
$$\boxed{a = f_1 - f_0}$$

### 1.4 Integrate the Polynomial

$$\int_0^1 P(t)\,dt = \int_0^1 (at + b)\,dt$$
$$= \left[\frac{at^2}{2} + bt\right]_0^1$$
$$= \frac{a}{2} + b$$

Substitute a and b:
$$= \frac{f_1 - f_0}{2} + f_0$$
$$= \frac{f_1 - f_0 + 2f_0}{2}$$
$$= \frac{f_1 + f_0}{2}$$

### 1.5 Final Formula

Transform back to original variable (multiply by h):
$$\int_{x_0}^{x_1} f(x)\,dx \approx h \cdot \frac{1}{2}(f_0 + f_1)$$

$$\boxed{\int_{x_0}^{x_1} f(x)\,dx \approx \frac{h}{2}[f_0 + f_1]}$$

**Weights:** w₀ = 1/2, w₁ = 1/2

---

## Part 2: Worked Example — ∫₀¹ 1/(x²+1) dx

**Step 1:** h = 1

**Step 2:** Nodes: x₀ = 0, x₁ = 1

**Step 3:** Function values:
- f₀ = 1
- f₁ = 0.5

**Step 4:** Coefficients:
- a = 0.5 - 1 = -0.5
- b = 1

**Step 5:** Polynomial: P(t) = -0.5t + 1

**Step 6:** Integral:
$$I \approx 1 \cdot \frac{1}{2}(1 + 0.5) = 0.75$$

**Step 7:** Error: |π/4 - 0.75| = |0.7854 - 0.75| = 0.0354

---

# Simpson 1/3 Rule (n=2) — Coefficient Method Derivation

## Part 1: Polynomial Setup and Interpolation

### 1.1 Assume Quadratic Polynomial

For n=2 (3 nodes), we use a **quadratic polynomial**:
$$P(t) = at^2 + bt + c$$

**Node positions in t-space:** t = 0, 1, 2

### 1.2 Set Up Interpolation Equations

$$P(0) = c = f_0$$
$$P(1) = a + b + c = f_1$$
$$P(2) = 4a + 2b + c = f_2$$

### 1.3 Solve for Coefficients

From equation 1:
$$\boxed{c = f_0}$$

From equations 2 and 3, substitute c = f₀:

Equation 2': $a + b = f_1 - f_0$ ... (i)

Equation 3': $4a + 2b = f_2 - f_0$ ... (ii)

From (i): $b = f_1 - f_0 - a$

Substitute into (ii):
$$4a + 2(f_1 - f_0 - a) = f_2 - f_0$$
$$4a + 2f_1 - 2f_0 - 2a = f_2 - f_0$$
$$2a = f_2 - 2f_1 + f_0$$
$$\boxed{a = \frac{f_2 - 2f_1 + f_0}{2}}$$

From (i):
$$b = f_1 - f_0 - \frac{f_2 - 2f_1 + f_0}{2}$$
$$= \frac{2(f_1 - f_0) - (f_2 - 2f_1 + f_0)}{2}$$
$$= \frac{2f_1 - 2f_0 - f_2 + 2f_1 - f_0}{2}$$
$$\boxed{b = \frac{4f_1 - f_2 - 3f_0}{2}}$$

### 1.4 Integrate the Polynomial

$$\int_0^2 P(t)\,dt = \int_0^2 (at^2 + bt + c)\,dt$$
$$= \left[\frac{at^3}{3} + \frac{bt^2}{2} + ct\right]_0^2$$
$$= \frac{8a}{3} + 2b + 2c$$

Substitute coefficients:
$$= \frac{8}{3} \cdot \frac{f_2 - 2f_1 + f_0}{2} + 2 \cdot \frac{4f_1 - f_2 - 3f_0}{2} + 2f_0$$
$$= \frac{4(f_2 - 2f_1 + f_0)}{3} + 4f_1 - f_2 - 3f_0 + 2f_0$$
$$= \frac{4f_2 - 8f_1 + 4f_0}{3} + 4f_1 - f_2 - f_0$$
$$= \frac{4f_2 - 8f_1 + 4f_0 + 12f_1 - 3f_2 - 3f_0}{3}$$
$$= \frac{f_2 + 4f_1 + f_0}{3}$$

### 1.5 Final Formula

Transform back (multiply by h):
$$\int_{x_0}^{x_2} f(x)\,dx \approx h \cdot \frac{1}{3}(f_0 + 4f_1 + f_2)$$

$$\boxed{\int_{x_0}^{x_2} f(x)\,dx \approx \frac{h}{3}[f_0 + 4f_1 + f_2]}$$

**Weights:** w₀ = 1/3, w₁ = 4/3, w₂ = 1/3

---

## Part 2: Worked Example — ∫₀¹ 1/(x²+1) dx

**Step 1:** h = 0.5

**Step 2:** Nodes: x₀ = 0, x₁ = 0.5, x₂ = 1

**Step 3:** Function values:
- f₀ = 1
- f₁ = 0.8
- f₂ = 0.5

**Step 4:** Coefficients:
- c = 1
- a = (0.5 - 2(0.8) + 1)/2 = (0.5 - 1.6 + 1)/2 = -0.1/2 = -0.05
- b = (4(0.8) - 0.5 - 3(1))/2 = (3.2 - 0.5 - 3)/2 = -0.3/2 = -0.15

**Step 5:** Polynomial: P(t) = -0.05t² - 0.15t + 1

**Step 6:** Integral:
$$I \approx 0.5 \cdot \frac{1}{3}(1 + 4(0.8) + 0.5) = 0.5 \cdot \frac{4.7}{3} = 0.783333$$

**Step 7:** Error: |0.7854 - 0.7833| = 0.002065

---

# Simpson 3/8 Rule (n=3) — Coefficient Method Derivation

## Part 1: Polynomial Setup and Interpolation

### 1.1 Assume Cubic Polynomial

For n=3 (4 nodes), we use a **cubic polynomial**:
$$P(t) = at^3 + bt^2 + ct + d$$

**Node positions in t-space:** t = 0, 1, 2, 3

### 1.2 Set Up Interpolation Equations

$$P(0) = d = f_0$$
$$P(1) = a + b + c + d = f_1$$
$$P(2) = 8a + 4b + 2c + d = f_2$$
$$P(3) = 27a + 9b + 3c + d = f_3$$

### 1.3 Solve for Coefficients

From equation 1:
$$\boxed{d = f_0}$$

Substitute d = f₀ into equations 2-4:

Equation 2': $a + b + c = f_1 - f_0$ ... (i)
Equation 3': $8a + 4b + 2c = f_2 - f_0$ ... (ii)
Equation 4': $27a + 9b + 3c = f_3 - f_0$ ... (iii)

From (i): $c = f_1 - f_0 - a - b$ ... (*)

Substitute (*) into (ii):
$$8a + 4b + 2(f_1 - f_0 - a - b) = f_2 - f_0$$
$$8a + 4b + 2f_1 - 2f_0 - 2a - 2b = f_2 - f_0$$
$$6a + 2b = f_2 - 2f_1 + f_0$$
$$3a + b = \frac{f_2 - 2f_1 + f_0}{2}$$ ... (iv)

Substitute (*) into (iii):
$$27a + 9b + 3(f_1 - f_0 - a - b) = f_3 - f_0$$
$$27a + 9b + 3f_1 - 3f_0 - 3a - 3b = f_3 - f_0$$
$$24a + 6b = f_3 - 3f_1 + 2f_0$$
$$4a + b = \frac{f_3 - 3f_1 + 2f_0}{6}$$ ... (v)

From (iv) and (v):
$$(4a + b) - (3a + b) = \frac{f_3 - 3f_1 + 2f_0}{6} - \frac{f_2 - 2f_1 + f_0}{2}$$
$$a = \frac{f_3 - 3f_1 + 2f_0 - 3(f_2 - 2f_1 + f_0)}{6}$$
$$a = \frac{f_3 - 3f_1 + 2f_0 - 3f_2 + 6f_1 - 3f_0}{6}$$
$$\boxed{a = \frac{f_3 - 3f_2 + 3f_1 - f_0}{6}}$$

From (iv):
$$b = \frac{f_2 - 2f_1 + f_0}{2} - 3a$$
$$= \frac{f_2 - 2f_1 + f_0}{2} - 3 \cdot \frac{f_3 - 3f_2 + 3f_1 - f_0}{6}$$
$$= \frac{3(f_2 - 2f_1 + f_0) - (f_3 - 3f_2 + 3f_1 - f_0)}{6}$$
$$= \frac{3f_2 - 6f_1 + 3f_0 - f_3 + 3f_2 - 3f_1 + f_0}{6}$$
$$\boxed{b = \frac{-f_3 + 6f_2 - 9f_1 + 4f_0}{6}}$$

From (*):
$$c = f_1 - f_0 - a - b$$
$$= f_1 - f_0 - \frac{f_3 - 3f_2 + 3f_1 - f_0}{6} - \frac{-f_3 + 6f_2 - 9f_1 + 4f_0}{6}$$
$$= f_1 - f_0 - \frac{f_3 - 3f_2 + 3f_1 - f_0 - f_3 + 6f_2 - 9f_1 + 4f_0}{6}$$
$$= f_1 - f_0 - \frac{3f_2 - 6f_1 + 3f_0}{6}$$
$$\boxed{c = f_1 - f_0 + \frac{f_1}{2} - \frac{f_2}{2} - \frac{f_0}{2}}$$

### 1.4 Integrate the Polynomial

$$\int_0^3 P(t)\,dt = \int_0^3 (at^3 + bt^2 + ct + d)\,dt$$
$$= \left[\frac{at^4}{4} + \frac{bt^3}{3} + \frac{ct^2}{2} + dt\right]_0^3$$
$$= \frac{81a}{4} + 9b + \frac{9c}{2} + 3d$$

After substituting and simplifying (multiply through by common denominator):
$$= \frac{3}{8}(f_0 + 3f_1 + 3f_2 + f_3)$$

### 1.5 Final Formula

Transform back (multiply by h):
$$\int_{x_0}^{x_3} f(x)\,dx \approx h \cdot \frac{3}{8}(f_0 + 3f_1 + 3f_2 + f_3)$$

$$\boxed{\int_{x_0}^{x_3} f(x)\,dx \approx \frac{3h}{8}[f_0 + 3f_1 + 3f_2 + f_3]}$$

**Weights (normalized):** w₀ = 3/8, w₁ = 9/8, w₂ = 9/8, w₃ = 3/8

---

## Part 2: Worked Example — ∫₀¹ 1/(x²+1) dx

**Step 1:** h = 1/3

**Step 2:** Nodes: x₀ = 0, x₁ = 1/3, x₂ = 2/3, x₃ = 1

**Step 3:** Function values:
- f₀ = 1
- f₁ = 0.9
- f₂ ≈ 0.6923
- f₃ = 0.5

**Step 4:** Apply formula:
$$I \approx \frac{3 \times (1/3)}{8}[1 + 3(0.9) + 3(0.6923) + 0.5]$$
$$= \frac{1}{8}[1 + 2.7 + 2.077 + 0.5]$$
$$= \frac{6.277}{8} = 0.78461$$

**Step 5:** Error: |0.7854 - 0.7846| = 0.0008

---

# Boole's Rule (n=4) — Coefficient Method Derivation

## Part 1: Polynomial Setup and Interpolation

### 1.1 Assume Quartic Polynomial

For n=4 (5 nodes), we use a **quartic (degree 4) polynomial**:
$$P(t) = at^4 + bt^3 + ct^2 + dt + e$$

**Node positions in t-space:** t = 0, 1, 2, 3, 4

### 1.2 Set Up Interpolation Equations

$$P(0) = e = f_0$$
$$P(1) = a + b + c + d + e = f_1$$
$$P(2) = 16a + 8b + 4c + 2d + e = f_2$$
$$P(3) = 81a + 27b + 9c + 3d + e = f_3$$
$$P(4) = 256a + 64b + 16c + 4d + e = f_4$$

### 1.3 Solve for Coefficients

From equation 1:
$$\boxed{e = f_0}$$

Substitute e = f₀ into the remaining equations (similar to Simpson 3/8 but with more algebra).

After solving this 4×4 system (details omitted for brevity), the coefficients are determined in terms of f₀, f₁, f₂, f₃, f₄.

### 1.4 Integrate the Polynomial

$$\int_0^4 P(t)\,dt = \left[\frac{at^5}{5} + \frac{bt^4}{4} + \frac{ct^3}{3} + \frac{dt^2}{2} + et\right]_0^4$$

After substituting the solved coefficients and simplifying:
$$= \frac{2}{45}(7f_0 + 32f_1 + 12f_2 + 32f_3 + 7f_4)$$

### 1.5 Final Formula

Transform back (multiply by h):
$$\int_{x_0}^{x_4} f(x)\,dx \approx h \cdot \frac{2}{45}(7f_0 + 32f_1 + 12f_2 + 32f_3 + 7f_4)$$

$$\boxed{\int_{x_0}^{x_4} f(x)\,dx \approx \frac{2h}{45}[7f_0 + 32f_1 + 12f_2 + 32f_3 + 7f_4]}$$

**Weight pattern:** 7, 32, 12, 32, 7

---

## Part 2: Worked Example — ∫₀¹ 1/(x²+1) dx

**Step 1:** h = 0.25

**Step 2:** Nodes: x₀ = 0, x₁ = 0.25, x₂ = 0.5, x₃ = 0.75, x₄ = 1

**Step 3:** Function values:
- f₀ = 1
- f₁ ≈ 0.9412
- f₂ = 0.8
- f₃ ≈ 0.64
- f₄ = 0.5

**Step 4:** Apply formula:
$$I \approx \frac{2(0.25)}{45}[7(1) + 32(0.9412) + 12(0.8) + 32(0.64) + 7(0.5)]$$
$$= \frac{0.5}{45}[7 + 30.118 + 9.6 + 20.48 + 3.5]$$
$$= \frac{70.698}{90} = 0.7855$$

**Step 5:** Error: |0.7854 - 0.7855| ≈ 0.0001

---

# Summary: Coefficient Method for All Newton–Cotes Rules

## General Algorithm

| Step | Procedure |
|---|---|
| 1 | Assume polynomial of degree n: $P(t) = a_n t^n + ... + a_1 t + a_0$ |
| 2 | Set up n+1 interpolation equations: $P(i) = f_i$, i = 0, 1, ..., n |
| 3 | Solve the system for coefficients $a_0, a_1, ..., a_n$ |
| 4 | Integrate: $\int_0^n P(t)\,dt = \sum w_i f_i$ |
| 5 | Extract weights: $w_i$ appears as coefficient of $f_i$ |
| 6 | Final formula: $\int_a^b f(x)\,dx \approx h \sum w_i f_i$ |

## Comparison of All Methods

| n | Method | Degree | # Nodes | Formula | Weights |
|---|---|---|---|---|---|
| 1 | Trapezoidal | 1 | 2 | $\frac{h}{2}(f_0 + f_1)$ | 1, 1 |
| 2 | Simpson 1/3 | 2 | 3 | $\frac{h}{3}(f_0 + 4f_1 + f_2)$ | 1, 4, 1 |
| 3 | Simpson 3/8 | 3 | 4 | $\frac{3h}{8}(f_0 + 3f_1 + 3f_2 + f_3)$ | 3, 9, 9, 3 |
| 4 | Boole | 4 | 5 | $\frac{2h}{45}(7f_0 + 32f_1 + 12f_2 + 32f_3 + 7f_4)$ | 7, 32, 12, 32, 7 |

---

**This coefficient method shows the algebraic foundation of all Newton–Cotes rules!**

