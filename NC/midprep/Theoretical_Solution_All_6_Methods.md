# Theoretical Solution: Integration of f(x) = 1/(x²+1) from 0 to 1 using Closed Newton–Cotes Methods

**Exact Value:** ∫₀¹ 1/(x²+1) dx = arctan(1) - arctan(0) = π/4 ≈ 0.7853981634

---

## Method 1: Trapezoidal Rule (n=1)

**Formula (single panel):**
$$I \approx \frac{h}{2}[f(x_0) + f(x_1)]$$

where h = b - a = 1 - 0 = 1, x₀ = 0, x₁ = 1

**Step 1: Identify nodes and spacing**
- x₀ = 0
- x₁ = 1
- h = 1

**Step 2: Evaluate function at nodes**
- f(x₀) = f(0) = 1/(0² + 1) = 1/1 = **1**
- f(x₁) = f(1) = 1/(1² + 1) = 1/2 = **0.5**

**Step 3: Apply formula**
$$I \approx \frac{1}{2}(1 + 0.5) = \frac{1.5}{2} = \mathbf{0.75}$$

**Error:**
$$E = |0.75 - 0.7854| = \mathbf{0.0354}$$

---

## Method 2: Simpson's 1/3 Rule (n=2)

**Formula (single panel):**
$$I \approx \frac{h}{3}[f(x_0) + 4f(x_1) + f(x_2)]$$

where h = (b - a)/2 = 1/2 = 0.5, with 3 equally spaced nodes

**Step 1: Identify nodes and spacing**
- h = 0.5
- x₀ = 0
- x₁ = 0 + 1·(0.5) = 0.5
- x₂ = 0 + 2·(0.5) = 1.0

**Step 2: Evaluate function at nodes**
- f(x₀) = f(0) = 1/(0² + 1) = **1**
- f(x₁) = f(0.5) = 1/(0.5² + 1) = 1/(0.25 + 1) = 1/1.25 = **0.8**
- f(x₂) = f(1) = 1/(1² + 1) = 1/2 = **0.5**

**Step 3: Apply formula**
$$I \approx \frac{0.5}{3}[1 + 4(0.8) + 0.5]$$
$$= \frac{0.5}{3}[1 + 3.2 + 0.5]$$
$$= \frac{0.5}{3}(4.7)$$
$$= \frac{2.35}{3} = \mathbf{0.783333}$$

**Error:**
$$E = |0.783333 - 0.785398| = \mathbf{0.002065}$$

---

## Method 3: Simpson's 3/8 Rule (n=3)

**Formula (single panel):**
$$I \approx \frac{3h}{8}[f(x_0) + 3f(x_1) + 3f(x_2) + f(x_3)]$$

where h = (b - a)/3 = 1/3 ≈ 0.3333, with 4 equally spaced nodes

**Step 1: Identify nodes and spacing**
- h = 1/3 ≈ 0.3333
- x₀ = 0
- x₁ = 0 + 1·(1/3) = 1/3 ≈ 0.3333
- x₂ = 0 + 2·(1/3) = 2/3 ≈ 0.6667
- x₃ = 0 + 3·(1/3) = 1

**Step 2: Evaluate function at nodes**
- f(x₀) = f(0) = 1/(0² + 1) = **1**
- f(x₁) = f(1/3) = 1/((1/3)² + 1) = 1/(1/9 + 1) = 1/(10/9) = 9/10 = **0.9**
- f(x₂) = f(2/3) = 1/((2/3)² + 1) = 1/(4/9 + 1) = 1/(13/9) = 9/13 ≈ **0.6923**
- f(x₃) = f(1) = 1/(1² + 1) = 1/2 = **0.5**

**Step 3: Apply formula**
$$I \approx \frac{3 \cdot (1/3)}{8}[1 + 3(0.9) + 3(0.6923) + 0.5]$$
$$= \frac{1}{8}[1 + 2.7 + 2.0769 + 0.5]$$
$$= \frac{1}{8}(6.2769)$$
$$= \mathbf{0.7846}$$

**Error:**
$$E = |0.7846 - 0.7854| = \mathbf{0.000783}$$

---

## Method 4: Boole's Rule (Closed n=4, 5-point)

**Formula (single panel):**
$$I \approx \frac{2h}{45}[7f(x_0) + 32f(x_1) + 12f(x_2) + 32f(x_3) + 7f(x_4)]$$

where h = (b - a)/4 = 1/4 = 0.25, with 5 equally spaced nodes

**Step 1: Identify nodes and spacing**
- h = 0.25
- x₀ = 0
- x₁ = 0.25
- x₂ = 0.50
- x₃ = 0.75
- x₄ = 1.0

**Step 2: Evaluate function at nodes**
- f(x₀) = f(0) = 1/(0² + 1) = **1**
- f(x₁) = f(0.25) = 1/(0.25² + 1) = 1/1.0625 ≈ **0.9412**
- f(x₂) = f(0.50) = 1/(0.50² + 1) = 1/1.25 = **0.8**
- f(x₃) = f(0.75) = 1/(0.75² + 1) = 1/1.5625 ≈ **0.6400**
- f(x₄) = f(1) = 1/(1² + 1) = **0.5**

**Step 3: Apply formula**
$$I \approx \frac{2(0.25)}{45}[7(1) + 32(0.9412) + 12(0.8) + 32(0.6400) + 7(0.5)]$$
$$= \frac{0.5}{45}[7 + 30.118 + 9.6 + 20.48 + 3.5]$$
$$= \frac{0.5}{45}(70.698)$$
$$= \frac{35.349}{45} = \mathbf{0.785533}$$

**Error:**
$$E = |0.785533 - 0.785398| = \mathbf{0.000135}$$

---

## Method 5: Closed Newton–Cotes n=5 (6-point)

**Formula (single panel):**
$$I \approx \frac{5h}{288}[19f(x_0) + 75f(x_1) + 50f(x_2) + 50f(x_3) + 75f(x_4) + 19f(x_5)]$$

where h = (b - a)/5 = 1/5 = 0.2, with 6 equally spaced nodes

**Step 1: Identify nodes and spacing**
- h = 0.2
- x₀ = 0.0
- x₁ = 0.2
- x₂ = 0.4
- x₃ = 0.6
- x₄ = 0.8
- x₅ = 1.0

**Step 2: Evaluate function at nodes**
- f(x₀) = f(0.0) = 1/(0² + 1) = **1.0**
- f(x₁) = f(0.2) = 1/(0.2² + 1) = 1/1.04 ≈ **0.9615**
- f(x₂) = f(0.4) = 1/(0.4² + 1) = 1/1.16 ≈ **0.8621**
- f(x₃) = f(0.6) = 1/(0.6² + 1) = 1/1.36 ≈ **0.7353**
- f(x₄) = f(0.8) = 1/(0.8² + 1) = 1/1.64 ≈ **0.6098**
- f(x₅) = f(1.0) = 1/(1² + 1) = **0.5**

**Step 3: Apply formula**
$$I \approx \frac{5(0.2)}{288}[19(1.0) + 75(0.9615) + 50(0.8621) + 50(0.7353) + 75(0.6098) + 19(0.5)]$$
$$= \frac{1}{288}[19 + 72.1125 + 43.105 + 36.765 + 45.735 + 9.5]$$
$$= \frac{226.218}{288} = \mathbf{0.785470}$$

**Error:**
$$E = |0.785470 - 0.785398| = \mathbf{0.000072}$$

---

## Method 6: Closed Newton–Cotes n=6 (7-point)

**Formula (single panel):**
$$I \approx \frac{6h}{840}[41f(x_0) + 216f(x_1) + 27f(x_2) + 272f(x_3) + 27f(x_4) + 216f(x_5) + 41f(x_6)]$$

where h = (b - a)/6 = 1/6 ≈ 0.1667, with 7 equally spaced nodes

**Step 1: Identify nodes and spacing**
- h = 1/6 ≈ 0.16667
- x₀ = 0
- x₁ = 1/6 ≈ 0.16667
- x₂ = 2/6 = 1/3 ≈ 0.33333
- x₃ = 3/6 = 1/2 = 0.5
- x₄ = 4/6 = 2/3 ≈ 0.66667
- x₅ = 5/6 ≈ 0.83333
- x₆ = 1.0

**Step 2: Evaluate function at nodes**
- f(x₀) = f(0) = 1/(0² + 1) = **1.0**
- f(x₁) = f(1/6) = 1/((1/6)² + 1) = 1/(1/36 + 1) = 36/37 ≈ **0.9730**
- f(x₂) = f(1/3) = 1/((1/3)² + 1) = 1/(1/9 + 1) = 9/10 = **0.9**
- f(x₃) = f(1/2) = 1/(1/4 + 1) = 4/5 = **0.8**
- f(x₄) = f(2/3) = 1/((2/3)² + 1) = 1/(4/9 + 1) = 9/13 ≈ **0.6923**
- f(x₅) = f(5/6) = 1/((5/6)² + 1) = 1/(25/36 + 1) = 36/61 ≈ **0.5902**
- f(x₆) = f(1) = 1/(1² + 1) = **0.5**

**Step 3: Apply formula**
$$I \approx \frac{6(1/6)}{840}[41(1.0) + 216(0.9730) + 27(0.9) + 272(0.8) + 27(0.6923) + 216(0.5902) + 41(0.5)]$$
$$= \frac{1}{840}[41 + 210.168 + 24.3 + 217.6 + 18.692 + 127.483 + 20.5]$$
$$= \frac{659.743}{840} = \mathbf{0.785408}$$

**Error:**
$$E = |0.785408 - 0.785398| = \mathbf{0.000010}$$

---

## Comparison Summary Table

| Method | n | Single-Panel Approx | Absolute Error | Rel. Error |
|--------|---|---|---|---|
| Trapezoidal | 1 | 0.75000 | 0.03540 | 0.0451 |
| Simpson 1/3 | 2 | 0.78333 | 0.00207 | 0.0026 |
| Simpson 3/8 | 3 | 0.78462 | 0.00078 | 0.0010 |
| Boole's | 4 | 0.78553 | 0.00014 | 0.0002 |
| Closed n=5 | 5 | 0.78547 | 0.00007 | 0.0001 |
| Closed n=6 | 6 | 0.78541 | 0.00001 | 0.00001 |

---

## Key Observations

1. **Convergence:** Error decreases as n increases; higher-order rules use more nodes and achieve better accuracy with fewer panels.

2. **Order of Accuracy:**
   - n=1: O(h²) — slowest
   - n=2,3: O(h⁴) — moderate
   - n=4,5,6: O(h⁶) or better — fast convergence

3. **Practical Choice:** For hand calculation, Simpson's 1/3 (n=2) offers excellent accuracy with minimal nodes. For higher accuracy, use composite Simpson 1/3 (divide [0,1] into many equal parts and apply n=2 rule repeatedly).

4. **Weights:** Coefficients are carefully chosen using Lagrange interpolation to maximize the degree of precision. The sum of normalized weights always equals the interval width (h for composite, or h multiplied by n for single panel).

---

## How to Use These for Composite Rules

If you divide [0,1] into N equal subintervals of width Δh = 1/N:

**Composite Simpson 1/3** (requires N even):
Apply the Simpson 1/3 formula to each pair of consecutive subintervals. For N=4:
- Panel 1: [0, 0.25, 0.5]
- Panel 2: [0.5, 0.75, 1.0]
- Sum the two approximations (with center point counted once)

This gives **much higher accuracy** than the single-panel approximation shown above.

---

**End of Theoretical Solution**
