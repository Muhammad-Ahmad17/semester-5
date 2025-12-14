
%f(x) = 4(x) +sin(x) - e^x

clc
clear
close all

%f = @(x) (4*x + sin(x) - exp(x));
f = @(x) (x^2 - 12)
a = input("Enter value of a: ");
b = input("Enter value of b: ");

while (f(a) * f(b) > 0)
    disp('f(a) and f(b) have the same sign. Enter new values.');
    a = input("Enter new value of a: ");
    b = input("Enter new value of b: ");
end

tol = 1e-6;       % tolerance
Xm_old = a;       % store old midpoint
maxIter = 15;     % maximum iterations
i = 0;            % iteration counter
err = inf;        % initialize error

while (err > tol) && (i < maxIter)
    i = i + 1;
    Xm = (a + b) / 2.0;

    if (f(b) * f(Xm)) < 0
        a = Xm;
    else
        b = Xm;
    end

    % Compute percentage relative error
    if i == 1
        err = inf; % no error in first iteration
        fprintf('Iter %2d: Xm = %.8f, Error = ----\n', i, Xm);
    else
        err = abs((Xm - Xm_old) / Xm) * 100;
        fprintf('Iter %2d: Xm = %.8f, Error = %.6f%%\n', i, Xm, err);
    end

    Xm_old = Xm;
end

fprintf('\nApproximate root = %.4f found in %d iterations (tolerance %.1e)\n', Xm, i, tol);