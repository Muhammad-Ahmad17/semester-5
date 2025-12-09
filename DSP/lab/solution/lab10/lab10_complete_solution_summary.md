# DSP LAB 10: COMPLETE SOLUTION SUMMARY
## Digital IIR Filter Design & Viva Preparation

**Status**: ✅ READY FOR VIVA  
**Due**: Tomorrow (Viva exam)  
**Lab Type**: Digital Signal Processing - IIR Filter Design

---

## 📋 WHAT'S INCLUDED

### **Files Created**
1. ✅ `lab10_prelab_task_a.m` - **Butterworth Low-Pass Filter**
2. ✅ `lab10_task1.m` - **Chebyshev Type-II Band-Pass Filter**
3. ✅ `lab10_task2.m` - **Elliptic (Cauer) Band-Pass Filter**
4. ✅ `lab10_viva_qa.md` - **Viva Q&A & Key Concepts**

### **Lab Structure**
```
DSP/lab/solution/
├── lab10_prelab_task_a.m       (Complete with explanations)
├── lab10_task1.m               (Complete with explanations)
├── lab10_task2.m               (Complete with explanations)
├── lab10_viva_qa.md            (Viva preparation)
└── LAB10_COMPLETE_SOLUTION_SUMMARY.md (This file)
```

---

## 🔍 QUICK TASK OVERVIEW

### **Pre-Lab Task A: Butterworth Low-Pass Filter**
```
Input:  Passband edge: 0.2π rad/sample
        Stopband edge: 0.3π rad/sample
        Passband ripple: 7 dB
        Stopband attenuation: 16 dB

Output: Transfer function H(z) = B(z) / A(z)
        Filter order: N (calculated)
        Pole-zero plot
        Magnitude & phase response

Key Concept: Butterworth filters have MAXIMALLY FLAT passband
```

**How to run:**
```matlab
run lab10_prelab_task_a.m
% Displays filter order calculation, coefficients, and plots
```

---

### **Lab Task 1: Chebyshev Type-II Band-Pass Filter**
```
Input:  Filter order: 20
        Passband: 800 Hz
        Stopband: 2000 Hz
        Sampling frequency: 6000 Hz
        Passband ripple: 0.75 dB
        Stopband attenuation: 60 dB

Output: Digital band-pass filter coefficients
        Magnitude & phase response plots
        Pole-zero plot
        Stability verification

Key Concept: Chebyshev Type-II has NO ripple in passband,
             but RIPPLE in stopband (steeper rolloff than Butterworth)
```

**How to run:**
```matlab
run lab10_task1.m
% Displays 4 figures:
% 1. Magnitude & Phase response
% 2. Pole-zero plot
% 3-4. Additional analysis plots
```

**What to explain in viva:**
- Why we use Chebyshev Type-II (steeper rolloff needed)
- How bilinear transform converts analog → digital
- What `lp2bp()` does (creates band-pass from low-pass)
- Verify all poles are inside unit circle (stability)

---

### **Lab Task 2: Elliptic (Cauer) Band-Pass Filter**
```
Input:  Registration number: [YOUR_REG_NO]
        Calculate N parameter from registration number:
        - If RegNo < 15:  N = 10 × RegNo
        - If RegNo ≥ 15:  N = RegNo
        
        Passband: (N-5) to (N+5) MHz
        Stopband: (N-5.5) to (N+5.5) MHz
        Sampling frequency: 8N MHz
        Passband ripple: 0.5 dB
        Stopband attenuation: 30 dB

Output: Digital band-pass filter for RF application
        Magnitude & phase response (in MHz)
        Pole-zero plot
        Group delay analysis

Key Concept: Elliptic filters have SHARPEST transition
             (ripple in BOTH passband and stopband)
             LOWEST order for given specifications
```

**IMPORTANT: Before running lab10_task2.m**
1. Open the file: `lab10_task2.m`
2. Find line: `REG_NO = 15;  % ← CHANGE THIS TO YOUR ACTUAL REGISTRATION NUMBER`
3. Replace `15` with your registration number (e.g., `12`, `18`, `20`)
4. Save and run

**How to run:**
```matlab
run lab10_task2.m
% Displays 5 figures:
% 1. Magnitude & Phase response (in MHz)
% 2. Pole-zero plot
% 3. Group delay
% 4-5. Additional analysis
```

**What to explain in viva:**
- How registration number parameterizes the filter
- Why Elliptic filter chosen (sharpest transition for narrow band)
- Verify specifications are met
- Explain pole-zero plot

---

## 🧠 KEY CONCEPTS TO UNDERSTAND

### **1. Bilinear Transform**
- **What**: Converts analog filter (s-plane) to digital filter (z-plane)
- **Formula**: `s = 2·Fs·(z-1)/(z+1)`
- **Why**: Preserves stability (poles always inside unit circle)
- **How in MATLAB**: `[b_dig, a_dig] = bilinear(b_analog, a_analog, Fs)`

### **2. Normalized Frequencies**
- **What**: Frequencies scaled to sampling frequency
- **Range**: [0, 1] where 1 = Nyquist frequency (Fs/2)
- **Formula**: `Ω_norm = f / (Fs/2)`
- **Why**: Makes design independent of actual sampling rate

### **3. Filter Types**
| Type | Passband | Stopband | Best For |
|---|---|---|---|
| **Butterworth** | Flat | Monotonic | General purpose |
| **Chebyshev I** | Ripple | Monotonic | Passband ripple OK |
| **Chebyshev II** | Flat | Ripple | No passband ripple |
| **Elliptic** | Ripple | Ripple | **SHARPEST transition** |

### **4. Low-Pass to Band-Pass Transformation**
- **What**: Converts 1 LP pole to 2 BP poles
- **Effect**: Doubles filter order
- **MATLAB**: `[b_bp, a_bp] = lp2bp(b_lp, a_lp, Wc, Bw)`
- **Parameters**:
  - `Wc` = center frequency = (Ωp + Ωs)/2
  - `Bw` = bandwidth = Ωs - Ωp

### **5. Stability Criterion**
- **Requirement**: All poles MUST satisfy |pole| < 1
- **Check**: `poles = roots(a); is_stable = all(abs(poles) < 1);`
- **If unstable**: Filter output diverges (unbounded growth)

---

## 📊 WHAT TO SHOW IN VIVA

**Prepare these printouts/files:**

1. ✅ **Pre-Lab Task A**
   - Filter order calculation (showing work)
   - Numerator and denominator coefficients
   - Magnitude response plot
   - Pole-zero plot (all poles inside unit circle)

2. ✅ **Lab Task 1**
   - Normalized frequency calculations
   - Chebyshev prototype characteristics
   - Bilinear transform step
   - Band-pass transformation (Wc and Bw values)
   - Magnitude response with passband/stopband markers
   - Pole-zero verification

3. ✅ **Lab Task 2**
   - Your registration number and calculated N parameter
   - Frequency specifications (in MHz)
   - Elliptic filter order calculation
   - Magnitude response covering full frequency range
   - Pole-zero plot (all poles inside unit circle)
   - Group delay (if asked)

4. ✅ **Viva Q&A document**
   - Read through `lab10_viva_qa.md` multiple times
   - Prepare brief explanations for each topic
   - Practice explaining bilinear transform (common question)

---

## 🎯 VIVA TIPS & COMMON QUESTIONS

### **Expected Questions**
1. ✓ "What is the bilinear transform? Explain the formula."
2. ✓ "Why use Chebyshev Type-II in Task 1?"
3. ✓ "Why use Elliptic in Task 2?"
4. ✓ "How do you verify filter stability?"
5. ✓ "What does lp2bp() do?"
6. ✓ "Explain the pole-zero plot."
7. ✓ "Does your filter meet the specifications?"
8. ✓ "What happens if you increase filter order?"

### **Answer Strategy**
- **Always explain WHY** not just WHAT
- **Use analogies** (e.g., "bilinear transform is a mapping between time domains")
- **Point to plots** (magnitude response, pole-zero plot)
- **Show calculations** (normalized frequencies, filter order)
- **Relate to specifications** (how design meets Rp, Rs)

### **If You Get Stuck**
- Draw pole-zero plot by hand
- Explain why poles/zeros are positioned there
- Use MATLAB output to verify answers
- Ask for clarification politely

---

## 🚀 EXECUTION CHECKLIST

Before viva, complete these steps:

- [ ] Change `REG_NO` in `lab10_task2.m` to YOUR registration number
- [ ] Run `lab10_prelab_task_a.m` → Verify output looks correct
- [ ] Run `lab10_task1.m` → Check magnitude response looks like band-pass
- [ ] Run `lab10_task2.m` → Verify plots are in MHz scale
- [ ] Print all figures (magnitude/phase, pole-zero plots)
- [ ] Read through `lab10_viva_qa.md` → Understand key concepts
- [ ] Practice explaining:
  - Bilinear transform formula and why it's used
  - Difference between filter types
  - Why Chebyshev II for Task 1
  - Why Elliptic for Task 2
- [ ] Verify all plots show **poles inside unit circle** (stability)
- [ ] Check **magnitude response meets specifications** (Rp dB in passband, Rs dB in stopband)

---

## 📂 FILE LOCATIONS

All solution files are in:
```
/home/muhammad-ahmad/Desktop/semester-5/DSP/lab/solution/
```

### **Quick Access**
- **Pre-Lab Task A**: `lab10_prelab_task_a.m`
- **Task 1**: `lab10_task1.m`
- **Task 2**: `lab10_task2.m` (Edit REG_NO first!)
- **Viva Q&A**: `lab10_viva_qa.md`
- **This Summary**: `lab10_complete_solution_summary.md`

---

## 🔧 TECHNICAL DETAILS (For Deep Viva)

### **Butterworth Filter (Pre-Lab)**
- Butterworth order calculation:
  ```
  N = ceil( log10((10^(Rs/10) - 1) / (10^(Rp/10) - 1)) / (2 * log10(Ωs/Ωp)) )
  ```
- Characteristic: "Maximally flat" magnitude (no ripple)
- Cutoff frequency: `ωc = ωp / (10^(Rp/20) - 1)^(1/(2N))`

### **Chebyshev Type-II Filter (Task 1)**
- Used because: Need sharp cutoff without passband ripple
- Order given: N = 20 (fixed for this lab)
- Characteristic: Equiripple in stopband, flat passband
- Design formula: Use MATLAB `cheby2(20, Rs, Ωp)`

### **Elliptic (Cauer) Filter (Task 2)**
- Used because: Need sharpest transition for narrow RF band (±5.5 MHz)
- Order calculated: `[N_min, Wn_opt] = ellipord(Wp, Ws, Rp, Rs)`
- Characteristic: Equiripple in both passband and stopband
- Advantage: **Lowest order** for given Rp, Rs
- Formula: `ellip(N, Rp, Rs, Wn, 'bandpass')`

### **Bilinear Transform Details**
- Maps `s = σ + jω` to `z = rejθ`
- Frequency warping: `ωd = 2 * arctan(ωa * Ts / 2)`
- No aliasing in frequency domain (entire jω-axis maps to unit circle)
- Preserves pole-zero structure and stability

---

## ✅ COMPLETION STATUS

| Task | Status | File | Notes |
|---|---|---|---|
| Pre-Lab A | ✅ Complete | lab10_prelab_task_a.m | Ready to run |
| Task 1 | ✅ Complete | lab10_task1.m | Ready to run |
| Task 2 | ⏳ Pending | lab10_task2.m | **Edit REG_NO first!** |
| Viva Q&A | ✅ Complete | lab10_viva_qa.md | Study & practice |

---

## 📞 TROUBLESHOOTING

**Problem: "Undefined function 'lp2bp'"**
- Solution: Ensure Signal Processing Toolbox is installed
- Check: `help lp2bp`

**Problem: "Magnitude response looks wrong"**
- Check: Are you plotting correct frequency range?
- Verify: `freqz(b, a, frequencies)` gives correct values
- Use: `fvtool(b, a)` for built-in verification

**Problem: "Poles outside unit circle"**
- Check: Did you apply bilinear transform?
- Verify: Filter order is reasonable for specifications
- Solution: May need to reduce order or relax specifications

**Problem: "Getting different results than expected"**
- Check: Registration number in Task 2 is correct
- Verify: Normalized frequencies are in [0, 1] range
- Ensure: All MATLAB commands match lab manual

---

## 🎓 LEARNING OUTCOMES

After completing this lab, you should understand:

✅ How analog prototypes are designed  
✅ Why bilinear transform is used for digital filter design  
✅ Differences between Butterworth, Chebyshev, and Elliptic filters  
✅ How to transform low-pass filters to other types (BP, HP, BS)  
✅ How to verify filter stability using pole-zero plots  
✅ How to verify filter meets frequency specifications  
✅ MATLAB tools for IIR filter design  

---

## 🎯 FINAL REMINDER

**Tomorrow is your viva!**

1. Run all three MATLAB scripts today
2. Print all figures
3. Read through the Viva Q&A document
4. Practice explaining key concepts
5. Make sure `lab10_task2.m` has YOUR registration number
6. Bring printed plots and scripts to viva
7. Be confident! You understand IIR filter design now.

**Good luck! 🚀**

---

*Generated: Dec 8, 2024*  
*For: DSP Lab 10 (Digital IIR Filter Design)*  
*Status: Ready for Viva Examination*
