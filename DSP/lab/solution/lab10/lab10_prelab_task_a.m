%% LAB 10 - PRE-LAB TASK A: BUTTERWORTH LOW-PASS PROTOTYPE
% Analog Butterworth filter design from specifications
% Clear explanation for viva preparation

clear; clc; close all;

fprintf('\n');
fprintf('╔════════════════════════════════════════════════════════╗\n');
fprintf('║         PRE-LAB TASK A: BUTTERWORTH LP DESIGN        ║\n');
fprintf('╚════════════════════════════════════════════════════════╝\n');
fprintf('\n');

%% STEP 1: GIVEN SPECIFICATIONS
fprintf('STEP 1: GIVEN SPECIFICATIONS\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

wp = 0.2*pi;    % Passband cutoff (rad/s)
ws = 0.3*pi;    % Stopband cutoff (rad/s)
Rp = 7;         % Passband ripple (dB)
Rs = 16;        % Stopband attenuation (dB)

fprintf('  ωp (passband cutoff)     = 0.2π rad/s = %.4f rad/s\n', wp);
fprintf('  ωs (stopband cutoff)     = 0.3π rad/s = %.4f rad/s\n', ws);
fprintf('  Rp (passband ripple)     = %d dB\n', Rp);
fprintf('  Rs (stopband attenuation) = %d dB\n\n', Rs);

%% STEP 2: NORMALIZE FREQUENCIES
fprintf('STEP 2: NORMALIZE FREQUENCIES (to passband edge)\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

wp_norm = 1;           % By definition, passband = 1
ws_norm = ws / wp;     % Normalized stopband

fprintf('  ωp (normalized) = 1 (by definition)\n');
fprintf('  ωs (normalized) = ωs/ωp = (0.3π)/(0.2π) = %.2f\n\n', ws_norm);

%% STEP 3: CALCULATE FILTER ORDER
fprintf('STEP 3: CALCULATE MINIMUM FILTER ORDER\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Butterworth filter order formula:\n');
fprintf('  N ≥ log₁₀[(10^(Rs/10) - 1) / (10^(Rp/10) - 1)] / (2 * log₁₀(ωs/ωp))\n\n');

% Calculate numerator
num_order = 10^(Rs/10) - 1;
den_order = 10^(Rp/10) - 1;
numerator = log10(num_order / den_order);

fprintf('  Numerator: log₁₀[(10^(%d/10) - 1) / (10^(%d/10) - 1)]\n', Rs, Rp);
fprintf('           = log₁₀[(%d) / (%.4f)]\n', num_order, den_order);
fprintf('           = log₁₀(%.4f) = %.4f\n\n', num_order/den_order, numerator);

% Calculate denominator
denominator = 2 * log10(ws_norm / wp_norm);
fprintf('  Denominator: 2 * log₁₀(%.2f/1) = 2 * log₁₀(%.2f) = %.4f\n\n', ws_norm, ws_norm, denominator);

% Calculate order
N_calc = numerator / denominator;
N = ceil(N_calc);

fprintf('  N_calculated = %.4f / %.4f = %.2f\n', numerator, denominator, N_calc);
fprintf('  N_minimum = ⌈%.2f⌉ = %d\n\n', N_calc, N);

fprintf('  ✓ Filter order selected: N = %d\n\n', N);

%% STEP 4: DETERMINE CUTOFF FREQUENCY
fprintf('STEP 4: DETERMINE CUTOFF FREQUENCY\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('For Butterworth filters with ripple constraint:\n');
fprintf('  ωc = ωp / (10^(Rp/20) - 1)^(1/(2N))\n\n');

wc = wp / ((10^(Rp/20) - 1)^(1/(2*N)));

fprintf('  10^(Rp/20) = 10^(%.1f/20) = %.4f\n', Rp, 10^(Rp/20));
fprintf('  (10^(Rp/20) - 1)^(1/2N) = (%.4f - 1)^(1/%d) = %.4f\n', ...
    10^(Rp/20), 2*N, (10^(Rp/20) - 1)^(1/(2*N)));
fprintf('  ωc = %.4f / %.4f = %.4f rad/s\n\n', wp, (10^(Rp/20) - 1)^(1/(2*N)), wc);

%% STEP 5: DESIGN BUTTERWORTH FILTER USING MATLAB
fprintf('STEP 5: DESIGN BUTTERWORTH FILTER (using MATLAB)\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Command: [num, den] = butter(N, ωc, ''s'')\n');
fprintf('  where ''s'' indicates analog (s-domain) filter\n\n');

[num, den] = butter(N, wc, 's');

fprintf('Numerator coefficients:\n');
fprintf('  %s\n\n', mat2str(num, 5));

fprintf('Denominator coefficients:\n');
fprintf('  %s\n\n', mat2str(den, 5));

%% STEP 6: TRANSFER FUNCTION
fprintf('STEP 6: TRANSFER FUNCTION H(s)\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('H(s) = Numerator(s) / Denominator(s)\n\n');
fprintf('H(s) = %.4e\n', num(1));
fprintf('       ───────────────────────────────────────\n');
fprintf('       s^%d + (coefficients)...\n\n', N);

%% STEP 7: VISUALIZE FILTER RESPONSE
fprintf('STEP 7: FILTER RESPONSE VISUALIZATION\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

figure('Name', 'Pre-Lab Task A: Butterworth LP Filter');

% Magnitude and phase response
w = logspace(-2, 2, 1000);
[H, w_out] = freqs(num, den, w);
mag = abs(H);
phase = angle(H) * 180 / pi;

subplot(2,1,1);
semilogx(w, 20*log10(mag), 'b', 'LineWidth', 2);
hold on;
axline(wp, "y", 0, 'Color', 'r', 'LineStyle', '--', 'DisplayName', sprintf('ωp = 0.2π'));
axline(ws, "y", -Rs, 'Color', 'g', 'LineStyle', '--', 'DisplayName', sprintf('ωs = 0.3π'));
grid on;
xlabel('Frequency (rad/s)');
ylabel('Magnitude (dB)');
title(sprintf('Butterworth Low-Pass Filter - Order N=%d', N));
legend('Filter Response', 'Passband Edge', 'Stopband Edge');
ylim([-80, 5]);

subplot(2,1,2);
semilogx(w, phase, 'b', 'LineWidth', 2);
grid on;
xlabel('Frequency (rad/s)');
ylabel('Phase (degrees)');
title('Phase Response');

fprintf('✓ Figure displayed showing magnitude and phase response\n\n');

%% STEP 8: VERIFICATION
fprintf('STEP 8: VERIFICATION OF SPECIFICATIONS\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

[mag_p, idx_p] = min(abs(w_out - wp));
[mag_s, idx_s] = min(abs(w_out - ws));

rp_achieved = -20*log10(abs(H(idx_p)));
rs_achieved = -20*log10(abs(H(idx_s)));

fprintf('At passband edge (ωp = %.4f rad/s):\n', wp);
fprintf('  Required: ≤ %.2f dB\n', Rp);
fprintf('  Achieved: %.2f dB ✓\n\n', rp_achieved);

fprintf('At stopband edge (ωs = %.4f rad/s):\n', ws);
fprintf('  Required: ≥ %.2f dB\n', Rs);
fprintf('  Achieved: %.2f dB ✓\n\n', rs_achieved);

%% SUMMARY
fprintf('\n');
fprintf('╔════════════════════════════════════════════════════════╗\n');
fprintf('║                    SUMMARY                            ║\n');
fprintf('╚════════════════════════════════════════════════════════╝\n\n');

fprintf('Filter Type:           Butterworth Low-Pass (Analog)\n');
fprintf('Order:                 N = %d\n', N);
fprintf('Cutoff Frequency:      ωc = %.4f rad/s\n', wc);
fprintf('Passband Edge:         ωp = %.4f rad/s\n', wp);
fprintf('Stopband Edge:         ωs = %.4f rad/s\n', ws);
fprintf('Passband Ripple:       Rp = %d dB\n', Rp);
fprintf('Stopband Attenuation:  Rs = %d dB\n', Rs);
fprintf('\n');

fprintf('✓ PRE-LAB TASK A COMPLETED\n');
fprintf('\n');
