clc
clear
close all

% Define the function and its derivative
% f  = @(x) cos(x);
% df = @(x) -sin(x);

% f  = @(x) atan(x - 3);
% df = @(x) 1 ./ (1 + (x - 3).^2);

% f  = @(x) x^3 - x - 3;
% df = @(x) 3*x^2 - 1;

f  = @(x) -x.^2 + x - 3;
df = @(x) -2*x + 1;

% Initial guess
x0 = input("Enter initial guess: ");

tol = 1e-6;       % tolerance
maxIter = 35;     % maximum iterations
i = 0;            % iteration counter
err = inf;        % initialize error

while (err > tol) && (i < maxIter)
    i = i + 1;

    % Newton-Raphson formula
    x1 = x0 - f(x0)/df(x0);

    % Compute relative error
    if i == 1

        err = inf;
        fprintf('Iter %2d: x = %.8f, Error = ----\n', i, x1);
    else
        err = abs((x1 - x0)/x1) * 100;
        fprintf('Iter %2d: x = %.8f, Error = %.6f%%\n', i, x1, err);
    end

    % Update guess
    x0 = x1;
end

fprintf('\nApproximate root = %.6f found in %d iterations (tolerance %.1e)\n', x1, i, tol);


