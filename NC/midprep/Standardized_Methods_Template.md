# All Newton–Cotes Methods: General nth Formula with Arbitrary Step Size h

## Part 0: Key Insight

**The formulas work for ANY step size h!** Whether you have:
- h = 1, h = 0.5, h = 0.25, or h = 0.001
- Any arbitrary interval [a, b]

The **general formula structure remains the same**, only h changes!

---

## Part 1: General Theory for ANY Closed Newton–Cotes Rule

### 1.1 General Problem Setup

**Given:**
- Integral: $\int_a^b f(x)\,dx$
- Step size: **h (arbitrary)** = (b - a)/n
- Number of subintervals: n
- Total nodes: n + 1 (from x₀ to xₙ)
- Node locations: $x_i = a + i \cdot h$, where $i = 0, 1, ..., n$

### 1.2 General Transformation to Dimensionless Variable

Define: $t = \frac{x - a}{h}$

Then:
- x = a + h·t
- dx = h·dt
- When x = a: t = 0
- When x = b: t = n

The integral transforms:
$$\int_a^b f(x)\,dx = h \int_0^n f(a + h \cdot t)\,dt$$

### 1.3 General Polynomial Interpolation

Assume a polynomial of degree n:
$$P(t) = a_n t^n + a_{n-1} t^{n-1} + \cdots + a_1 t + a_0$$

Set up n+1 interpolation conditions:
$$P(i) = f_i, \quad i = 0, 1, 2, \ldots, n$$

This gives:
$$a_n \cdot i^n + a_{n-1} \cdot i^{n-1} + \cdots + a_1 \cdot i + a_0 = f_i$$

### 1.4 General Solution

Solve this linear system (Vandermonde matrix) to find $a_0, a_1, \ldots, a_n$ in terms of $f_0, f_1, \ldots, f_n$.

### 1.5 General Integration Formula

$$\int_0^n P(t)\,dt = w_0 f_0 + w_1 f_1 + \cdots + w_n f_n$$

where the weights $w_i = \int_0^n L_i(t)\,dt$ are **independent of h and [a,b]**.

### 1.6 **General Final Formula (with arbitrary h)**

$$\boxed{\int_a^b f(x)\,dx \approx h \sum_{i=0}^{n} w_i \cdot f_i}$$

**KEY POINT:** The weights $w_i$ depend ONLY on the rule (n), NOT on h or the specific interval!

---

## Part 2: Trapezoidal Rule (n=1) with Arbitrary h

### 2.1 General Formula

**Number of nodes:** 2
**Interval:** [a, b]
**Step size:** h = (b - a)/1 = b - a

**General nth formula:**
$$\boxed{\int_a^b f(x)\,dx \approx \frac{h}{2}[f_0 + f_1]}$$

where:
- $f_0 = f(a)$
- $f_1 = f(b)$
- h = b - a

### 2.2 This Works for ANY h!

**Example 1:** h = 1
$$I \approx \frac{1}{2}[f(a) + f(b)]$$

**Example 2:** h = 0.5
$$I \approx \frac{0.5}{2}[f(a) + f(a+0.5)]$$

**Example 3:** h = 0.001
$$I \approx \frac{0.001}{2}[f(a) + f(a+0.001)]$$

**Example 4:** h = π/6 (arbitrary!)
$$I \approx \frac{\pi/6}{2}[f(a) + f(a+\pi/6)]$$

**The formula scales linearly with h!**

---

## Part 3: Simpson 1/3 Rule (n=2) with Arbitrary h

### 3.1 General Formula

**Number of nodes:** 3
**Interval:** [a, b]
**Step size:** h = (b - a)/2

**General nth formula:**
$$\boxed{\int_a^b f(x)\,dx \approx \frac{h}{3}[f_0 + 4f_1 + f_2]}$$

where:
- $f_0 = f(a)$
- $f_1 = f(a + h)$
- $f_2 = f(b)$
- h = (b - a)/2

### 3.2 This Works for ANY h!

**Example 1:** [0, 1], h = 0.5
$$I \approx \frac{0.5}{3}[f(0) + 4f(0.5) + f(1)]$$

**Example 2:** [0, 2], h = 1 (same interval, different h)
$$I \approx \frac{1}{3}[f(0) + 4f(1) + f(2)]$$

**Example 3:** [a, b], h = arbitrary = (b-a)/2
$$I \approx \frac{(b-a)}{6}[f(a) + 4f\left(\frac{a+b}{2}\right) + f(b)]$$

**Example 4:** h = 0.001 (very small)
$$I \approx \frac{0.001}{3}[f(a) + 4f(a + 0.001) + f(a + 0.002)]$$

**Same formula, different h values!**

### 3.3 Key: The Weights Don't Change

| h value | Formula |
|---|---|
| 0.5 | $\frac{0.5}{3}[f_0 + 4f_1 + f_2]$ |
| 1.0 | $\frac{1}{3}[f_0 + 4f_1 + f_2]$ |
| 0.001 | $\frac{0.001}{3}[f_0 + 4f_1 + f_2]$ |
| h (arbitrary) | $\frac{h}{3}[f_0 + 4f_1 + f_2]$ |

**The weights 1, 4, 1 are ALWAYS the same!** Only h changes!

---

## Part 4: Simpson 3/8 Rule (n=3) with Arbitrary h

### 4.1 General Formula

**Number of nodes:** 4
**Interval:** [a, b]
**Step size:** h = (b - a)/3

**General nth formula:**
$$\boxed{\int_a^b f(x)\,dx \approx \frac{3h}{8}[f_0 + 3f_1 + 3f_2 + f_3]}$$

where:
- $f_0 = f(a)$
- $f_1 = f(a + h)$
- $f_2 = f(a + 2h)$
- $f_3 = f(b)$
- h = (b - a)/3

### 4.2 Works for ANY h!

**Example 1:** [0, 1], h = 1/3
$$I \approx \frac{3 \cdot (1/3)}{8}[f_0 + 3f_1 + 3f_2 + f_3] = \frac{1}{8}[f_0 + 3f_1 + 3f_2 + f_3]$$

**Example 2:** [0, 3], h = 1 (3 times larger interval)
$$I \approx \frac{3 \cdot 1}{8}[f_0 + 3f_1 + 3f_2 + f_3] = \frac{3}{8}[f_0 + 3f_1 + 3f_2 + f_3]$$

**Example 3:** [0, 0.003], h = 0.001
$$I \approx \frac{3 \cdot 0.001}{8}[f_0 + 3f_1 + 3f_2 + f_3]$$

**Coefficient method works the same; answer scales with h!**

### 4.3 Coefficient Method for n=3 with Arbitrary h

**Step 1:** Assume $P(t) = at^3 + bt^2 + ct + d$

**Step 2:** Set up interpolation at t = 0, 1, 2, 3:
$$P(0) = d = f_0$$
$$P(1) = a + b + c + d = f_1$$
$$P(2) = 8a + 4b + 2c + d = f_2$$
$$P(3) = 27a + 9b + 3c + d = f_3$$

**Step 3:** Solve for a, b, c, d (coefficients depend only on $f_i$, not h)

**Step 4:** Integrate from t = 0 to t = 3:
$$\int_0^3 P(t)\,dt = \frac{3}{8}(f_0 + 3f_1 + 3f_2 + f_3)$$

**Step 5:** Transform back to x-space (multiply by h):
$$\int_a^b f(x)\,dx \approx h \cdot \frac{3}{8}(f_0 + 3f_1 + 3f_2 + f_3) = \frac{3h}{8}[f_0 + 3f_1 + 3f_2 + f_3]$$

**The h appears as a multiplicative factor!**

---

## Part 5: Boole's Rule (n=4) with Arbitrary h

### 5.1 General Formula

**Number of nodes:** 5
**Interval:** [a, b]
**Step size:** h = (b - a)/4

**General nth formula:**
$$\boxed{\int_a^b f(x)\,dx \approx \frac{2h}{45}[7f_0 + 32f_1 + 12f_2 + 32f_3 + 7f_4]}$$

where:
- $f_i = f(a + i \cdot h)$ for i = 0, 1, 2, 3, 4
- h = (b - a)/4

### 5.2 Works for ANY h!

**Example 1:** [0, 1], h = 0.25
$$I \approx \frac{2 \cdot 0.25}{45}[7f_0 + 32f_1 + 12f_2 + 32f_3 + 7f_4] = \frac{0.5}{45}[...]$$

**Example 2:** [0, 4], h = 1
$$I \approx \frac{2 \cdot 1}{45}[7f_0 + 32f_1 + 12f_2 + 32f_3 + 7f_4] = \frac{2}{45}[...]$$

**Example 3:** h = 0.001
$$I \approx \frac{2 \cdot 0.001}{45}[7f_0 + 32f_1 + 12f_2 + 32f_3 + 7f_4]$$

**Same weights (7, 32, 12, 32, 7) for all h!**

---

## Part 6: n=5 Rule with Arbitrary h

### 6.1 General Formula

**Number of nodes:** 6
**Interval:** [a, b]
**Step size:** h = (b - a)/5

**General nth formula:**
$$\boxed{\int_a^b f(x)\,dx \approx \frac{5h}{288}[19f_0 + 75f_1 + 50f_2 + 50f_3 + 75f_4 + 19f_5]}$$

where h can be ANY value = (b - a)/5

### 6.2 Examples with Different h

| Interval [a,b] | h value | Formula |
|---|---|---|
| [0, 1] | 0.2 | $\frac{5(0.2)}{288}[...] = \frac{1}{288}[...]$ |
| [0, 5] | 1.0 | $\frac{5(1)}{288}[...] = \frac{5}{288}[...]$ |
| [0, 0.0005] | 0.0001 | $\frac{5(0.0001)}{288}[...]$ |
| [a, b] | (b-a)/5 | $\frac{(b-a)}{288/5}[...]$ |

---

## Part 7: n=6 Rule with Arbitrary h

### 7.1 General Formula

**Number of nodes:** 7
**Interval:** [a, b]
**Step size:** h = (b - a)/6

**General nth formula:**
$$\boxed{\int_a^b f(x)\,dx \approx \frac{h}{140}[41f_0 + 216f_1 + 27f_2 + 272f_3 + 27f_4 + 216f_5 + 41f_6]}$$

where h = (b - a)/6 (arbitrary!)

---

## Part 8: Master Summary Table — Arbitrary h

| n | Rule | Nodes | h = (b-a)/n | General Formula |
|---|---|---|---|---|
| 1 | Trapezoidal | 2 | h arbitrary | $\frac{h}{2}[f_0 + f_1]$ |
| 2 | Simpson 1/3 | 3 | h arbitrary | $\frac{h}{3}[f_0 + 4f_1 + f_2]$ |
| 3 | Simpson 3/8 | 4 | h arbitrary | $\frac{3h}{8}[f_0 + 3f_1 + 3f_2 + f_3]$ |
| 4 | Boole | 5 | h arbitrary | $\frac{2h}{45}[7f_0 + 32f_1 + 12f_2 + 32f_3 + 7f_4]$ |
| 5 | n=5 | 6 | h arbitrary | $\frac{5h}{288}[19f_0 + 75f_1 + 50f_2 + 50f_3 + 75f_4 + 19f_5]$ |
| 6 | n=6 | 7 | h arbitrary | $\frac{h}{140}[41f_0 + 216f_1 + 27f_2 + 272f_3 + 27f_4 + 216f_5 + 41f_6]$ |

**KEY:** Weights are FIXED for each rule. Only h changes!

---

## Part 9: Practical Example — Same Integral, Different h

**Integrate:** $\int_0^1 \frac{1}{x^2+1}\,dx$ (exact = π/4 ≈ 0.785398)

### Using Simpson 1/3 with h = 0.5 (standard)

h = 0.5, nodes at x = 0, 0.5, 1:
$$I \approx \frac{0.5}{3}[f(0) + 4f(0.5) + f(1)]$$
$$= \frac{0.5}{3}[1 + 4(0.8) + 0.5]$$
$$= \frac{0.5}{3}[4.7] = 0.783333$$

### Using Simpson 1/3 with h = 0.25 (twice as many intervals!)

Wait! If h = 0.25, then we have 5 nodes, not 3. We need to **compose** the rule (use composite Simpson 1/3).

But if we keep n=2 (3 nodes) with h = 0.25:
$$I \approx \frac{0.25}{3}[f(0) + 4f(0.25) + f(0.5)]$$

This gives only part of the integral!

---

## Part 10: Why h Must Be (b-a)/n

**Important note:** The step size h is NOT arbitrary in the sense that:
$$h = \frac{b - a}{n}$$

**Why?** Because we have exactly n+1 nodes:
- x₀ = a
- x₁ = a + h
- x₂ = a + 2h
- ...
- xₙ = a + n·h = b

If h ≠ (b-a)/n, then xₙ ≠ b!

### But the Formula SCALES with h

If you **choose different interval lengths** [a, b], then h changes accordingly:

**Same rule (Simpson 1/3), different intervals:**

| Interval | h | Formula | Notes |
|---|---|---|---|
| [0, 1] | 0.5 | $\frac{0.5}{3}[...]$ | Covers 2 subintervals |
| [0, 2] | 1.0 | $\frac{1}{3}[...]$ | Covers 2 subintervals |
| [0, 10] | 5.0 | $\frac{5}{3}[...]$ | Covers 2 subintervals |

**All use the SAME weights (1, 4, 1)!**
**Only h scales the result!**

---

## Part 11: Customized Step Size — Composite Rules

If you want a **custom step size** (like h = 0.001), you use:

$$\int_a^b f(x)\,dx \approx \sum_{j=0}^{N-1} \int_{x_j}^{x_{j+1}} f(x)\,dx$$

where each sub-integral uses the Newton–Cotes formula with step size h = 0.001.

**This is the COMPOSITE rule** (which we covered earlier).

---

## Part 12: Key Takeaway

**The nth formula for any Newton–Cotes rule is:**

$$\boxed{\int_a^b f(x)\,dx \approx h \sum_{i=0}^{n} w_i \cdot f_i}$$

where:
- **h = (b - a)/n** (not arbitrary; determined by number of nodes)
- **w_i** = fixed weights (depend only on rule number n)
- **f_i = f(a + i·h)** function values at equally spaced nodes
- **This formula works for ANY interval [a, b] and ANY number of nodes n!**

**The beauty:** Same weights, scales linearly with h!

