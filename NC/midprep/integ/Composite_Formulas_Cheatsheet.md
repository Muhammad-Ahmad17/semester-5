# Newton–Cotes Integration Rules — Composite Formula Cheatsheet

## Trapezoidal Rule (n=1)

$$I = \frac{h}{2}[y_0 + 2(y_1 + y_2 + \cdots + y_{n-1}) + y_n]$$

**Explanation:** Approximates f(x) with straight lines between each pair of consecutive points. Each interval forms a trapezoid. The area under each trapezoid is h/2 × (sum of two heights). Interior points are counted twice (coefficient 2) because they're shared between adjacent trapezoids.

**When to use:** When you need a quick, simple approximation. O(h²) accuracy.

where:
- h = (b - a)/n
- n = number of intervals
- y_i = f(x_i)

---

## Simpson 1/3 Rule (n=2)

$$I = \frac{h}{3}[y_0 + 4(y_1 + y_3 + y_5 + \cdots + y_{n-1}) + 2(y_2 + y_4 + y_6 + \cdots + y_{n-2}) + y_n]$$

**Explanation:** Approximates f(x) with parabolas (quadratic polynomials) fitted through every 3 consecutive points. Every pair of intervals (h = (b-a)/2 for each pair) uses the single-panel formula h/3(y₀ + 4y₁ + y₂). The "1/3" name comes from the h/3 coefficient. Odd-indexed interior points get coefficient 4 (they're at the peaks of parabolas), even-indexed get coefficient 2 (they're shared between adjacent parabolas).

**When to use:** Best balance of accuracy and simplicity. O(h⁴) accuracy. Very popular!

where:
- h = (b - a)/n
- **n must be EVEN**
- Odd indices (1, 3, 5, ..., n-1) have coefficient 4
- Even indices (2, 4, 6, ..., n-2) have coefficient 2
- Endpoints (y₀, yₙ) have coefficient 1

---

## Simpson 3/8 Rule (n=3)

$$I = \frac{3h}{8}[y_0 + 3(y_1 + y_2 + y_4 + y_5 + y_7 + y_8 + \cdots) + 2(y_3 + y_6 + y_9 + \cdots) + y_n]$$

Or equivalently:

$$I = \frac{3h}{8}[y_0 + 3\sum_{i \equiv 1,2 \pmod{3}} y_i + 2\sum_{i \equiv 0 \pmod{3}, i>0, i<n} y_i + y_n]$$

**Explanation:** Approximates f(x) with cubic polynomials (degree 3) fitted through every 4 consecutive points. Every group of 3 intervals uses formula 3h/8(y₀ + 3y₁ + 3y₂ + y₃). The "3/8" name comes from the coefficient. The weight pattern repeats every 3 intervals: points at positions (i mod 3) = 1,2 get coefficient 3; points at (i mod 3) = 0 (except endpoints) get coefficient 2.

**When to use:** When n must be divisible by 3. Also O(h⁴) accurate like Simpson 1/3, but different weight distribution.

where:
- h = (b - a)/n
- **n must be MULTIPLE OF 3**
- Indices where (i mod 3) = 1 or 2: coefficient 3
- Indices where (i mod 3) = 0 (except 0 and n): coefficient 2
- Endpoints (y₀, yₙ): coefficient 1

---

## Boole's Rule (n=4)

$$I = \frac{2h}{45}[7y_0 + 32(y_1 + y_5 + y_9 + \cdots) + 12(y_2 + y_6 + y_{10} + \cdots) + 32(y_3 + y_7 + y_{11} + \cdots) + 7(y_4 + y_8 + y_{12} + \cdots) + y_n]$$

Or more clearly:

$$I = \frac{2h}{45}[7y_0 + 32\sum_{i \equiv 1 \pmod{4}} y_i + 12\sum_{i \equiv 2 \pmod{4}} y_i + 32\sum_{i \equiv 3 \pmod{4}} y_i + 7\sum_{i \equiv 0 \pmod{4}, i>0} y_i + y_n]$$

**Explanation:** Approximates f(x) with quartic polynomials (degree 4) through 5 consecutive points. Every 4 intervals uses single formula 2h/45(7y₀ + 32y₁ + 12y₂ + 32y₃ + 7y₄). The weights (7, 32, 12, 32) repeat, giving maximum weight 32 to the interior points. Much higher accuracy than Simpson's rules!

**When to use:** When you need better accuracy and n is divisible by 4. O(h⁶) accuracy — excellent!

**Weight pattern repeats every 4 intervals:** 7, 32, 12, 32

where:
- h = (b - a)/n
- **n must be MULTIPLE OF 4**
- Coefficient pattern: 7, 32, 12, 32, repeating
- Last y_n gets coefficient 7

---

## n=5 Rule (6-point Closed Newton–Cotes)

$$I = \frac{5h}{288}[19y_0 + 75(y_1 + y_6 + y_{11} + y_{16} + \cdots) + 50(y_2 + y_3 + y_7 + y_8 + y_{12} + y_{13} + \cdots) + 75(y_4 + y_9 + y_{14} + \cdots) + y_n]$$

**Weight pattern (repeats every 5 intervals):** 19, 75, 50, 50, 75

**Explanation:** Uses degree-5 polynomials fitted through 6 consecutive points. The weight pattern repeats every 5 intervals: position 0 → 19, position 1 → 75, position 2 → 50, position 3 → 50, position 4 → 75, then repeats at position 5 (becomes position 0 of next group). So y₄, y₉, y₁₄... all get coefficient 75, and y₂, y₃, y₇, y₈... all get coefficient 50.

**When to use:** When n is divisible by 5 and you need very high accuracy. O(h⁶) convergence.

where:
- h = (b - a)/n
- **n must be MULTIPLE OF 5**
- Coefficient pattern repeats: 19, 75, 50, 50, 75, repeating every 5 points
- y₁, y₆, y₁₁... get 75
- y₂, y₃, y₇, y₈... get 50
- y₄, y₉, y₁₄... get 75

---

## n=6 Rule (7-point Closed Newton–Cotes)

$$I = \frac{h}{140}[41y_0 + 216(y_1 + y_7 + y_{13} + \cdots) + 27(y_2 + y_6 + y_8 + y_{12} + \cdots) + 272(y_3 + y_9 + y_{15} + \cdots) + 27(y_4 + y_{10} + \cdots) + 216(y_5 + y_{11} + \cdots) + y_n]$$

**Weight pattern (repeats every 6 intervals):** 41, 216, 27, 272, 27, 216

**Explanation:** The most accurate closed Newton–Cotes rule using degree-6 polynomials through 7 consecutive points. The weight pattern repeats: position 0 → 41, position 1 → 216, position 2 → 27, position 3 → 272 (maximum!), position 4 → 27, position 5 → 216, then repeats. Notice the **symmetric pattern** (41...216...27...272...27...216) that emphasizes the center position 3 with weight 272. This gives exceptional accuracy.

**When to use:** When absolute maximum accuracy is required and n is divisible by 6. O(h⁸) convergence — highest accuracy among closed Newton–Cotes!

where:
- h = (b - a)/n
- **n must be MULTIPLE OF 6**
- Coefficient pattern repeats: 41, 216, 27, 272, 27, 216, then repeats
- Center position always gets 272 (maximum weight)

---

## Quick Reference Table

| Rule | Coefficient | Divisor | Pattern (repeating) | n Multiple |
|---|---|---|---|---|
| **Trapezoidal** | h/2 | 1 | 1, 2, 2, ..., 2, 1 | Any |
| **Simpson 1/3** | h/3 | 1 | 1, 4, 2, 4, 2, ..., 4, 1 | Even |
| **Simpson 3/8** | 3h/8 | 1 | 1, 3, 3, 2, 3, 3, 2, ..., 1 | ÷3 |
| **Boole** | 2h/45 | 1 | 7, 32, 12, 32, ..., 7 | ÷4 |
| **n=5** | 5h/288 | 1 | 19, 75, 50, 50, 75, ... | ÷5 |
| **n=6** | h/140 | 1 | 41, 216, 27, 272, 27, 216, ... | ÷6 |

---

## Worked Solutions with Examples

### Example n=5: ∫₀¹ 1/(x²+1) dx using n=5 Rule with n=10 intervals

**Step 1:** h = (1 - 0)/10 = 0.1

**Step 2:** Calculate y values at x = 0, 0.1, 0.2, ..., 1.0:
- y₀ = 1.0000
- y₁ = 0.9901
- y₂ = 0.9615
- y₃ = 0.9174
- y₄ = 0.8621
- y₅ = 0.8000
- y₆ = 0.7353
- y₇ = 0.6711
- y₈ = 0.6098
- y₉ = 0.5525
- y₁₀ = 0.5000

**Step 3:** Group by weight pattern (19, 75, 50, 50, 75, repeating):
- Position 0 (y₀): coefficient 19 → 19(1.0000) = 19.0000
- Position 1 (y₁, y₆): coefficient 75 → 75(0.9901 + 0.7353) = 75(1.7254) = 129.405
- Position 2 (y₂, y₇): coefficient 50 → 50(0.9615 + 0.6711) = 50(1.6326) = 81.630
- Position 3 (y₃, y₈): coefficient 50 → 50(0.9174 + 0.6098) = 50(1.5272) = 76.360
- Position 4 (y₄, y₉): coefficient 75 → 75(0.8621 + 0.5525) = 75(1.4146) = 106.095
- Position 5 (y₅, y₁₀): coefficient 75 → 75(0.8000 + 0.5000) = 75(1.3000) = 97.500

**Step 4:** Apply formula:
$$I \approx \frac{5 \times 0.1}{288}[19.0 + 129.405 + 81.630 + 76.360 + 106.095 + 97.500]$$
$$= \frac{0.5}{288}[509.990]$$
$$= \frac{509.990}{288}$$
$$= 1.7708...$$

Wait, this needs recalculation. Let me recalculate correctly:

The pattern repeats every 5 intervals. With n=10 intervals, we have positions 0-10 (11 points):
- Position 0 mod 5 = 0 (y₀): 19
- Position 1 mod 5 = 1 (y₁, y₆): 75
- Position 2 mod 5 = 2 (y₂, y₇): 50  
- Position 3 mod 5 = 3 (y₃, y₈): 50
- Position 4 mod 5 = 4 (y₄, y₉): 75
- Position 5 mod 5 = 0 (y₅): 19
- Position 6 mod 5 = 1 (y₁₀): 75... but y₁₀ is endpoint!

For composite rules, endpoints typically use their pattern value. Let me use the correct approach:

$$I \approx \frac{5h}{288}[19y_0 + 75(y_1 + y_6) + 50(y_2 + y_3 + y_7 + y_8) + 75(y_4 + y_9) + 19y_5 + \text{next cycle}]$$

Actually, with 11 points and repeating pattern, we apply it carefully:

$$I \approx \frac{0.5}{288}[19(1.0) + 75(0.9901 + 0.7353) + 50(0.9615 + 0.9174 + 0.6711 + 0.6098) + 75(0.8621 + 0.5525) + 19(0.8)]$$

Wait, let me reconsider. With n=10, the last point y₁₀ should get treated. Let me use n=5 with 6 points instead for clarity:

**Step 1:** h = (1 - 0)/5 = 0.2

**Step 2:** Calculate y values at x = 0, 0.2, 0.4, 0.6, 0.8, 1.0:
- y₀ = 1.0000
- y₁ = 0.9615
- y₂ = 0.8621
- y₃ = 0.7353
- y₄ = 0.6098
- y₅ = 0.5000

**Step 3:** Apply n=5 formula with 6 points:
$$I \approx \frac{5(0.2)}{288}[19y_0 + 75y_1 + 50y_2 + 50y_3 + 75y_4 + 19y_5]$$
$$= \frac{1}{288}[19(1.0) + 75(0.9615) + 50(0.8621) + 50(0.7353) + 75(0.6098) + 19(0.5)]$$
$$= \frac{1}{288}[19 + 72.1125 + 43.105 + 36.765 + 45.735 + 9.5]$$
$$= \frac{226.2175}{288}$$
$$= 0.78543$$

**Error:** |π/4 - 0.78543| = |0.785398 - 0.78543| ≈ 0.000032

---

### Example n=6: ∫₀¹ 1/(x²+1) dx using n=6 Rule with n=6 intervals

**Step 1:** h = (1 - 0)/6 = 1/6 ≈ 0.16667

**Step 2:** Calculate y values at x = 0, 1/6, 2/6, 3/6, 4/6, 5/6, 1:
- y₀ = 1.0000
- y₁ = 1/(1/36 + 1) = 36/37 ≈ 0.97297
- y₂ = 1/(4/36 + 1) = 1/(1/9 + 1) = 9/10 = 0.90000
- y₃ = 1/(9/36 + 1) = 1/(1/4 + 1) = 4/5 = 0.80000
- y₄ = 1/(16/36 + 1) = 1/(4/9 + 1) = 9/13 ≈ 0.69231
- y₅ = 1/(25/36 + 1) = 36/61 ≈ 0.59016
- y₆ = 0.5000

**Step 3:** Apply n=6 formula (weight pattern: 41, 216, 27, 272, 27, 216):
$$I \approx \frac{h}{140}[41y_0 + 216y_1 + 27y_2 + 272y_3 + 27y_4 + 216y_5 + 41y_6]$$
$$= \frac{1/6}{140}[41(1.0) + 216(0.97297) + 27(0.9) + 272(0.8) + 27(0.69231) + 216(0.59016) + 41(0.5)]$$
$$= \frac{1}{840}[41 + 210.1752 + 24.3 + 217.6 + 18.6923 + 127.4346 + 20.5]$$
$$= \frac{1}{840}[659.7921]$$
$$= 0.78546$$

**Error:** |π/4 - 0.78546| = |0.785398 - 0.78546| ≈ 0.000062

---

## How to Use (Step-by-Step)

**Example:** ∫₀¹ 1/(x²+1) dx using Simpson 1/3 with n=4 intervals

**Step 1:** h = (1 - 0)/4 = 0.25

**Step 2:** Calculate y values at x = 0, 0.25, 0.5, 0.75, 1:
- y₀ = 1
- y₁ = 1/(0.25² + 1) = 0.9412
- y₂ = 1/(0.5² + 1) = 0.8
- y₃ = 1/(0.75² + 1) = 0.64
- y₄ = 1/(1² + 1) = 0.5

**Step 3:** Identify odd and even indices:
- Odd (1, 3): y₁ = 0.9412, y₃ = 0.64
- Even (2): y₂ = 0.8

**Step 4:** Apply Simpson 1/3:
$$I = \frac{0.25}{3}[1 + 4(0.9412 + 0.64) + 2(0.8) + 0.5]$$
$$= \frac{0.25}{3}[1 + 4(1.5812) + 1.6 + 0.5]$$
$$= \frac{0.25}{3}[1 + 6.3248 + 1.6 + 0.5]$$
$$= \frac{0.25}{3}[9.4248]$$
$$= \frac{2.3562}{3} = 0.7854$$

---

## Summary: What to Remember for Exam

1. **Trapezoidal:** All interior points get coefficient 2
2. **Simpson 1/3:** Odd points get 4, even points get 2
3. **Simpson 3/8:** Repeating pattern 1, 3, 3, 2
4. **Boole:** Repeating pattern 7, 32, 12, 32
5. **Higher n:** Use repeating weight patterns

**Key:** h = (b - a)/n, multiply all values by h/(divisor)

