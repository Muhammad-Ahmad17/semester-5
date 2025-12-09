# Short answer (one-line)

Because you need **linear phase in the passband**, use a **linear-phase FIR bandpass** — design it with the **window method** (or Kaiser for a tuned result). The window method is simple, predictable and lets you trade transition width vs stopband attenuation clearly — exactly the topics an examiner will ask about in viva.

---

# Deep — everything you should be able to say in a viva (organized, precise)

### 1) Fundamental tradeoffs (say this early)

* **Linear phase requirement ⇒ FIR.**
  Exact linear phase is achieved by a symmetric FIR (coefficients symmetric about center). IIR filters inherently have nonlinear phase unless you do phase-equalization (adds complexity).
* **Minimum order vs linear phase:**
  IIR can reach the same amplitude specs with *much lower order* than FIR, but **IIR violates linear phase**. If linear phase is mandatory, you accept a larger FIR order or use additional tricks (phase equalization, minimum-phase FIR, multirate techniques) — all more complex than a straightforward FIR.
* **Window method choice:** simple, intuitive, easy to explain in viva. For best minimum-order for amplitude specification use equiripple (Parks–McClellan), but it is more complex to explain/implement. Window method is preferred when you want a robust, easy design and clear explanations of tradeoffs.

---

### 2) The math you should write down and explain

**Ideal bandpass impulse response (centered, length N+1, α = N/2):**

$$h_d[n] = \frac{\sin(\omega_2 (n-\alpha)) - \sin(\omega_1 (n-\alpha))}{\pi (n-\alpha)}, \quad n\ne\alpha$$

and

$$h_d[\alpha] = \frac{\omega_2 - \omega_1}{\pi}$$

where $\omega_1$ and $\omega_2$ are lower and upper cutoffs in **rad/sample**.

**Windowed FIR coefficients:**

$$h[n] = h_d[n]\cdot w[n], \quad n=0,\dots,N$$

with $w[n]$ the chosen window (Hamming, Blackman, Kaiser, …). Emphasize symmetry ($h[n]=h[N-n]$) ⇒ linear phase.

---

### 3) How filter order (N) relates to specs (what you should show in viva)

* **Transition width ($\Delta f$)** (Hz) and sampling ($f_s$) give ($\Delta\omega = 2\pi \Delta f / f_s$) (rad/sample).
* For Kaiser window the standard approximate design formula is:

$$N \approx \left\lceil \frac{A_s - 8}{2.285 \cdot \Delta\omega} \right\rceil$$

where $A_s$ is desired stopband attenuation in dB and $\Delta\omega$ is in rad/sample. (Use ceiling to get integer N; actual N chosen to satisfy symmetry: choose odd/even depending on Type I/II filter.)

* **Why this matters:** smaller ($\Delta\omega$) (narrower transition) ⇒ larger (N). Higher stopband attenuation ($A_s$) ⇒ larger (N).

**Kaiser β (to pick the window shape):**

$$\beta = \begin{cases}
0.1102(A_s-8.7), & A_s > 50 \\
0.5842(A_s-21)^{0.4} + 0.07886(A_s-21), & 21 \le A_s \le 50 \\
0, & A_s < 21
\end{cases}$$

---

### 4) Typical window properties you must memorize / state in viva

* **Rectangular** — smallest main-lobe width, worst sidelobes (~21 dB attenuation). (Produces severe Gibbs ripple.)
* **Hanning (Hann)** — ~44 dB stopband attenuation.
* **Hamming** — ~53 dB stopband attenuation. Often used for modest ripple requirements.
* **Blackman** — ~74 dB attenuation (better stopband, wider main lobe).
* **Kaiser** — adjustable by β; use when you need to meet a particular (A_s) and (\Delta f).
  Explain: *main-lobe width ⇄ transition width; sidelobe level ⇄ stopband attenuation*. So choice depends on required stopband attenuation and acceptable transition width.

---

### 5) A short numeric example you can talk through in viva

### 5) A short numeric example you can talk through in viva

(Show you can compute N from specs.)

* Suppose: $f_s=1000$ Hz, passband from 200–300 Hz, stopbands 180–320 Hz ⇒ worst transition width $\Delta f=20$ Hz. Required stopband attenuation $A_s=60$ dB.
* Compute $\Delta\omega = 2\pi(20)/1000 = 0.12566$ rad/sample.
* Kaiser formula: $N \approx (60-8)/(2.285\times0.12566) \approx 52/0.287 \approx 181$ ⇒ choose $N=181$ (or next convenient odd/even to preserve symmetry).
* Pick Kaiser β for $A_s=60$: $\beta \approx 0.1102(60-8.7)\approx5.7$.
* Then compute $h_d[n]$ and multiply by Kaiser window. Emphasize: large N — this is the cost of linear phase.

(You can use these numbers to demonstrate group delay = N/2 samples and point out latency.)

### 6) Viva-friendly answers to expected questions

**Q: Why not use an IIR (Butterworth/Chebyshev)?**
A: Because the requirement *explicitly* needs linear phase in the passband. IIR offers lower order but non-linear phase. You could cascade IIR with an equalizer or use phase-linearization, but that adds complexity and may not give exact linear phase over the full passband.

**Q: Why window method and not Parks–McClellan (equiripple)?**
A: Window method is simple to explain and implement, and gives intuitive control of sidelobe vs transition width. Parks–McClellan typically yields a lower order for the same ripple/attenuation (more optimal), but it’s algorithmically heavier and its ripple behavior must be explained in equiripple terms. If the viva asks for the *minimum order* solution, mention Parks–McClellan as the way to get almost minimum order for linear-phase FIR.

**Q: How do you guarantee linear phase?**
A: Choose symmetric coefficients (even symmetry) so phase is of form $-\omega N/2 + \text{const}$ ⇒ constant group delay $N/2$ samples.

**Q: What is the Gibbs phenomenon and how windowing helps?**
A: Truncating the ideal infinite impulse response causes oscillatory ripples (Gibbs). Multiplying by a window smooths the truncation (reduces sidelobes) at the expense of widening the main lobe (worse transition).

**Q: Implementation and complexity concerns?**
A: Complexity = $(N+1)$ multiplies per output (but symmetry reduces to $\sim(N/2)$ multiplies). Memory = $(N+1)$ coefficients. Group delay = $N/2$ samples — important for real-time/latency-sensitive systems.

**Q: Quantization effects?**
A: Coefficient quantization changes frequency response and can create slight asymmetry (destroying perfect linear phase) — use symmetric quantization or store mirrored coefficients. Use sufficient word length or implement structure (e.g., use shift-and-add if coefficients chosen for that).

**Q: Ways to reduce order but keep linear phase?**
A: Use multirate (decimate before filtering and then interpolate), use halfband or multistage filters, or use equiripple design which is order-optimal among linear-phase FIR designs.

---

### 7) Checklist / step-by-step to show in viva (short)

1. State specs: $f_{p1}, f_{p2}, f_{s1}, f_{s2}, A_p, A_s, f_s$. Convert to rad/sample: $\omega = 2\pi f/f_s$.
2. Compute transition width $\Delta f = \min(f_{p1}-f_{s1}, f_{s2}-f_{p2})$ and $\Delta\omega$.
3. Decide window: if you have a required $A_s$ use Kaiser; else choose Hamming/Blackman by desired attenuation.
4. Use Kaiser formula (or empirical rules for other windows) to estimate $N$.
5. Compute ideal impulse $h_d[n]$ for bandpass; multiply by window $w[n]$.
6. Normalize (if needed), test amplitude and phase responses, adjust $N$ if specs not met.
7. Implement using symmetry (store $N/2+1$ coefficients), analyze quantization and group delay.

---

### 8) Quick pointer on what to say if examiner challenges “minimum order”

* Admit: **IIR is minimum-order** for amplitude specs, but it fails the linear-phase requirement.
* If examiner insists on minimizing N while keeping linear phase: propose **Parks–McClellan** (equiripple FIR) — it produces the smallest N for given ripple/attenuation among linear-phase FIR methods.
* If allowed to relax exact linearity to “approximate linear phase,” mention **phase equalized IIR** or **all-pass phase correction**, but highlight complexity and imperfect results.

---

### 9) One-minute viva summary you can recite

### 9) One-minute viva summary you can recite

"I chose a linear-phase FIR bandpass because the requirement mandates linear phase in the passband; FIR with symmetric taps gives exact linear phase. I used the window method for simplicity and explanatory clarity. Window choice (Hamming, Blackman, Kaiser) trades transition width for sidelobe level (stopband attenuation). If extremely low order is required while keeping linear phase, use Parks–McClellan; if low order is required but linear phase can be relaxed, consider IIR with phase equalization. Design steps: convert specs to rad/sample, compute transition width, estimate order (Kaiser formula), form ideal h_d[n] and apply the window, then verify amplitude and phase and optimize."

---

### 10) Final tips for viva delivery

* Always **mention units and normalization** (Hz vs rad/sample vs normalized to Nyquist). Examiners love being precise about normalization.
* If they like numbers, give a short numeric example (like the one above) — it proves you can apply formulas.
* Emphasize **group delay = N/2** and real-time latency consequences.
* Mention **implementation tricks** (use symmetry to halve multipliers, or polyphase for decimation/interpolation).

---

**Key Takeaway:** Master these 10 sections, and you will ace the viva. The examiner wants to see you can explain the why, the math, and the tradeoffs—not just run MATLAB commands.
