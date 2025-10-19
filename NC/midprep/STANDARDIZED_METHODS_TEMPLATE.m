%═══════════════════════════════════════════════════════════════════════════════
% STANDARDIZED ROOT-FINDING METHODS - UNIVERSAL TEMPLATE
%═══════════════════════════════════════════════════════════════════════════════
% Use these as your reference - All follow the SAME pattern!
%═══════════════════════════════════════════════════════════════════════════════

%╔═══════════════════════════════════════════════════════════════════════════════╗
%║                         METHOD 1: BISECTION                                   ║
%╚═══════════════════════════════════════════════════════════════════════════════╝

clc; clear; close all;

% STEP 1: Define function
f = @(x) (x^2 - 12);

% STEP 2: Get interval input [a, b]
a = input('Enter value of a: ');
b = input('Enter value of b: ');

% STEP 3: Validate interval
while f(a) * f(b) > 0
    disp('f(a) and f(b) have the same sign. Enter new values.');
    a = input('Enter new value of a: ');
    b = input('Enter new value of b: ');
end

% STEP 4: Set parameters
tol = 1e-6;
maxIter = 50;
i = 0;
err = inf;

% STEP 5: Print header
fprintf('\n%s\n', '═══════════════════════════════════════════════════════════════');
fprintf('%-5s %-15s %-15s %-15s\n', 'Iter', 'Xm', 'f(Xm)', 'Error(%)');
fprintf('%s\n', '═══════════════════════════════════════════════════════════════');

% STEP 6: Iteration
while (err > tol) && (i < maxIter)
    i = i + 1;
    
    % FORMULA: Bisection
    Xm = (a + b) / 2.0;
    
    if f(a) * f(Xm) < 0
        b = Xm;
    else
        a = Xm;
    end
        
    % ERROR: Calculate
    if i == 1
        err = inf;
    else
        err = abs((Xm - Xm_old) / Xm) * 100;
    end
    
    % PRINT: Results
    fprintf('%-5d %-15.8f %-15.8f %-15.6f\n', i, Xm, f(Xm), err);
    
    % UPDATE: For next iteration
    Xm_old = Xm;
end

% STEP 7: Final result
fprintf('%s\n', '═══════════════════════════════════════════════════════════════');
fprintf('\n✅ Root ≈ %.8f | Iterations: %d | Error: %.6e\n\n', Xm, i, err);


%╔═══════════════════════════════════════════════════════════════════════════════╗
%║                    METHOD 2: NEWTON-RAPHSON                                   ║
%╚═══════════════════════════════════════════════════════════════════════════════╝

clc; clear; close all;

% STEP 1: Define function and derivative
f = @(x) (exp(x) - 4*x);
df = @(x) (exp(x) - 4);

% STEP 2: Get initial guess
x0 = input('Enter initial guess x0: ');

% STEP 3: (Skip - open method, no validation needed)

% STEP 4: Set parameters
tol = 1e-6;
maxIter = 50;
i = 0;
err = inf;

% STEP 5: Print header
fprintf('\n%s\n', '═══════════════════════════════════════════════════════════════');
fprintf('%-5s %-15s %-15s %-15s\n', 'Iter', 'x', 'f(x)', 'Error(%)');
fprintf('%s\n', '═══════════════════════════════════════════════════════════════');

% STEP 6: Iteration
while (err > tol) && (i < maxIter)
    i = i + 1;
    
    % FORMULA: Newton-Raphson
    f0 = f(x0);
    f1 = df(x0);
    x_new = x0 - f0 / f1;
    
    % ERROR: Calculate
    if i == 1
        err = inf;
    else
        err = abs((x_new - x0) / x_new) * 100;
    end
    
    % PRINT: Results
    fprintf('%-5d %-15.8f %-15.8f %-15.6f\n', i, x_new, f(x_new), err);
    
    % UPDATE: For next iteration
    x0 = x_new;
end

% STEP 7: Final result
fprintf('%s\n', '═══════════════════════════════════════════════════════════════');
fprintf('\n✅ Root ≈ %.8f | Iterations: %d | Error: %.6e\n\n', x_new, i, err);


%╔═══════════════════════════════════════════════════════════════════════════════╗
%║                       METHOD 3: SECANT METHOD                                 ║
%╚═══════════════════════════════════════════════════════════════════════════════╝

clc; clear; close all;

% STEP 1: Define function (NO derivative needed!)
f = @(x) cos(x);

% STEP 2: Get TWO initial guesses
x0 = input('Enter first initial guess x0: ');
x1 = input('Enter second initial guess x1: ');

% STEP 3: (Skip - open method)

% STEP 4: Set parameters
tol = 1e-6;
maxIter = 50;
i = 0;
err = inf;

% STEP 5: Print header
fprintf('\n%s\n', '═══════════════════════════════════════════════════════════════');
fprintf('%-5s %-15s %-15s %-15s\n', 'Iter', 'x', 'f(x)', 'Error(%)');
fprintf('%s\n', '═══════════════════════════════════════════════════════════════');

% STEP 6: Iteration
while (err > tol) && (i < maxIter)
    i = i + 1;
    
    % FORMULA: Secant Method
    x2 = x1 - f(x1) * (x1 - x0) / (f(x1) - f(x0));
    
    % ERROR: Calculate
    if i == 1
        err = inf;
    else
        err = abs((x2 - x1) / x2) * 100;
    end
    
    % PRINT: Results
    fprintf('%-5d %-15.8f %-15.8f %-15.6f\n', i, x2, f(x2), err);
    
    % UPDATE: For next iteration
    x0 = x1;
    x1 = x2;
end

% STEP 7: Final result
fprintf('%s\n', '═══════════════════════════════════════════════════════════════');
fprintf('\n✅ Root ≈ %.8f | Iterations: %d | Error: %.6e\n\n', x2, i, err);


%╔═══════════════════════════════════════════════════════════════════════════════╗
%║                    METHOD 4: REGULA FALSI                                     ║
%╚═══════════════════════════════════════════════════════════════════════════════╝

clc; clear; close all;

% STEP 1: Define function
f = @(x) (x^3 - 2*x - 5);

% STEP 2: Get interval input [a, b]
a = input('Enter value of a: ');
b = input('Enter value of b: ');

% STEP 3: Validate interval
while f(a) * f(b) > 0
    disp('f(a) and f(b) have the same sign. Enter new values.');
    a = input('Enter new value of a: ');
    b = input('Enter new value of b: ');
end

% STEP 4: Set parameters
tol = 1e-6;
maxIter = 50;
i = 0;
err = inf;

% STEP 5: Print header
fprintf('\n%s\n', '═══════════════════════════════════════════════════════════════');
fprintf('%-5s %-10s %-10s %-15s %-15s\n', 'Iter', 'a', 'b', 'x_new', 'Error(%)');
fprintf('%s\n', '═══════════════════════════════════════════════════════════════');

% STEP 6: Iteration
while (err > tol) && (i < maxIter)
    i = i + 1;
    
    % FORMULA: Regula Falsi (Linear interpolation)
    x_new = (a*f(b) - b*f(a)) / (f(b) - f(a));
    
    % Check sign and update bounds
    if f(a) * f(x_new) < 0
        b = x_new;
    else
        a = x_new;
    end
    
    % ERROR: Calculate
    if i == 1
        err = inf;
    else
        err = abs((x_new - x_old) / x_new) * 100;
    end
    
    % PRINT: Results
    fprintf('%-5d %-10.6f %-10.6f %-15.8f %-15.6f\n', i, a, b, x_new, err);
    
    % UPDATE: For next iteration
    x_old = x_new;
end

% STEP 7: Final result
fprintf('%s\n', '═══════════════════════════════════════════════════════════════');
fprintf('\n✅ Root ≈ %.8f | Iterations: %d | Error: %.6e\n\n', x_new, i, err);


%╔═══════════════════════════════════════════════════════════════════════════════╗
%║                    METHOD 5: FIXED POINT ITERATION                            ║
%╚═══════════════════════════════════════════════════════════════════════════════╝

clc; clear; close all;

% STEP 1: Define rearranged function g(x) where x = g(x)
% Example: From x^3 + x - 1 = 0, rearrange to x = (1 - x^3)^(1/3)
% OR from 2*x - log(x) - 7 = 0, rearrange to x = exp(2*x - 7)

g = @(x) nthroot(2*x + 5, 3);  % Example rearrangement
dg = @(x) (2) / (3 * nthroot((2*x + 5)^2, 3));  % Derivative of g(x)

% STEP 2: Get interval [a, b] and initial guess
a = input('Enter value of a: ');
b = input('Enter value of b: ');
x0 = input('Enter initial guess x0: ');

% STEP 3: VALIDATE - Check Fixed Point Conditions
% Condition 1: g(a) and g(b) must be in [a, b]
ga = g(a);
gb = g(b);
cond1 = (ga >= a && ga <= b) && (gb >= a && gb <= b);

% Condition 2: |g`(x)| < 1 on [a, b]
x_test = linspace(a, b, 50);
dg_vals = abs(arrayfun(dg, x_test));
max_deriv = max(dg_vals);
cond2 = all(dg_vals < 1);

fprintf('\n%s\n', '═══════════════════════════════════════════════════════════════');
fprintf('Condition 1 (g(x) ∈ [a,b]): %s\n', cond_status(cond1));
fprintf('Condition 2 (|g''(x)| < 1): %s (max |g''(x)| = %.4f)\n', cond_status(cond2), max_deriv);
fprintf('%s\n', '═══════════════════════════════════════════════════════════════');

if ~(cond1 && cond2)
    fprintf('\n⚠️  WARNING: One or both conditions failed. Method may NOT converge!\n\n');
end

% STEP 4: Set parameters
tol = 1e-6;
maxIter = 50;
i = 0;
err = inf;

% STEP 5: Print header
fprintf('\n%s\n', '═══════════════════════════════════════════════════════════════');
fprintf('%-5s %-15s %-15s %-15s\n', 'Iter', 'x', 'g(x)', 'Error');
fprintf('%s\n', '═══════════════════════════════════════════════════════════════');

% STEP 6: Iteration
while (err > tol) && (i < maxIter)
    i = i + 1;
    
    % FORMULA: Fixed Point (Simple iteration)
    x_new = g(x0);
    
    % ERROR: Calculate (absolute difference)
    if i == 1
        err = inf;
    else
        err = abs(x_new - x0);
    end
    
    % PRINT: Results
    fprintf('%-5d %-15.10f %-15.10f %-15.2e\n', i, x_new, g(x_new), err);
    
    % UPDATE: For next iteration
    x0 = x_new;
end

% STEP 7: Final result
fprintf('%s\n', '═══════════════════════════════════════════════════════════════');
fprintf('\n✅ Root ≈ %.10f | Iterations: %d | Error: %.2e\n\n', x_new, i, err);

% Helper function for condition status
function out = cond_status(cond)
    if cond
        out = '✅ Passed';
    else
        out = '❌ Failed';
    end
end


%╔═══════════════════════════════════════════════════════════════════════════════╗
%║              METHOD 6: MODIFIED NEWTON-RAPHSON                                ║
%╚═══════════════════════════════════════════════════════════════════════════════╝

clc; clear; close all;

% STEP 1: Define function and derivatives (BOTH first AND second!)
f = @(x) (x^3 - 5*x^2 + 7*x - 3);
df = @(x) (3*x^2 - 10*x + 7);
d2f = @(x) (6*x - 10);

% STEP 2: Get initial guess
x0 = input('Enter initial guess x0: ');

% STEP 3: (Skip - open method)

% STEP 4: Set parameters
tol = 1e-6;
maxIter = 50;
i = 0;
err = inf;

% STEP 5: Print header
fprintf('\n%s\n', '═══════════════════════════════════════════════════════════════');
fprintf('%-5s %-15s %-15s %-15s\n', 'Iter', 'x', 'f(x)', 'Error(%)');
fprintf('%s\n', '═══════════════════════════════════════════════════════════════');

% STEP 6: Iteration
while (err > tol) && (i < maxIter)
    i = i + 1;
    
    % FORMULA: Modified Newton-Raphson
    f0 = f(x0);
    f1 = df(x0);
    f2 = d2f(x0);
    x_new = x0 - (f0 * f1) / (f1^2 - f0 * f2);
    
    % ERROR: Calculate
    if i == 1
        err = inf;
    else
        err = abs((x_new - x0) / x_new) * 100;
    end
    
    % PRINT: Results
    fprintf('%-5d %-15.8f %-15.8f %-15.6f\n', i, x_new, f(x_new), err);
    
    % UPDATE: For next iteration
    x0 = x_new;
end

% STEP 7: Final result
fprintf('%s\n', '═══════════════════════════════════════════════════════════════');
fprintf('\n✅ Root ≈ %.8f | Iterations: %d | Error: %.6e\n\n', x_new, i, err);

