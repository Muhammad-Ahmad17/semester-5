function fixed_point(g, a, b, x0, tol, max_iter)
    % g     : function handle for x = g(x)
    % a, b  : interval [a, b]
    % x0    : initial guess
    % tol   : tolerance
    % max_iter : max iterations

    % Symbolic check
    syms x
    g_sym = g(x);
    g_prime = diff(g_sym, x);

    % Check Condition 1: g(a), g(b) ∈ [a, b]
    ga = double(g(a));
    gb = double(g(b));
    cond1 = (ga >= a && ga <= b) && (gb >= a && gb <= b);

    % Check Condition 2: |g'(x)| < 1 on [a, b]
    g_prime_func = matlabFunction(g_prime); % Convert to function handle

    % Evaluate |g'(x)| at multiple points in [a, b]
    x_vals = linspace(a, b, 100);
    g_prime_vals = abs(g_prime_func(x_vals));
    max_deriv = max(g_prime_vals);
    cond2 = all(g_prime_vals < 1);

    fprintf('Condition 1 (g(x) in [a,b]): %s\n', tf(cond1));
    fprintf('Condition 2 (|g''(x)| < 1): %s (max |g''(x)| = %.4f)\n', tf(cond2), max_deriv);

    if ~(cond1 && cond2)
        fprintf(' One or both conditions failed. Iteration may not converge.\n');
        return;
    end

    % If both conditions passed, begin iteration
    fprintf('\n Conditions satisfied. Starting Fixed Point Iteration...\n\n');

    iter = 0;
    x = x0;

    while iter < max_iter
        x_next = g(x);
        err = abs(x_next - x); % Xn-Xn-1

        fprintf('Iter %d: x = %.10f, Error = %.10f\n', iter, x_next, err);

        if err < tol
            fprintf('\n Converged to %.10f after %d iterations.\n', x_next, iter);
            return;
        end

        x = x_next; % set future x to current x
        iter = iter + 1;
    end

    fprintf(' Did not converge within %d iterations.\n', max_iter);
end

function out = tf(cond)
    if cond
        out = ' Passed';
    else
        out = ' Failed';
    end
end
