%% Composite Newton-Cotes Methods for Piecewise Integral
% Integral: I = ∫₀¹⁰⁰ f(x) dx
% where f(x) is piecewise defined as:
%   f(x) = 0,                                        0 <= x < 30
%   f(x) = -9.1688e-6*x³ + 2.7961e-3*x² - 0.28487*x + 9.6778,  30 <= x <= 100
%   f(x) = 0,                                        x > 100

clear; clc; close all;

%% Define the piecewise function
function y = f(x)
    y = zeros(size(x));
    
    % Region 1: 0 <= x < 30, f(x) = 0
    mask1 = (x >= 0) & (x < 30);
    y(mask1) = 0;
    
    % Region 2: 30 <= x <= 100, f(x) = cubic polynomial
    mask2 = (x >= 30) & (x <= 100);
    y(mask2) = -9.1688e-6 * x(mask2).^3 + 2.7961e-3 * x(mask2).^2 - ...
              0.2848700 * x(mask2) + 9.6778;
    
    % Region 3: 100 < x, f(x) = 0
    mask3 = (x > 100);
    y(mask3) = 0;
end

%% Integration parameters
a = 0;      % Lower limit
b = 100;    % Upper limit
N_values = [20, 50, 100, 200, 500];  % Number of intervals

fprintf('\n====================================================================\n');
fprintf('  COMPOSITE NEWTON-COTES METHODS FOR PIECEWISE INTEGRAL\n');
fprintf('  Integration: ∫₀¹⁰⁰ f(x) dx\n');
fprintf('====================================================================\n\n');

%% Method 1: Composite Trapezoidal Rule (n=1, O(h²))
fprintf('\n========================================\n');
fprintf('METHOD 1: COMPOSITE TRAPEZOIDAL RULE (n=1)\n');
fprintf('Convergence: O(h²)\n');
fprintf('Formula: I ≈ (h/2)[y₀ + 2(y₁+...+y_{N-1}) + y_N]\n');
fprintf('========================================\n\n');

trap_results = [];
for N = N_values
    h = (b - a) / N;
    x = linspace(a, b, N+1);
    y_vals = f(x);
    
    I_trap = (h/2) * (y_vals(1) + 2*sum(y_vals(2:N)) + y_vals(N+1));
    
    trap_results = [trap_results; N, h, I_trap];
    fprintf('N = %4d,  h = %8.6f,  I = %.10f\n', N, h, I_trap);
end

%% Method 2: Composite Simpson's 1/3 Rule (n=2, O(h⁴))
fprintf('\n========================================\n');
fprintf('METHOD 2: COMPOSITE SIMPSON 1/3 RULE (n=2)\n');
fprintf('Convergence: O(h⁴)\n');
fprintf('Formula: I ≈ (h/3)[y₀ + 4(y₁+y₃+...) + 2(y₂+y₄+...) + y_N]\n');
fprintf('========================================\n\n');

simp13_results = [];
for N = N_values
    if mod(N, 2) ~= 0, N = N + 1; end  % Ensure N is even
    
    h = (b - a) / N;
    x = linspace(a, b, N+1);
    y_vals = f(x);
    
    sum_odd = sum(y_vals(2:2:N));      % y₁, y₃, y₅, ...
    sum_even = sum(y_vals(3:2:N-1));   % y₂, y₄, y₆, ...
    
    I_simp13 = (h/3) * (y_vals(1) + 4*sum_odd + 2*sum_even + y_vals(N+1));
    
    simp13_results = [simp13_results; N, h, I_simp13];
    fprintf('N = %4d,  h = %8.6f,  I = %.10f\n', N, h, I_simp13);
end

%% Method 3: Composite Simpson's 3/8 Rule (n=3, O(h⁴))
fprintf('\n========================================\n');
fprintf('METHOD 3: COMPOSITE SIMPSON 3/8 RULE (n=3)\n');
fprintf('Convergence: O(h⁴)\n');
fprintf('Formula: I ≈ (3h/8)[y₀ + 3(y₁+y₂+y₄+y₅+...) + 2(y₃+y₆+...) + y_N]\n');
fprintf('========================================\n\n');

simp38_results = [];
for N = N_values
    if mod(N, 3) ~= 0, N = N + (3 - mod(N, 3)); end  % Ensure N divisible by 3
    
    h = (b - a) / N;
    x = linspace(a, b, N+1);
    y_vals = f(x);
    
    I_simp38 = (3*h/8) * (y_vals(1) + y_vals(N+1));
    for i = 2:N
        weight_index = mod(i-1, 3);
        if weight_index == 0
            weight = 3;
        else  % weight_index == 1 or 2
            weight = 9;
        end
        I_simp38 = I_simp38 + (3*h/8) * weight * y_vals(i);
    end
    
    simp38_results = [simp38_results; N, h, I_simp38];
    fprintf('N = %4d,  h = %8.6f,  I = %.10f\n', N, h, I_simp38);
end

%% Method 4: Composite Boole's Rule (n=4, O(h⁶))
fprintf('\n========================================\n');
fprintf('METHOD 4: COMPOSITE BOOLE''S RULE (n=4)\n');
fprintf('Convergence: O(h⁶)\n');
fprintf('Formula: I ≈ (2h/45) with weights [7, 32, 12, 32, 7] repeating\n');
fprintf('========================================\n\n');

boole_results = [];
for N = N_values
    if mod(N, 4) ~= 0, N = N + (4 - mod(N, 4)); end  % Ensure N divisible by 4
    
    h = (b - a) / N;
    x = linspace(a, b, N+1);
    y_vals = f(x);
    
    I_boole = (2*h/45) * (y_vals(1) + y_vals(N+1));
    for i = 2:N
        weight_index = mod(i-1, 4);
        switch weight_index
            case 0, weight = 7;
            case 1, weight = 32;
            case 2, weight = 12;
            case 3, weight = 32;
        end
        I_boole = I_boole + (2*h/45) * weight * y_vals(i);
    end
    
    boole_results = [boole_results; N, h, I_boole];
    fprintf('N = %4d,  h = %8.6f,  I = %.10f\n', N, h, I_boole);
end

%% Method 5: Composite n=5 Rule (6-point rule, O(h⁶))
fprintf('\n========================================\n');
fprintf('METHOD 5: COMPOSITE n=5 RULE (6-point rule)\n');
fprintf('Convergence: O(h⁶)\n');
fprintf('Formula: I ≈ (5h/288) with weights [19, 75, 50, 50, 75] repeating\n');
fprintf('========================================\n\n');

n5_results = [];
for N = N_values
    if mod(N, 5) ~= 0, N = N + (5 - mod(N, 5)); end  % Ensure N divisible by 5
    
    h = (b - a) / N;
    x = linspace(a, b, N+1);
    y_vals = f(x);
    
    I_n5 = (5*h/288) * (y_vals(1) + y_vals(N+1));
    for i = 2:N
        weight_index = mod(i-1, 5);
        switch weight_index
            case 0, weight = 19;
            case 1, weight = 75;
            case 2, weight = 50;
            case 3, weight = 50;
            case 4, weight = 75;
        end
        I_n5 = I_n5 + (5*h/288) * weight * y_vals(i);
    end
    
    n5_results = [n5_results; N, h, I_n5];
    fprintf('N = %4d,  h = %8.6f,  I = %.10f\n', N, h, I_n5);
end

%% Method 6: Composite Weddle's Rule (n=6, 7-point rule, O(h⁸))
fprintf('\n========================================\n');
fprintf('METHOD 6: COMPOSITE WEDDLE''S RULE (n=6, 7-point rule)\n');
fprintf('Convergence: O(h⁸) - Highest accuracy\n');
fprintf('Formula: I ≈ (h/140) with weights [41, 216, 27, 272, 27, 216] repeating\n');
fprintf('========================================\n\n');

weddle_results = [];
for N = N_values
    if mod(N, 6) ~= 0, N = N + (6 - mod(N, 6)); end  % Ensure N divisible by 6
    
    h = (b - a) / N;
    x = linspace(a, b, N+1);
    y_vals = f(x);
    
    I_weddle = (h/140) * (y_vals(1) + y_vals(N+1));
    for i = 2:N
        weight_index = mod(i-1, 6);
        switch weight_index
            case 0, weight = 41;
            case 1, weight = 216;
            case 2, weight = 27;
            case 3, weight = 272;
            case 4, weight = 27;
            case 5, weight = 216;
        end
        I_weddle = I_weddle + (h/140) * weight * y_vals(i);
    end
    
    weddle_results = [weddle_results; N, h, I_weddle];
    fprintf('N = %4d,  h = %8.6f,  I = %.10f\n', N, h, I_weddle);
end

%% Summary Comparison Table
fprintf('\n====================================================================\n');
fprintf('  SUMMARY - COMPARISON AT N = 500 INTERVALS\n');
fprintf('====================================================================\n\n');

N_summary = 500;
fprintf('%-30s %15s %12s\n', 'Method', 'Integral Value', 'Convergence');
fprintf('%-30s %15s %12s\n', repmat('-', 1, 30), repmat('-', 1, 15), repmat('-', 1, 12));

fprintf('%-30s %15.10f %12s\n', 'Trapezoidal (n=1)', trap_results(end,3), 'O(h²)');
fprintf('%-30s %15.10f %12s\n', 'Simpson 1/3 (n=2)', simp13_results(end,3), 'O(h⁴)');
fprintf('%-30s %15.10f %12s\n', 'Simpson 3/8 (n=3)', simp38_results(end,3), 'O(h⁴)');
fprintf('%-30s %15.10f %12s\n', 'Boole''s Rule (n=4)', boole_results(end,3), 'O(h⁶)');
fprintf('%-30s %15.10f %12s\n', 'n=5 Rule (6-point)', n5_results(end,3), 'O(h⁶)');
fprintf('%-30s %15.10f %12s\n', 'Weddle''s Rule (n=6)', weddle_results(end,3), 'O(h⁸)');

fprintf('\n');
fprintf('Note: All methods converge to the same value as N increases (h → 0).\n');
fprintf('Weddle''s Rule (O(h⁸)) provides highest accuracy for smooth functions.\n');
fprintf('Simpson 1/3 (O(h⁴)) offers good balance between accuracy and computation.\n');

%% Visualization
figure('Position', [100 100 1200 600]);

% Plot 1: Convergence comparison
subplot(1, 2, 1);
semilogy(trap_results(:,1), abs(trap_results(:,3) - weddle_results(end,3)), 'o-', 'LineWidth', 2, 'DisplayName', 'Trapezoidal');
hold on;
semilogy(simp13_results(:,1), abs(simp13_results(:,3) - weddle_results(end,3)), 's-', 'LineWidth', 2, 'DisplayName', 'Simpson 1/3');
semilogy(simp38_results(:,1), abs(simp38_results(:,3) - weddle_results(end,3)), '^-', 'LineWidth', 2, 'DisplayName', 'Simpson 3/8');
semilogy(boole_results(:,1), abs(boole_results(:,3) - weddle_results(end,3)), 'd-', 'LineWidth', 2, 'DisplayName', 'Boole''s');
semilogy(n5_results(:,1), abs(n5_results(:,3) - weddle_results(end,3)), 'v-', 'LineWidth', 2, 'DisplayName', 'n=5 Rule');
xlabel('Number of Intervals (N)', 'FontSize', 11);
ylabel('Absolute Error', 'FontSize', 11);
title('Convergence Comparison of Newton-Cotes Methods', 'FontSize', 12, 'FontWeight', 'bold');
grid on;
legend('FontSize', 10);

% Plot 2: Integral values convergence
subplot(1, 2, 2);
plot(trap_results(:,1), trap_results(:,3), 'o-', 'LineWidth', 2, 'DisplayName', 'Trapezoidal');
hold on;
plot(simp13_results(:,1), simp13_results(:,3), 's-', 'LineWidth', 2, 'DisplayName', 'Simpson 1/3');
plot(simp38_results(:,1), simp38_results(:,3), '^-', 'LineWidth', 2, 'DisplayName', 'Simpson 3/8');
plot(boole_results(:,1), boole_results(:,3), 'd-', 'LineWidth', 2, 'DisplayName', 'Boole''s');
plot(n5_results(:,1), n5_results(:,3), 'v-', 'LineWidth', 2, 'DisplayName', 'n=5 Rule');
plot(weddle_results(:,1), weddle_results(:,3), 'x-', 'LineWidth', 2, 'DisplayName', 'Weddle''s');
xlabel('Number of Intervals (N)', 'FontSize', 11);
ylabel('Integral Value I', 'FontSize', 11);
title('Integral Value Convergence (All Methods)', 'FontSize', 12, 'FontWeight', 'bold');
grid on;
legend('FontSize', 10);

fprintf('\nGraphs generated and saved as figure.\n');
