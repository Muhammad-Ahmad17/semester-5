%% Composite Newton-Cotes Methods for Piecewise Integral
% Evaluate: I = ∫₀¹⁰⁰ f(x) dx
% f(x) = 0 (0≤x<30) | -9.1688e-6*x³ + 2.7961e-3*x² - 0.28487*x + 9.6778 (30≤x≤100)

clear; clc; close all;

% Piecewise function
function y = f(x)
    y = zeros(size(x));
    mask = (x >= 30) & (x <= 100);
    y(mask) = -9.1688e-6 * x(mask).^3 + 2.7961e-3 * x(mask).^2 - 0.2848700 * x(mask) + 9.6778;
end

% Integration parameters
a = 0; b = 100;
N_values = [20, 50, 100, 200, 500];

fprintf('\n%s\n', repmat('=',1,70));
fprintf('COMPOSITE NEWTON-COTES METHODS FOR PIECEWISE INTEGRAL\n');
fprintf('I = ∫₀¹⁰⁰ f(x) dx\n');
fprintf('%s\n\n', repmat('=',1,70));

%% TRAPEZOIDAL
fprintf('\n%s\n', repmat('-',1,70));
fprintf('METHOD 1: COMPOSITE TRAPEZOIDAL RULE (n=1, O(h²))\n');
fprintf('%s\n', repmat('-',1,70));
for N = N_values
    h = (b - a) / N;
    x = linspace(a, b, N+1);
    y_vals = f(x);
    I = (h/2) * (y_vals(1) + 2*sum(y_vals(2:N)) + y_vals(N+1));
    fprintf('N = %3d,  h = %8.6f,  I = %.10f\n', N, h, I);
end

%% SIMPSON 1/3
fprintf('\n%s\n', repmat('-',1,70));
fprintf('METHOD 2: COMPOSITE SIMPSON 1/3 RULE (n=2, O(h⁴))\n');
fprintf('%s\n', repmat('-',1,70));
for N = N_values
    if mod(N, 2) ~= 0, N = N + 1; end
    h = (b - a) / N;
    x = linspace(a, b, N+1);
    y_vals = f(x);
    I = (h/3) * (y_vals(1) + 4*sum(y_vals(2:2:N)) + 2*sum(y_vals(3:2:N-1)) + y_vals(N+1));
    fprintf('N = %3d,  h = %8.6f,  I = %.10f\n', N, h, I);
end

%% SIMPSON 3/8
fprintf('\n%s\n', repmat('-',1,70));
fprintf('METHOD 3: COMPOSITE SIMPSON 3/8 RULE (n=3, O(h⁴))\n');
fprintf('%s\n', repmat('-',1,70));
for N = N_values
    if mod(N, 3) ~= 0, N = N + (3 - mod(N, 3)); end
    h = (b - a) / N;
    x = linspace(a, b, N+1);
    y_vals = f(x);
    I = (3*h/8) * (y_vals(1) + y_vals(N+1));
    for i = 2:N
        w = (mod(i-1, 3) == 0) ? 3 : 9;
        I = I + (3*h/8) * w * y_vals(i);
    end
    fprintf('N = %3d,  h = %8.6f,  I = %.10f\n', N, h, I);
end

%% BOOLE'S
fprintf('\n%s\n', repmat('-',1,70));
fprintf('METHOD 4: COMPOSITE BOOLE''S RULE (n=4, O(h⁶))\n');
fprintf('%s\n', repmat('-',1,70));
for N = N_values
    if mod(N, 4) ~= 0, N = N + (4 - mod(N, 4)); end
    h = (b - a) / N;
    x = linspace(a, b, N+1);
    y_vals = f(x);
    I = (2*h/45) * (y_vals(1) + y_vals(N+1));
    for i = 2:N
        w = [7, 32, 12, 32];
        I = I + (2*h/45) * w(mod(i-1, 4) + 1) * y_vals(i);
    end
    fprintf('N = %3d,  h = %8.6f,  I = %.10f\n', N, h, I);
end

%% n=5 RULE
fprintf('\n%s\n', repmat('-',1,70));
fprintf('METHOD 5: COMPOSITE n=5 RULE (6-point, O(h⁶))\n');
fprintf('%s\n', repmat('-',1,70));
for N = N_values
    if mod(N, 5) ~= 0, N = N + (5 - mod(N, 5)); end
    h = (b - a) / N;
    x = linspace(a, b, N+1);
    y_vals = f(x);
    I = (5*h/288) * (y_vals(1) + y_vals(N+1));
    for i = 2:N
        w = [19, 75, 50, 50, 75];
        I = I + (5*h/288) * w(mod(i-1, 5) + 1) * y_vals(i);
    end
    fprintf('N = %3d,  h = %8.6f,  I = %.10f\n', N, h, I);
end

%% WEDDLE'S
fprintf('\n%s\n', repmat('-',1,70));
fprintf('METHOD 6: COMPOSITE WEDDLE''S RULE (n=6, 7-point, O(h⁸)) - BEST\n');
fprintf('%s\n', repmat('-',1,70));
for N = N_values
    if mod(N, 6) ~= 0, N = N + (6 - mod(N, 6)); end
    h = (b - a) / N;
    x = linspace(a, b, N+1);
    y_vals = f(x);
    I = (h/140) * (y_vals(1) + y_vals(N+1));
    for i = 2:N
        w = [41, 216, 27, 272, 27, 216];
        I = I + (h/140) * w(mod(i-1, 6) + 1) * y_vals(i);
    end
    fprintf('N = %3d,  h = %8.6f,  I = %.10f\n', N, h, I);
end

fprintf('\n%s\n', repmat('=',1,70));
fprintf('SUMMARY AT N=500:\n');
fprintf('All methods converge as h → 0 (N → ∞)\n');
fprintf('Recommend: Simpson 1/3 (practical) or Weddle (maximum accuracy)\n');
fprintf('%s\n', repmat('=',1,70));
