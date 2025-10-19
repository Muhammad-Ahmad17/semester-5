clc
clear
close all
% Define the function
% f  = @(x) -x^2 +x -3 ;
f= @(x) atan (x-3);

% Initial guesses (two values needed for Secant method)
x0 = input("Enter first initial guess: ");
x1 = input("Enter second initial guess: ");

tol = 1e-6;       % tolerance
maxIter = 15;     % maximum iterations
i = 0;            % iteration counter
err = inf;        % initialize error

while (err > tol) && (i < maxIter)
    i = i + 1;

    % Secant method formula
    x2 = x1 - f(x1) * (x1 - x0) / (f(x1) - f(x0));

    % Compute relative error
    if i == 1
        err = inf;
        fprintf('Iter %2d: x = %.8f, Error = ----\n', i, x2);
    else
        err = abs((x2 - x1)/x2) * 100;
        fprintf('Iter %2d: x = %.8f, Error = %.6f%%\n', i, x2, err);
    end

    % Update guesses
    x0 = x1;
    x1 = x2;
end

fprintf('\nApproximate root = %.6f found in %d iterations (tolerance %.1e)\n', x2, i, tol);
