clc
clear
close all

% Define the function (you can replace this with any function you like)
f = @(x) x.^3 - 2*x - 5;

% Input interval
a = input('Enter value of a: ');
b = input('Enter value of b: ');

% Check if the initial guesses bracket a root
while f(a) * f(b) > 0
    disp('f(a) and f(b) have the same sign. Enter new values.');
    a = input('Enter new value of a: ');
    b = input('Enter new value of b: ');
end

tol = 1e-6;       % tolerance
maxIter = 50;     % maximum iterations
i = 0;            % iteration counter
err = inf;        % initialize error

fprintf('\nIter    a           b           x_new         f(x_new)        Error(%%)\n');
fprintf('--------------------------------------------------------------------------\n');

while (err > tol) && (i < maxIter)
    i = i + 1;

    % Regula Falsi formula (linear interpolation)
    x_new = (a*f(b) - b*f(a)) / (f(b) - f(a));

    % Compute relative error
    if i == 1
        err = inf;
    else
        err = abs((x_new - x_old)/x_new)*100;
    end

    % Display iteration data
    fprintf('%3d   %10.6f   %10.6f   %10.6f   %12.6f   %10.6f\n', ...
            i, a, b, x_new, f(x_new), err);

    % Check sign and update bounds
    if f(a) * f(x_new) < 0
        b = x_new;
    elseif f(a) * f(x_new) > 0
        a = x_new;
    else
        % Root found exactly
        break;
    end

    % Store current x for next iteration
    x_old = x_new;
end

fprintf('\nApproximate root = %.6f found in %d iterations (tolerance %.1e)\n', ...
        x_new, i, tol);
