# Newton Forward & Backward Interpolation — Proof Notes

> Path: `NC/midprep/Newton_Interpolation_Proofs.md`
> Goal: provide exam-ready derivations with readable math blocks.

## Table of Contents
1. [Notation & Preliminaries](#notation--preliminaries)
2. [Newton Forward Interpolation](#1-newton-forward-interpolation)
3. [Newton Backward Interpolation](#2-newton-backward-interpolation)
4. [Truncation Error Templates](#3-truncation-error-templates)
5. [Exam Presentation Checklist](#4-exam-presentation-checklist)

---

## Notation & Preliminaries

- Equally spaced nodes: \(x_k = x_0 + k h\) with step size \(h\).
- Function samples: \(y_k = f(x_k)\).
- **Forward differences**
  - \(\Delta y_k = y_{k+1} - y_k\)
  - \(\Delta^m y_k = \Delta(\Delta^{m-1} y_k)\)
- **Backward differences**
  - \(\nabla y_k = y_k - y_{k-1}\)
  - \(\nabla^m y_k = \nabla(\nabla^{m-1} y_k)\)
- Reduced variable for forward form: \(p = \dfrac{x - x_0}{h}\)
- Reduced variable for backward form: \(p = \dfrac{x - x_n}{h}\)
- Falling factorial shorthand: \((p)_m = p(p-1)\cdots(p-m+1)\)

These definitions match the class PPT, so you can paste formulas directly.

---

## 1. Newton Forward Interpolation

### Theorem (Forward Form)
For nodes near the **top** of the difference table, the interpolating polynomial is

$$
f(x) = y_0
		+ p\,\Delta y_0
		+ \frac{p(p-1)}{2!} \Delta^2 y_0
		+ \frac{p(p-1)(p-2)}{3!} \Delta^3 y_0
		+ \cdots
		+ \frac{(p)_n}{n!} \Delta^n y_0,
$$
where \(p = (x - x_0)/h\).

### Derivation Outline
1. **Newton basis.** Any degree-\(n\) polynomial can be written as
	$$
	P(x) = a_0 + a_1(x - x_0) + a_2(x - x_0)(x - x_1) + \cdots + a_n \prod_{k=0}^{n-1} (x - x_k).
	$$
2. **Exploit equal spacing.** Substitute \(x_k = x_0 + k h\) and define \(p = (x - x_0)/h\). Each factor becomes
	$$
	\prod_{k=0}^{m-1} (x - x_k) = h^m (p)_m.
	$$
3. **Match finite differences.** Evaluate the polynomial at the tabulated points and use the operator identity \(\Delta (p)_m = m (p)_{m-1}\) at \(p = 0\) to isolate coefficients:
	$$
	a_m = \frac{\Delta^m y_0}{m!\, h^m}.
	$$
4. **Assemble the series.** Substitute \(a_m\) into \(P(x)\); the powers of \(h\) cancel, yielding the stated formula.
5. **Uniqueness.** The Newton basis functions are linearly independent, so the coefficient set found above is unique—hence \(P(x)\) equals the interpolating polynomial \(f(x)\).

### Intuition
- The forward difference column captures slopes anchored at \(x_0\); higher-order terms vanish automatically when \(x\) coincides with a later tabulated node.
- Using \(p\) keeps the algebra dimensionless and shows quickly when the series terms become small.

---

## 2. Newton Backward Interpolation

### Theorem (Backward Form)
When \(x\) is closer to \(x_n\) than to \(x_0\), expand about the last node:

$$
f(x) = y_n
		+ p\,\nabla y_n
		+ \frac{p(p+1)}{2!} \nabla^2 y_n
		+ \frac{p(p+1)(p+2)}{3!} \nabla^3 y_n
		+ \cdots
		+ \frac{(p+ n - 1)!}{n!\,(p-1)!}\, \nabla^n y_n,
$$
with \(p = (x - x_n)/h\).

### Derivation Outline
1. **Reverse Newton basis.** Write
	$$
	P(x) = b_0 + b_1(x - x_n) + b_2(x - x_n)(x - x_{n-1}) + \cdots + b_n \prod_{j=0}^{n-1} (x - x_{n-j}).
	$$
2. **Normalize factors.** Using \(x_{n-j} = x_n - j h\) gives
	$$
	\prod_{j=0}^{m-1} (x - x_{n-j}) = h^m p(p+1)\cdots(p+m-1).
	$$
3. **Relate to backward differences.** Apply \(\nabla\) repeatedly at \(x_n\) and use \(\nabla (p)_m = m (p+1)_{m-1}\) evaluated at \(p = 0\) to isolate
	$$
	b_m = \frac{\nabla^m y_n}{m!\, h^m}.
	$$
4. **Insert coefficients.** Cancelling \(h^m\) again delivers the backward series shown above.
5. **Consistency.** When \(x = x_n - r h\) (integer \(r\)), terms of order greater than \(r\) vanish, so \(P(x)\) collapses to \(y_{n-r}\), matching the table exactly.

### When to choose each form
- **Forward form** minimizes rounding/truncation error near \(x_0\).
- **Backward form** is more stable when evaluating near \(x_n\) because backward differences \(\nabla^m y_n\) remain small there.

---

## 3. Truncation Error Templates

Both Newton series come from the Taylor expansion of \(f(x)\) and inherit similar remainder terms:

- **Forward expansion error**
  $$
  E_f(x) = \frac{p(p-1)\cdots(p-n)}{(n+1)!} h^{n+1} f^{(n+1)}(\xi), \qquad \xi \in [x_0, x_0 + n h].
  $$

- **Backward expansion error**
  $$
  E_b(x) = \frac{p(p+1)\cdots(p+n)}{(n+1)!} h^{n+1} f^{(n+1)}(\eta), \qquad \eta \in [x_n - n h, x_n].
  $$

These expressions explain why \(|p| < 1\) (i.e., evaluating near the anchor node) yields smaller truncation errors.

---

## 4. Exam Presentation Checklist
1. State the Newton basis and mention equal spacing.
2. Introduce \(p\) and rewrite the basis products as \(h^m (p)_m\).
3. Show how forward/backward differences isolate coefficients \(a_m\) or \(b_m\).
4. Write the final series (forward or backward) with factorial denominators.
5. Optionally mention the corresponding error term or which form is more stable for the given \(x\).

Following this script typically secures full proof marks while keeping the math tidy.
@import "./README_QUIZ_PREP.txt"
