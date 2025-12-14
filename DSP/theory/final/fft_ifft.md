## FFT / IFFT Quick Problem-Solving Guide

This is a compact playbook to go from “surface-level” to “can solve exam-style problems”. Focus on these checkpoints.

---

### 1) Core definitions (must recall fast)
- **DFT:** $X[k] = \sum_{n=0}^{N-1} x[n] e^{-j 2\pi kn/N}$
- **IDFT:** $x[n] = \frac{1}{N} \sum_{k=0}^{N-1} X[k] e^{+j 2\pi kn/N}$
- **FFT:** an $O(N\log N)$ way to compute the DFT (algorithm, not a different transform).
- **Bin frequency:** $f_k = k\, f_s / N$ ; resolution $\Delta f = f_s / N$.
- **Nyquist:** highest unaliased freq is $f_s/2$ (bin $N/2$ when $N$ even).

---

### 2) What to do in typical questions
1) **Identify $N$ and $f_s$** (often hidden). Compute $\Delta f$ and $f_k$.
2) **Map time sinusoids to bins:**
	- For $x[n] = A\cos(2\pi f_0 n / f_s + \phi)$, the positive bin index is $k_0 = f_0 N / f_s$.
	- Real signals: energy appears as symmetric bins $k_0$ and $N-k_0$ (complex conjugates).
3) **Compute magnitude/phase:** $|X[k]| = \sqrt{\Re^2 + \Im^2}$, $\angle X[k] = \operatorname{atan2}(\Im, \Re)$.
4) **Use symmetries:**
	- Time shift: $x[n-n_0] \Rightarrow X[k] e^{-j 2\pi k n_0/N}$.
	- Modulation: $x[n] e^{j 2\pi k_0 n/N} \Rightarrow X[k-k_0]$ (bin shift).
	- Conjugate symmetry for real $x[n]$: $X[k] = X^*[N-k]$.
5) **Energy check (Parseval):** $\sum |x[n]|^2 = \frac{1}{N}\sum |X[k]|^2$.
6) **Circular convolution via DFT:** $y[n] = \operatorname{IDFT}( \operatorname{DFT}(x)\cdot\operatorname{DFT}(h) )$; zero-pad to avoid wrap.
7) **IFFT problems:** Same as FFT but with $+j$ sign and final $1/N$ scaling.

---

### 3) Radix-2 FFT (hand-calculation template)
Use when $N=2^m$ (4, 8, 16 points). Steps:
1) **Bit-reverse order** inputs (or keep straight and combine carefully).
2) **Butterfly per stage:** for pair $(a,b)$ with twiddle $W_N^k = e^{-j2\pi k/N}$:
	- $X_1 = a + W b$
	- $X_2 = a - W b$
3) **Twiddles you must know by heart:**
	- $W_4^1 = e^{-j\pi/2} = -j$
	- $W_8^1 = e^{-j\pi/4} = \tfrac{1}{\sqrt{2}}(1-j)$
	- $W_8^2 = e^{-j\pi/2} = -j$
	- $W_8^3 = e^{-j3\pi/4} = -\tfrac{1}{\sqrt{2}}(1+j)$

**8-point mini-example (very common):** $x=[1,2,3,4,0,0,0,0]$.
1) Evens: $[1,3,0,0] \Rightarrow$ 4-pt FFT: $E=[4,1,0,-1]$.
2) Odds: $[2,4,0,0] \Rightarrow$ 4-pt FFT: $O=[6,2,0,-2]$.
3) Combine with $W_8^k$:
	- $k=0$: $X[0]=4+6=10$, $X[4]=4-6=-2$
	- $k=1$: $X[1]=1 + W_8^1\cdot2 = 1 + (\tfrac{1}{\sqrt{2}}-j\tfrac{1}{\sqrt{2}})2 = 1+\sqrt{2} - j\sqrt{2}$
				 $X[5]=1 - W_8^1\cdot2 = 1-\sqrt{2} + j\sqrt{2}$
	- $k=2$: $X[2]=0 + (-j)\cdot0 = 0$, $X[6]=0 - (-j)\cdot0 = 0$
	- $k=3$: $X[3]=-1 + W_8^3\cdot(-2) = -1 + [ -\tfrac{1}{\sqrt{2}}(1+j)](-2)$
				 $= -1 + 2/\sqrt{2} + j 2/\sqrt{2} = -1 + \sqrt{2} + j\sqrt{2}$
				 $X[7]=-1 - W_8^3\cdot(-2) = -1 - \sqrt{2} - j\sqrt{2}$
You can simplify numerics if required, but the pattern is what matters.

---

### 4) Very common exam tasks and how to attack them fast
1) **Given a real sinusoid, find FFT bins:** compute $k_0$, place symmetric complex conjugate lines at $k_0$ and $N-k_0$ with half-amplitudes in complex form.
2) **Compute magnitude spectrum:** take $|X[k]|$; for real signals, often only $0..N/2$ is asked.
3) **Find phase spectrum:** use $\operatorname{atan2}(\Im,\Re)$; expect $0$, $\pi$, $\pm\pi/2$, $\pm\pi/4$ for simple cases.
4) **Time shift:** multiply spectrum by $e^{-j2\pi k n_0/N}$; phase ramps linearly with $k$.
5) **Modulation / frequency shift:** multiply time signal by $e^{j2\pi k_0 n/N}$ to shift spectrum by $k_0$ bins.
6) **Circular convolution length:** Without padding, length-$N$ DFT wraps; to avoid aliasing, zero-pad to at least $N\ge L_x+L_h-1$.
7) **IFFT reconstruction:** use +j exponent and divide by $N$; check with Parseval for correctness.
8) **Aliasing check:** if a tone has $f_0 > f_s/2$, it folds to $f_a = |f_0 - m f_s|$ inside $[0, f_s/2]$.

---

### 5) Quick reference: small DFTs
- **2-pt DFT:** $[a,b] \to [a+b, a-b]$
- **4-pt DFT:** twiddles are $1,-j,-1,+j$; memorize to speed up.
- **Common twiddle values:**
  - $e^{-j\pi/4} = 0.707 - j0.707$
  - $e^{-j\pi/2} = -j$
  - $e^{-j3\pi/4} = -0.707 - j0.707$

---

### 6) Practice drills (do these to lock it in)
1) **Bin mapping:** $f_s=8000$ Hz, $N=256$. What bin for a 500 Hz tone? (Ans: $k=16$).
2) **Magnitude spectrum:** Compute FFT of $[1,1,1,1]$ (Ans: $[4,0,0,0]$).
3) **Time shift:** If $x[n]\to x[n-2]$, how does $X[k]$ change? (Ans: multiply by $e^{-j2\pi k 2/N}$).
4) **Zero-padding & convolution:** Two sequences of lengths 5 and 4; minimum $N$ to avoid circular aliasing? (Ans: $N\ge 8$; choose power of 2, $N=8$ or 16 for FFT).
5) **Parseval check:** $x=[1, -1, 1, -1]$, verify $\sum |x|^2$ equals $(1/N)\sum |X|^2$.

---

### 7) Fast checklist before solving any FFT/IFFT problem
- Find $N$, $f_s$, $\Delta f$, Nyquist.
- Locate tone frequencies → bin indices.
- Apply symmetry rules (real signals → conjugate symmetric spectrum).
- Use known twiddles; avoid recalculating $e^{-j2\pi/N}$ every time.
- For shifts/modulations, adjust phase or bin index accordingly.
- For convolution, confirm padding length.
- For IFFT, remember the $1/N$ scale and +j exponent.

---

### 8) If you get stuck
- Compute a tiny case (4-pt or 8-pt) by hand to see the pattern.
- Sketch magnitude/phase; symmetry often reveals mistakes.
- Check energy with Parseval to catch scaling/sign errors.

---

This sheet is optimized for quick recall during problem solving. Use the drills to get comfortable with bin mapping, basic butterflies, and common twiddles—these cover most exam-style FFT/IFFT questions.
