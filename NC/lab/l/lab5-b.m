f  = @(x) exp(x) - 5*x;
df = @(x) exp(x) - 5;

% Take initial guess from user
x0 = input('Enter initial guess x0: ');

tol = 1e-6;
maxIter = 20;

for i = 1:maxIter
    f0 = f(x0);
    f1 = df(x0);

    % Modified Newton-Raphson formula as per the given equation
    denom = f1^2 + 1 - f0 * f1;
    x1 = x0 - (f0 * f1) / denom;
    err = abs(x1 - x0);

    fprintf('Iter %2d: x = %.8f, f(x) = %.8f, Error = %.6f\n', i, x1, f(x1), err);

    if err < tol
        break;
    end

    x0 = x1;
end

fprintf('\nApproximate root = %.8f found in %d iterations (tolerance %.1e)\n', x1, i, tol);