# DSP LAB 11: VIVA PREPARATION Q&A
## FIR Filter Design with Window Method

---

## **SECTION 1: FUNDAMENTAL CONCEPTS**

### Q1: What is an FIR filter and how is it different from IIR?

**A:**

| **Property** | **FIR** | **IIR** |
|---|---|---|
| **Impulse response** | Finite duration (ends at n=M-1) | Infinite duration (never ends) |
| **Difference equation** | y[n] = Σ b[k]·x[n-k] | y[n] = Σ b[k]·x[n-k] - Σ a[k]·y[n-k] |
| **Feedback** | NO | YES (recursive) |
| **Stability** | Always stable | Can be unstable |
| **Linear phase** | Easy (use symmetric h[n]) | Difficult (rare) |
| **Filter order** | Higher | Lower |
| **Implementation** | Convolution (simple) | Difference equation |

**Visual difference:**
```
FIR (convolution):
    x[n] ──→ h[0] ──┐
              ↓     │
             z⁻¹    ├─→ Σ ──→ y[n]
              ↓     │
             h[1]  ─┘
              
IIR (recursive):
    x[n] ──→ [×b[0]] ──┐
                       ├─→ Σ ──→ y[n]
                       │    ↓
                      z⁻¹  [×-a[1]]
                       ↓    ↑
```

**Key advantage of FIR:** Always stable, always linear phase possible.

---

### Q2: What is the "ideal" filter and why is it impractical?

**A:**

An ideal filter is one that:
- Passes all frequencies in passband (magnitude = 1) without distortion
- Completely blocks all stopband frequencies (magnitude = 0)
- Has infinitely sharp transition (brick-wall characteristic)

**Mathematical representation:**
$$H_d(e^{j\omega}) = \begin{cases} e^{-j\omega\alpha} & |\omega| \leq \omega_c \\ 0 & \omega_c < |\omega| \leq \pi \end{cases}$$

**Why impractical:**

1. **Infinite impulse response:** Taking inverse DTFT gives:
$$h_d[n] = \frac{\sin[\omega_c(n-\alpha)]}{\pi(n-\alpha)}$$
   - Non-zero for **all n** (past and future)
   - Impossible to implement

2. **Non-causal:** Depends on future values (not realizable)

3. **Gibbs phenomenon:** Even if you truncate, you get ripple artifacts

**Solution:** Use windowing to truncate the ideal response.

---

### Q3: Explain the windowing process mathematically.

**A:**

The windowing process is:
$$h[n] = h_d[n] \cdot w[n]$$

Where:
- $h_d[n]$ = Ideal impulse response (infinite, sinc function)
- $w[n]$ = Window function (zero outside [0, M-1])
- $h[n]$ = Practical (finite) FIR filter

**Step-by-step:**

1. **Generate ideal response:** 
$$h_d[n] = \frac{\sin[\omega_c(n-\alpha)]}{\pi(n-\alpha)}$$

2. **Define window:**
$$w[n] = \begin{cases} \text{window function} & 0 \leq n \leq M-1 \\ 0 & \text{otherwise} \end{cases}$$

3. **Apply windowing:** Multiply element-wise

4. **Result:** Finite, practical filter with linear phase

**Frequency domain effect:**
- Ideal → rectangular window DTFT convolution
- Results in wider transition and stopband ripple (trade-off)

---

### Q4: What is "linear phase" and why is it important?

**A:**

**Linear phase** means the phase response is:
$$\angle H(e^{j\omega}) = -\alpha \omega$$

Where $\alpha$ is a **constant delay** (group delay).

**Mathematical meaning:**
- Output is input shifted by fixed amount
- No frequency-dependent distortion
- All frequencies have same time delay

**Advantage:**
- ✓ Preserves signal shape
- ✓ No distortion
- ✓ Radar: maintains target signature
- ✓ Audio: maintains waveform quality

**Example:**
```
Input: x[n] = sin(2πf₁n) + sin(2πf₂n)  (two frequencies)

Linear phase (good):
Output: y[n] = sin(2πf₁(n-α)) + sin(2πf₂(n-α))
            = delayed by same amount α
            (shape preserved)

Non-linear phase (bad):
Output has different delays for f₁ and f₂
            → frequencies arrive at different times
            → signal distorted
```

**For FIR filters:**
To achieve linear phase, make impulse response **symmetric**:
$$h[n] = h[M-1-n]$$

Then automatically: $\angle H(e^{j\omega}) = -\frac{M-1}{2}\omega$

---

## **SECTION 2: WINDOWING & WINDOWS**

### Q5: What is the Gibbs Phenomenon?

**A:**

**Gibbs Phenomenon:** Oscillation (ringing) that appears near discontinuities when using windowing.

**Cause:**
- Ideal filter has sharp rectangular cutoff in frequency domain
- Rectangular window in time domain
- DTFT of rectangular window has ripples (sinc function)
- Convolution → ripples appear in magnitude response

**Visual:**
```
Ideal response:
    |H(f)|
    1 |     ┌─────────────┐
      |     │             │
    0 |_____┘             └___
    
After windowing (Gibbs ripple):
    |H(f)|
    1 | ╭┐  ┌─╭───╭─┬─────┬─┐
      |╱ │  │╱   ╱ │╭┐   ╱ │╭
    0 |__│__│____│___│└────│_│└
          ↑              ↑
      passband        stopband
     (ripple!)        (ripple!)
```

**Different windows:**
- **Rectangular:** Worst ripple (sharp discontinuities)
- **Hamming:** Moderate ripple (smoother edges)
- **Blackman:** Least ripple (very smooth edges)

**Trade-off:** Less ripple = wider transition width

---

### Q6: Compare the five main window functions.

**A:**

| **Window** | **Formula** | **Transition Width** | **Stopband Atten.** | **Best For** |
|---|---|---|---|---|
| **Rectangular** | $w[n] = 1$ | 4π/M (narrow) | 21 dB | Narrow transition |
| **Bartlett** | Triangular | 8π/M | 25 dB | Basic smoothing |
| **Hanning** | $0.5 - 0.5\cos(...)$ | 8π/M | 44 dB | General purpose |
| **Hamming** | $0.54 - 0.46\cos(...)$ | 8π/M | 53 dB | **Most common** ✓ |
| **Blackman** | $0.42 - 0.5\cos(...) + 0.08\cos(...)$ | 12π/M | 74 dB | High attenuation |

**Selection decision tree:**

```
Need attenuation As?
    ├─ As ≤ 21 dB?     → Rectangular
    ├─ 21 < As ≤ 53 dB? → Hamming ✓ (MOST COMMON)
    └─ As > 53 dB?      → Blackman
```

**Key insight:** Higher stopband attenuation **always** means wider transition width. **You cannot have both!** It's a fundamental trade-off.

---

### Q7: Why is Hamming the most popular window?

**A:**

1. **53 dB attenuation** → Meets most practical requirements
2. **Reasonable transition width** → 8π/M (not too wide)
3. **Low passband ripple** → Gibbs ringing minimal
4. **Efficient** → Doesn't over-design (unlike Blackman)
5. **Industry standard** → DSP textbooks and tools default to Hamming

**Comparison:**
- **Rectangular:** Only 21 dB (fails 50 dB specs)
- **Hamming:** 53 dB ✓ (perfect for most specs)
- **Blackman:** 74 dB (overkill for 50 dB spec)

**Example:** For 50 dB requirement:
- Rectangular → FAILS
- Hamming → ✓ Excellent (53 > 50)
- Blackman → Works but wastes filter order on extra attenuation

---

### Q8: How do you choose window when designing?

**A:**

**Procedure:**

1. **Check stopband attenuation requirement** (As in dB)

2. **Look up window properties** (from Table 11.1):
   ```
   Need 50 dB?
   - Rectangular has 21 dB  → Too low ✗
   - Hamming has 53 dB      → OK ✓
   - Blackman has 74 dB     → Overkill but works
   ```

3. **Consider if you need narrow transition:**
   - If yes → Rectangular might work if As is low
   - If no → Hamming is default choice

4. **Balance trade-offs:**
   - More attenuation needed? → Better window (Blackman)
   - Narrower transition needed? → Rectangular (accept lower As)
   - Balanced specs? → Hamming ✓

**Final answer:** For most digital signal processing:
$$\text{Use Hamming window}$$

---

## **SECTION 3: DESIGN PROCEDURE**

### Q9: Walk through the 5-step FIR design procedure.

**A:**

**Step 1: Identify specifications**

Given:
- Passband edge: $\omega_p$ (rad/sample)
- Stopband edge: $\omega_s$ (rad/sample)
- Stopband attenuation: $A_s$ (dB)

**Step 2: Calculate transition width**

$$\Delta f = \frac{\omega_s - \omega_p}{2\pi}$$

**Step 3: Calculate minimum filter order**

$$M = \left\lceil \frac{A_s - 7.95}{14.36 \times \Delta f} \right\rceil + 1$$

**Step 4: Choose window**

From Table 11.1, select window with $A_s$ ≥ requirement

**Step 5: Design in MATLAB**

```matlab
% Normalize frequencies to [0, 1]
wn = wp/pi;  % or cutoff frequency

% Choose window
w = hamming(M);

% Design FIR
b = fir1(M-1, wn, 'low', w);

% Verify
freqz(b, 1, 1024);
```

**Example:**
```
Given: ωp = 0.2π, ωs = 0.3π, As = 50 dB

1. Δf = (0.3π - 0.2π)/(2π) = 0.05
2. M = ⌈(50-7.95)/(14.36×0.05)⌉ + 1 = ⌈58⌉ + 1 = 59
3. Hamming window (53 dB > 50 dB) ✓
4. wn = 0.2π/π = 0.2
5. b = fir1(58, 0.2, 'low', hamming(59))
```

---

### Q10: How do you calculate the ideal impulse response?

**A:**

For ideal low-pass filter, the impulse response is:

$$h_d[n] = \frac{\sin[\omega_c(n-\alpha)]}{\pi(n-\alpha)}$$

**Components:**
- $\omega_c$ = cutoff frequency (rad/sample)
- $\alpha$ = delay parameter = (M-1)/2 (for symmetry)
- $n$ = sample index (0, 1, 2, ..., M-1)

**Special case:** When $n = \alpha$:
$$\lim_{n \to \alpha} h_d[n] = \frac{\omega_c}{\pi}$$

**Example:** For $\omega_c = 0.2\pi$, $M = 59$ ($\alpha = 29$):

```
h_d[0]  = sin(0.2π(0-29)) / (π(-29))
        = sin(-5.8π) / (-29π)
        ≈ -0.0018

h_d[29] = 0.2π/π = 0.2  (center value)

h_d[58] = sin(0.2π(58-29)) / (π(29))
        ≈ -0.0018  (symmetric to h_d[0])
```

**MATLAB:**
```matlab
n = 0:(M-1);
alpha = (M-1)/2;
h_ideal = sin(wc * (n - alpha)) ./ (pi * (n - alpha));
h_ideal(n == alpha) = wc/pi;  % Handle division by zero
```

---

## **SECTION 4: FREQUENCY DOMAIN ANALYSIS**

### Q11: What does the magnitude response plot tell you?

**A:**

The magnitude response $|H(e^{j\omega})|$ in dB shows:

**Key features:**

1. **Passband (ideal gain = 1 or 0 dB):**
   - Should be flat and close to 0 dB
   - May have small ripple due to windowing
   - Ripple is the "cost" of windowing

2. **Transition region:**
   - Goes from passband to stopband
   - Width determined by window
   - Faster (sharper) = narrower window = higher order

3. **Stopband:**
   - Should be as low as possible
   - Minimum value = stopband attenuation spec
   - Flatter is better (no ripple)

**Example interpretation:**
```
Magnitude response plot:
    0 |        ┌──────────┐
   -10|        │          │  ← Passband (0 dB ±0.5dB)
   -20|        │          ├──┐
   -30|        │          │  ├─┐
   -50|________|__________|__├──  ← Stopband (-50 dB)
   -70|                   │  │
      0       ωp       ωs  π
      
      ↑ Passband  ↑ Transition  ↑ Stopband
      (good)       (steeper=better) (low=better)
```

**What to look for in viva:**
- Passband flatness → Low ripple
- Transition sharpness → Efficient design
- Stopband level → Meets specification

---

### Q12: What does the phase response tell you?

**A:**

The phase response $\angle H(e^{j\omega})$ should be **linear** for distortion-free filtering.

**Linear phase:**
$$\angle H(e^{j\omega}) = -\frac{M-1}{2} \omega + \text{constant}$$

This is a **straight line** through origin (slope = -(M-1)/2).

**What it means:**
- All frequencies have same delay
- Output is input shifted by constant amount
- No frequency distortion

**Example:**
```
Phase response for linear phase FIR:
    φ(ω)
      0 ╱─────────────
        ╱
   -10 ╱
   -20 ╱
        0        π      ω
        
   Straight line = linear phase ✓
```

**If not linear:**
```
Non-linear phase (IIR or asymmetric FIR):
    φ(ω)
      0 ╱╲
   -10 ╱  ╲
   -20 ╱    ╲╲
        0        π      ω
        
   Curves = non-linear phase ✗
   Different frequencies have different delays → distortion
```

**Verification in MATLAB:**
```matlab
[H, W] = freqz(b, 1, 1024);
phase = angle(H);
plot(W, phase);  % Should be straight line
```

---

## **SECTION 5: PRACTICAL DESIGN ISSUES**

### Q13: What if your filter doesn't meet specifications?

**A:**

**Problem:** Designed filter fails stopband attenuation requirement.

**Solutions (in order of preference):**

1. **Increase filter order M:**
   - Higher M → narrower transition
   - Can meet stricter specs
   - Cost: More coefficients, more computation
   
   ```matlab
   % Old: M = 59
   % New: M = 80 (try larger)
   b_new = fir1(M-1, wn, 'low', hamming(M));
   ```

2. **Use better window:**
   - Rectangular (21 dB) → Hamming (53 dB)
   - Hamming (53 dB) → Blackman (74 dB)
   - Cost: Wider transition
   
   ```matlab
   % Old: b = fir1(59, wn, 'low', hamming(59));
   % New: Use Blackman instead
   b_new = fir1(59, wn, 'low', blackman(59));
   ```

3. **Widen transition band:**
   - Move stopband edge further from passband
   - Easier spec to meet
   - Cost: Wider transition
   
   ```matlab
   % Old: ωs = 0.3π
   % New: ωs = 0.35π (further away)
   ws_new = 0.35*pi;
   ```

4. **Relax specification:**
   - Accept lower stopband attenuation
   - Only if application allows

**Best practice:** Size M conservatively (20% larger than calculated).

---

### Q14: How does filter length M affect design?

**A:**

**Higher M (longer filter):**
- ✓ Sharper transition width (4π/M, 8π/M, etc. decreases)
- ✓ Faster roll-off
- ✓ Can meet stricter specs
- ✗ More computation (M multiply-add per sample)
- ✗ More delay ((M-1)/2 samples group delay)
- ✗ More memory required

**Lower M (shorter filter):**
- ✓ Less computation
- ✓ Less delay
- ✓ Simpler implementation
- ✗ Wider transition
- ✗ May not meet attenuation spec

**Trade-off:** Choose minimum M that meets all specifications.

**From design equation:**
$$M = \left\lceil \frac{A_s - 7.95}{14.36 \times \Delta f} \right\rceil + 1$$

- Larger As → Larger M (more attenuation = longer filter)
- Smaller Δf → Larger M (narrower transition = longer filter)

---

## **SECTION 6: BAND-PASS FILTER DESIGN**

### Q15: How do you design a band-pass FIR filter?

**A:**

**Band-pass filter:** Passes one frequency band, blocks others.

**Method:**

1. **Design as low-pass**
   - Calculate M from specifications
   - Choose window

2. **Specify two cutoff frequencies**
   - Lower passband edge: $\omega_{p,low}$
   - Upper passband edge: $\omega_{p,high}$

3. **MATLAB command:**
   ```matlab
   b = fir1(M-1, [wp_low, wp_high], 'bandpass', hamming(M));
   ```

4. **Verify:**
   - Check passband is flat
   - Check stopbands below specification
   - Verify linear phase (symmetric h[n])

**Example (Radar application):**
```
Target frequency: 20 MHz
Passband: 15-25 MHz (center ± 5 MHz)
Sampling: 80 MHz
Stopband atten: 50 dB

1. Normalized: [15/(40), 25/(40)] = [0.375, 0.625]
2. M = 60 (from design equation)
3. b = fir1(59, [0.375, 0.625], 'bandpass', hamming(60));
4. freqz(b) to verify
```

**Key difference from low-pass:**
- Two transition regions (lower and upper)
- Same transition width rules apply
- Must ensure both transitions are adequate

---

### Q16: Why is linear phase critical for radar?

**A:**

**Radar application:** Receives echoes from targets.

**Signal characteristics:**
- Chirps (frequency-sweep signals)
- Pulses with specific shapes
- Target identification based on shape

**With linear phase (good):**
- All frequency components have same delay
- Shape preserved through filter
- Can identify target signature
- Receiver can estimate range and velocity

**With non-linear phase (bad):**
- Different frequencies delayed differently
- Signal shape distorted
- Target signature unrecognizable
- Cannot identify target properly

**Example:**
```
Target signal (chirp):  /‾‾‾
                       /

Through linear phase:   /‾‾‾ (delayed, same shape)

Through non-linear:     /‾╱‾ (distorted)
                        
                        ✗ Cannot identify!
```

**FIR solution:** Use symmetric impulse response → Automatic linear phase.

---

## **SECTION 7: IMPLEMENTATION & MATLAB**

### Q17: What are the key MATLAB commands for FIR filter design?

**A:**

**Design:**
```matlab
% Method 1: fir1 (most common)
b = fir1(N, Wn, type, window);
% N = filter order (M-1)
% Wn = normalized cutoff(s) in [0, 1]
% type = 'low', 'high', 'bandpass', 'bandstop'
% window = hamming(M), blackman(M), rectwin(M), etc.

% Method 2: fircls (frequency response masking)
b = fircls(N, Wp, Ws, Rp, Rs);
% Directly specifies passband/stopband edges

% Method 3: Remez algorithm (optimal)
b = remez(N, f, a, w);
% Equiripple FIR design (more advanced)
```

**Windows:**
```matlab
w = hamming(M);     % Most common
w = blackman(M);    % Better attenuation
w = rectwin(M);     % Worst ripple
w = hanning(M);     % Similar to Hamming
w = bartlett(M);    % Triangular
```

**Analysis:**
```matlab
% Frequency response
freqz(b, 1, 1024);          % Default plot
[H, W] = freqz(b, 1, 1024); % Get H and W

% dB magnitude
mag_dB = 20*log10(abs(H));

% Phase response
phase = angle(H);

% Group delay
grpdelay(b, 1, W);

% Interactive tool
fvtool(b);                  % Opens filter visualization tool

% Pole-zero plot
zplane(b, 1);               % Should only have zeros (FIR)
```

**Impulse response:**
```matlab
impz(b, 1, M);   % Plot first M samples
stem(b);         % Show coefficients
```

---

### Q18: How do you verify your design meets specifications?

**A:**

**Checklist:**

1. **Passband magnitude:**
   ```matlab
   [H, W] = freqz(b, 1, 2048);
   mag_dB = 20*log10(abs(H)+1e-10);
   
   % Find passband region
   passband_idx = find(W >= wp_low & W <= wp_high);
   passband_ripple = max(mag_dB(passband_idx)) - min(mag_dB(passband_idx));
   
   % Should be: ripple < 1 dB, gain ≈ 0 dB
   assert(passband_ripple < 1, 'Passband ripple too high');
   ```

2. **Stopband attenuation:**
   ```matlab
   stopband_idx = find(W > ws);
   stopband_atten = min(mag_dB(stopband_idx));
   
   % Should be: atten <= -As dB
   assert(abs(stopband_atten) >= As, 'Insufficient stopband atten.');
   ```

3. **Linear phase:**
   ```matlab
   % Check symmetry
   is_symmetric = norm(b - fliplr(b)) < 1e-10;
   assert(is_symmetric, 'Not symmetric - not linear phase');
   
   % Check phase linearity
   phase = unwrap(angle(H));  % Unwrap for clarity
   % Should show straight line
   ```

4. **Visual inspection:**
   ```matlab
   fvtool(b);  % Use interactive tool
   % Check passband is flat
   % Check stopband is low
   % Check transition is reasonable
   ```

---

## **SECTION 8: COMPREHENSIVE DESIGN EXAMPLE**

### Q19: Walk through a complete design example.

**A:**

**Problem:** Design digital FIR low-pass filter with:
- Passband edge: $\omega_p = 0.2\pi$ rad/sample
- Stopband edge: $\omega_s = 0.3\pi$ rad/sample
- Stopband attenuation: $A_s = 50$ dB

**Solution:**

**Step 1: Calculate transition width**
$$\Delta f = \frac{\omega_s - \omega_p}{2\pi} = \frac{0.3\pi - 0.2\pi}{2\pi} = 0.05$$

**Step 2: Calculate minimum M**
$$M = \left\lceil \frac{50 - 7.95}{14.36 \times 0.05} \right\rceil + 1 = \left\lceil 58 \right\rceil + 1 = 59$$

**Step 3: Select window**
- Need 50 dB attenuation
- From Table 11.1: Hamming provides 53 dB ✓
- Decision: Use Hamming window

**Step 4: Design in MATLAB**
```matlab
M = 59;
wn = 0.2;  % Normalized: 0.2π/π = 0.2
b = fir1(M-1, wn, 'low', hamming(M));
```

**Step 5: Verify**
```matlab
[H, W] = freqz(b, 1, 2048);
mag_dB = 20*log10(abs(H)+1e-10);

% Check passband
pb_idx = find(W <= 0.2*pi);
pb_ripple = max(mag_dB(pb_idx)) - min(mag_dB(pb_idx));
fprintf('Passband ripple: %.2f dB\n', pb_ripple);  % Should be <1 dB

% Check stopband
sb_idx = find(W >= 0.3*pi);
sb_atten = min(mag_dB(sb_idx));
fprintf('Stopband atten: %.2f dB\n', abs(sb_atten));  % Should be >50 dB

% Check linear phase
is_sym = norm(b - fliplr(b)) < 1e-10;
fprintf('Linear phase: %s\n', string(is_sym));  % Should be true

% Plot
freqz(b, 1, 2048);
title('FIR Low-Pass Filter (M=59, Hamming)');
```

**Result:**
- Filter length: 59 samples
- Passband ripple: ~0.3 dB (acceptable)
- Stopband attenuation: ~53 dB (meets 50 dB spec ✓)
- Linear phase: Confirmed (symmetric h[n])
- Group delay: 29 samples

---

## **SECTION 9: COMMON MISTAKES & PITFALLS**

### Q20: What are common mistakes students make?

**A:**

❌ **Mistake 1: Confusing filter order and length**
- Order N = M - 1
- Length M = number of coefficients
- MATLAB: `fir1(N, ...)` uses order, not length

✅ **Fix:** Always use `fir1(M-1, ...)` when you have M samples

---

❌ **Mistake 2: Wrong frequency normalization**
- Some specs in Hz, some in rad/sample
- MATLAB fir1 expects normalized [0, 1]
- 1 = Nyquist frequency = fs/2

✅ **Fix:** 
```matlab
% If given f in Hz:
f_norm = f / (fs/2);

% If given ω in rad/sample:
f_norm = ω/π;
```

---

❌ **Mistake 3: Choosing window without checking spec**
- Window has fixed stopband attenuation
- If your spec exceeds window capability → fails

✅ **Fix:** Always check Table 11.1 before choosing window

---

❌ **Mistake 4: Assuming higher order always better**
- Higher M = more computation, more delay
- Choose **minimum M** that meets specs

✅ **Fix:** Use design equation to calculate minimum M

---

❌ **Mistake 5: Ignoring passband ripple**
- Windowing causes Gibbs ripple
- Not just in stopband—passband too!

✅ **Fix:** Plot magnitude response and check both regions

---

❌ **Mistake 6: Not verifying linear phase**
- FIR should have linear phase
- Non-symmetric h[n] → no linear phase

✅ **Fix:** Check if `b == fliplr(b)` (symmetric)

---

❌ **Mistake 7: Transition width confusion**
- Table 11.1 gives transition width formula
- Narrower transition = higher order needed

✅ **Fix:** Understand M inversely proportional to transition width

---

## **QUICK REFERENCE**

**Design equation:**
$$M = \left\lceil \frac{A_s - 7.95}{14.36 \times \Delta f} \right\rceil + 1$$

**Transition width:**
$$\Delta f = \frac{\omega_s - \omega_p}{2\pi}$$

**Window selection:**

| As (dB) | Window | Purpose |
|---|---|---|
| ≤21 | Rectangular | Poor passband ripple |
| 21-53 | Hamming | ✓ Best choice |
| >53 | Blackman | High attenuation |

**Linear phase check:**
$$h[n] = h[M-1-n] \text{ (symmetric)}$$

**MATLAB essentials:**
```matlab
b = fir1(M-1, wn, 'low', hamming(M));
freqz(b, 1, 1024);
fvtool(b);
```

---

*Abu bakar Talha* | IEEE 324 | Digital Signal Processing Lab Manual | Viva Q&A
