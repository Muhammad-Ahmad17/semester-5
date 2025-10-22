Excellent catch, Muhammad 👏 — yes, **SNR (Signal-to-Noise Ratio)** was mentioned in your earlier list and should definitely be in the viva prep sheet because your instructor asked about it **alongside quantization and noise topics**.

Let’s fix that properly: here’s the **updated version of the relevant section** with SNR fully included, and its explanation in both **theory** and **MATLAB context**.

---

## 🎚️ **Quantization, Noise, and SNR**

### 🔸 **Quantization**

1. **Quantization** – Process of mapping a large set of input values to a smaller set (rounding off amplitude levels).
2. **Quantization Levels** – The number of discrete amplitude values available after quantization (e.g., `2^n` for n-bit quantization).
3. **Quantization Step Size (Δ)** – The difference between two adjacent quantized levels.
4. **Quantization Error** – The difference between the original analog sample and the quantized value.

   * Formula: `e = x - xq`
5. **Quantization Index** – The integer value representing the index (or level number) assigned to each quantized sample.
6. **Quantization Types:**

   * **Uniform Quantization:** Equal step size for all levels.
   * **Non-uniform Quantization:** Step size varies (used in speech/audio coding).

---

### 🔸 **Noise in DSP**

1. **Noise** – Random unwanted variation in signal amplitude.
2. In MATLAB, noise is often simulated using:

   * `rand()` → Uniform noise
   * `randn()` → Gaussian noise
3. **Noise Power** – Average power of noise signal, usually represented by its variance.
4. **Reducing Noise:**

   * Increase sample rate
   * Use filters (`filter`, `lowpass`, etc.)
   * Improve quantization resolution (more bits)

---

### 🔸 **SNR (Signal-to-Noise Ratio)**

1. **Definition:** Ratio of signal power to noise power, representing signal quality.
2. **Formula (Power Ratio):**

   ```
   SNR = Psignal / Pnoise
   ```
3. **In Decibels (dB):**

   ```
   SNR(dB) = 10 * log10(Psignal / Pnoise)
   ```

   or equivalently

   ```
   SNR(dB) = 20 * log10(Vsignal / Vnoise)
   ```
4. **Interpretation:**

   * **Higher SNR → cleaner signal** (less noise)
   * **Lower SNR → noisy signal**
5. **Relation with Quantization:**

   * For an ideal n-bit quantizer:

     ```
     SNR = 6.02n + 1.76 dB
     ```
   * So, increasing bits per sample increases SNR.
6. **In MATLAB Example:**

   ```matlab
   x = audioread('audio.wav');
   noisy = x + 0.05*randn(size(x));
   snrValue = snr(x, noisy - x);
   fprintf('SNR = %.2f dB\n', snrValue);
   ```

---

### 🔸 **Echo & SNR Relation**

* Adding echo increases the **signal duration** and may slightly **lower SNR** if the delayed signal overlaps too much with the original.
* Filters (`filter()`, `lowpass()`) can improve effective SNR by removing unwanted frequencies.

---
Excellent catch, Muhammad 👏 — yes, **SNR (Signal-to-Noise Ratio)** was mentioned in your earlier list and should definitely be in the viva prep sheet because your instructor asked about it **alongside quantization and noise topics**.

Let’s fix that properly: here’s the **updated version of the relevant section** with SNR fully included, and its explanation in both **theory** and **MATLAB context**.

---

## 🎚️ **Quantization, Noise, and SNR**

### 🔸 **Quantization**

1. **Quantization** – Process of mapping a large set of input values to a smaller set (rounding off amplitude levels).
2. **Quantization Levels** – The number of discrete amplitude values available after quantization (e.g., `2^n` for n-bit quantization).
3. **Quantization Step Size (Δ)** – The difference between two adjacent quantized levels.
4. **Quantization Error** – The difference between the original analog sample and the quantized value.

   * Formula: `e = x - xq`
5. **Quantization Index** – The integer value representing the index (or level number) assigned to each quantized sample.
6. **Quantization Types:**

   * **Uniform Quantization:** Equal step size for all levels.
   * **Non-uniform Quantization:** Step size varies (used in speech/audio coding).

---

### 🔸 **Noise in DSP**

1. **Noise** – Random unwanted variation in signal amplitude.
2. In MATLAB, noise is often simulated using:

   * `rand()` → Uniform noise
   * `randn()` → Gaussian noise
3. **Noise Power** – Average power of noise signal, usually represented by its variance.
4. **Reducing Noise:**

   * Increase sample rate
   * Use filters (`filter`, `lowpass`, etc.)
   * Improve quantization resolution (more bits)

---

### 🔸 **SNR (Signal-to-Noise Ratio)**

1. **Definition:** Ratio of signal power to noise power, representing signal quality.
2. **Formula (Power Ratio):**

   ```
   SNR = Psignal / Pnoise
   ```
3. **In Decibels (dB):**

   ```
   SNR(dB) = 10 * log10(Psignal / Pnoise)
   ```

   or equivalently

   ```
   SNR(dB) = 20 * log10(Vsignal / Vnoise)
   ```
4. **Interpretation:**

   * **Higher SNR → cleaner signal** (less noise)
   * **Lower SNR → noisy signal**
5. **Relation with Quantization:**

   * For an ideal n-bit quantizer:

     ```
     SNR = 6.02n + 1.76 dB
     ```
   * So, increasing bits per sample increases SNR.
6. **In MATLAB Example:**

   ```matlab
   x = audioread('audio.wav');
   noisy = x + 0.05*randn(size(x));
   snrValue = snr(x, noisy - x);
   fprintf('SNR = %.2f dB\n', snrValue);
   ```

---

### 🔸 **Echo & SNR Relation**

* Adding echo increases the **signal duration** and may slightly **lower SNR** if the delayed signal overlaps too much with the original.
* Filters (`filter()`, `lowpass()`) can improve effective SNR by removing unwanted frequencies.

---

Would you like me to integrate this **SNR + Quantization section** neatly into the **complete DSP Viva Masterlist (with one-line answers)** and format it into a **printable PDF cheat sheet** for quick revision before viva?
Excellent catch, Muhammad 👏 — yes, **SNR (Signal-to-Noise Ratio)** was mentioned in your earlier list and should definitely be in the viva prep sheet because your instructor asked about it **alongside quantization and noise topics**.

Let’s fix that properly: here’s the **updated version of the relevant section** with SNR fully included, and its explanation in both **theory** and **MATLAB context**.

---

## 🎚️ **Quantization, Noise, and SNR**

### 🔸 **Quantization**

1. **Quantization** – Process of mapping a large set of input values to a smaller set (rounding off amplitude levels).
2. **Quantization Levels** – The number of discrete amplitude values available after quantization (e.g., `2^n` for n-bit quantization).
3. **Quantization Step Size (Δ)** – The difference between two adjacent quantized levels.
4. **Quantization Error** – The difference between the original analog sample and the quantized value.

   * Formula: `e = x - xq`
5. **Quantization Index** – The integer value representing the index (or level number) assigned to each quantized sample.
6. **Quantization Types:**

   * **Uniform Quantization:** Equal step size for all levels.
   * **Non-uniform Quantization:** Step size varies (used in speech/audio coding).

---

### 🔸 **Noise in DSP**

1. **Noise** – Random unwanted variation in signal amplitude.
2. In MATLAB, noise is often simulated using:

   * `rand()` → Uniform noise
   * `randn()` → Gaussian noise
3. **Noise Power** – Average power of noise signal, usually represented by its variance.
4. **Reducing Noise:**

   * Increase sample rate
   * Use filters (`filter`, `lowpass`, etc.)
   * Improve quantization resolution (more bits)

---

### 🔸 **SNR (Signal-to-Noise Ratio)**

1. **Definition:** Ratio of signal power to noise power, representing signal quality.
2. **Formula (Power Ratio):**

   ```
   SNR = Psignal / Pnoise
   ```
3. **In Decibels (dB):**

   ```
   SNR(dB) = 10 * log10(Psignal / Pnoise)
   ```

   or equivalently

   ```
   SNR(dB) = 20 * log10(Vsignal / Vnoise)
   ```
4. **Interpretation:**

   * **Higher SNR → cleaner signal** (less noise)
   * **Lower SNR → noisy signal**
5. **Relation with Quantization:**

   * For an ideal n-bit quantizer:

     ```
     SNR = 6.02n + 1.76 dB
     ```
   * So, increasing bits per sample increases SNR.
6. **In MATLAB Example:**

   ```matlab
   x = audioread('audio.wav');
   noisy = x + 0.05*randn(size(x));
   snrValue = snr(x, noisy - x);
   fprintf('SNR = %.2f dB\n', snrValue);
   ```

---

### 🔸 **Echo & SNR Relation**

* Adding echo increases the **signal duration** and may slightly **lower SNR** if the delayed signal overlaps too much with the original.
* Filters (`filter()`, `lowpass()`) can improve effective SNR by removing unwanted frequencies.

---

Excellent catch, Muhammad 👏 — yes, **SNR (Signal-to-Noise Ratio)** was mentioned in your earlier list and should definitely be in the viva prep sheet because your instructor asked about it **alongside quantization and noise topics**.

Let’s fix that properly: here’s the **updated version of the relevant section** with SNR fully included, and its explanation in both **theory** and **MATLAB context**.

---

## 🎚️ **Quantization, Noise, and SNR**

### 🔸 **Quantization**

1. **Quantization** – Process of mapping a large set of input values to a smaller set (rounding off amplitude levels).
2. **Quantization Levels** – The number of discrete amplitude values available after quantization (e.g., `2^n` for n-bit quantization).
3. **Quantization Step Size (Δ)** – The difference between two adjacent quantized levels.
4. **Quantization Error** – The difference between the original analog sample and the quantized value.

   * Formula: `e = x - xq`
5. **Quantization Index** – The integer value representing the index (or level number) assigned to each quantized sample.
6. **Quantization Types:**

   * **Uniform Quantization:** Equal step size for all levels.
   * **Non-uniform Quantization:** Step size varies (used in speech/audio coding).

---

### 🔸 **Noise in DSP**

1. **Noise** – Random unwanted variation in signal amplitude.
2. In MATLAB, noise is often simulated using:

   * `rand()` → Uniform noise
   * `randn()` → Gaussian noise
3. **Noise Power** – Average power of noise signal, usually represented by its variance.
4. **Reducing Noise:**

   * Increase sample rate
   * Use filters (`filter`, `lowpass`, etc.)
   * Improve quantization resolution (more bits)

---

### 🔸 **SNR (Signal-to-Noise Ratio)**

1. **Definition:** Ratio of signal power to noise power, representing signal quality.
2. **Formula (Power Ratio):**

   ```
   SNR = Psignal / Pnoise
   ```
3. **In Decibels (dB):**

   ```
   SNR(dB) = 10 * log10(Psignal / Pnoise)
   ```

   or equivalently

   ```
   SNR(dB) = 20 * log10(Vsignal / Vnoise)
   ```
4. **Interpretation:**

   * **Higher SNR → cleaner signal** (less noise)
   * **Lower SNR → noisy signal**
5. **Relation with Quantization:**

   * For an ideal n-bit quantizer:

     ```
     SNR = 6.02n + 1.76 dB
     ```
   * So, increasing bits per sample increases SNR.
6. **In MATLAB Example:**

   ```matlab
   x = audioread('audio.wav');
   noisy = x + 0.05*randn(size(x));
   snrValue = snr(x, noisy - x);
   fprintf('SNR = %.2f dB\n', snrValue);
   ```

---

### 🔸 **Echo & SNR Relation**

* Adding echo increases the **signal duration** and may slightly **lower SNR** if the delayed signal overlaps too much with the original.
* Filters (`filter()`, `lowpass()`) can improve effective SNR by removing unwanted frequencies.

---

Would you like me to integrate this **SNR + Quantization section** neatly into the **complete DSP Viva Masterlist (with one-line answers)** and format it into a **printable PDF cheat sheet** for quick revision before viva?
Excellent catch, Muhammad 👏 — yes, **SNR (Signal-to-Noise Ratio)** was mentioned in your earlier list and should definitely be in the viva prep sheet because your instructor asked about it **alongside quantization and noise topics**.

Let’s fix that properly: here’s the **updated version of the relevant section** with SNR fully included, and its explanation in both **theory** and **MATLAB context**.

---

## 🎚️ **Quantization, Noise, and SNR**

### 🔸 **Quantization**

1. **Quantization** – Process of mapping a large set of input values to a smaller set (rounding off amplitude levels).
2. **Quantization Levels** – The number of discrete amplitude values available after quantization (e.g., `2^n` for n-bit quantization).
3. **Quantization Step Size (Δ)** – The difference between two adjacent quantized levels.
4. **Quantization Error** – The difference between the original analog sample and the quantized value.

   * Formula: `e = x - xq`
5. **Quantization Index** – The integer value representing the index (or level number) assigned to each quantized sample.
6. **Quantization Types:**

   * **Uniform Quantization:** Equal step size for all levels.
   * **Non-uniform Quantization:** Step size varies (used in speech/audio coding).

---

### 🔸 **Noise in DSP**

1. **Noise** – Random unwanted variation in signal amplitude.
2. In MATLAB, noise is often simulated using:

   * `rand()` → Uniform noise
   * `randn()` → Gaussian noise
3. **Noise Power** – Average power of noise signal, usually represented by its variance.
4. **Reducing Noise:**

   * Increase sample rate
   * Use filters (`filter`, `lowpass`, etc.)
   * Improve quantization resolution (more bits)

---

### 🔸 **SNR (Signal-to-Noise Ratio)**

1. **Definition:** Ratio of signal power to noise power, representing signal quality.
2. **Formula (Power Ratio):**

   ```
   SNR = Psignal / Pnoise
   ```
3. **In Decibels (dB):**

   ```
   SNR(dB) = 10 * log10(Psignal / Pnoise)
   ```

   or equivalently

   ```
   SNR(dB) = 20 * log10(Vsignal / Vnoise)
   ```
4. **Interpretation:**

   * **Higher SNR → cleaner signal** (less noise)
   * **Lower SNR → noisy signal**
5. **Relation with Quantization:**

   * For an ideal n-bit quantizer:

     ```
     SNR = 6.02n + 1.76 dB
     ```
   * So, increasing bits per sample increases SNR.
6. **In MATLAB Example:**

   ```matlab
   x = audioread('audio.wav');
   noisy = x + 0.05*randn(size(x));
   snrValue = snr(x, noisy - x);
   fprintf('SNR = %.2f dB\n', snrValue);
   ```

---

### 🔸 **Echo & SNR Relation**

* Adding echo increases the **signal duration** and may slightly **lower SNR** if the delayed signal overlaps too much with the original.
* Filters (`filter()`, `lowpass()`) can improve effective SNR by removing unwanted frequencies.

---

Would you like me to integrate this **SNR + Quantization section** neatly into the **complete DSP Viva Masterlist (with one-line answers)** and format it into a **printable PDF cheat sheet** for quick revision before viva?
Excellent catch, Muhammad 👏 — yes, **SNR (Signal-to-Noise Ratio)** was mentioned in your earlier list and should definitely be in the viva prep sheet because your instructor asked about it **alongside quantization and noise topics**.

Let’s fix that properly: here’s the **updated version of the relevant section** with SNR fully included, and its explanation in both **theory** and **MATLAB context**.

---

## 🎚️ **Quantization, Noise, and SNR**

### 🔸 **Quantization**

1. **Quantization** – Process of mapping a large set of input values to a smaller set (rounding off amplitude levels).
2. **Quantization Levels** – The number of discrete amplitude values available after quantization (e.g., `2^n` for n-bit quantization).
3. **Quantization Step Size (Δ)** – The difference between two adjacent quantized levels.
4. **Quantization Error** – The difference between the original analog sample and the quantized value.

   * Formula: `e = x - xq`
5. **Quantization Index** – The integer value representing the index (or level number) assigned to each quantized sample.
6. **Quantization Types:**

   * **Uniform Quantization:** Equal step size for all levels.
   * **Non-uniform Quantization:** Step size varies (used in speech/audio coding).

---

### 🔸 **Noise in DSP**

1. **Noise** – Random unwanted variation in signal amplitude.
2. In MATLAB, noise is often simulated using:

   * `rand()` → Uniform noise
   * `randn()` → Gaussian noise
3. **Noise Power** – Average power of noise signal, usually represented by its variance.
4. **Reducing Noise:**

   * Increase sample rate
   * Use filters (`filter`, `lowpass`, etc.)
   * Improve quantization resolution (more bits)

---

### 🔸 **SNR (Signal-to-Noise Ratio)**

1. **Definition:** Ratio of signal power to noise power, representing signal quality.
2. **Formula (Power Ratio):**

   ```
   SNR = Psignal / Pnoise
   ```
3. **In Decibels (dB):**

   ```
   SNR(dB) = 10 * log10(Psignal / Pnoise)
   ```

   or equivalently

   ```
   SNR(dB) = 20 * log10(Vsignal / Vnoise)
   ```
4. **Interpretation:**

   * **Higher SNR → cleaner signal** (less noise)
   * **Lower SNR → noisy signal**
5. **Relation with Quantization:**

   * For an ideal n-bit quantizer:

     ```
     SNR = 6.02n + 1.76 dB
     ```
   * So, increasing bits per sample increases SNR.
6. **In MATLAB Example:**

   ```matlab
   x = audioread('audio.wav');
   noisy = x + 0.05*randn(size(x));
   snrValue = snr(x, noisy - x);
   fprintf('SNR = %.2f dB\n', snrValue);
   ```

---

### 🔸 **Echo & SNR Relation**

* Adding echo increases the **signal duration** and may slightly **lower SNR** if the delayed signal overlaps too much with the original.
* Filters (`filter()`, `lowpass()`) can improve effective SNR by removing unwanted frequencies.

---

Excellent catch, Muhammad 👏 — yes, **SNR (Signal-to-Noise Ratio)** was mentioned in your earlier list and should definitely be in the viva prep sheet because your instructor asked about it **alongside quantization and noise topics**.

Let’s fix that properly: here’s the **updated version of the relevant section** with SNR fully included, and its explanation in both **theory** and **MATLAB context**.
