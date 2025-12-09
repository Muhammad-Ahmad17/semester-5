# LAB # 11

## To follow the design of Linear Phase FIR filters based on Windows using MATLAB
(Open Ended Lab)

### Objectives
- To construct FIR filters based on Windows
- To perform Open Ended Lab Task (Lab Task 2)

### Pre Lab

The basic idea behind the window design is to choose a proper ideal frequency selective filter (which always has non-causal, infinite duration impulse response) with ideal impulse to obtain a linear phase in impulse response to obtain a linear phase and causal FIR filter. Therefore, the emphasis in this method is on selecting an appropriate windowing function and appropriate ideal filter. We will consider an ideal lowpass filter with unity magnitude gain and linear-phase characteristics over its pass band, and zero response over its stop band.

As an ideal LPF of bandwidth $w_c$ or is given by:

$$H_d(e^{j\omega}) = \begin{cases} e^{-j\omega\alpha} & |\omega| \leq \omega_c \\ 0 & \omega_c < |\omega| \leq \pi \end{cases}$$

where $w_c$ is also called the cut-off frequency, and $\alpha$ is called the sample delay (note that from the DTFT properties, $e^{-j\omega\alpha}$ implies shift in the positive direction or delay). The impulse response of the filter is of infinite duration and is given by:

$$h_d(n) = \mathcal{F}^{-1}\{H_d(e^{j\omega})\} = \frac{1}{2\pi}\int_{-\pi}^{\pi} H_d(e^{j\omega})e^{j\omega n}d\omega$$

$$= \frac{1}{2\pi}\int_{-\omega_c}^{\omega_c} e^{-j\omega(\alpha-n)}d\omega$$

$$= \frac{\sin[\omega_c(n-\alpha)]}{\pi(n-\alpha)}$$

Note that $h_d(n)$ is symmetric with respect to $\alpha$, a fact useful for linear phase FIR filters.

To obtain an FIR filter from $h_d(n)$, one has to truncate $h_d(n)$ on both sides. To obtain a causal and linear-phase FIR filter length M, we must have:

$$h(n) = \begin{cases} h_d(n) & 0 \leq n \leq M-1 \\ 0 & \alpha = \frac{M-1}{2} \end{cases}$$

and

This operation is called "windowing". In general, $h(n)$ can be thought of as being formed by the product of $h_d(n)$ and a window function $w(n)$ as follows:

$$h(n) = h_d(n) \cdot w(n)$$

$$w(n) = \begin{cases} \text{some symmetric basis} & 0 \leq n \leq M-1 \\ 0 & \text{otherwise} \end{cases}$$

Depending on how we define $w(n)$ above, we obtain different window designs.

---

## WINDOW DESIGN TECHNIQUES

### Table 11.1: Summary of commonly used window function characteristics

| Window Name | Transition Approximate | Width $\Delta\omega$ Exact Values | Min. Stopband Attenuation |
|---|---|---|---|
| Rectangular | $\frac{4\pi}{M}$ | $\frac{1.8\pi}{M}$ | 21 dB |
| | | | |
| Bartlett | $\frac{8\pi}{M}$ | $\frac{6.1\pi}{M}$ | 25 dB |
| | | | |
| Hanning | $\frac{8\pi}{M}$ | $\frac{6.2\pi}{M}$ | 44 dB |
| | | | |
| Hamming | $\frac{8\pi}{M}$ | $\frac{6.6\pi}{M}$ | 53 dB |
| | | | |
| Blackman | $\frac{12\pi}{M}$ | $\frac{11\pi}{M}$ | 74 dB |

### Design Equations

Given $\omega_p$, $\omega_s$, $\beta_p$, and $A_s$:

$$\text{Norm. transition width} = \Delta f = \frac{\omega_s - \omega_p}{2\pi}$$

$$\text{Filter order } M \approx \frac{A_s - 7.95}{14.36 \Delta f} + 1$$

$$\text{Parameter } \beta = \begin{cases} 0.1102(A_s - 8.7) & A_s \geq 50 \\ 0.5842(A_s - 21)^{0.4} + 0.07886(A_s - 21) & 21 \leq A_s < 50 \end{cases}$$

### MATLAB Implementation

MATLAB provides several routines to implement window functions discussed in this section. A brief description of these routines is given below:

- `w=rectwin(M)` returns the M-point rectangular window function in array w.
- `w=triang(M)` returns the M-point Bartlett triangular window function in array w.
- `w=hann(M)` returns the K-point Hanning window function in array w.
- `w=hamming(M)` returns the M-point Hamming window function in array w.
- `w=blackman(M)` returns the B-point Blackman window function in array w.
- `w=blackmanM(beta)` returns the beta-scaled Kaplan window function in array w.

Using these routines, we can use MATLAB to design FIR filters based on the window technique, which also requires an ideal low pass impulse response. It is convenient to have a simple routine that creates h(n).

---

## Pre-Lab Tasks
*(Attach extra A4 White page if needed)*

Design a high pass FIR filter using window method. Select an appropriate window function and justify your choice by comparing it with any of other window function for same high pass filter.

---

## Lab Tasks

### Lab task 1

Design a digital FIR lowpass filter with the following specifications:

**Passband cut off:** $\omega_c = 0.2\pi$

**Stopband Attenuation:** $A_s = 50dB$

Choose an appropriate window function from Table 11.1. Determine the impulse response and provide a plot of the frequency response of the designed filter.

#### Solution
[Space for solution]

---

### Lab task 2 (Open Ended Lab Task)

Design a filter for digital signal processing with following specifications: At the receiver side of radar a bandpass filter is used to extract the information from transmitted signal. Furthermore, the wave shape of a returned radar signal contain important information about the target's properties it is required that the phase response of filter should be linear in passband and filter should have minimum order. Other parameters are given below

**Stop band attenuation** $A_s = A$ dB

**Pass band edge frequencies:** (N-5) – (N+5) MHz

**Sampling rate:** 4N MHz

Where,

$$N = \begin{cases} 10R & \text{if } R < 15 \\ R & \text{otherwise} \end{cases} \quad MHz \quad R = \text{Your Reg. No.}$$

$$A = \begin{cases} 70 & R \leq 15 \\ 50 & 16 \leq R \leq 25 \\ 40 & 26 \leq R \leq 35 \\ 21 & R > 35 \end{cases}$$

#### Tasks:

i) Explain with reason which filter type you would prefer based on the specifications

ii) Compute the normalized values of edge frequencies

iii) Display the Magnitude response of the designed filter. (Also use data cursors to show Stop band attenuation)

iv) Display the Phase Response of the designed filter.

#### Solution
[Space for solution]

---

*Abu bakar Talha* | IEEE 324 | Digital Signal Processing Lab Manual | Page 62-65
