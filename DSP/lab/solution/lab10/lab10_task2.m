%% LAB 10 - LAB TASK 2: ELLIPTIC (CAUER) BAND-PASS FILTER
% Complete solution with detailed explanations for viva
% NOTE: Replace REG_NO with your actual registration number

clear; clc; close all;

fprintf('\n');
fprintf('╔════════════════════════════════════════════════════════╗\n');
fprintf('║    LAB TASK 2: ELLIPTIC (CAUER) BAND-PASS FILTER     ║\n');
fprintf('╚════════════════════════════════════════════════════════╝\n');
fprintf('\n');

%% CONFIGURATION: SET YOUR REGISTRATION NUMBER HERE
REG_NO = 15;  % ← CHANGE THIS TO YOUR ACTUAL REGISTRATION NUMBER (e.g., 12, 15, 20, etc.)

%% STEP 1: CALCULATE FILTER PARAMETER N FROM REGISTRATION NUMBER
fprintf('STEP 1: CALCULATE FILTER PARAMETER N\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Registration Number Formula:\n');
fprintf('  if RegNo < 15:   N = 10 × RegNo\n');
fprintf('  if RegNo ≥ 15:   N = RegNo\n\n');

if REG_NO < 15
    N = 10 * REG_NO;
    fprintf('Since RegNo (%d) < 15:\n', REG_NO);
    fprintf('  N = 10 × %d = %d\n\n', REG_NO, N);
else
    N = REG_NO;
    fprintf('Since RegNo (%d) ≥ 15:\n', REG_NO);
    fprintf('  N = %d\n\n', REG_NO);
end

fprintf('Filter Parameter:          N = %d\n\n', N);

%% STEP 2: FREQUENCY SPECIFICATIONS FROM N
fprintf('STEP 2: FREQUENCY SPECIFICATIONS (Parameterized by N)\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

% Passband: (N-5) to (N+5) MHz
% Stopband: (N-5.5) to (N+5.5) MHz
fp_low = N - 5;         % Passband lower edge (MHz)
fp_high = N + 5;        % Passband upper edge (MHz)
fs_low = N - 5.5;       % Stopband lower edge (MHz)
fs_high = N + 5.5;      % Stopband upper edge (MHz)
fs_samp = 8*N;          % Sampling frequency (MHz)
fs_samp_hz = fs_samp * 1e6;  % In Hz

fprintf('Passband edges:            %d MHz to %d MHz\n', fp_low, fp_high);
fprintf('Stopband edges:            %.1f MHz to %.1f MHz\n', fs_low, fs_high);
fprintf('Sampling frequency:        %d MHz = %d Hz\n', fs_samp, fs_samp_hz);
fprintf('Nyquist frequency:         %d MHz = %d Hz\n\n', fs_samp/2, fs_samp_hz/2);

% Convert to Hz for digital filter design
fp_low_hz = fp_low * 1e6;
fp_high_hz = fp_high * 1e6;
fs_low_hz = fs_low * 1e6;
fs_high_hz = fs_high * 1e6;

%% STEP 3: FILTER SPECIFICATIONS
fprintf('STEP 3: FILTER SPECIFICATIONS\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

Rp = 0.5;               % Passband ripple (dB)
Rs = 30;                % Stopband attenuation (dB)

fprintf('Passband ripple:           Rp = %.2f dB\n', Rp);
fprintf('Stopband attenuation:      Rs = %d dB\n\n', Rs);

fprintf('Why Elliptic Filter?\n');
fprintf('  • Sharpest transition between passband and stopband\n');
fprintf('  • Ripple in BOTH passband and stopband\n');
fprintf('  • Lowest filter order for given specifications\n');
fprintf('  • Optimal for band-pass applications\n\n');

%% STEP 4: NORMALIZE FREQUENCIES
fprintf('STEP 4: NORMALIZE FREQUENCIES TO NYQUIST\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

Wn_p_low = fp_low_hz / (fs_samp_hz/2);
Wn_p_high = fp_high_hz / (fs_samp_hz/2);
Wn_s_low = fs_low_hz / (fs_samp_hz/2);
Wn_s_high = fs_high_hz / (fs_samp_hz/2);

fprintf('Normalized frequencies:\n');
fprintf('  Passband lower:    Ωp_low = %.6f\n', Wn_p_low);
fprintf('  Passband upper:    Ωp_high = %.6f\n', Wn_p_high);
fprintf('  Stopband lower:    Ωs_low = %.6f\n', Wn_s_low);
fprintf('  Stopband upper:    Ωs_high = %.6f\n\n', Wn_s_high);

%% STEP 5: DETERMINE FILTER ORDER USING ELLIPORD
fprintf('STEP 5: DETERMINE MINIMUM FILTER ORDER\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('MATLAB command: [N_min, Wn_opt] = ellipord(Wp, Ws, Rp, Rs)\n');
fprintf('  where Wp = passband edge frequency\n');
fprintf('        Ws = stopband edge frequency\n');
fprintf('        Rp = passband ripple (dB)\n');
fprintf('        Rs = stopband attenuation (dB)\n\n');

% For band-pass filter
Wp = [Wn_p_low, Wn_p_high];
Ws = [Wn_s_low, Wn_s_high];

[N_min, Wn_opt] = ellipord(Wp, Ws, Rp, Rs);

fprintf('Minimum filter order required: N_min = %d\n', N_min);
fprintf('Optimal cutoff frequencies: [%.6f, %.6f]\n\n', Wn_opt(1), Wn_opt(2));

%% STEP 6: DESIGN ELLIPTIC FILTER
fprintf('STEP 6: DESIGN ELLIPTIC (CAUER) FILTER\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('MATLAB command: [b, a] = ellip(N, Rp, Rs, Wn, ''bandpass'')\n');
fprintf('  where N = filter order\n');
fprintf('        Rp = passband ripple (dB)\n');
fprintf('        Rs = stopband attenuation (dB)\n');
fprintf('        Wn = [low_cutoff, high_cutoff]\n');
fprintf('        ''bandpass'' = filter type\n\n');

[num_elliptic, den_elliptic] = ellip(N_min, Rp, Rs, Wn_opt, 'bandpass');

fprintf('Elliptic filter numerator length: %d\n', length(num_elliptic));
fprintf('Elliptic filter denominator length: %d\n\n', length(den_elliptic));

%% STEP 7: VERIFY STABILITY
fprintf('STEP 7: STABILITY VERIFICATION\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

poles = roots(den_elliptic);
zeros = roots(num_elliptic);

% Check if all poles are inside unit circle
poles_inside = all(abs(poles) < 1);

fprintf('Number of poles:  %d\n', length(poles));
fprintf('Number of zeros:  %d\n', length(zeros));
fprintf('All poles inside unit circle (|pole| < 1): %s\n\n', ...
    char(string(poles_inside)));

if poles_inside
    fprintf('✓ FILTER IS STABLE\n\n');
else
    fprintf('✗ WARNING: Filter is unstable!\n\n');
end

%% STEP 8: FREQUENCY RESPONSE
fprintf('STEP 8: FREQUENCY RESPONSE ANALYSIS\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

% Compute frequency response
[H, w] = freqz(num_elliptic, den_elliptic, 2048);
f_hz = w / (2*pi) * fs_samp_hz;  % Convert to Hz
mag_db = 20*log10(abs(H));

fprintf('Generating frequency response plots...\n\n');

figure('Name', sprintf('Lab Task 2: Elliptic BP Filter (N=%d)', N));

% Magnitude response
subplot(2,1,1);
plot(f_hz/1e6, mag_db, 'b', 'LineWidth', 2);
hold on;
axline(fp_low, "x", 'Color', 'r', 'LineStyle', '--', 'LineWidth', 1.5, ...
    'DisplayName', sprintf('fp_low = %d MHz', fp_low));
axline(fp_high, "x", 'Color', 'r', 'LineStyle', '--', 'LineWidth', 1.5, ...
    'DisplayName', sprintf('fp_high = %d MHz', fp_high));
axline(fs_low, "x", 'Color', 'g', 'LineStyle', '--', 'LineWidth', 1.5, ...
    'DisplayName', sprintf('fs_low = %.1f MHz', fs_low));
axline(fs_high, "x", 'Color', 'g', 'LineStyle', '--', 'LineWidth', 1.5, ...
    'DisplayName', sprintf('fs_high = %.1f MHz', fs_high));
grid on;
xlabel('Frequency (MHz)');
ylabel('Magnitude (dB)');
title(sprintf('Elliptic Band-Pass Filter (N=%d) - Magnitude Response', N));
legend('Filter', 'Location', 'southeast');
xlim([N-15, N+15]);
ylim([-80, 5]);

% Phase response
phase = angle(H) * 180 / pi;
subplot(2,1,2);
plot(f_hz/1e6, phase, 'b', 'LineWidth', 2);
grid on;
xlabel('Frequency (MHz)');
ylabel('Phase (degrees)');
title('Phase Response');
xlim([N-15, N+15]);

fprintf('✓ Magnitude and Phase response displayed\n\n');

%% STEP 9: POLE-ZERO PLOT
fprintf('STEP 9: POLE-ZERO PLOT\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

figure('Name', 'Pole-Zero Plot');
zplane(num_elliptic, den_elliptic);
title(sprintf('Pole-Zero Plot - Elliptic BP Filter (N=%d)', N));
grid on;

fprintf('✓ Pole-Zero plot displayed\n\n');

%% STEP 10: GROUP DELAY
fprintf('STEP 10: GROUP DELAY ANALYSIS\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

[gd, w_gd] = grpdelay(num_elliptic, den_elliptic, 1024);
f_gd_hz = w_gd / (2*pi) * fs_samp_hz;

figure('Name', 'Group Delay');
plot(f_gd_hz/1e6, gd, 'b', 'LineWidth', 2);
grid on;
xlabel('Frequency (MHz)');
ylabel('Group Delay (samples)');
title('Group Delay Response');
xlim([N-10, N+10]);

fprintf('✓ Group delay plotted\n\n');

%% STEP 11: FILTER CHARACTERISTICS SUMMARY
fprintf('STEP 11: FILTER CHARACTERISTICS SUMMARY\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Filter Configuration:\n');
fprintf('  Registration Number:       %d\n', REG_NO);
fprintf('  Filter Parameter N:        %d\n', N);
fprintf('  Sampling Frequency:        %d MHz (%d Hz)\n', fs_samp, fs_samp_hz);
fprintf('\n');

fprintf('Frequency Specifications:\n');
fprintf('  Passband:                  %d - %d MHz\n', fp_low, fp_high);
fprintf('  Stopband:                  %.1f - %.1f MHz\n', fs_low, fs_high);
fprintf('  Passband Ripple:           %.2f dB\n', Rp);
fprintf('  Stopband Attenuation:      %d dB\n', Rs);
fprintf('\n');

fprintf('Design Results:\n');
fprintf('  Filter Type:               Elliptic (Cauer) Band-Pass\n');
fprintf('  Minimum Order Required:    %d\n', N_min);
fprintf('  Number of Poles:           %d\n', length(poles));
fprintf('  Number of Zeros:           %d\n', length(zeros));
fprintf('  Stability:                 %s\n', ...
    char(string(poles_inside)));
fprintf('\n');

fprintf('Key Characteristics:\n');
fprintf('  ✓ Sharper transition than Butterworth or Chebyshev\n');
fprintf('  ✓ Equiripple in both passband and stopband\n');
fprintf('  ✓ Lowest order for given specifications\n');
fprintf('  ✓ Optimal for band-pass applications\n');
fprintf('  ✓ All poles inside unit circle (stable)\n');
fprintf('\n');

fprintf('╔════════════════════════════════════════════════════════╗\n');
fprintf('║            LAB TASK 2 COMPLETED                       ║\n');
fprintf('║       (Replace REG_NO with your actual number)        ║\n');
fprintf('╚════════════════════════════════════════════════════════╝\n\n');
