Perfect, Muhammad ✅ — let’s go step-by-step the right way:
First, I’ll **explain each numerical method** clearly (how it works, where it’s used, and its concept),
then I’ll show you the **MATLAB code** for it in a simple, memorable form.

We’ll go in the most logical order:
**(1) Bisection → (2) Regula Falsi → (3) Newton-Raphson → (4) Secant → (5) Fixed Point Iteration**

---

## 🧮 **1️⃣ Bisection Method**

### 🔍 **Concept**

The **Bisection method** is one of the simplest root-finding algorithms.
It’s based on the **Intermediate Value Theorem**:

> If `f(a)` and `f(b)` have opposite signs (i.e., one positive and one negative),
> then there exists a root between `a` and `b`.

We repeatedly:

1. Compute midpoint → `Xm = (a + b)/2`
2. Check sign of `f(Xm)`
3. Replace either `a` or `b` with `Xm`
4. Repeat until the difference (error) is very small.

✅ **Conditions:**

* Must have `f(a)*f(b) < 0`
* Always converges (guaranteed, but slow)

---

### 💻 **MATLAB Code: Bisection Method**

```matlab
clc; clear; close all;

% Define the function
f = @(x) x.^2 - 12;  % Example: root at sqrt(12)

% Input the interval
a = input('Enter value of a: ');
b = input('Enter value of b: ');

% Ensure valid interval
while f(a)*f(b) > 0
    disp('f(a) and f(b) have same sign. Enter again.');
    a = input('Enter value of a: ');
    b = input('Enter value of b: ');
end

tol = 1e-6; maxIter = 25; i = 0; err = inf; Xm_old = a;

fprintf('\nIter    a          b          Xm         Error(%%)\n');
fprintf('-----------------------------------------------\n');

while err > tol && i < maxIter
    i = i + 1;
    Xm = (a + b)/2;      % Midpoint

    if f(a)*f(Xm) < 0
        b = Xm;
    else
        a = Xm;
    end

    if i > 1
        err = abs((Xm - Xm_old)/Xm)*100;
    else
        err = inf;
    end

    fprintf('%3d   %9.6f   %9.6f   %9.6f   %9.6f\n', i, a, b, Xm, err);
    Xm_old = Xm;
end

fprintf('\nApproximate root = %.6f found in %d iterations\n', Xm, i);
```

---

## ⚡ **2️⃣ Regula Falsi (False Position) Method**

### 🔍 **Concept**

Regula Falsi is **similar to Bisection**, but instead of the midpoint,
it uses a **straight-line interpolation** between `(a, f(a))` and `(b, f(b))`.

The root approximation formula is:
[
x_{new} = \frac{a f(b) - b f(a)}{f(b) - f(a)}
]

✅ **Conditions:**

* Same as Bisection → `f(a)*f(b) < 0`
* Converges faster than Bisection (in most cases)

---

### 💻 **MATLAB Code: Regula Falsi**

```matlab
clc; clear; close all;

f = @(x) x.^3 - 2*x - 5;  % Example function

a = input('Enter value of a: ');
b = input('Enter value of b: ');

while f(a)*f(b) > 0
    disp('f(a) and f(b) have same sign. Enter again.');
    a = input('Enter value of a: ');
    b = input('Enter value of b: ');
end

tol = 1e-6; maxIter = 50; i = 0; err = inf;

fprintf('\nIter    a           b           x_new         f(x_new)        Error(%%)\n');
fprintf('--------------------------------------------------------------------------\n');

while err > tol && i < maxIter
    i = i + 1;
    x_new = (a*f(b) - b*f(a)) / (f(b) - f(a));  % Regula Falsi formula

    if i > 1
        err = abs((x_new - x_old)/x_new)*100;
    else
        err = inf;
    end

    fprintf('%3d   %10.6f   %10.6f   %10.6f   %12.6f   %10.6f\n', ...
        i, a, b, x_new, f(x_new), err);

    if f(a)*f(x_new) < 0
        b = x_new;
    else
        a = x_new;
    end

    x_old = x_new;
end

fprintf('\nApproximate root = %.6f found in %d iterations\n', x_new, i);
```

---

## ⚙️ **3️⃣ Newton–Raphson Method**

### 🔍 **Concept**

The Newton-Raphson method uses the **tangent line** at a point to find the next approximation.

Formula:
[
x_{n+1} = x_n - \frac{f(x_n)}{f'(x_n)}
]

✅ **Conditions:**

* Needs derivative ( f'(x) )
* Converges very fast **if** starting guess is near the root

⚠️ **Can diverge** if slope is too flat or initial guess is bad.

---

### 💻 **MATLAB Code: Newton-Raphson**

```matlab
clc; clear; close all;

f  = @(x) -x.^2 + x - 3;
df = @(x) -2*x + 1;

x0 = input('Enter initial guess: ');
tol = 1e-6; maxIter = 30; err = inf; i = 0;

fprintf('\nIter    x_old        x_new        Error(%%)\n');
fprintf('-------------------------------------------\n');

while err > tol && i < maxIter
    i = i + 1;
    x1 = x0 - f(x0)/df(x0);

    if i > 1
        err = abs((x1 - x0)/x1)*100;
    else
        err = inf;
    end

    fprintf('%3d   %10.6f   %10.6f   %10.6f\n', i, x0, x1, err);
    x0 = x1;
end

fprintf('\nApproximate root = %.6f found in %d iterations\n', x1, i);
```

---

## 🔢 **4️⃣ Secant Method**

### 🔍 **Concept**

The **Secant method** is like Newton-Raphson,
but it **doesn’t require derivative** — instead it approximates it using two previous points.

Formula:
[
x_{n+1} = x_n - f(x_n)\frac{(x_n - x_{n-1})}{f(x_n) - f(x_{n-1})}
]

✅ **Conditions:**

* No derivative needed
* Faster than Bisection, slower than Newton-Raphson

---

### 💻 **MATLAB Code: Secant Method**

```matlab
clc; clear; close all;

f = @(x) cos(x);  % Example

x0 = input('Enter first guess x0: ');
x1 = input('Enter second guess x1: ');

tol = 1e-6; maxIter = 30; err = inf; i = 0;

fprintf('\nIter    x0          x1          x2          Error(%%)\n');
fprintf('------------------------------------------------------\n');

while err > tol && i < maxIter
    i = i + 1;
    x2 = x1 - f(x1)*(x1 - x0)/(f(x1) - f(x0));

    if i > 1
        err = abs((x2 - x1)/x2)*100;
    else
        err = inf;
    end

    fprintf('%3d   %10.6f   %10.6f   %10.6f   %10.6f\n', i, x0, x1, x2, err);
    x0 = x1;
    x1 = x2;
end

fprintf('\nApproximate root = %.6f found in %d iterations\n', x2, i);
```

---

## 🔁 **5️⃣ Fixed-Point Iteration Method**

### 🔍 **Concept**

We rewrite ( f(x) = 0 ) in the form:
[
x = g(x)
]
Then iterate:
[
x_{n+1} = g(x_n)
]

✅ **Convergence condition:**
[
|g'(x)| < 1
]
If not, the method diverges.

---

### 💻 **MATLAB Code: Fixed-Point Iteration**

```matlab
clc; clear; close all; syms x;

% Input function
f_str = input('Enter f(x): ', 's');
f_sym = str2sym(f_str);
f = matlabFunction(f_sym, 'Vars', x);

% Define g(x)
g_sym = x - f_sym;     % Example transformation
g = matlabFunction(g_sym, 'Vars', x);

x0 = input('Enter initial guess x0: ');
tol = 1e-6; maxIter = 50; i = 0; err = inf;

fprintf('\nIter    x_old        x_new        Error(%%)\n');
fprintf('-------------------------------------------\n');

while err > tol && i < maxIter
    i = i + 1;
    x_new = g(x0);
    err = abs((x_new - x0)/x_new)*100;
    fprintf('%3d   %10.6f   %10.6f   %10.6f\n', i, x0, x_new, err);
    x0 = x_new;
end

fprintf('\nApproximate root = %.6f found in %d iterations\n', x_new, i);
```

---

## 🧾 **Summary for Memorization**

| #   | Method             | Formula / Idea                | Derivative Needed? | Bracketing? | Speed     | Key Idea             |
| --- | ------------------ | ----------------------------- | ------------------ | ----------- | --------- | -------------------- |
| 1️⃣ | **Bisection**      | `(a+b)/2`                     | ❌                  | ✅           | Slow      | Divide interval      |
| 2️⃣ | **Regula Falsi**   | `(a*f(b)-b*f(a))/(f(b)-f(a))` | ❌                  | ✅           | Medium    | Linear interpolation |
| 3️⃣ | **Newton-Raphson** | `x - f/df`                    | ✅                  | ❌           | Very Fast | Tangent line         |
| 4️⃣ | **Secant**         | `x - f*(x-x0)/(f-f0)`         | ❌                  | ❌           | Fast      | Two-point slope      |
| 5️⃣ | **Fixed Point**    | `x = g(x)`                    | ❌                  | ❌           | Medium    | Self substitution    |

---

Would you like me to generate a **single `.md` file** that contains:

* explanations
* formulas
* and MATLAB codes
  for all 5 methods (like a quick revision handbook)?
