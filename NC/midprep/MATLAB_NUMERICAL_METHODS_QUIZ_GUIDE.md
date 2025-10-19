# 🎓 MATLAB Numerical Methods Quiz Preparation Guide

**Quiz Date:** October 20, 2025  
**Topics:** Root Finding Methods (Bisection, Newton-Raphson, Secant, Regula Falsi, Fixed Point, Modified Newton)

---

## 📋 UNIVERSAL MATLAB TEMPLATE FOR ALL ROOT-FINDING METHODS

**USE THIS EXACT PATTERN - Change only the formulas and method names!**

```matlab
%═══════════════════════════════════════════════════════════════════════════════
% METHOD NAME: [Method Name Here]
%═══════════════════════════════════════════════════════════════════════════════

clc
clear
close all

%─────────────────────────────────────────────────────────────────────────────
% STEP 1: DEFINE FUNCTION AND DERIVATIVES (If needed)
%─────────────────────────────────────────────────────────────────────────────

f = @(x) [FUNCTION HERE];           % Main function

% Uncomment derivative if method requires it
% df = @(x) [DERIVATIVE HERE];      % First derivative (Newton, Modified Newton)
% d2f = @(x) [2ND DERIVATIVE HERE]; % Second derivative (Modified Newton)

%─────────────────────────────────────────────────────────────────────────────
% STEP 2: INPUT INITIAL GUESS(ES)
%─────────────────────────────────────────────────────────────────────────────

% For Bisection/Regula Falsi (bracket methods):
% a = input('Enter value of a: ');
% b = input('Enter value of b: ');

% For Newton/Secant/Fixed Point (open methods):
x0 = input('Enter initial guess x0: ');

%─────────────────────────────────────────────────────────────────────────────
% STEP 3: VALIDATE INPUT (For bracket methods)
%─────────────────────────────────────────────────────────────────────────────

% Uncomment for bracket methods:
% while f(a) * f(b) > 0
%     disp('f(a) and f(b) must have opposite signs!');
%     a = input('Enter new value of a: ');
%     b = input('Enter new value of b: ');
% end

%─────────────────────────────────────────────────────────────────────────────
% STEP 4: SET PARAMETERS
%─────────────────────────────────────────────────────────────────────────────

tol = 1e-6;        % Tolerance (convergence criterion)
maxIter = 50;      % Maximum iterations
i = 0;             % Iteration counter
err = inf;         % Initialize error

%─────────────────────────────────────────────────────────────────────────────
% STEP 5: PRINT HEADER
%─────────────────────────────────────────────────────────────────────────────

fprintf('\n%s\n', '═══════════════════════════════════════════════════════════════');
fprintf('%-5s %-15s %-15s %-15s %-15s\n', ...
    'Iter', 'x_value', 'f(x)', 'Error(%%)', 'Other');
fprintf('%s\n', '═══════════════════════════════════════════════════════════════');

%─────────────────────────────────────────────────────────────────────────────
% STEP 6: ITERATION LOOP
%─────────────────────────────────────────────────────────────────────────────

while (err > tol) && (i < maxIter)
    i = i + 1;

    % *** PASTE YOUR METHOD-SPECIFIC FORMULA HERE ***
    % Different for each method!
    
    % Compute error
    if i == 1
        err = inf;  % First iteration has no previous value
    else
        err = abs((x_new - x_old) / x_new) * 100;  % Relative error
    end

    % Print iteration results
    fprintf('%-5d %-15.8f %-15.8f %-15.6f\n', i, x_new, f(x_new), err);

    % Update for next iteration
    x_old = x_new;
    % x0 = x_new;  (uncomment based on method)
end

%─────────────────────────────────────────────────────────────────────────────
% STEP 7: DISPLAY FINAL RESULT
%─────────────────────────────────────────────────────────────────────────────

fprintf('%s\n', '═══════════════════════════════════════════════════════════════');
fprintf('\n✅ Approximate root = %.8f\n', x_new);
fprintf('✅ Found in %d iterations\n', i);
fprintf('✅ Tolerance = %.1e\n', tol);
fprintf('✅ Final error = %.6e\n\n', err);

```

---

## 🔍 METHOD-SPECIFIC FORMULAS (The ONLY thing that changes!)

### **1. BISECTION METHOD**
- **Category:** Closed/Bracket method
- **Requires:** Function only
- **Formula in Loop:**
```matlab
Xm = (a + b) / 2;                    % Midpoint

if f(a) * f(Xm) < 0
    b = Xm;                          % Root in [a, Xm]
else
    a = Xm;                          % Root in [Xm, b]
end

x_new = Xm;
```

---

### **2. NEWTON-RAPHSON METHOD**
- **Category:** Open method
- **Requires:** Function + First derivative
- **Formula in Loop:**
```matlab
f0 = f(x0);
f1 = df(x0);

x_new = x0 - f0 / f1;                % Newton's formula

x0 = x_new;
```

---

### **3. SECANT METHOD**
- **Category:** Open method
- **Requires:** Function only (no derivative!)
- **Requires:** TWO initial guesses (x0, x1)
- **Formula in Loop:**
```matlab
x2 = x1 - f(x1) * (x1 - x0) / (f(x1) - f(x0));

x0 = x1;
x1 = x2;
x_new = x2;
```

---

### **4. REGULA FALSI METHOD (False Position)**
- **Category:** Closed/Bracket method
- **Requires:** Function only
- **Formula in Loop:**
```matlab
x_new = (a*f(b) - b*f(a)) / (f(b) - f(a));  % Linear interpolation

if f(a) * f(x_new) < 0
    b = x_new;                       % Root in [a, x_new]
else
    a = x_new;                       % Root in [x_new, b]
end
```

---

### **5. FIXED POINT ITERATION**
- **Category:** Open method
- **Requires:** Rearranged function g(x) where x = g(x)
- **Condition:** |g'(x)| < 1 at root for convergence
- **Formula in Loop:**
```matlab
x_new = g(x0);                       % Simple iteration formula

x0 = x_new;
```

---

### **6. MODIFIED NEWTON-RAPHSON METHOD**
- **Category:** Open method
- **Requires:** Function + First & Second derivatives
- **Used for:** Multiple roots (when f(x) = 0 AND f'(x) = 0)
- **Formula in Loop (Variant 1):**
```matlab
f0 = f(x0);
f1 = df(x0);
f2 = d2f(x0);

x_new = x0 - (f0 * f1) / (f1^2 - f0 * f2);

x0 = x_new;
```

---

## 📊 QUICK REFERENCE TABLE

| Method | Type | Input | Convergence | Best For |
|--------|------|-------|-------------|----------|
| **Bisection** | Closed | f(x), [a,b] | Slow, Guaranteed | Bracketing |
| **Newton-Raphson** | Open | f(x), f'(x), x0 | Fast, Quadratic | Simple roots |
| **Secant** | Open | f(x), x0, x1 | Fast | No derivative |
| **Regula Falsi** | Closed | f(x), [a,b] | Medium | Bracketing |
| **Fixed Point** | Open | g(x), x0 | Variable | Rearranged form |
| **Modified NR** | Open | f(x), f'(x), f''(x), x0 | Very Fast | Multiple roots |

---

## 💡 KEY CONCEPTS TO REMEMBER

### **Error Calculations**
```matlab
% Absolute Error
err_abs = abs(x_new - x_old);

% Relative Error (percentage)
err_rel = abs((x_new - x_old) / x_new) * 100;
```

### **Stopping Criteria**
```matlab
% Stop when:
while (err > tol) && (i < maxIter)
    % - Error is less than tolerance, OR
    % - Maximum iterations reached
end
```

### **Convergence Checks**
```matlab
% Bracket methods (Bisection, Regula Falsi):
if f(a) * f(b) > 0
    % Same sign = no root in interval!
end

% Fixed Point:
if abs(dg_dx) >= 1
    % Won't converge!
end
```

---

## 🧠 COMMON MISTAKES TO AVOID

1. ❌ **Forget `clc; clear; close all;`** → Always clean workspace
2. ❌ **Wrong error formula** → Use relative error: `abs((new - old)/new) * 100`
3. ❌ **Check bracket validity** → f(a)·f(b) < 0 MUST be true
4. ❌ **Update variables wrongly** → Track which variable gets updated
5. ❌ **Print header before loop** → Makes table readable
6. ❌ **Forget to initialize err = inf** → So first iteration doesn't trigger early stop
7. ❌ **Wrong derivative** → Double-check your calculus!

---

## 📝 STEP-BY-STEP WRITING STRATEGY FOR QUIZ

**When writing by hand, follow this order:**

1. **Write header section** (clc, clear, close all)
2. **Define function** (f = @(x) ...)
3. **Get input** (x0 = input(...))
4. **Set parameters** (tol, maxIter, i=0, err=inf)
5. **Print header** (fprintf for table)
6. **Write while loop condition** (err > tol && i < maxIter)
7. **Update iteration counter** (i = i + 1)
8. **Write method formula** (THE KEY PART!)
9. **Calculate error** (err = abs((new-old)/new)*100)
10. **Print results** (fprintf)
11. **Update variables** (x0 = x_new, etc)
12. **Close loop and print final result**

---

## 🎯 YOUR ACTUAL CODE ANALYSIS

### Your codes follow this structure:
✅ Proper initialization  
✅ Good error calculations  
✅ Clear output formatting  
✅ Proper stopping criteria  

### Standardize by using the template above!

---

## 📚 PRACTICE PROBLEMS

### **Bisection Method**
Find root of $f(x) = x^3 - 2x - 5$ in interval [2, 3]

### **Newton-Raphson**
Find root of $f(x) = e^x - 5x$ with x₀ = 1

### **Secant Method**
Find root of $f(x) = \cos(x)$ with x₀ = 0, x₁ = 1

### **Regula Falsi**
Find root of $f(x) = x^3 - 2x - 5$ in interval [2, 3]

### **Fixed Point**
Rearrange $x^3 + x - 1 = 0$ to $x = 1/(x^2 + 1)$ and iterate from x₀ = 0.5

---

## ✨ FINAL TIPS FOR QUIZ

1. **Use the template!** Write it down first
2. **Identify the method clearly** (what's being asked)
3. **Choose correct formula** from the table
4. **Show error calculation** step-by-step
5. **Keep formatting consistent** for easy grading
6. **Test your code mentally** before submitting
7. **Label all outputs** clearly

---

**Good luck! 🍀 You've got this!**

