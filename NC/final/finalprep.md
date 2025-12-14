# NC Final Exam - Quick Preparation Guide (Simple Code - No Functions)

## ⏰ Study Plan: 2 Hours (MATLAB Code Focus)

---

## 📌 SECTION 1: ROOT FINDING METHODS (15 min)

### 1.1 Bisection Method
**Mathematical Formula:**
$$c = \frac{a + b}{2}$$

**Algorithm Steps:**
1. Start with interval [a, b] where f(a)·f(b) < 0 (different signs)
2. Calculate midpoint: $c = \frac{a+b}{2}$
3. Check f(c):
   - If f(c) = 0 → c is the root
   - If f(a)·f(c) < 0 → root in [a, c], set b = c
   - Else → root in [c, b], set a = c
4. Repeat until |b - a| < tolerance

```matlab
% Bisection Method
f = @(x) x^2 - 4;  % Example function
a = 0; b = 3; tol = 0.001;

while (b - a) > tol
    c = (a + b) / 2;  % Midpoint
    if f(c) == 0
        break;
    elseif f(a) * f(c) < 0  % Root in [a, c]
        b = c;
    else                     % Root in [c, b]
        a = c;
    end
end
root = (a + b) / 2;
disp(['Root: ', num2str(root)]);
```
**Key**: Binary search, always converges, log₂(n) iterations

---

### 1.2 Newton-Raphson Method
**Mathematical Formula:**
$$x_{n+1} = x_n - \frac{f(x_n)}{f'(x_n)}$$

**Derivation:** Using Taylor series:
$$f(x) \approx f(x_n) + f'(x_n)(x - x_n) = 0$$
$$x = x_n - \frac{f(x_n)}{f'(x_n)}$$

**Algorithm:**
1. Choose initial guess x₀
2. Calculate: $x_{n+1} = x_n - \frac{f(x_n)}{f'(x_n)}$
3. Repeat until convergence

```matlab
% Newton-Raphson Method
f = @(x) x^2 - 4;      % f(x)
df = @(x) 2*x;         % f'(x) derivative
x0 = 2; tol = 0.001;   % Initial guess

while abs(f(x0)) > tol
    x0 = x0 - f(x0) / df(x0);  % Newton's formula
end
disp(['Root: ', num2str(x0)]);
```
**Key**: Uses derivative, quadratic convergence, needs f'(x)

---

### 1.3 Secant Method
**Mathematical Formula:**
$$x_{n+1} = x_n - f(x_n) \cdot \frac{x_n - x_{n-1}}{f(x_n) - f(x_{n-1})}$$

**Key Idea:** Approximates derivative using finite difference:
$$f'(x_n) \approx \frac{f(x_n) - f(x_{n-1})}{x_n - x_{n-1}}$$

**Algorithm:**
1. Choose two initial points x₀ and x₁
2. Calculate: $x_{n+1} = x_n - f(x_n) \cdot \frac{x_n - x_{n-1}}{f(x_n) - f(x_{n-1})}$
3. Shift: x₀ = x₁, x₁ = x₂, repeat

```matlab
% Secant Method
f = @(x) x^2 - 4;
x0 = 1; x1 = 3; tol = 0.001;  % Two initial points

while abs(x1 - x0) > tol
    x2 = x1 - f(x1) * (x1 - x0) / (f(x1) - f(x0));
    x0 = x1;  % Shift values
    x1 = x2;
end
disp(['Root: ', num2str(x1)]);
```
**Key**: No derivative needed, faster than bisection

---

### 1.4 Fixed Point Iteration
**Mathematical Formula:**
$$x_{n+1} = g(x_n)$$

**Convergence Condition:** Method converges if $|g'(x)| < 1$ near root

**Algorithm Steps:**
1. Rearrange f(x) = 0 to form x = g(x)
2. Choose initial guess x₀
3. Calculate: $x_{n+1} = g(x_n)$
4. Repeat until $|x_{n+1} - x_n| < \text{tolerance}$

```matlab
% Fixed Point Iteration
g = @(x) sqrt(4 - x^2);  % Rearrange: f(x)=0 becomes x=g(x)
x0 = 1; tol = 0.001;     % Initial guess

while abs(g(x0) - x0) > tol
    x0 = g(x0);  % Apply iteration formula
end
disp(['Root: ', num2str(x0)]);
```
**Key**: Check |g'(x)| < 1 for convergence; rearrangement is crucial

---

## 📌 SECTION 2: LINEAR SYSTEMS - DIRECT METHODS (20 min)

### 2.1 Gauss Elimination
**Mathematical Steps:**

**Step 1: Forward Elimination** - Convert to upper triangular form
$$m_{ij} = \frac{a_{ij}}{a_{jj}}, \quad a'_{ij} = a_{ij} - m_{ij} \cdot a_{jj}$$

**Step 2: Back Substitution** - Solve from bottom to top
$$x_i = \frac{b_i - \sum_{j=i+1}^{n} a_{ij} x_j}{a_{ii}}$$

**Example System:**
$$\begin{bmatrix} 2 & 1 & -1 \\ -3 & -1 & 2 \\ -2 & 1 & 2 \end{bmatrix} \begin{bmatrix} x_1 \\ x_2 \\ x_3 \end{bmatrix} = \begin{bmatrix} 8 \\ -11 \\ -3 \end{bmatrix}$$

```matlab
% Gauss Elimination
A = [2 1 -1; -3 -1 2; -2 1 2];
b = [8; -11; -3];
n = length(b);

% STEP 1: Forward Elimination (to upper triangular)
for k = 1:n-1
    for i = k+1:n
        factor = A(i,k) / A(k,k);  % Multiplier
        A(i,k:n) = A(i,k:n) - factor * A(k,k:n);  % Row operation
        b(i) = b(i) - factor * b(k);
    end
end

% STEP 2: Back Substitution (solve from xₙ to x₁)
x = zeros(n,1);
for i = n:-1:1
    x(i) = (b(i) - A(i,i+1:n) * x(i+1:n)) / A(i,i);
end
disp('Solution:'); disp(x);
```
**Key**: Two-phase method - eliminate to triangle, then substitute

---

### 2.2 Doolittle Method (LU Decomposition)
**Mathematical Formula:**

Decompose A = LU where L has 1's on diagonal and U is upper triangular:

$$u_{ij} = a_{ij} - \sum_{k=1}^{i-1} l_{ik} \cdot u_{kj}, \quad l_{ij} = \frac{a_{ij} - \sum_{k=1}^{j-1} l_{ik} \cdot u_{kj}}{u_{jj}}$$

**Algorithm Steps:**
1. For each column i (from 1 to n):
   - Calculate U row: $u_{ij} = a_{ij} - \sum_{k=1}^{i-1} l_{ik} \cdot u_{kj}$ for j ≥ i
   - Calculate L column: $l_{ji} = \frac{a_{ji} - \sum_{k=1}^{i-1} l_{jk} \cdot u_{ki}}{u_{ii}}$ for j > i
2. Solve Ly = b, then Ux = y

```matlab
% Doolittle Method (LU Decomposition)
A = [2 1 -1; -3 -1 2; -2 1 2];
n = size(A, 1);
L = eye(n);    % L has 1's on diagonal
U = zeros(n);  % U is upper triangular

for i = 1:n
    % Calculate U row i
    for j = i:n
        U(i,j) = A(i,j) - L(i,1:i-1) * U(1:i-1,j);  % Sum of products
    end
    % Calculate L column i
    for k = i+1:n
        L(k,i) = (A(k,i) - L(k,1:i-1) * U(1:i-1,i)) / U(i,i);
    end
end

disp('L:'); disp(L);
disp('U:'); disp(U);
```
**Key**: A = L·U where L has unit diagonal, faster for multiple RHS

---

### 2.3 Cholesky Method
**Mathematical Formula:**

For symmetric positive definite matrix A = L·L^T where L is lower triangular:

$$l_{ii} = \sqrt{a_{ii} - \sum_{k=1}^{i-1} l_{ik}^2}, \quad l_{ji} = \frac{a_{ji} - \sum_{k=1}^{i-1} l_{jk} \cdot l_{ik}}{l_{ii}}$$

**Requirements:** A must be symmetric AND positive definite

**Algorithm Steps:**
1. For each row i (from 1 to n):
   - Calculate diagonal: $l_{ii} = \sqrt{a_{ii} - \sum_{k=1}^{i-1} l_{ik}^2}$
   - Calculate below diagonal: $l_{ji} = \frac{a_{ji} - \sum_{k=1}^{i-1} l_{jk} \cdot l_{ik}}{l_{ii}}$ for j > i
2. Solve Ly = b, then L^T x = y

```matlab
% Cholesky Method (for symmetric positive definite matrices)
A = [4 2 2; 2 4 2; 2 2 4];  % Must be symmetric & pos. definite
n = size(A, 1);
L = zeros(n);

for i = 1:n
    % Diagonal element
    for j = 1:i-1
        L(i,j) = (A(i,j) - L(i,1:j-1) * L(j,1:j-1)') / L(j,j);
    end
    L(i,i) = sqrt(A(i,i) - L(i,1:i-1) * L(i,1:i-1)');  % Key formula
end

disp('L:'); disp(L);
```
**Key**: Most efficient for symmetric systems, check A is positive definite first

---

## 📌 SECTION 3: LINEAR SYSTEMS - ITERATIVE METHODS (15 min)

### 3.1 Gauss-Jacobi Method
**Mathematical Formula:**

$$x_i^{(k+1)} = \frac{1}{a_{ii}} \left( b_i - \sum_{j \neq i} a_{ij} x_j^{(k)} \right)$$

**Key Feature:** Uses **OLD values** $x_j^{(k)}$ for all i (simultaneous update)

**Algorithm Steps:**
1. Start with initial guess $x^{(0)}$
2. For iteration k (until convergence):
   - For each equation i: calculate $x_i^{(k+1)}$ using all OLD values from iteration k
   - Check convergence: $\|x^{(k+1)} - x^{(k)}\| < \text{tolerance}$
3. All equations update simultaneously after collecting new values

```matlab
% Gauss-Jacobi Method (all equations update simultaneously)
A = [10 1 1; 1 10 1; 1 1 10];
b = [12; 12; 12];
x0 = [0; 0; 0];
tol = 0.001; max_iter = 100;
n = length(b);
x = x0;

for iter = 1:max_iter
    x_new = zeros(n,1);  % Calculate all NEW values first
    for i = 1:n
        % Use x (old iteration) for all j
        x_new(i) = (b(i) - A(i,[1:i-1,i+1:n]) * x([1:i-1,i+1:n])) / A(i,i);
    end
    if norm(x_new - x) < tol
        break;
    end
    x = x_new;  % Update all at once after complete iteration
end
disp('Solution:'); disp(x);
```
**Key**: Parallel computation possible; often slower than Gauss-Seidel but simpler logic

---

### 3.2 Gauss-Seidel Method
**Mathematical Formula:**

$$x_i^{(k+1)} = \frac{1}{a_{ii}} \left( b_i - \sum_{j=1}^{i-1} a_{ij} x_j^{(k+1)} - \sum_{j=i+1}^{n} a_{ij} x_j^{(k)} \right)$$

**Key Feature:** Uses **NEW values** for j < i and **OLD values** for j > i (sequential update)

**Algorithm Steps:**
1. Start with initial guess $x^{(0)}$
2. For iteration k (until convergence):
   - For each equation i (in order 1 to n):
     - Use NEW values $x_1^{(k+1)}, \ldots, x_{i-1}^{(k+1)}$ (already computed)
     - Use OLD values $x_{i+1}^{(k)}, \ldots, x_n^{(k)}$ (from previous iteration)
     - Calculate $x_i^{(k+1)}$ immediately
   - Check convergence: $\|x^{(k+1)} - x^{(k)}\| < \text{tolerance}$

```matlab
% Gauss-Seidel Method (sequential update - mixed NEW and OLD values)
A = [10 1 1; 1 10 1; 1 1 10];
b = [12; 12; 12];
x0 = [0; 0; 0];
tol = 0.001; max_iter = 100;
n = length(b);
x = x0;

for iter = 1:max_iter
    x_old = x;
    for i = 1:n
        % Use x(1:i-1) = NEW values from current iteration
        % Use x(i+1:n) = OLD values from previous iteration
        x(i) = (b(i) - A(i,1:i-1)*x(1:i-1) - A(i,i+1:n)*x(i+1:n)) / A(i,i);
    end
    if norm(x - x_old) < tol
        break;
    end
end
disp('Solution:'); disp(x);
```
**Key**: Faster convergence than Gauss-Jacobi; sequential nature prevents parallelization

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
**Mathematical Formula:**

$$f(x) = \sum_{i=0}^{n} y_i \cdot L_i(x), \quad L_i(x) = \prod_{j=0, j \neq i}^{n} \frac{x - x_j}{x_i - x_j}$$

**Key Idea:** Each $L_i(x)$ is 1 at $x_i$ and 0 at all other data points

**Algorithm Steps:**
1. For each basis function L_i(x):
   - Multiply terms: $(x-x_0)(x-x_1)\cdots(x-x_{i-1})(x-x_{i+1})\cdots(x-x_n)$ in numerator
   - Divide by: $(x_i-x_0)(x_i-x_1)\cdots(x_i-x_{i-1})(x_i-x_{i+1})\cdots(x_i-x_n)$
2. Multiply each L_i by corresponding y_i and sum all terms

```matlab
% Lagrange Interpolation (works for any interval spacing)
x_data = [0 1 2 3];
y_data = [1 2 4 8];
x = 1.5;  % Interpolation point

n = length(x_data);
y = 0;  % Accumulator for sum

for i = 1:n
    L = 1;  % Basis function L_i
    for j = 1:n
        if i ~= j
            % Multiply: (x - x_j) / (x_i - x_j)
            L = L * (x - x_data(j)) / (x_data(i) - x_data(j));
        end
    end
    y = y + y_data(i) * L;  % Add y_i * L_i(x)
end
disp(['Interpolated value at ', num2str(x), ': ', num2str(y)]);
```
**Key**: Works for **unequal intervals**, no special data arrangement needed

---

### 5.2 Newton Forward Difference Interpolation
**Mathematical Formula:**

$$f(x) = f(x_0) + p \cdot \Delta f(x_0) + \frac{p(p-1)}{2!} \Delta^2 f(x_0) + \frac{p(p-1)(p-2)}{3!} \Delta^3 f(x_0) + \cdots$$

where $p = \frac{x - x_0}{h}$ and h = interval width

**Key Idea:** Uses forward differences from first data point; good when x is near START

**Algorithm Steps:**
1. Build forward difference table: $\Delta f_i = f_{i+1} - f_i$, $\Delta^2 f_i = \Delta f_{i+1} - \Delta f_i$, etc.
2. Calculate parameter: $p = \frac{x - x_0}{h}$
3. Apply formula: f(x) = f(x₀) + p·Δf(x₀) + p(p-1)/2!·Δ²f(x₀) + ...

```matlab
% Newton Forward Difference Interpolation (Equal intervals, x near START)
x_data = [0 1 2 3 4];
y_data = [1 2 4 8 16];
x = 1.5;  % Between x_0 and x_4, closer to start
h = x_data(2) - x_data(1);
p = (x - x_data(1)) / h;  % Parameter p = (x - x0)/h

% Build difference table
diff_table = y_data;
y = y_data(1);  % Start with f(x0)
term = 1;

for k = 1:length(y_data)-1
    diff_table = diff_table(1:end-1) - diff_table(2:end);  % Δᵏ
    term = term * (p - k + 1) / k;  % Coefficient: p(p-1)(p-2).../(k!)
    y = y + term * diff_table(1);  % Add term to interpolation
end
disp(['Interpolated value: ', num2str(y)]);
```
**Key**: For **equal intervals** near **START of data**, uses forward differences from x₀

---

### 5.3 Newton Backward Difference Interpolation
### 5.3 Newton Backward Difference Interpolation
**Mathematical Formula:**

$$f(x) = f(x_n) + p \cdot \nabla f(x_n) + \frac{p(p+1)}{2!} \nabla^2 f(x_n) + \frac{p(p+1)(p+2)}{3!} \nabla^3 f(x_n) + \cdots$$

where $p = \frac{x - x_n}{h}$ (note: p negative when x < xₙ) and h = interval width

**Key Idea:** Uses backward differences from LAST data point; good when x is near END

**Algorithm Steps:**
1. Build backward difference table using LAST row: $\nabla f_i = f_i - f_{i-1}$
2. Calculate parameter: $p = \frac{x - x_n}{h}$ (from END point)
3. Apply formula using backward differences, coefficient has (p+1), (p+2), etc.

```matlab
% Newton Backward Difference Interpolation (Equal intervals, x near END)
x_data = [0 1 2 3 4];
y_data = [1 2 4 8 16];
x = 3.5;  % Near END of data
h = x_data(2) - x_data(1);
p = (x - x_data(end)) / h;  % Parameter p = (x - xn)/h

% Build difference table (backward differences from end)
diff_table = y_data;
y = y_data(end);  % Start with f(xn)
term = 1;

for k = 1:length(y_data)-1
    diff_table = diff(diff_table);  % Calculate next difference
    term = term * (p + k - 1) / k;  % Coefficient: p(p+1)(p+2).../(k!)
    y = y + term * diff_table(end);  % Use last element (backward)
end
disp(['Interpolated value: ', num2str(y)]);
```
**Key**: For **equal intervals** near **END of data**, uses backward differences from xₙ

---

### 5.4 Newton Divided Difference Interpolation
**Mathematical Formula:**

Divided differences: $[x_i, x_j] = \frac{f(x_j) - f(x_i)}{x_j - x_i}$

$$f(x) = f[x_0] + f[x_0,x_1](x-x_0) + f[x_0,x_1,x_2](x-x_0)(x-x_1) + \cdots$$

**Key Feature:** Works for **unequal intervals** (no restriction on spacing)

**Algorithm Steps:**
1. Build divided difference table: 
   - First column: y values
   - Each next column: $\frac{\text{diff of prev col}}{\text{x spacing}}$
2. Use first element of each diagonal as coefficient
3. Evaluate: f(x) = f[x₀] + f[x₀,x₁]·(x-x₀) + f[x₀,x₁,x₂]·(x-x₀)(x-x₁) + ...

```matlab
% Newton Divided Difference Interpolation (works for UNEQUAL intervals)
x_data = [0 1 3 6];
y_data = [1 2 7 25];
x = 2;

n = length(x_data);
dd = y_data;  % First column of divided difference table

% Build divided difference table
for k = 1:n-1
    for i = 1:n-k
        % Divided difference: [x_i ... x_{i+k}]
        dd(i) = (dd(i+1) - dd(i)) / (x_data(i+k) - x_data(i));
    end
end

% Evaluate polynomial using divided differences
y = dd(1);
prod = 1;
for k = 1:n-1
    prod = prod * (x - x_data(k));  % (x-x0)(x-x1)...
    y = y + dd(k+1) * prod;  % Add coefficient * product term
end
disp(['Interpolated value: ', num2str(y)]);
```
**Key**: **Only method for unequal intervals**, most flexible approach

---

## 📌 SECTION 6: NUMERICAL INTEGRATION (20 min)

### 6.1 Trapezoidal Rule
**Mathematical Formula:**
$$I \approx \frac{h}{2}(f_0 + 2f_1 + 2f_2 + ... + 2f_{n-1} + f_n)$$
where $h = \frac{b-a}{n}$

**Geometric Idea:** Sum of n trapezoids under the curve

**Step-by-Step:**
1. Divide [a,b] into n equal intervals: $h = \frac{b-a}{n}$
2. Calculate function values: $f_i = f(x_i)$ at $x_i = a + ih$
3. Apply formula: $I = \frac{h}{2}(f_0 + 2f_1 + ... + 2f_{n-1} + f_n)$

**Example:** Integrate f(x) = x² from 0 to 2, n=4
```
h = 0.5
x: [0    0.5   1.0   1.5   2.0]
f: [0  0.25   1.0  2.25   4.0]
I = 0.5/2 * (0 + 2(0.25) + 2(1) + 2(2.25) + 4)
  = 0.25 * 11 = 2.75
```

```matlab
% Trapezoidal Rule
f = @(x) x^2;      % Function to integrate
a = 0; b = 2;      % Limits
n = 4;             % Number of intervals
h = (b - a) / n;   % Step size

x = a:h:b;         % Grid points
y = f(x);          % Function values

I = (h/2) * (y(1) + 2*sum(y(2:end-1)) + y(end));
disp(['Integral: ', num2str(I)]);
```
**Key**: 2-point rule, O(h²) error, simple and fast

---

### 6.2 Simpson's 1/3 Rule
**Mathematical Formula:**
$$I \approx \frac{h}{3}(f_0 + 4f_1 + 2f_2 + 4f_3 + ... + 4f_{n-1} + f_n)$$
where $h = \frac{b-a}{n}$ and **n must be even**

**Coefficient Pattern:**
- First & last: coefficient 1
- Odd indices (1,3,5,...): coefficient 4
- Even indices (2,4,6,...): coefficient 2

**Algorithm:**
1. Divide [a,b] into **even** n intervals: $h = \frac{b-a}{n}$
2. Calculate function values at all points
3. Apply: $I = \frac{h}{3}(f_0 + 4f_1 + 2f_2 + 4f_3 + ...)$

**Example:** f(x) = x² from 0 to 2, n=4
```
h = 0.5
x: [0    0.5   1.0   1.5   2.0]
f: [0  0.25   1.0  2.25   4.0]
I = 0.5/3 * (0 + 4(0.25) + 2(1) + 4(2.25) + 4)
  = (0.5/3) * 16 ≈ 2.667
```

```matlab
% Simpson's 1/3 Rule
f = @(x) x^2;
a = 0; b = 2; n = 4;  % Must be even
h = (b - a) / n;

x = a:h:b;
y = f(x);

% Odd indices: 2:2:end-1, Even indices: 3:2:end-2
I = (h/3) * (y(1) + 4*sum(y(2:2:end-1)) + 2*sum(y(3:2:end-2)) + y(end));
disp(['Integral: ', num2str(I)]);
```
**Key**: 3-point rule, O(h⁴) error, **most accurate & commonly used**

---

### 6.3 Simpson's 3/8 Rule
**Mathematical Formula:**
$$I \approx \frac{3h}{8}(f_0 + 3f_1 + 3f_2 + 2f_3 + 3f_4 + 3f_5 + 2f_6 + \cdots + f_n)$$

**Key Requirement:** n must be **divisible by 3**

**Coefficient Pattern:** f₀ has coefficient 1, then repeating pattern [3,3,2] for each group of 3 intervals

**Algorithm Steps:**
1. Divide [a,b] into n intervals (n ÷ 3 must be integer): $h = \frac{b-a}{n}$
2. Calculate function values at $x_i = a + ih$
3. Sum: coefficients = 1 + (3+3+2)·(groups) + 1
4. Apply: $I = \frac{3h}{8} \times \text{sum}$

```matlab
% Simpson's 3/8 Rule (n must be divisible by 3)
f = @(x) x^2;
a = 0; b = 3; n = 3;  % n divisible by 3
h = (b - a) / n;
x = a:h:b;
y = f(x);

% Coefficient pattern: [1, 3, 3, 2, 3, 3, 2, ..., 1]
% y(2:3:end-1) → every 3rd starting at 2 (coeff=3)
% y(3:3:end-2) → every 3rd starting at 3 (coeff=3)
I = (3*h/8) * (y(1) + 3*sum(y(2:3:end-1)) + 3*sum(y(3:3:end-2)) + y(end));
disp(['Integral: ', num2str(I)]);
```
**Key**: Accurate for cubic polynomials (O(h⁵) error), better than Simpson 1/3 for smooth functions

---

### 6.4 Boole's Rule (5-point)
**Mathematical Formula:**
$$I \approx \frac{2h}{45}(7f_0 + 32f_1 + 12f_2 + 32f_3 + 7f_4)$$

**Key Requirement:** Exactly **4 intervals** (5 points fixed), n = 4 only

**Coefficient Pattern:** [7, 32, 12, 32, 7] - symmetric weights

**Algorithm Steps:**
1. Divide [a,b] into exactly 4 equal intervals: $h = \frac{b-a}{4}$
2. Calculate function at 5 points: $x_i = a + ih$ for i = 0,1,2,3,4
3. Apply formula with fixed coefficients: I = (2h/45)·(7f₀ + 32f₁ + 12f₂ + 32f₃ + 7f₄)
4. Accuracy: O(h⁶)

```matlab
% Boole's Rule (exactly 4 intervals, 5-point formula)
f = @(x) x^2;
a = 0; b = 4;
h = (b - a) / 4;  % Must divide into 4 intervals
x = a:h:b;        % Will have exactly 5 points
y = f(x);

% Fixed coefficients: [7, 32, 12, 32, 7]
I = (2*h/45) * (7*y(1) + 32*y(2) + 12*y(3) + 32*y(4) + 7*y(5));
disp(['Integral: ', num2str(I)]);
```
**Key**: Fixed 4-interval rule, very accurate (O(h⁶)), asymmetric first/last coefficients prevent extension

---

### 6.5 Newton-Cotes 6-point
**Mathematical Formula:**
$$I \approx \frac{5h}{288}(19f_0 + 75f_1 + 50f_2 + 50f_3 + 75f_4 + 19f_5)$$

**Key Requirement:** Exactly **5 intervals** (6 points fixed), n = 5 only

**Coefficient Pattern:** [19, 75, 50, 50, 75, 19] - highly symmetric weights

**Algorithm Steps:**
1. Divide [a,b] into exactly 5 equal intervals: $h = \frac{b-a}{5}$
2. Calculate function at 6 points: $x_i = a + ih$ for i = 0,1,2,3,4,5
3. Apply formula with fixed coefficients: I = (5h/288)·(19f₀ + 75f₁ + 50f₂ + 50f₃ + 75f₄ + 19f₅)
4. Accuracy: O(h⁸)

```matlab
% Newton-Cotes 6-point (exactly 5 intervals)
f = @(x) x^2;
a = 0; b = 5;
h = (b - a) / 5;  % Must divide into 5 intervals
x = a:h:b;        % Will have exactly 6 points
y = f(x);

% Fixed coefficients: [19, 75, 50, 50, 75, 19]
I = (5*h/288) * (19*y(1) + 75*y(2) + 50*y(3) + 50*y(4) + 75*y(5) + 19*y(6));
disp(['Integral: ', num2str(I)]);
```
**Key**: Very high accuracy (O(h⁸)), fixed 5-interval rule, specialized high-order Newton-Cotes formula

---

### 6.6 Weddle's Rule (7-point)
**Mathematical Formula:**
$$I \approx \frac{3h}{10}(f_0 + 5f_1 + f_2 + 6f_3 + f_4 + 5f_5 + f_6)$$

**Key Requirement:** Exactly **6 intervals** (7 points fixed), n = 6 only

**Coefficient Pattern:** [1, 5, 1, 6, 1, 5, 1] - simple repeating pattern, elegant symmetry

**Algorithm Steps:**
1. Divide [a,b] into exactly 6 equal intervals: $h = \frac{b-a}{6}$
2. Calculate function at 7 points: $x_i = a + ih$ for i = 0,1,2,3,4,5,6
3. Apply formula with fixed coefficients: I = (3h/10)·(f₀ + 5f₁ + f₂ + 6f₃ + f₄ + 5f₅ + f₆)
4. Accuracy: O(h⁸)

```matlab
% Weddle's Rule (exactly 6 intervals, 7-point formula)
f = @(x) x^2;
a = 0; b = 6;
h = (b - a) / 6;  % Must divide into 6 intervals
x = a:h:b;        % Will have exactly 7 points
y = f(x);

% Fixed coefficients: [1, 5, 1, 6, 1, 5, 1] - elegant pattern
I = (3*h/10) * (y(1) + 5*y(2) + y(3) + 6*y(4) + y(5) + 5*y(6) + y(7));
disp(['Integral: ', num2str(I)]);
```
**Key**: Very elegant simple pattern [1,5,1,6,1,5,1], O(h⁸) accuracy, fixed 6-interval rule

---

## 📌 SECTION 7: DIFFERENTIAL EQUATIONS (15 min)

### 7.1 Euler Method
**Mathematical Formula:**
$$y_{i+1} = y_i + h \cdot f(t_i, y_i)$$

**Geometric Idea:** Uses tangent line (slope) at $(t_i, y_i)$

**Algorithm:**
1. Start with initial condition: $y(t_0) = y_0$
2. For each step i:
   - Calculate slope: $k = f(t_i, y_i)$
   - Update: $y_{i+1} = y_i + h \cdot k$
   - Next time: $t_{i+1} = t_i + h$
3. Repeat until reaching $t_{end}$

**Example:** Solve dy/dt = t + y, y(0) = 1 on [0,1] with h=0.1
```
t₀=0, y₀=1
t₁=0.1: y₁ = 1 + 0.1(0+1) = 1.1
t₂=0.2: y₂ = 1.1 + 0.1(0.1+1.1) = 1.22
...
```

```matlab
% Euler Method
f = @(t, y) t + y;  % dy/dt
t0 = 0; y0 = 1;     % Initial condition
h = 0.1;            % Step size
t_end = 1;

t = t0:h:t_end;
n = length(t);
y = zeros(1, n);
y(1) = y0;

for i = 2:n
    y(i) = y(i-1) + h * f(t(i-1), y(i-1));
end

plot(t, y, 'b-o'); xlabel('t'); ylabel('y');
```
**Key**: O(h) error, simplest ODE solver, **least accurate**

---

### 7.2 Modified Euler (Heun's Method)
**Mathematical Formula:**
$$k_1 = f(t_i, y_i)$$
$$k_2 = f(t_{i+1}, y_i + h \cdot k_1)$$
$$y_{i+1} = y_i + \frac{h}{2}(k_1 + k_2)$$

**Key Idea:** Average of slopes at start and predicted endpoint

**Algorithm:**
1. Calculate slope at current point: $k_1 = f(t_i, y_i)$
2. Predict next point: $\tilde{y}_{i+1} = y_i + h \cdot k_1$
3. Calculate slope at predicted point: $k_2 = f(t_{i+1}, \tilde{y}_{i+1})$
4. Update using average: $y_{i+1} = y_i + \frac{h}{2}(k_1 + k_2)$

```matlab
% Modified Euler (Heun's Method)
f = @(t, y) t + y;
t0 = 0; y0 = 1;
h = 0.1; t_end = 1;

t = t0:h:t_end;
n = length(t);
y = zeros(1, n);
y(1) = y0;

for i = 2:n
    k1 = f(t(i-1), y(i-1));              % Slope at current
    k2 = f(t(i), y(i-1) + h*k1);         % Slope at predicted next
    y(i) = y(i-1) + (h/2) * (k1 + k2);   % Average of slopes
end

plot(t, y, 'b-o'); xlabel('t'); ylabel('y');
```
**Key**: O(h²) error, 2-stage RK, **faster than Euler**

---

### 7.3 Runge-Kutta 4th Order (RK4)
**Mathematical Formula:**
$$k_1 = f(t_i, y_i)$$
$$k_2 = f(t_i + \frac{h}{2}, y_i + \frac{h}{2}k_1)$$
$$k_3 = f(t_i + \frac{h}{2}, y_i + \frac{h}{2}k_2)$$
$$k_4 = f(t_i + h, y_i + h \cdot k_3)$$
$$y_{i+1} = y_i + \frac{h}{6}(k_1 + 2k_2 + 2k_3 + k_4)$$

**Weighted Slopes:** Uses 4 slopes with weights 1:2:2:1
- k₁: slope at start
- k₂: slope at midpoint (using k₁)
- k₃: slope at midpoint (using k₂)
- k₄: slope at end (using k₃)

**Algorithm:**
1. Calculate 4 slopes at different points
2. Take weighted average: $y_{i+1} = y_i + \frac{h}{6}(k_1 + 2k_2 + 2k_3 + k_4)$

```matlab
% Runge-Kutta 4th Order (RK4) - Most accurate
f = @(t, y) t + y;
t0 = 0; y0 = 1;
h = 0.1; t_end = 1;

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
**Key**: O(h⁴) error, **most accurate**, 4-stage RK, **widely used**

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
