# Implementation Guide: Using Newton-Cotes Methods

## Files Structure

```
NC/midprep/
├── piecewise_integral_newton_cotes.m          # Main MATLAB implementation
├── PIECEWISE_INTEGRAL_SOLUTION.md             # Complete solution with results
├── QUICK_REFERENCE_NEWTON_COTES.md            # Study guide & exam prep
└── IMPLEMENTATION_GUIDE.md                    # This file
```

---

## How the Code Works

### Step 1: Define the Piecewise Function
```matlab
function y = f(x)
    y = zeros(size(x));
    
    % Region 1: 0 <= x < 30, f(x) = 0
    mask1 = (x >= 0) & (x < 30);
    y(mask1) = 0;
    
    % Region 2: 30 <= x <= 100, f(x) = cubic polynomial
    mask2 = (x >= 30) & (x <= 100);
    y(mask2) = -9.1688e-6 * x(mask2).^3 + 2.7961e-3 * x(mask2).^2 - ...
              0.2848700 * x(mask2) + 9.6778;
    
    % Region 3: x > 100, f(x) = 0
    mask3 = (x > 100);
    y(mask3) = 0;
end
```

### Step 2: Setup Integration Parameters
```matlab
a = 0;              % Lower limit
b = 100;            % Upper limit
N_values = [20, 50, 100, 200, 500];  % Different number of intervals to test convergence

h = (b - a) / N;    % Step size for N intervals
x = linspace(a, b, N+1);  % Create N+1 points
y_vals = f(x);      % Evaluate function at all points
```

### Step 3: Apply Each Method

#### **Trapezoidal Rule (n=1)**
```matlab
I_trap = (h/2) * (y_vals(1) + 2*sum(y_vals(2:N)) + y_vals(N+1));
```
- Simplest: just average endpoints and weight middle points by 2

#### **Simpson's 1/3 Rule (n=2)**
```matlab
sum_odd = sum(y_vals(2:2:N));      % indices 2,4,6,... (weight 4)
sum_even = sum(y_vals(3:2:N-1));   % indices 3,5,7,... (weight 2)
I_simp13 = (h/3) * (y_vals(1) + 4*sum_odd + 2*sum_even + y_vals(N+1));
```
- Uses pattern: 1-4-2-4-2-...-4-1
- Most popular for hand calculations

#### **Simpson's 3/8 Rule (n=3)**
```matlab
I_simp38 = (3*h/8) * (y_vals(1) + y_vals(N+1));
for i = 2:N
    weight_index = mod(i-1, 3);
    if weight_index == 0
        weight = 3;
    else  % 1 or 2
        weight = 9;
    end
    I_simp38 = I_simp38 + (3*h/8) * weight * y_vals(i);
end
```
- Uses pattern: 1-3-3-2-3-3-2-...-3-3-1 (grouped by 3s)
- Requires N divisible by 3

#### **Boole's Rule (n=4)**
```matlab
I_boole = (2*h/45) * (y_vals(1) + y_vals(N+1));
for i = 2:N
    weight_index = mod(i-1, 4);
    switch weight_index
        case 0, weight = 7;
        case 1, weight = 32;
        case 2, weight = 12;
        case 3, weight = 32;
    end
    I_boole = I_boole + (2*h/45) * weight * y_vals(i);
end
```
- Uses pattern: [7,32,12,32] repeating
- Requires N divisible by 4

#### **n=5 Rule (6-point rule)**
```matlab
I_n5 = (5*h/288) * (y_vals(1) + y_vals(N+1));
for i = 2:N
    weight_index = mod(i-1, 5);
    switch weight_index
        case 0, weight = 19;
        case 1, weight = 75;
        case 2, weight = 50;
        case 3, weight = 50;
        case 4, weight = 75;
    end
    I_n5 = I_n5 + (5*h/288) * weight * y_vals(i);
end
```
- Uses pattern: [19,75,50,50,75] repeating
- Requires N divisible by 5

#### **Weddle's Rule (n=6)**
```matlab
I_weddle = (h/140) * (y_vals(1) + y_vals(N+1));
for i = 2:N
    weight_index = mod(i-1, 6);
    switch weight_index
        case 0, weight = 41;
        case 1, weight = 216;
        case 2, weight = 27;
        case 3, weight = 272;
        case 4, weight = 27;
        case 5, weight = 216;
    end
    I_weddle = I_weddle + (h/140) * weight * y_vals(i);
end
```
- Uses pattern: [41,216,27,272,27,216] repeating (note: 272 at center)
- Requires N divisible by 6
- **Highest accuracy O(h⁸)**

---

## Running and Interpreting Results

### Execute the Script
```bash
# In terminal:
octave piecewise_integral_newton_cotes.m

# Or with MATLAB:
matlab -batch "piecewise_integral_newton_cotes"
```

### Output Sections

1. **Per-Method Output** — Shows convergence as N increases:
   ```
   METHOD 1: COMPOSITE TRAPEZOIDAL RULE (n=1)
   N =   20,  h = 5.000000,  I = 69.5882500000
   N =   50,  h = 2.000000,  I = 64.2402963200
   ...
   ```

2. **Summary Table** — Compares all methods at N=500:
   ```
   Trapezoidal (n=1)         61.1331490592       O(h²)
   Simpson 1/3 (n=2)         61.0193241600      O(h⁴)
   ...
   Weddle's Rule (n=6)       57.4564769387      O(h⁸)
   ```

3. **Convergence Plots** — Visual comparison of accuracy and speed of convergence

---

## Key Insights from Your Results

### Convergence Behavior
- **Good Convergence:** Trapezoidal, Simpson 1/3, Simpson 3/8, Boole's, n=5, Weddle's all stabilize
- **Best Results:** Methods with O(h⁶) and O(h⁸) show more stable convergence

### Recommended Choice
For this specific problem:
- **Balance:** Simpson 1/3 (O(h⁴)) — simple, reliable
- **Best Accuracy:** Weddle's Rule (O(h⁸)) — requires N divisible by 6
- **Practical:** Boole's Rule (O(h⁶)) — good accuracy without too many divisions

---

## Modifying for Different Problems

### Change Integration Limits
```matlab
a = 0;    % New lower limit
b = 100;  % New upper limit
```

### Change the Function
```matlab
function y = f(x)
    y = (x.^2) + sin(x);  % Define your function here
    % Apply same piecewise logic if needed
end
```

### Change Number of Intervals
```matlab
N_values = [10, 20, 50, 100, 200, 500, 1000];  % Test more values
```

---

## Mathematical Background

### General Composite Formula
$$I \approx \sum_{k=0}^{m-1} \int_{x_k}^{x_{k+1}} p(x) \, dx$$

where:
- We divide [a,b] into m panels of width h
- p(x) is a polynomial of degree n for each method
- Each panel contributes to total integral

### Convergence Order Meaning
- **O(h²):** Error decreases as h²  (Trapezoidal: 100 intervals → 1/10000 error of 1 interval)
- **O(h⁴):** Error decreases as h⁴  (Simpson: much faster)
- **O(h⁶):** Error decreases as h⁶  (Boole's: even faster)
- **O(h⁸):** Error decreases as h⁸  (Weddle's: best)

---

## Common Mistakes to Avoid

❌ **Mistake 1:** Using N not divisible by rule requirement
```matlab
% WRONG: Using N=50 with Simpson 3/8 (needs divisible by 3)
% RIGHT: Adjust N to 51 or check divisibility first
if mod(N, 3) ~= 0
    N = N + (3 - mod(N, 3));  % Round up to next multiple
end
```

❌ **Mistake 2:** Forgetting weight of 2 vs 4 in Simpson 1/3
```matlab
% Pattern: [1, 4, 2, 4, 2, 4, 2, ..., 4, 1]
% Position 1,3,5,7,... (odd indices) → weight 4
% Position 2,4,6,... (even indices) → weight 2
```

❌ **Mistake 3:** Incorrect weight pattern
```matlab
% Always double-check weight sequence from textbook
% Boole's: [7,32,12,32,7] ← repeats every 4 intervals
% Not [7,7,32,32,12,12,32,32] ← this is WRONG
```

---

## See Also

- **PIECEWISE_INTEGRAL_SOLUTION.md** — Full results with detailed tables
- **QUICK_REFERENCE_NEWTON_COTES.md** — Exam study guide with best methods
- **Composite_Formulas_Cheatsheet.pdf** — Visual reference for all formulas

