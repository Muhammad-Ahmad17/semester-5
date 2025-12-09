# DSP LAB 10: VIVA PREPARATION Q&A
## Digital IIR Filter Design (Butterworth, Chebyshev, Elliptic)

---

## **SECTION 1: FUNDAMENTAL CONCEPTS**

### Q1: What is an IIR (Infinite Impulse Response) filter?
**A:** 
- IIR filters have **feedback** from output to input (recursive structure)
- Output depends on both current & previous inputs AND previous outputs
- Difference equation: y[n] = Σ(bk·x[n-k]) - Σ(ak·y[n-k])
- Advantages: Lower order than FIR for same specifications
- Disadvantage: Can be unstable if poles are outside unit circle

### Q2: What is the relationship between analog and digital filters?
**A:**
- **Analog filters** operate in s-plane (continuous-time, Laplace transform)
- **Digital filters** operate in z-plane (discrete-time, Z-transform)
- We design **analog prototypes** (well-studied), then **transform to digital**
- Transformation method: **Bilinear transform** or **Impulse invariance**

### Q3: What is the bilinear transform?
**A:**
- Formula: **s = 2·Fs·(z−1)/(z+1)**
- Maps: s-plane (analog) → z-plane (digital)
- Key mapping:
  - Left half of s-plane → Inside unit circle (stability)
  - jω axis (s-plane) → Unit circle (z-plane)
- Frequency warping: ωd = 2·arctan(ωa·Ts/2)
- Advantage: **Preserves stability** (no pole migration outside unit circle)

### Q4: Why use analog prototypes instead of designing directly in digital domain?
**A:**
- Analog filter theory is **mature & well-established** (Butterworth, Chebyshev since 1930s-1950s)
- **Simpler to design**: Use tabulated design tables
- **Better understanding**: Pole-zero placement is intuitive
- **Frequency transformations** (LP→BP, LP→HP, LP→BS) are simple in analog domain
- After designing analog prototype, use bilinear transform for digital conversion

---

## **SECTION 2: FILTER TYPES (Butterworth vs Chebyshev vs Elliptic)**

### Q5: Compare Butterworth, Chebyshev Type-I, Chebyshev Type-II, and Elliptic filters.

| **Property** | **Butterworth** | **Chebyshev I** | **Chebyshev II** | **Elliptic** |
|---|---|---|---|---|
| **Passband** | Maximally flat | Equiripple | Maximally flat | Equiripple |
| **Stopband** | Monotonic | Monotonic | Equiripple | Equiripple |
| **Rolloff** | Slow (20 dB/decade/order) | Steep | Steeper | Steepest |
| **Order for specs** | Highest | Lower | Lower | **Lowest** |
| **Use case** | General purpose | Sharp transition needed | No passband ripple | Sharpest transition |

### Q6: What does "equiripple" mean?
**A:**
- Equal ripple = **Oscillations of equal amplitude** in frequency response
- **Chebyshev Type-I**: Ripple in passband, monotonic stopband
- **Chebyshev Type-II**: Flat passband, ripple in stopband
- **Elliptic**: Ripple in **both** passband and stopband
- Ripple amount is controlled by **Rp (passband ripple)** and **Rs (stopband attenuation)**

### Q7: What is passband ripple (Rp) and stopband attenuation (Rs)?
**A:**
- **Passband Ripple (Rp, dB)**: Maximum allowed dip in passband
  - Example: Rp = 0.75 dB → magnitude can vary by ±0.75 dB in passband
  - Formula: 20·log10(√(1 + ε²)) where ε = ripple factor
  
- **Stopband Attenuation (Rs, dB)**: Minimum attenuation in stopband
  - Example: Rs = 60 dB → signals in stopband attenuated by 60 dB
  - Formula: 20·log10(1/δ) where δ = stopband ripple

### Q8: Why choose Elliptic filter for Lab Task 2?
**A:**
- **Sharpest transition** between passband and stopband
- **Lowest order** for given Rp and Rs specifications
- **Both frequencies sharp**: passband upper edge AND stopband lower edge close together
- **Small bandwidth** (only ±5.5 MHz band) benefits from sharp rolloff
- Tradeoff: Ripple in both bands (but acceptable for this application)

---

## **SECTION 3: FILTER DESIGN PROCESS**

### Q9: What are the steps to design a digital band-pass filter?

**Step-by-step process:**

1. **Define specifications** (given)
   - Passband frequency range
   - Stopband frequency range
   - Rp (passband ripple), Rs (stopband attenuation)
   - Sampling frequency Fs

2. **Normalize frequencies**
   - Ωp = 2π·fp / Fs (or fp / Fn where Fn = Fs/2)
   - Ωs = 2π·fs / Fs

3. **Design analog low-pass prototype**
   - Use `butter()`, `cheby1()`, `cheby2()`, or `ellip()`
   - Design in analog domain with normalized frequency
   - Output: Transfer function H(s) = B(s) / A(s)

4. **Apply bilinear transform**
   - Use `bilinear()` to convert analog → digital
   - Ensures all poles stay inside unit circle (stability)

5. **Transform low-pass to desired type**
   - For **band-pass**: Use `lp2bp()` with center frequency and bandwidth
   - Wc = (Ωp + Ωs) / 2 (center frequency)
   - Bw = Ωs - Ωp (bandwidth)

6. **Verify response**
   - Use `freqz()` for magnitude and phase
   - Use `zplane()` for pole-zero plot
   - Check specifications are met

### Q10: What does the `lp2bp()` function do?
**A:**
- **Transforms low-pass filter to band-pass**
- Formula creates **second-order transform** for each first-order LP section
- Creates zeros at DC (0 Hz) and Nyquist (Fs/2)
- **Double the filter order**: N_bp = 2 × N_lp
- Example: LP prototype with 20 poles → BP filter with ~40 poles

---

## **SECTION 4: MATLAB IMPLEMENTATION**

### Q11: Explain the MATLAB commands used in Lab 10.

#### **Butterworth Design (`butter`)**
```matlab
[b, a] = butter(N, Wn, 's')              % Analog prototype (s-domain)
[b, a] = butter(N, Wn)                   % Digital filter
[N, Wn_opt] = buttord(Wp, Ws, Rp, Rs)   % Calculate minimum order
```

#### **Chebyshev Type-I Design (`cheby1`)**
```matlab
[b, a] = cheby1(N, Rp, Wn, 's')          % Analog prototype
[b, a] = cheby1(N, Rp, Wn)               % Digital filter
[N, Wn_opt] = cheb1ord(Wp, Ws, Rp, Rs)  % Calculate minimum order
```

#### **Chebyshev Type-II Design (`cheby2`)**
```matlab
[b, a] = cheby2(N, Rs, Wn, 's')          % Analog prototype
[b, a] = cheby2(N, Rs, Wn)               % Digital filter
[N, Wn_opt] = cheb2ord(Wp, Ws, Rp, Rs)  % Calculate minimum order
```

#### **Elliptic (Cauer) Design (`ellip`)**
```matlab
[b, a] = ellip(N, Rp, Rs, Wn, 's')       % Analog prototype
[b, a] = ellip(N, Rp, Rs, Wn)            % Digital filter
[N, Wn_opt] = ellipord(Wp, Ws, Rp, Rs)  % Calculate minimum order
```

#### **Bilinear Transform**
```matlab
[b_dig, a_dig] = bilinear(b_analog, a_analog, Fs)
% Converts analog (s-domain) to digital (z-domain)
```

#### **Frequency Transformations**
```matlab
[b_bp, a_bp] = lp2bp(b_lp, a_lp, Wc, Bw)   % LP to BP
[b_hp, a_hp] = lp2hp(b_lp, a_lp, Wc)       % LP to HP
[b_bs, a_bs] = lp2bs(b_lp, a_lp, Wc, Bw)   % LP to BS
```

#### **Analysis Functions**
```matlab
[H, w] = freqz(b, a, N)           % Frequency response
zplane(b, a)                        % Pole-zero plot
[gd, w] = grpdelay(b, a)          % Group delay
```

### Q12: What do the output variables `b` and `a` represent?
**A:**
- **`b` = Numerator coefficients** of transfer function H(z) = B(z) / A(z)
- **`a` = Denominator coefficients**
- The **order** of `a` determines the **number of poles**
- The **order** of `b` determines the **number of zeros**
- For IIR filters: typically len(a) > len(b)

### Q13: What is a normalized frequency in MATLAB?
**A:**
- **Normalized frequency** = actual frequency / Nyquist frequency
- Nyquist frequency = Fs / 2
- Range: [0, 1] where 1 = Fs/2
- Example: If Fs = 6000 Hz, fp = 800 Hz
  - Ωp_norm = 800 / 3000 = 0.2667

---

## **SECTION 5: POLE-ZERO ANALYSIS**

### Q14: Why is pole-zero plot important?
**A:**
- **Poles**: Denominator roots (control stability)
  - All poles MUST be **inside unit circle** (|pole| < 1) for stability
  - If |pole| ≥ 1 → Filter is unstable (output grows without bound)

- **Zeros**: Numerator roots (control frequency response)
  - Zeros ON unit circle → Complete attenuation at that frequency
  - Zeros inside → Reduced (not zero) response at that frequency

- **Band-pass characteristics**:
  - Zeros at DC (z = 1) → Attenuation at 0 Hz
  - Zeros at Nyquist (z = -1) → Attenuation at Fs/2
  - Poles arranged to create passband

### Q15: How do you check filter stability from pole-zero plot?
**A:**
- **Plot pole-zero diagram** using `zplane(b, a)`
- **Verify all poles** are **strictly inside unit circle** (circle of radius 1)
- **Mathematically**: Calculate `abs(poles)` and check all < 1
```matlab
poles = roots(a);
is_stable = all(abs(poles) < 1);
```

---

## **SECTION 6: FREQUENCY RESPONSE & SPECIFICATIONS**

### Q16: What does magnitude response in dB mean?
**A:**
- **20·log₁₀(|H(jω)|)** where |H(jω)| is magnitude of frequency response
- **Advantages**:
  - Logarithmic scale shows small changes clearly
  - 20 dB ≈ 10× attenuation
  - 40 dB ≈ 100× attenuation
  - 60 dB ≈ 1000× attenuation
- **Passband**: Magnitude ≈ 0 dB (ideally flat)
- **Stopband**: Magnitude < -Rs dB (attenuated)

### Q17: What is phase response and why is it important?
**A:**
- **Phase** = angle of H(jω) in degrees or radians
- **Linear phase**: Phase changes linearly with frequency (no distortion)
- **Non-linear phase**: Introduces phase distortion
- **Group delay**: dφ/dω (how much delay signal experiences)
- **Importance**: Audio/speech applications need minimal phase distortion

### Q18: Explain the specifications used in Lab 10.

**Task 1 (Chebyshev Type-II):**
- Passband edge: 800 Hz
- Stopband edge: 2000 Hz
- Stopband attenuation: 60 dB (strong rejection)
- Passband ripple: 0.75 dB (no ripple in passband)

**Task 2 (Elliptic):**
- Passband: (N-5) to (N+5) MHz (narrow 10 MHz band)
- Stopband: (N-5.5) to (N+5.5) MHz (sharp edges)
- Passband ripple: 0.5 dB (small ripple acceptable)
- Stopband attenuation: 30 dB (moderate rejection)

---

## **SECTION 7: COMMON MISTAKES & DEBUGGING**

### Q19: What are common errors in filter design?
**A:**
1. **Forgetting to normalize frequencies** → Wrong cutoff
2. **Using wrong filter type** → Doesn't meet specifications
3. **Poles outside unit circle** → Unstable filter
4. **Not checking specifications** → Filter doesn't meet requirements
5. **Confusion about low-pass vs band-pass** → Must use `lp2bp()` transformation
6. **Forgetting bilinear transform** → Analog filter, not digital

### Q20: What if poles appear outside unit circle?
**A:**
- **Problem**: Filter is **UNSTABLE** → output grows without bound
- **Causes**:
  - Bilinear transform error
  - Filter order too high (numerical precision issues)
  - Design error in specifications
- **Solution**:
  - Lower filter order
  - Use `poly2str()` to check denominator coefficients
  - Verify bilinear transform was applied correctly

### Q21: Filter passes specifications but frequency response looks wrong?
**A:**
- **Check**:
  1. Are frequencies normalized to Nyquist correctly?
  2. Is bilinear transform applied?
  3. Is `lp2bp()` transformation correct (Wc and Bw)?
  4. Did you plot correct frequency range?
- **Debug**:
  - Manually compute magnitude at key frequencies
  - Use `freqz()` to verify response
  - Compare with `fvtool()` (Filter Visualization Tool)

---

## **SECTION 8: QUICK REFERENCE (FOR VIVA)**

### Filter Selection Guide
| **Application** | **Choice** | **Reason** |
|---|---|---|
| General purpose | Butterworth | Simple, no ripple |
| Sharp transition, no passband ripple | Chebyshev II | Steeper than Butterworth |
| Sharp transition, allows ripple | Chebyshev I | Lower order than Chebyshev II |
| **Sharpest transition** | **Elliptic** | **Lowest order** |

### Key Formulas
- **Bilinear transform**: s = 2Fs(z−1)/(z+1)
- **Normalized frequency**: Ω = 2πf/Fs or f/(Fs/2)
- **Center frequency (BP)**: Wc = (Ωp + Ωs)/2
- **Bandwidth (BP)**: Bw = Ωs − Ωp
- **Magnitude (dB)**: 20·log₁₀(|H(jω)|)
- **Stability condition**: |poles| < 1

### MATLAB Workflow
```
1. Specify frequencies (Hz) → normalize to [0, 1]
2. Design analog prototype (butter/cheby1/cheby2/ellip)
3. Apply bilinear transform
4. Apply lp2bp() for band-pass
5. Verify with freqz() and zplane()
6. Check: All poles inside unit circle?
7. Check: Magnitude response meets Rp, Rs?
```

---

## **VIVA TIPS**

✅ **Be ready to explain**:
1. Why we use analog prototypes
2. What bilinear transform does
3. Difference between filter types
4. How to check stability
5. What each MATLAB function does

✅ **Bring to viva**:
- All three MATLAB scripts (Pre-Lab A, Task 1, Task 2)
- Printouts of frequency response plots
- Pole-zero plots
- Working magnitude response verification

✅ **Common viva questions** (prepare answers):
- "Why is your filter stable?"
- "Does it meet the specifications?"
- "What happens if you increase filter order?"
- "Why did you choose [filter type]?"
- "Explain the bilinear transform"

✅ **If you get stuck**:
- Draw pole-zero plot by hand
- Explain why poles/zeros are where they are
- Show frequency response reasoning
- Relate to Nyquist theorem

---

**Good luck with your viva! 🎯**
