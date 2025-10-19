clc
clear
close all
syms x

% Input function
f_str = input('Enter function f(x): ', 's');
f_sym = str2sym(f_str);
f = matlabFunction(f_sym, 'Vars', x);

% Generate candidate g(x) forms
g_candidates = {
    x - f_sym, ...
    x + f_sym, ...
    (2*x + 5)^(1/3), ...
    nthroot(2*x + 5, 3), ...
    (f_sym + x)/2
};

fprintf('\nGenerated possible g(x) forms:\n');
for i = 1:length(g_candidates)
    fprintf('g%d(x) = %s\n', i, char(g_candidates{i}));
end

x0 = input('\nEnter initial guess x0: ');

fprintf('\nChecking convergence at x0 = %.4f...\n', x0);
best_idx = -1; best_slope = inf;

for i = 1:length(g_candidates)
    g_sym = g_candidates{i};
    try
        dg = diff(g_sym, x);
        g_fun = matlabFunction(g_sym, 'Vars', x);
        dg_fun = matlabFunction(dg, 'Vars', x);
        slope = abs(dg_fun(x0));
        f_check = abs(f(g_fun(x0)));
        fprintf('g%d(x): |g''(x0)| = %.6f, f(g(x0)) = %.6f\n', i, slope, f_check);
        if slope < 1 && f_check < 10
            if slope < best_slope
                best_slope = slope;
                best_idx = i;
            end
        end
    catch
        fprintf('g%d(x): skipped (symbolic issue)\n', i);
    end
end

if best_idx == -1
    error('No g(x) satisfies convergence condition near x0.');
end

fprintf('\n✅ Selected g%d(x) = %s  (|g''(x0)| = %.6f)\n', ...
    best_idx, char(g_candidates{best_idx}), best_slope);

% Perform iteration
g = matlabFunction(g_candidates{best_idx}, 'Vars', x);
tol = 1e-6; maxIter = 50; i = 0; err = inf;

fprintf('\nIter    x_old        x_new        Error(%%)\n');
fprintf('-------------------------------------------\n');

while (err > tol) && (i < maxIter)
    i = i + 1;
    x_new = g(x0);
    err = abs((x_new - x0)/x_new) * 100;
    fprintf('%3d   %10.6f   %10.6f   %10.6f\n', i, x0, x_new, err);
    x0 = x_new;
end

fprintf('\nApproximate root = %.6f found in %d iterations (tolerance %.1e)\n', ...
    x_new, i, tol);
