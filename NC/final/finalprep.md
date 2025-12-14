# NC Final Exam - Quick Preparation Guide (Simple Code - No Functions)

## ⏰ Study Plan: 2 Hours (MATLAB Code Focus)

---

## 📌 SECTION 1: ROOT FINDING METHODS (15 min)

### 1.1 Bisection Method
```matlab
% Bisection Method
f = @(x) x^2 - 4;  % Example function
a = 0; b = 3; tol = 0.001;

while (b - a) > tol
    c = (a + b) / 2;
    if f(c) == 0
        break;
    elseif f(a) * f(c) < 0
        b = c;
    else
        a = c;
    end
end
root = (a + b) / 2;
disp(['Root: ', num2str(root)]);
```
**Key**: Binary search, always converges, slow

---

### 1.2 Newton-Raphson Method
```matlab
% Newton-Raphson Method
f = @(x) x^2 - 4;
df = @(x) 2*x;
x0 = 2; tol = 0.001;

while abs(f(x0)) > tol
    x0 = x0 - f(x0) / df(x0);
end
disp(['Root: ', num2str(x0)]);
```
**Key**: Uses derivative, fast convergence, needs f'(x)

---

### 1.3 Secant Method
```matlab
% Secant Method
f = @(x) x^2 - 4;
x0 = 1; x1 = 3; tol = 0.001;

while abs(x1 - x0) > tol
    x2 = x1 - f(x1) * (x1 - x0) / (f(x1) - f(x0));
    x0 = x1;
    x1 = x2;
end
disp(['Root: ', num2str(x1)]);
```
**Key**: No derivative needed, uses two initial points

---

### 1.4 Fixed Point Iteration
```matlab
% Fixed Point Iteration
g = @(x) sqrt(4 - x^2);  % Rearrange f(x)=0 to x=g(x)
x0 = 1; tol = 0.001;

while abs(g(x0) - x0) > tol
    x0 = g(x0);
end
disp(['Root: ', num2str(x0)]);
```
**Key**: Rearrange f(x)=0 to x=g(x)

---

## 📌 SECTION 2: LINEAR SYSTEMS - DIRECT METHODS (20 min)

### 2.1 Gauss Elimination
```matlab
% Gauss Elimination
A = [2 1 -1; -3 -1 2; -2 1 2];
b = [8; -11; -3];
n = length(b);

% Forward elimination
for k = 1:n-1
    for i = k+1:n
        factor = A(i,k) / A(k,k);
        A(i,k:n) = A(i,k:n) - factor * A(k,k:n);
        b(i) = b(i) - factor * b(k);
    end
end

% Back substitution
x = zeros(n,1);
for i = n:-1:1
    x(i) = (b(i) - A(i,i+1:n) * x(i+1:n)) / A(i,i);
end
disp('Solution:'); disp(x);
```
**Key**: Convert to triangular form, then back substitute

---

### 2.2 Doolittle Method (LU Decomposition)
```matlab
% Doolittle Method (LU Decomposition)
A = [2 1 -1; -3 -1 2; -2 1 2];
n = size(A, 1);
L = eye(n);
U = zeros(n);

for i = 1:n
    for j = i:n
        U(i,j) = A(i,j) - L(i,1:i-1) * U(1:i-1,j);
    end
    for k = i+1:n
        L(k,i) = (A(k,i) - L(k,1:i-1) * U(1:i-1,i)) / U(i,i);
    end
end

disp('L:'); disp(L);
disp('U:'); disp(U);
```
**Key**: A = L*U, L is lower triangular, U is upper triangular

---

### 2.3 Cholesky Method
```matlab
% Cholesky Method (for symmetric positive definite)
A = [4 2 2; 2 4 2; 2 2 4];
n = size(A, 1);
L = zeros(n);

for i = 1:n
    for j = 1:i-1
        L(i,j) = (A(i,j) - L(i,1:j-1) * L(j,1:j-1)') / L(j,j);
    end
    L(i,i) = sqrt(A(i,i) - L(i,1:i-1) * L(i,1:i-1)');
end

disp('L:'); disp(L);
```
**Key**: For symmetric positive definite matrices only

---

## 📌 SECTION 3: LINEAR SYSTEMS - ITERATIVE METHODS (15 min)

### 3.1 Gauss-Jacobi Method
```matlab
% Gauss-Jacobi Method
A = [10 1 1; 1 10 1; 1 1 10];
b = [12; 12; 12];
x0 = [0; 0; 0];
tol = 0.001; max_iter = 100;
n = length(b);
x = x0;

for iter = 1:max_iter
    x_new = zeros(n,1);
    for i = 1:n
        x_new(i) = (b(i) - A(i,[1:i-1,i+1:n]) * x([1:i-1,i+1:n])) / A(i,i);
    end
    if norm(x_new - x) < tol
        break;
    end
    x = x_new;
end
disp('Solution:'); disp(x);
```
**Key**: Uses old values for all iterations, simpler

---

### 3.2 Gauss-Seidel Method
```matlab
% Gauss-Seidel Method
A = [10 1 1; 1 10 1; 1 1 10];
b = [12; 12; 12];
x0 = [0; 0; 0];
tol = 0.001; max_iter = 100;
n = length(b);
x = x0;

for iter = 1:max_iter
    x_old = x;
    for i = 1:n
        x(i) = (b(i) - A(i,1:i-1)*x(1:i-1) - A(i,i+1:n)*x(i+1:n)) / A(i,i);
    end
    if norm(x - x_old) < tol
        break;
    end
end
disp('Solution:'); disp(x);
```
**Key**: Uses updated values immediately, faster convergence

---

## 📌 SECTION 4: DIFFERENCE OPERATORS (10 min)

### Key Formulas & Simple Code

```matlab
% Forward Difference Operator
x = 0:0.1:1;
y = sin(x);
h = 0.1;
forward_diff = (y(2:end) - y(1:end-1)) / h;

% Backward Difference Operator
backward_diff = (y(2:end) - y(1:end-1)) / h;

% Central Difference Operator
central_diff = (y(3:end) - y(1:end-2)) / (2*h);

% Shift Operator (E)
E_y = y(2:end);  % f(x+h)

% Mean Value Operator
% μ·f(x) = [f(x+h/2) + f(x-h/2)] / 2
```

| Operator | Formula |
|----------|---------|
| **Forward Difference** | ∇f(x) = f(x+h) - f(x) |
| **Backward Difference** | ∇f(x) = f(x) - f(x-h) |
| **Central Difference** | ∇f(x) = [f(x+h) - f(x-h)] / 2h |
| **Shift Operator** | E·f(x) = f(x+h) |

---

## 📌 SECTION 5: INTERPOLATION (15 min)

### 5.1 Lagrange Interpolation
```matlab
% Lagrange Interpolation
x_data = [0 1 2 3];
y_data = [1 2 4 8];
x = 1.5;  % Interpolation point

n = length(x_data);
y = 0;

for i = 1:n
    L = 1;
    for j = 1:n
        if i ~= j
            L = L * (x - x_data(j)) / (x_data(i) - x_data(j));
        end
    end
    y = y + y_data(i) * L;
end
disp(['Interpolated value at ', num2str(x), ': ', num2str(y)]);
```
**Key**: Works for unequal intervals

---

### 5.2 Newton Forward Difference Interpolation
```matlab
% Newton Forward Difference Interpolation
x_data = [0 1 2 3 4];
y_data = [1 2 4 8 16];
x = 1.5;  % Equal intervals
h = x_data(2) - x_data(1);
p = (x - x_data(1)) / h;

% Build difference table
diff_table = y_data;
y = y_data(1);
term = 1;

for k = 1:length(y_data)-1
    diff_table = diff_table(1:end-1) - diff_table(2:end);
    term = term * (p - k + 1) / k;
    y = y + term * diff_table(1);
end
disp(['Interpolated value: ', num2str(y)]);
```
**Key**: For equal intervals, forward values

---

### 5.3 Newton Backward Difference Interpolation
```matlab
% Newton Backward Difference Interpolation
x_data = [0 1 2 3 4];
y_data = [1 2 4 8 16];
x = 3.5;  % Near end
h = x_data(2) - x_data(1);
p = (x - x_data(end)) / h;

% Build difference table (backward)
diff_table = y_data;
y = y_data(end);
term = 1;

for k = 1:length(y_data)-1
    diff_table = [diff(diff_table)];
    term = term * (p + k - 1) / k;
    y = y + term * diff_table(end);
end
disp(['Interpolated value: ', num2str(y)]);
```
**Key**: For equal intervals, backward values

---

### 5.4 Newton Divided Difference Interpolation
```matlab
% Newton Divided Difference Interpolation
x_data = [0 1 3 6];
y_data = [1 2 7 25];
x = 2;  % Works for unequal intervals

n = length(x_data);
dd = y_data;

% Build divided difference table
for k = 1:n-1
    for i = 1:n-k
        dd(i) = (dd(i+1) - dd(i)) / (x_data(i+k) - x_data(i));
    end
end

% Evaluate polynomial
y = dd(1);
prod = 1;
for k = 1:n-1
    prod = prod * (x - x_data(k));
    y = y + dd(k+1) * prod;
end
disp(['Interpolated value: ', num2str(y)]);
```
**Key**: Works for unequal intervals

---

## 📌 SECTION 6: NUMERICAL INTEGRATION (20 min)

### 6.1 Trapezoidal Rule
```matlab
% Trapezoidal Rule
f = @(x) x^2;
a = 0; b = 2; n = 4;
h = (b - a) / n;
x = a:h:b;
y = f(x);
I = (h/2) * (y(1) + 2*sum(y(2:end-1)) + y(end));
disp(['Integral: ', num2str(I)]);
```
**Formula**: I = (h/2)[f₀ + 2f₁ + 2f₂ + ... + fₙ]

---

### 6.2 Simpson's 1/3 Rule
```matlab
% Simpson's 1/3 Rule
f = @(x) x^2;
a = 0; b = 2; n = 4;  % Must be even
h = (b - a) / n;
x = a:h:b;
y = f(x);
I = (h/3) * (y(1) + 4*sum(y(2:2:end-1)) + 2*sum(y(3:2:end-2)) + y(end));
disp(['Integral: ', num2str(I)]);
```
**Formula**: I = (h/3)[f₀ + 4f₁ + 2f₂ + 4f₃ + ... + fₙ]

---

### 6.3 Simpson's 3/8 Rule
```matlab
% Simpson's 3/8 Rule
f = @(x) x^2;
a = 0; b = 3; n = 3;  % n must be divisible by 3
h = (b - a) / n;
x = a:h:b;
y = f(x);
I = (3*h/8) * (y(1) + 3*sum(y(2:3:end-1)) + 3*sum(y(3:3:end-2)) + y(end));
disp(['Integral: ', num2str(I)]);
```
**Formula**: I = (3h/8)[f₀ + 3f₁ + 3f₂ + 2f₃ + ...]

---

### 6.4 Boole's Rule (5-point)
```matlab
% Boole's Rule (5-point)
f = @(x) x^2;
a = 0; b = 4;
h = (b - a) / 4;
x = a:h:b;
y = f(x);
I = (2*h/45) * (7*y(1) + 32*y(2) + 12*y(3) + 32*y(4) + 7*y(5));
disp(['Integral: ', num2str(I)]);
```
**Formula**: I = (2h/45)[7f₀ + 32f₁ + 12f₂ + 32f₃ + 7f₄]

---

### 6.5 Newton-Cotes 6-point
```matlab
% Newton-Cotes 6-point
f = @(x) x^2;
a = 0; b = 5;
h = (b - a) / 5;
x = a:h:b;
y = f(x);
I = (5*h/288) * (19*y(1) + 75*y(2) + 50*y(3) + 50*y(4) + 75*y(5) + 19*y(6));
disp(['Integral: ', num2str(I)]);
```

---

### 6.6 Weddle's Rule (7-point)
```matlab
% Weddle's Rule (7-point)
f = @(x) x^2;
a = 0; b = 6;
h = (b - a) / 6;
x = a:h:b;
y = f(x);
I = (3*h/10) * (y(1) + 5*y(2) + y(3) + 6*y(4) + y(5) + 5*y(6) + y(7));
disp(['Integral: ', num2str(I)]);
```
**Formula**: I = (3h/10)[f₀ + 5f₁ + f₂ + 6f₃ + f₄ + 5f₅ + f₆]

---

## 📌 SECTION 7: DIFFERENTIAL EQUATIONS (15 min)

### 7.1 Euler Method
```matlab
% Euler Method
% dy/dt = f(t, y)
f = @(t, y) t + y;  % Example: dy/dt = t + y
t0 = 0; y0 = 1; h = 0.1; t_end = 1;
t = t0:h:t_end;
n = length(t);
y = zeros(1, n);
y(1) = y0;

for i = 2:n
    y(i) = y(i-1) + h * f(t(i-1), y(i-1));
end
plot(t, y, 'b-o'); xlabel('t'); ylabel('y');
```
**Formula**: yᵢ₊₁ = yᵢ + h·f(tᵢ, yᵢ)

---

### 7.2 Modified Euler (Heun's Method)
```matlab
% Modified Euler (Heun's Method)
f = @(t, y) t + y;
t0 = 0; y0 = 1; h = 0.1; t_end = 1;
t = t0:h:t_end;
n = length(t);
y = zeros(1, n);
y(1) = y0;

for i = 2:n
    k1 = f(t(i-1), y(i-1));
    k2 = f(t(i), y(i-1) + h*k1);
    y(i) = y(i-1) + (h/2) * (k1 + k2);
end
plot(t, y, 'b-o'); xlabel('t'); ylabel('y');
```
**Formula**: yᵢ₊₁ = yᵢ + (h/2)(k₁ + k₂)

---

### 7.3 Runge-Kutta 4th Order (RK4)
```matlab
% Runge-Kutta 4th Order
f = @(t, y) t + y;
t0 = 0; y0 = 1; h = 0.1; t_end = 1;
t = t0:h:t_end;
n = length(t);
y = zeros(1, n);
y(1) = y0;

for i = 2:n
    k1 = f(t(i-1), y(i-1));
    k2 = f(t(i-1) + h/2, y(i-1) + h*k1/2);
    k3 = f(t(i-1) + h/2, y(i-1) + h*k2/2);
    k4 = f(t(i), y(i-1) + h*k3);
    y(i) = y(i-1) + (h/6) * (k1 + 2*k2 + 2*k3 + k4);
end
plot(t, y, 'b-o'); xlabel('t'); ylabel('y');
```
**Formula**: yᵢ₊₁ = yᵢ + (h/6)(k₁ + 2k₂ + 2k₃ + k₄)

---

## 🎯 Quick Memory Checklist

### Root Finding
- [ ] Bisection: binary search, always converges
- [ ] Newton-Raphson: uses derivative, fast
- [ ] Secant: no derivative, two initial points
- [ ] Fixed Point: rearrange to x = g(x)

### Linear Systems
- [ ] Gauss: forward elimination + back substitution
- [ ] Doolittle: A = LU
- [ ] Cholesky: for symmetric positive definite
- [ ] Jacobi: uses old values
- [ ] Seidel: uses new values immediately

### Integration
- [ ] Trapezoidal: 2-point rule
- [ ] Simpson 1/3: 3-point rule, most common
- [ ] Simpson 3/8: 3-point formula variation
- [ ] Boole's: 5-point rule
- [ ] Weddle's: 7-point rule

### ODE Solvers
- [ ] Euler: simplest, least accurate
- [ ] Modified Euler: 2-stage RK
- [ ] RK4: 4-stage, most accurate

---

## ⏰ Last-Minute Tips (During Exam)

1. **Copy & Paste Ready**: All codes are ready to copy directly
2. **Modify as needed**: Change f, a, b, n values based on problem
3. **Common variables**:
   - `f = @(x) ...` for function definition
   - `h = (b-a)/n` for step size
   - `tol = 0.001` for tolerance
4. **Output**: Use `disp()` for results
5. **Plotting**: Use `plot()` for visualization

---

## 💪 Good Luck! You've Got This!
