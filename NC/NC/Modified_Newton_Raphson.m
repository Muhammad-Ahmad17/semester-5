clc
clear
close all

% Define function and its derivatives
f  = @(x) x.^3 - 5*x.^2 + 7*x - 3;
df = @(x) 3*x.^2 - 10*x + 7;
d2f = @(x) 6*x - 10;

% Take initial guess from user
x0 = input('Enter initial guess x0: ');

tol = 1e-6;
maxIter = 20;

for i = 1:maxIter
    f0 = f(x0);
    f1 = df(x0);
    f2 = d2f(x0);

    % Modified Newton-Raphson formula
    x1 = x0 - (f0 * f1) / (f1^2 - f0 * f2);
    err = abs(x1 - x0);

    fprintf('Iter %2d: x = %.8f, f(x) = %.8f, Error = %.6f\n', i, x1, f(x1), err);

    if err < tol
        break;
    end

    x0 = x1;
end

fprintf('\nApproximate root = %.8f found in %d iterations (tolerance %.1e)\n', x1, i, tol);
