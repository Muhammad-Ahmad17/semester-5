# Newton Forward & Backward Divided Difference Solution

Data set:
\[
\begin{aligned}
\mathbf{x} &= [5,\;7,\;11,\;13,\;17],\\
\mathbf{y} &= [150,\;392,\;1452,\;2366,\;5202].
\end{aligned}
\]
We seek \(f(12)\) and \(f(14)\) using both Newton forward (anchored at \(x_0=5\)) and Newton backward (anchored at \(x_4=17\)) divided-difference forms.

---

## 1. Newton Forward Divided Difference (ascending nodes)

### 1.1 Divided-difference table
| order | value |
| --- | --- |
| \(f[x_0]\) | 150 |
| \(f[x_1]\) | 392 |
| \(f[x_2]\) | 1452 |
| \(f[x_3]\) | 2366 |
| \(f[x_4]\) | 5202 |

Higher-order entries:
\[
\begin{aligned}
&f[x_0,x_1] = 121, && f[x_1,x_2] = 265, && f[x_2,x_3] = 457, && f[x_3,x_4] = 709, \\
&f[x_0,x_1,x_2] = 24, && f[x_1,x_2,x_3] = 32, && f[x_2,x_3,x_4] = 42, \\
&f[x_0,x_1,x_2,x_3] = 1, && f[x_1,x_2,x_3,x_4] = 1, \\
&f[x_0,x_1,x_2,x_3,x_4] = 0.
\end{aligned}
\]
(Values follow from the recursive definition \(f[x_i,\ldots,x_{i+k}] = \frac{f[x_{i+1},\ldots,x_{i+k}] - f[x_i,\ldots,x_{i+k-1}]}{x_{i+k}-x_i}\).)

### 1.2 Forward-form interpolating polynomial

The Newton forward polynomial anchored at \(x_0=5\) is
\[
\begin{aligned}
P_{\text{f}}(x)
&= f[x_0]
 + (x-5) f[x_0,x_1]
 + (x-5)(x-7) f[x_0,x_1,x_2] \\
&\quad + (x-5)(x-7)(x-11) f[x_0,x_1,x_2,x_3]
 + (x-5)(x-7)(x-11)(x-13) f[x_0,\ldots,x_4].
\end{aligned}
\]
Substituting the coefficients above and simplifying gives
\[
P_{\text{f}}(x) = x^3 + x^2.
\]

### 1.3 Evaluations
\[
\begin{aligned}
P_{\text{f}}(12) &= 12^3 + 12^2 = 1872,\\
P_{\text{f}}(14) &= 14^3 + 14^2 = 2940.
\end{aligned}
\]

---

## 2. Newton Backward Divided Difference (descending nodes)

We re-order the nodes from the top of the table so that the backward scheme is anchored at \(x_4 = 17\).

### 2.1 Divided-difference table (descending)

| order | value at \(x_4\) |
| --- | --- |
| \(f[x_4]\) | 5202 |
| \(f[x_4,x_3]\) | 709 |
| \(f[x_4,x_3,x_2]\) | 42 |
| \(f[x_4,x_3,x_2,x_1]\) | 1 |
| \(f[x_4,x_3,x_2,x_1,x_0]\) | 0 |

(These match the downward diagonal of the full divided-difference table constructed with nodes \(17,13,11,7,5\).)

### 2.2 Backward-form interpolating polynomial
The Newton backward expression about \(x_4=17\) is
\[
\begin{aligned}
P_{\text{b}}(x)
&= f[x_4]
 + (x-17) f[x_4,x_3]
 + (x-17)(x-13) f[x_4,x_3,x_2] \\
&\quad + (x-17)(x-13)(x-11) f[x_4,x_3,x_2,x_1]
 + (x-17)(x-13)(x-11)(x-7) f[x_4,\ldots,x_0].
\end{aligned}
\]
Plugging in the coefficients yields the same simplified polynomial:
\[
P_{\text{b}}(x) = x^3 + x^2.
\]

### 2.3 Evaluations
\[
\begin{aligned}
P_{\text{b}}(12) &= 1872,\\
P_{\text{b}}(14) &= 2940.
\end{aligned}
\]

---

## 3. Conclusion
Both Newton forward and backward divided-difference constructions lead to the identical interpolant \(f(x) = x^3 + x^2\) for the given data, so the desired function values are
\(f(12) = 1872\) and \(f(14) = 2940\).
