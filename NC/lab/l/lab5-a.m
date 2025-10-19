clc
clear
close all

% Define the function (you can replace this with any function you like)
%f = @(x) sin(x) - x - 1;
f = @(x) 2*x - log(x) - 7;

% Input interval
a = input('Enter value of a: ');
b = input('Enter value of b: ') ;


tol = 1e-6;       % tolerance
maxIter = 20;     % maximum iterations
i = 0;            % iteration counter
err = Inf;        % initialize error

% Initialize x_old to something before loop (e.g., a)
x_old = a;        

fprintf('\nIter    a           b           x_new         f(x_new)        Error(%%)\n');

while (err > tol) && (i < maxIter)
    i = i + 1;

    % Regula Falsi formula (linear interpolation)
    x_new = (a*f(b) - b*f(a)) / (f(b) - f(a));

    % Compute relative error (skip error calculation for first iteration)
    err = abs((x_new - x_old)/x_new)*100;
   
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
        err = 0; % no error
        break;
    end

    % Store current x for next iteration
    x_old = x_new;
end

fprintf('\nApproximate root = %.6f found in %d iterations (tolerance %.1e)\n', ...
        x_new, i, tol);
