% Bisection Method in Octave
% Solves f(x) = 0 within [a, b]

clc; clear;

% --- Define your function here ---
f = @(x) 4*x + cos(X) - exp (x) ;   % Example: root near x ≈ 1.5

% --- Input interval ---
a = 0;
b = 0.5;

% --- Check if root exists in [a,b] ---
if f(a) * f(b) > 0
    error('No root in this interval: f(a) and f(b) must have opposite signs.');
end

% --- Parameters ---
tol = 1e-6;       % tolerance
max_iter = 100;   % maximum iterations

fprintf('Bisection Method\n');
fprintf('%-10s %-15s %-15s %-15s %-15s\n', 'Iter', 'a', 'b', 'c', 'f(c)');

for i = 1:max_iter
    c = (a + b) / 2;        % midpoint
    fc = f(c);

    fprintf('%-10d %-15.8f %-15.8f %-15.8f %-15.8f\n', i, a, b, c, fc);

    if abs(fc) < tol || (b - a)/2 < tol
        fprintf('\nRoot found: x = %.8f after %d iterations\n', c, i);
        break;
    end

    if f(a) * fc < 0
        b = c;
    else
        a = c;
    end
end

if i == max_iter
    fprintf('\nMethod did not converge within %d iterations.\n', max_iter);
end

