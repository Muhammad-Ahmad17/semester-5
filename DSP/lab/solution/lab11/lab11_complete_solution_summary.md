# DSP LAB 11: COMPLETE SOLUTION SUMMARY
## FIR Filter Design Using Windows & Band-Pass Filter Design

**Status**: ✅ READY FOR VIVA  
**Lab Type**: Digital Signal Processing - FIR Filter Design with Windowing

---

## 📋 WHAT'S INCLUDED

### **Files Created**
1. ✅ `lab11_prelab_task.m` - **FIR Low-Pass Filter Design (Hamming Window)**
2. ✅ `lab11_task1.m` - **FIR Low-Pass Filter Design (Window Selection & Comparison)**
3. ✅ `lab11_task2.m` - **Digital Band-Pass Filter Design (Open-Ended)**
4. ✅ `lab11_viva_qa.md` - **Viva Q&A & Key Concepts**

### **Lab Structure**
```
DSP/lab/solution/lab11/
├── lab11_prelab_task.m              (Complete with explanations)
├── lab11_task1.m                    (Complete with explanations)
├── lab11_task2.m                    (Complete with explanations)
├── lab11_viva_qa.md                 (Viva preparation)
└── LAB11_COMPLETE_SOLUTION_SUMMARY.md (This file)
```

---

## 🔍 TASK OVERVIEW & THEORY

### **Understanding Window-Based FIR Design**

The window design method follows this process:

1. **Start with ideal filter** → Infinite duration impulse response
2. **Choose a window** → Trade-off between transition width and stopband ripple
3. **Multiply (window)** → Truncate ideal response to finite length
4. **Result** → Practical FIR filter with linear phase

**Why windows?**
- Ideal filters have infinite impulse response → Impossible to implement
- Windowing gives us a practical, finite filter
- Window choice determines stopband attenuation vs transition width

**Mathematical basis:**
$$h[n] = h_d[n] \cdot w[n]$$

Where:
- $h_d[n] = \frac{\sin[\omega_c(n-\alpha)]}{\pi(n-\alpha)}$ (ideal response)
- $w[n]$ = window function (Hamming, Blackman, etc.)

---

## 📝 PRE-LAB TASK: FIR LOW-PASS FILTER (HAMMING WINDOW)

### **Specifications**

```
Passband cutoff:     ωc = 0.2π rad/sample
Stopband attenuation: As = 50 dB
Window type:         Hamming
```

### **Step-by-Step Solution**

#### **Step 1: Calculate normalized transition width**

$$\Delta f = \frac{\omega_s - \omega_p}{2\pi}$$

For this problem:
- We need to estimate $\omega_s$ from $A_s$ using Hamming properties
- Hamming window has ~53 dB stopband attenuation
- Use Table 11.1 to determine appropriate filter order

#### **Step 2: Calculate minimum filter order**

From the design equation:
$$M \approx \frac{A_s - 7.95}{14.36 \Delta f} + 1$$

**For 50 dB attenuation with Hamming:**
- Hamming provides 53 dB → Sufficient
- Estimate $\Delta f$ from given frequencies
- Calculate $M$

#### **Step 3: Design impulse response**

1. Generate ideal sinc impulse response:
   $$h_d[n] = \frac{\sin[\omega_c(n-\alpha)]}{\pi(n-\alpha)}$$

2. Create Hamming window:
   $$w[n] = 0.54 - 0.46\cos\left(\frac{2\pi n}{M-1}\right)$$

3. Apply windowing:
   $$h[n] = h_d[n] \cdot w[n]$$

#### **Step 4: Verify Properties**

- ✅ Is $h[n]$ symmetric? (Ensures linear phase)
- ✅ Does magnitude response meet specs?
- ✅ Is transition sharp enough?
- ✅ Does stopband have sufficient attenuation?

### **How to Run**

```matlab
run lab11_prelab_task.m
% Outputs:
% - Filter specifications and calculations
% - Impulse response h[n]
% - Magnitude response (dB scale)
% - Phase response (should be linear)
% - Pole-zero plot (all zeros, stable)
```

### **Expected Results**

```
Filter Order (M):        ~50-60 samples
Cutoff (-3dB):           ~0.2π
Stopband Attenuation:    ~53 dB (Hamming property)
Phase Linearity:         ~constant slope
Stability:               ✓ (All zeros, no poles)
```

---

## 🔬 LAB TASK 1: WINDOW COMPARISON & SELECTION

### **Specifications**

```
Passband cutoff:         ωc = 0.2π rad/sample
Stopband attenuation:    As = 50 dB
Transition width:        Δω = 0.1π rad/sample
Test windows:            Rectangular, Hamming, Blackman
```

### **Objectives**

1. **Design three filters** with same passband/stopband using different windows
2. **Compare characteristics:**
   - Passband ripple
   - Stopband attenuation
   - Transition width
   - Filter order needed

3. **Explain trade-offs** and justify window selection

### **Theory: Window Trade-offs**

| **Parameter** | **Rectangular** | **Hamming** | **Blackman** |
|---|---|---|---|
| **Min. Stopband Atten.** | 21 dB | 53 dB | 74 dB |
| **Transition Width** | 4π/M | 8π/M | 12π/M |
| **Passband Ripple** | None | Low | Very low |
| **Use Case** | Tight transition | General purpose | Need high attenuation |

**Decision tree:**
```
Need 50 dB attenuation?
    ├─ YES, narrow transition → Rectangular (won't work, only 21 dB)
    ├─ YES, general → Hamming (53 dB, medium transition)
    └─ YES, critical app → Blackman (74 dB, wider transition)
```

### **Key Insight: Higher Attenuation = Wider Transition**

- **Rectangular:** Narrow transition, poor attenuation (Gibbs phenomenon)
- **Hamming:** Balanced (most common choice)
- **Blackman:** Excellent attenuation, but wider transition

### **How to Run**

```matlab
run lab11_task1.m
% Generates:
% - Three frequency response plots (comparison)
% - Magnitude response for each window
% - Phase response overlay
% - Statistical comparison table
```

### **For Viva Preparation**

**Q: Why does Rectangular have highest Gibbs ripple?**  
A: Rectangular has sharp discontinuity at window edges → Fourier series exhibits overshoot

**Q: Why is Hamming best for general use?**  
A: Good balance - sufficient attenuation (53 dB) without excessive transition width

**Q: Can you use Blackman for this spec?**  
A: Yes, but overkill - 74 dB when 53 dB needed. Wastes filter order on extra attenuation.

---

## 🎯 LAB TASK 2: DIGITAL BAND-PASS FILTER (OPEN-ENDED)

### **Specifications (Radar Application)**

```
Application:             Radar signal processing
Filter Type:             Band-Pass FIR
Phase Response:          Must be LINEAR (no distortion)
Parameters from RegNo:   See table below
```

### **Parameter Calculation from Registration Number**

Let $R$ = Last 2 digits of your registration number

Calculate $N$ (frequency parameter):
$$N = \begin{cases} 10 \times R & \text{if } R < 15 \\ R & \text{if } R \geq 15 \end{cases}$$

Calculate $A_s$ (stopband attenuation in dB):
$$A_s = \begin{cases} 70 & R \leq 15 \\ 50 & 16 \leq R \leq 25 \\ 40 & 26 \leq R \leq 35 \\ 21 & R > 35 \end{cases}$$

### **Filter Design Specifications**

| **Parameter** | **Formula** | **Meaning** |
|---|---|---|
| **Passband edges** | $(N-5)$ to $(N+5)$ MHz | Center: $N$ MHz, width: 10 MHz |
| **Stopband edges** | $(N-5.5)$ to $(N+5.5)$ MHz | Transition: 0.5 MHz on each side |
| **Sampling rate** | $f_s = 4N$ MHz | Nyquist: 2N MHz |
| **Stopband Atten.** | $A_s$ dB | From table above |

### **Design Procedure**

#### **Step 1: Normalize Frequencies**

Normalized frequency (for MATLAB): $\omega_{norm} = \frac{f}{f_s/2}$

Example: If $N = 20$ MHz, $f_s = 80$ MHz:
- Lower passband: $(20-5)$ MHz = 15 MHz → norm = 15/(40) = 0.375
- Upper passband: $(20+5)$ MHz = 25 MHz → norm = 25/(40) = 0.625
- Lower stopband: $(20-5.5)$ MHz = 14.5 MHz → norm = 0.3625
- Upper stopband: $(20+5.5)$ MHz = 25.5 MHz → norm = 0.6375

#### **Step 2: Calculate Transition Width**

$$\Delta f = \frac{f_s - f_p}{f_s/2} = \frac{0.5 \text{ MHz}}{40 \text{ MHz}} = 0.0125$$

#### **Step 3: Calculate Filter Order**

$$M = \left\lceil\frac{A_s - 7.95}{14.36 \Delta f}\right\rceil + 1$$

#### **Step 4: Design Band-Pass FIR Filter**

```matlab
% Normalized passband edges
wp_low_norm = wp_low / (fs/2);
wp_high_norm = wp_high / (fs/2);

% Window selection
if As >= 50
    window_type = 'hamming';  % 53 dB sufficient
else
    window_type = 'hamming';  % or try blackman if As > 60
end

% Design
b = fir1(M-1, [wp_low_norm, wp_high_norm], 'bandpass', window(hamming(M)));

% Verify
freqz(b, 1, 1024, fs);  % Plot with actual frequency axis
fvtool(b);              % Interactive tool
```

#### **Step 5: Verify Specifications**

- ✅ Passband (N±5 MHz): Magnitude ≈ 0 dB (or -3dB at edges)
- ✅ Stopband: Magnitude < -$A_s$ dB
- ✅ Phase response: Linear (straight line = constant delay)
- ✅ Stability: ✓ (FIR always stable)

### **For Viva Preparation**

**Q: Why is linear phase critical for radar?**  
A: Radar needs to preserve signal shape to identify targets. Distortion (non-linear phase) corrupts the signature.

**Q: Why design band-pass instead of high-pass + low-pass?**  
A: Single filter is more efficient. Radar signal has specific frequency band of interest.

**Q: How to ensure linear phase?**  
A: Use symmetric window. For FIR with odd length and symmetric $h[n]$, phase is $-\alpha\omega$ where $\alpha = (M-1)/2$.

**Q: Why are stopband edges at ±5.5 MHz (not ±5)?**  
A: Allows transition zone. Sharp corners are impossible (Gibbs).

### **How to Run**

```matlab
run lab11_task2.m
% Interactive:
% 1. Enter your registration number
% 2. Automatic calculation of N and As
% 3. Design and display results
% 4. Use data cursors to verify specs
```

---

## 📊 KEY DESIGN EQUATIONS SUMMARY

### **Impulse Response Calculation**

Ideal low-pass:
$$h_d[n] = \frac{\sin[\omega_c(n-\alpha)]}{\pi(n-\alpha)}$$

After windowing:
$$h[n] = h_d[n] \cdot w[n]$$

### **Filter Order Calculation**

$$M = \left\lceil\frac{A_s - 7.95}{14.36 \Delta f}\right\rceil + 1$$

Where: $\Delta f = \frac{\omega_s - \omega_p}{2\pi}$

### **Window Properties (Table 11.1)**

| **Window** | **Transition Width** | **Stopband Atten.** |
|---|---|---|
| Rectangular | $\frac{4\pi}{M}$ | 21 dB |
| Hamming | $\frac{8\pi}{M}$ | 53 dB |
| Blackman | $\frac{12\pi}{M}$ | 74 dB |

### **Linear Phase Condition**

For FIR to have linear phase:
$$h[n] = \pm h[M-1-n]$$

This ensures: $\angle H(e^{j\omega}) = -\frac{M-1}{2}\omega$ (constant delay)

---

## ✅ CHECKLIST FOR SUCCESSFUL LAB

- [ ] **Understand windowing concept** — Why ideal → impractical → window → practical
- [ ] **Know all 5 window types** — Properties and trade-offs
- [ ] **Calculate M correctly** — Using design equation
- [ ] **Verify linear phase** — Check $h[n]$ symmetry
- [ ] **Plot frequency response** — Magnitude and phase
- [ ] **Use data cursors** — To verify passband/stopband specs
- [ ] **Compare windows** — Explain trade-offs
- [ ] **Prepare viva answers** — Theory + MATLAB code understanding

---

## 🎓 VIVA HINTS

**Common viva questions:**

1. "Why does windowing introduce ripple?" 
   → Because truncation = multiplication by rectangular window first → Gibbs phenomenon

2. "Can FIR filters have infinite duration?"
   → No, by definition FIR has finite h[n]. But computation can be long if M is large.

3. "Which window would you use for 80 dB attenuation?"
   → Blackman (74 dB) is insufficient → Need custom window or multiple stages

4. "How does filter order affect transition width?"
   → Higher M → narrower transition (inverse relationship)

5. "Can you make non-linear phase FIR filter?"
   → Yes, but it distorts signals. Window methods → automatic linear phase if symmetric.

---

## 📚 RELATED TOPICS FOR DEEPER UNDERSTANDING

- **Remez algorithm** — Alternative optimal FIR design (lecture)
- **Parks-McClellan** — Equiripple FIR design (advanced)
- **Multi-rate filtering** — Using FIR with decimation (next semester)
- **Filter banks** — Multiple FIR filters in parallel (advanced)

---

**Ready? Open the MATLAB files and run them step-by-step!**

Next → `lab11_viva_qa.md` for detailed Q&A preparation

---

*Abu bakar Talha* | IEEE 324 | Digital Signal Processing Lab Manual | Part 2: Solution Summary
