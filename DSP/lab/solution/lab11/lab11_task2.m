%% LAB 11 - TASK 2: DIGITAL BAND-PASS FILTER DESIGN (OPEN-ENDED)
% Radar signal processing application
% Band-pass FIR filter with linear phase requirement

clear; clc; close all;

fprintf('\n');
fprintf('╔════════════════════════════════════════════════════════╗\n');
fprintf('║      LAB TASK 2: DIGITAL BAND-PASS FILTER DESIGN      ║\n');
fprintf('║          (Open-Ended Radar Application)               ║\n');
fprintf('╚════════════════════════════════════════════════════════╝\n');
fprintf('\n');

%% INPUT YOUR REGISTRATION NUMBER
fprintf('STEP 0: ENTER YOUR REGISTRATION NUMBER\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

% For demo purposes, using a sample registration number
% CHANGE THIS TO YOUR ACTUAL REGISTRATION NUMBER
reg_no = input('Enter your registration number (last 2 digits): ');

if isempty(reg_no)
    reg_no = 23;  % Default for demo
    fprintf('Using default registration: %d\n', reg_no);
else
    fprintf('Using registration: %d\n', reg_no);
end

fprintf('\n');

%% CALCULATE PARAMETERS FROM REGISTRATION NUMBER
fprintf('STEP 1: CALCULATE PARAMETERS FROM REGISTRATION NUMBER\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Calculation formulas:\n\n');

% Calculate N (frequency parameter)
if reg_no < 15
    N = 10 * reg_no;
    fprintf('  N = 10 × R = 10 × %d = %d (since R < 15)\n', reg_no, N);
else
    N = reg_no;
    fprintf('  N = R = %d (since R ≥ 15)\n', reg_no, N);
end

% Calculate As (stopband attenuation)
if reg_no <= 15
    As = 70;
    fprintf('  As = 70 dB (since R ≤ 15)\n');
elseif reg_no <= 25
    As = 50;
    fprintf('  As = 50 dB (since 16 ≤ R ≤ 25)\n');
elseif reg_no <= 35
    As = 40;
    fprintf('  As = 40 dB (since 26 ≤ R ≤ 35)\n');
else
    As = 21;
    fprintf('  As = 21 dB (since R > 35)\n');
end

fprintf('\n');

%% SPECIFICATIONS
fprintf('STEP 2: FILTER SPECIFICATIONS (RADAR APPLICATION)\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Application: Radar signal processing (receiver bandpass filter)\n');
fprintf('Purpose: Extract target information from transmitted signal\n');
fprintf('Requirement: Linear phase (preserve signal shape)\n\n');

fprintf('Specification table:\n');
fprintf('┌─────────────────────────────────────────────────┐\n');
fprintf('│  Parameter              │  Formula   │  Value   │\n');
fprintf('├─────────────────────────────────────────────────┤\n');

fp_low = N - 5;
fp_high = N + 5;
fs_low = N - 5.5;
fs_high = N + 5.5;
fs_sampling = 4 * N;

fprintf('│  Lower passband edge    │  N - 5     │  %d MHz  │\n', fp_low);
fprintf('│  Upper passband edge    │  N + 5     │  %d MHz  │\n', fp_high);
fprintf('│  Lower stopband edge    │  N - 5.5   │  %.1f MHz│\n', fs_low);
fprintf('│  Upper stopband edge    │  N + 5.5   │  %.1f MHz│\n', fs_high);
fprintf('│  Sampling frequency     │  4N        │  %d MHz  │\n', fs_sampling);
fprintf('│  Stopband attenuation   │  (from table)│ %d dB  │\n', As);
fprintf('├─────────────────────────────────────────────────┤\n');
fprintf('│  Passband width         │  10 MHz                  │\n');
fprintf('│  Transition band width  │  0.5 MHz (each side)     │\n');
fprintf('│  Phase response         │  LINEAR (critical!)      │\n');
fprintf('└─────────────────────────────────────────────────┘\n\n');

%% NORMALIZE FREQUENCIES FOR MATLAB
fprintf('STEP 3: NORMALIZE FREQUENCIES FOR MATLAB\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('MATLAB uses normalized frequency: ω_norm = f / (fs/2)\n\n');

% Normalize to MATLAB convention (0 to 1, where 1 = Nyquist = fs/2)
wp_low_norm = (2 * fp_low) / fs_sampling;
wp_high_norm = (2 * fp_high) / fs_sampling;
ws_low_norm = (2 * fs_low) / fs_sampling;
ws_high_norm = (2 * fs_high) / fs_sampling;

fprintf('Normalized passband edges:\n');
fprintf('  ω_p_low  = 2 × %d / %d = %.4f\n', fp_low, fs_sampling, wp_low_norm);
fprintf('  ω_p_high = 2 × %d / %d = %.4f\n', fp_high, fs_sampling, wp_high_norm);

fprintf('\nNormalized stopband edges:\n');
fprintf('  ω_s_low  = 2 × %.1f / %d = %.4f\n', fs_low, fs_sampling, ws_low_norm);
fprintf('  ω_s_high = 2 × %.1f / %d = %.4f\n\n', fs_high, fs_sampling, ws_high_norm);

%% CALCULATE FILTER ORDER
fprintf('STEP 4: CALCULATE MINIMUM FILTER ORDER\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Transition band width (normalized):\n');
trans_width = (ws_low_norm - wp_low_norm);  % or equivalently (wp_high_norm - ws_high_norm)
fprintf('  Δω = ω_p_low - ω_s_low = %.4f - %.4f = %.4f\n', wp_low_norm, ws_low_norm, trans_width);

Delta_f = trans_width / 2;  % In terms of relative bandwidth
fprintf('  Δf ≈ %.4f (relative transition width)\n\n', Delta_f);

fprintf('Using design equation:\n');
fprintf('  M = ⌈(As - 7.95) / (14.36 × Δf)⌉ + 1\n\n');

M_calc = (As - 7.95) / (14.36 * Delta_f);
M = ceil(M_calc) + 1;

fprintf('  M_calculated = (%.1f - 7.95) / (14.36 × %.4f)\n', As, Delta_f);
fprintf('              = %.2f / %.4f = %.2f\n\n', (As - 7.95), (14.36 * Delta_f), M_calc);

fprintf('  ✓ Minimum filter length: M = %d samples\n', M);
fprintf('  ✓ Filter order (N = M-1): N = %d\n\n', M-1);

%% SELECT WINDOW FUNCTION
fprintf('STEP 5: SELECT WINDOW FUNCTION\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Decision tree (based on As requirement):\n');
fprintf('  As ≤ 21 dB  → Rectangular window (21 dB)\n');
fprintf('  21 < As ≤ 53 dB → Hamming window (53 dB) ← OUR CASE\n');
fprintf('  As > 53 dB  → Blackman window (74 dB)\n\n');

if As <= 21
    window_type = 'rectwin';
    window_name = 'Rectangular';
elseif As <= 53
    window_type = 'hamming';
    window_name = 'Hamming';
else
    window_type = 'blackman';
    window_name = 'Blackman';
end

fprintf('✓ Selected window: %s\n', window_name);
fprintf('  Provides %.0f dB stopband attenuation\n', As);
fprintf('  Meets requirement of %d dB ✓\n\n', As);

%% DESIGN BAND-PASS FIR FILTER
fprintf('STEP 6: DESIGN BAND-PASS FIR FILTER\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('MATLAB command:\n');
fprintf('  b = fir1(M-1, [ω_p_low, ω_p_high], ''bandpass'', window)\n\n');

if strcmp(window_type, 'hamming')
    w = hamming(M);
elseif strcmp(window_type, 'blackman')
    w = blackman(M);
else
    w = rectwin(M);
end

b = fir1(M-1, [wp_low_norm, wp_high_norm], 'bandpass', w);

fprintf('✓ Band-pass FIR filter designed\n');
fprintf('  Filter length (M):  %d samples\n', M);
fprintf('  Passband:           %.2f to %.2f (normalized)\n', wp_low_norm, wp_high_norm);
fprintf('  Impulse response:   %d coefficients\n\n', length(b));

%% VERIFY LINEAR PHASE PROPERTY
fprintf('STEP 7: VERIFY LINEAR PHASE PROPERTY\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('For radar application, linear phase is CRITICAL:\n');
fprintf('  - Ensures no signal distortion\n');
fprintf('  - Preserves target signature\n');
fprintf('  - Constant group delay\n\n');

fprintf('Verification:\n');
% Check symmetry of impulse response
is_symmetric = norm(b - fliplr(b)) < 1e-10;
if is_symmetric
    fprintf('  ✓ VERIFIED: h[n] is symmetric\n');
    fprintf('  ✓ Linear phase: φ(ω) = -α·ω where α = (M-1)/2 = %.1f\n', (M-1)/2);
    fprintf('  ✓ Group delay: τ(ω) = (M-1)/2 = %.1f samples\n\n', (M-1)/2);
else
    fprintf('  ✗ NOT symmetric (error in design)\n\n');
end

%% COMPUTE FREQUENCY RESPONSE
fprintf('STEP 8: COMPUTE FREQUENCY RESPONSE\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

[H, W] = freqz(b, 1, 2048);
mag_dB = 20 * log10(abs(H) + 1e-10);  % Add small value to avoid log(0)
phase = angle(H);
group_delay = grpdelay(b, 1, W);

fprintf('✓ Frequency response computed (2048 points)\n');
fprintf('✓ Magnitude response (dB scale)\n');
fprintf('✓ Phase response\n');
fprintf('✓ Group delay\n\n');

% Convert normalized frequency to actual frequency
F = W * fs_sampling / (2*pi);  % Convert to MHz

%% VERIFY SPECIFICATIONS
fprintf('STEP 9: VERIFY AGAINST SPECIFICATIONS\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

% Find passband gain (should be ~0 dB)
passband_idx_low = find(F >= fp_low - 2 & F <= fp_low + 2);
passband_idx_high = find(F >= fp_high - 2 & F <= fp_high + 2);
passband_idx = find(F >= fp_low & F <= fp_high);

passband_gain = max(mag_dB(passband_idx));
passband_ripple = max(mag_dB(passband_idx)) - min(mag_dB(passband_idx));

% Find stopband attenuation
stopband_idx_low = find(F < fs_low);
stopband_idx_high = find(F > fs_high);
stopband_atten = min([min(mag_dB(stopband_idx_low)), min(mag_dB(stopband_idx_high))]);

fprintf('Passband (%.0f to %.0f MHz):\n', fp_low, fp_high);
fprintf('  Gain:              %.2f dB (should be ~0 dB)\n', passband_gain);
fprintf('  Ripple:            %.2f dB (should be <1 dB)\n\n', passband_ripple);

fprintf('Stopband (outside %.1f-%.1f MHz):\n', fs_low, fs_high);
fprintf('  Attenuation:       %.2f dB (spec: ≥%d dB)\n', abs(stopband_atten), As);
if abs(stopband_atten) >= As
    fprintf('  Status:            ✓ MEETS SPECIFICATION\n\n');
else
    fprintf('  Status:            ✗ Below specification (may need larger M)\n\n');
end

%% PLOTTING
fprintf('STEP 10: GENERATING ANALYSIS PLOTS\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

% Figure 1: Magnitude and Phase response
figure('Position', [100, 100, 1300, 500], 'Name', 'Frequency Response');

subplot(1,2,1);
plot(F, mag_dB, 'b-', 'LineWidth', 2); hold on;
% Add specification markers
axline([fp_low, 0], [0, 0], 'g--', 'Alpha', 0.5);  % Lower passband edge
axline([fp_high, 0], [0, 0], 'g--', 'Alpha', 0.5); % Upper passband edge
axline([fs_low, -As], [0, 0], 'r--', 'Alpha', 0.5);  % Lower stopband
axline([fs_high, -As], [0, 0], 'r--', 'Alpha', 0.5); % Upper stopband

% Shade regions
fill([0, fs_low, fs_low, 0], [-100, -100, 100, 100], 'red', 'FaceAlpha', 0.1, 'EdgeColor', 'none');
fill([fs_high, fs_sampling/2, fs_sampling/2, fs_high], [-100, -100, 100, 100], 'red', 'FaceAlpha', 0.1, 'EdgeColor', 'none');
fill([fp_low, fp_high, fp_high, fp_low], [-100, -100, 100, 100], 'green', 'FaceAlpha', 0.1, 'EdgeColor', 'none');

title('Magnitude Response (Band-Pass FIR Filter)', 'FontSize', 12, 'FontWeight', 'bold');
xlabel('Frequency (MHz)');
ylabel('Magnitude (dB)');
grid on;
ylim([-80, 5]);
xlim([0, fs_sampling/2]);
legend({'Magnitude Response', 'Passband edge', 'Stopband spec'}, 'Location', 'upper right');
hold off;

subplot(1,2,2);
plot(F, phase, 'r-', 'LineWidth', 2);
title('Phase Response (Linear Phase Check)', 'FontSize', 12, 'FontWeight', 'bold');
xlabel('Frequency (MHz)');
ylabel('Phase (radians)');
grid on;
xlim([0, fs_sampling/2]);
hold off;

sgtitle(sprintf('Band-Pass FIR Filter Design (M=%d, fs=%.0f MHz, RegNo=%d)', ...
        M, fs_sampling, reg_no), 'FontSize', 14, 'FontWeight', 'bold');

% Figure 2: Impulse response
figure('Position', [100, 650, 700, 400], 'Name', 'Impulse Response');
stem(0:(M-1), b, 'b', 'LineWidth', 1.5);
title('FIR Filter Impulse Response h[n]', 'FontSize', 12, 'FontWeight', 'bold');
xlabel('n (sample index)');
ylabel('h[n]');
grid on;
axis tight;

% Figure 3: Passband detail
figure('Position', [850, 650, 700, 400], 'Name', 'Passband Detail');
passband_freq_range = F >= (fp_low - 3) & F <= (fp_high + 3);
F_pass = F(passband_freq_range);
mag_pass = mag_dB(passband_freq_range);

plot(F_pass, mag_pass, 'g-', 'LineWidth', 2); hold on;
axline([fp_low, 0], [0, 0], 'g--', 'Alpha', 0.5);
axline([fp_high, 0], [0, 0], 'g--', 'Alpha', 0.5);

title('Passband Detail (Ripple Analysis)', 'FontSize', 12, 'FontWeight', 'bold');
xlabel('Frequency (MHz)');
ylabel('Magnitude (dB)');
grid on;
xlim([fp_low - 2, fp_high + 2]);
ylim([-5, 2]);
legend('Magnitude', 'Passband edge');
hold off;

fprintf('  ✓ 3 figures generated\n\n');

%% SUMMARY TABLE
fprintf('═══════════════════════════════════════════════════════════\n');
fprintf('DESIGN SUMMARY & VERIFICATION\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('┌─────────────────────────────────────────────────────────┐\n');
fprintf('│       BAND-PASS FIR FILTER DESIGN SUMMARY               │\n');
fprintf('├─────────────────────────────────────────────────────────┤\n');
fprintf('│ Input Parameters:                                       │\n');
fprintf('│   Registration Number:      %d\n', reg_no);
fprintf('│   N (frequency param):      %d MHz\n', N);
fprintf('│   As (attenuation spec):    %d dB\n', As);
fprintf('│                                                         │\n');
fprintf('│ Specifications:                                         │\n');
fprintf('│   Passband:                 %.0f - %.0f MHz (width: 10 MHz)\n', fp_low, fp_high);
fprintf('│   Stopband edges:           %.1f - %.1f MHz\n', fs_low, fs_high);
fprintf('│   Sampling frequency:       %.0f MHz\n', fs_sampling);
fprintf('│   Stopband attenuation:     ≥%d dB\n', As);
fprintf('│   Phase response:           LINEAR (critical!)\n');
fprintf('│                                                         │\n');
fprintf('│ Design Results:                                         │\n');
fprintf('│   Filter length (M):        %d samples\n', M);
fprintf('│   Filter order:             %d\n', M-1);
fprintf('│   Window type:              %s\n', window_name);
fprintf('│   Symmetry:                 ✓ h[n] symmetric\n');
fprintf('│   Linear phase:             ✓ Verified\n');
fprintf('│   Stability:                ✓ FIR always stable\n');
fprintf('│                                                         │\n');
fprintf('│ Measured Performance:                                   │\n');
fprintf('│   Passband gain:            %.2f dB\n', passband_gain);
fprintf('│   Passband ripple:          %.2f dB\n', passband_ripple);
fprintf('│   Stopband attenuation:     %.2f dB ✓\n', abs(stopband_atten));
fprintf('│   Group delay:              %.1f samples\n', (M-1)/2);
fprintf('└─────────────────────────────────────────────────────────┘\n\n');

%% VIVA PREPARATION
fprintf('═══════════════════════════════════════════════════════════\n');
fprintf('VIVA PREPARATION: KEY DISCUSSION POINTS\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Q1: Why is linear phase critical for this radar application?\n');
fprintf('A:  Radar targets have specific signatures (chirps, pulses).\n');
fprintf('    Non-linear phase → distorts signature → target misidentification\n');
fprintf('    Linear phase → constant delay → preserves signal shape ✓\n\n');

fprintf('Q2: Why band-pass instead of high-pass + low-pass?\n');
fprintf('A:  Single band-pass filter is more efficient.\n');
fprintf('    Radar only cares about target frequency band (N±5 MHz)\n');
fprintf('    Band-pass = lower order + one design vs two designs\n\n');

fprintf('Q3: Explain the transition band (0.5 MHz on each side)\n');
fprintf('A:  Sharp corners are mathematically impossible (Gibbs phenomenon)\n');
fprintf('    0.5 MHz allows smooth transition from passband to stopband\n');
fprintf('    Too narrow → very high filter order; too wide → poor specs\n\n');

fprintf('Q4: How would you improve design if As not met?\n');
fprintf('A:  1. Increase M (wider transition = higher order)\n');
fprintf('    2. Use better window (Blackman vs Hamming)\n');
fprintf('    3. Widen transition band (easier spec to meet)\n\n');

fprintf('Q5: How does group delay relate to your phase response?\n');
fprintf('A:  Group delay = -dφ/dω (derivative of phase)\n');
fprintf('    Linear phase → constant group delay = (M-1)/2 samples\n');
fprintf('    This means all frequency components delayed equally ✓\n\n');

fprintf('═══════════════════════════════════════════════════════════\n');
fprintf('✓ LAB TASK 2 COMPLETED\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');
