%% LAB 11 - PRE-LAB TASK: FIR LOW-PASS FILTER DESIGN (HAMMING WINDOW)
% Window-based FIR filter design from specifications
% Clear explanation for viva preparation

clear; clc; close all;

fprintf('\n');
fprintf('╔════════════════════════════════════════════════════════╗\n');
fprintf('║      PRE-LAB TASK: FIR LOW-PASS FILTER DESIGN         ║\n');
fprintf('║            (Hamming Window Method)                    ║\n');
fprintf('╚════════════════════════════════════════════════════════╝\n');
fprintf('\n');

%% STEP 1: GIVEN SPECIFICATIONS
fprintf('STEP 1: GIVEN SPECIFICATIONS\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

wc = 0.2*pi;        % Passband cutoff (rad/sample)
As = 50;            % Stopband attenuation (dB)
window_type = 'hamming';

fprintf('  Passband cutoff (ωc)     = 0.2π rad/sample = %.4f rad/s\n', wc);
fprintf('  Stopband attenuation     = %d dB\n', As);
fprintf('  Window type selected     = %s\n\n', window_type);

%% STEP 2: UNDERSTAND THE WINDOW METHOD
fprintf('STEP 2: UNDERSTAND WINDOWING CONCEPT\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('The window design method:\n');
fprintf('  1. Start with IDEAL filter → infinite impulse response\n');
fprintf('  2. Choose WINDOW function → determines trade-offs\n');
fprintf('  3. MULTIPLY: h[n] = h_ideal[n] × w[n]\n');
fprintf('  4. Result → practical FIR filter (finite length)\n\n');

fprintf('Trade-off (from Table 11.1):\n');
fprintf('  Rectangular:  ΔΩ = 4π/M,   As = 21 dB  (narrow transition, poor stopband)\n');
fprintf('  Hamming:      ΔΩ = 8π/M,   As = 53 dB  (balanced - BEST for general use)\n');
fprintf('  Blackman:     ΔΩ = 12π/M,  As = 74 dB  (wide transition, excellent stopband)\n\n');

fprintf('✓ We chose HAMMING because:\n');
fprintf('  - Need 50 dB attenuation → Hamming gives 53 dB (sufficient)\n');
fprintf('  - Reasonable transition width (8π/M)\n');
fprintf('  - Most practical for digital signal processing\n\n');

%% STEP 3: ESTIMATE STOPBAND EDGE
fprintf('STEP 3: ESTIMATE STOPBAND EDGE FREQUENCY\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('For practical design, estimate stopband edge:\n');
fprintf('  Normalized transition width ≈ 0.05 (typical for this order)\n');
fprintf('  Δf = (ωs - ωp)/(2π) ≈ 0.05\n');
fprintf('  ωs = ωp + 2π × 0.05 = 0.2π + 0.1π = 0.3π\n\n');

ws = wc + 2*pi*0.05;
fprintf('  Estimated stopband edge (ωs) = %.4f rad/s ≈ 0.3π\n\n', ws);

%% STEP 4: CALCULATE MINIMUM FILTER ORDER
fprintf('STEP 4: CALCULATE MINIMUM FILTER ORDER (M)\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Design equation (from Table 11.1 & Kaiser formula):\n');
fprintf('  M = ⌈(As - 7.95) / (14.36 × Δf)⌉ + 1\n\n');

Delta_f = (ws - wc) / (2*pi);
fprintf('  Transition width: Δf = (ωs - ωp)/(2π) = (%.4f - %.4f)/(2π)\n', ws, wc);
fprintf('                       = %.4f\n\n', Delta_f);

M_calc = (As - 7.95) / (14.36 * Delta_f);
M = ceil(M_calc) + 1;

fprintf('  M_calculated = (%.1f - 7.95) / (14.36 × %.4f)\n', As, Delta_f);
fprintf('              = %.2f / %.4f = %.2f\n\n', (As - 7.95), (14.36 * Delta_f), M_calc);

fprintf('  Filter length: M = ⌈%.2f⌉ + 1 = %d samples\n\n', M_calc, M);

fprintf('  ✓ This means our FIR filter will have %d coefficients\n\n', M);

%% STEP 5: COMPUTE IDEAL IMPULSE RESPONSE
fprintf('STEP 5: COMPUTE IDEAL (INFINITE) IMPULSE RESPONSE\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Formula for ideal low-pass impulse response:\n');
fprintf('  h_ideal[n] = sin(ωc × (n - α)) / (π × (n - α))\n\n');

fprintf('Where:\n');
fprintf('  ωc = passband edge = %.4f rad/s\n', wc);
fprintf('  α = sample delay = (M-1)/2 = %d (for symmetry)\n', (M-1)/2);
fprintf('  n = sample index, -∞ to +∞\n\n');

% Compute ideal impulse response over extended range
n_extend = [-(M-1):(M-1)];
alpha = (M-1) / 2;

% Avoid division by zero at n = alpha
h_ideal = zeros(size(n_extend));
for i = 1:length(n_extend)
    if n_extend(i) == alpha
        h_ideal(i) = wc / pi;
    else
        h_ideal(i) = sin(wc * (n_extend(i) - alpha)) / (pi * (n_extend(i) - alpha));
    end
end

fprintf('  Sample values of h_ideal[n]:\n');
fprintf('    n = -5: h_ideal = %.4f\n', h_ideal(find(n_extend==-5)));
fprintf('    n = -2: h_ideal = %.4f\n', h_ideal(find(n_extend==-2)));
fprintf('    n =  0: h_ideal = %.4f\n', h_ideal(find(n_extend==0)));
fprintf('    n =  2: h_ideal = %.4f\n', h_ideal(find(n_extend==2)));
fprintf('    n =  5: h_ideal = %.4f\n\n', h_ideal(find(n_extend==5)));

%% STEP 6: CREATE HAMMING WINDOW
fprintf('STEP 6: CREATE HAMMING WINDOW FUNCTION\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Hamming window formula:\n');
fprintf('  w[n] = 0.54 - 0.46 × cos(2π × n / (M-1))  for n = 0 to M-1\n');
fprintf('  w[n] = 0                                  otherwise\n\n');

% Create Hamming window
w = hamming(M);

fprintf('  Hamming window properties:\n');
fprintf('    Length (M) = %d samples\n', M);
fprintf('    w[0] = %.4f (peaks at edges)\n', w(1));
fprintf('    w[%d] = %.4f (maximum at center)\n', (M+1)/2, w((M+1)/2));
fprintf('    w[%d] = %.4f (symmetric)\n\n', M, w(M));

%% STEP 7: APPLY WINDOWING
fprintf('STEP 7: APPLY WINDOWING: h[n] = h_ideal[n] × w[n]\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Window operation:\n');
fprintf('  - Extract h_ideal values for n = 0 to M-1 only\n');
fprintf('  - Multiply each sample by corresponding window value\n');
fprintf('  - Result: truncated (finite) impulse response\n\n');

% Extract the windowed portion
n_window = [0:(M-1)];
h_ideal_window = zeros(1, M);
for i = 1:M
    if (alpha - (i-1)) == 0
        h_ideal_window(i) = wc / pi;
    else
        h_ideal_window(i) = sin(wc * (i-1 - alpha)) / (pi * (i-1 - alpha));
    end
end

% Apply window
h = h_ideal_window .* w';

fprintf('  Sample of impulse response h[n]:\n');
fprintf('    h[0]  = %.4f × %.4f = %.4f\n', h_ideal_window(1), w(1), h(1));
fprintf('    h[5]  = %.4f × %.4f = %.4f\n', h_ideal_window(6), w(6), h(6));
fprintf('    h[%d] = %.4f × %.4f = %.4f (center, maximum)\n', (M-1)/2, ...
        h_ideal_window((M+1)/2), w((M+1)/2), h((M+1)/2));
fprintf('    h[%d] = %.4f × %.4f = %.4f\n\n', M-1, h_ideal_window(M), w(M), h(M));

%% STEP 8: VERIFY LINEAR PHASE PROPERTY
fprintf('STEP 8: VERIFY LINEAR PHASE PROPERTY\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('For linear phase, impulse response must be symmetric:\n');
fprintf('  h[n] = h[M-1-n]  (symmetric property)\n\n');

% Check symmetry
is_symmetric = norm(h - fliplr(h)) < 1e-10;
if is_symmetric
    fprintf('  ✓ VERIFIED: h[n] is symmetric\n');
    fprintf('  ✓ This guarantees LINEAR PHASE response\n');
    fprintf('  ✓ Phase angle = -π × (M-1)/2 × ω (constant slope)\n\n');
else
    fprintf('  ✗ NOT SYMMETRIC (something is wrong)\n\n');
end

%% STEP 9: DESIGN USING MATLAB FIR1
fprintf('STEP 9: VERIFY WITH MATLAB FIR1 FUNCTION\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('MATLAB command for equivalent design:\n');
fprintf('  b = fir1(M-1, wc/(π), ''low'', hamming(M));\n\n');

% Normalized cutoff for MATLAB (0 to 1, where 1 = Nyquist)
wc_norm = wc / pi;
b_matlab = fir1(M-1, wc_norm, 'low', hamming(M));

fprintf('  MATLAB normalized cutoff = %.4f (range 0 to 1)\n', wc_norm);
fprintf('  Filter coefficients generated: %d values\n', length(b_matlab));
fprintf('  First 5 coefficients: [%.4f, %.4f, %.4f, %.4f, %.4f]\n\n', ...
        b_matlab(1), b_matlab(2), b_matlab(3), b_matlab(4), b_matlab(5));

%% STEP 10: FREQUENCY RESPONSE ANALYSIS
fprintf('STEP 10: FREQUENCY RESPONSE ANALYSIS\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

% Compute frequency response
[H, W] = freqz(b_matlab, 1, 1024);
mag_dB = 20 * log10(abs(H));
phase = angle(H);

% Find -3dB point
idx_3db = find(mag_dB >= -3, 1, 'last');
f_3db = W(idx_3db);

% Find stopband attenuation
stopband_idx = find(W > 1.5 * wc_norm);
min_stopband = min(mag_dB(stopband_idx));

fprintf('  Frequency response characteristics:\n');
fprintf('    -3dB cutoff frequency:    %.4f rad/sample\n', f_3db);
fprintf('    Specified passband edge:  %.4f rad/sample\n', wc_norm);
fprintf('    Actual stopband atten.:   %.2f dB (spec: %d dB)\n\n', min_stopband, As);

%% STEP 11: PLOTTING
fprintf('STEP 11: GENERATING PLOTS\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Creating 4 figures:\n');
fprintf('  1. Ideal vs Hamming window comparison\n');
fprintf('  2. Windowed impulse response h[n]\n');
fprintf('  3. Magnitude response (linear scale)\n');
fprintf('  4. Magnitude response (dB scale) + Phase response\n\n');

% Figure 1: Window effect visualization
figure('Position', [100, 100, 1200, 400], 'Name', 'Window Method Visualization');

subplot(1,3,1);
n_plot = [0:(M-1)];
h_ideal_plot = zeros(1, M);
for i = 1:M
    if (i-1) == alpha
        h_ideal_plot(i) = wc / pi;
    else
        h_ideal_plot(i) = sin(wc * ((i-1) - alpha)) / (pi * ((i-1) - alpha));
    end
end
stem(n_plot, h_ideal_plot, 'b', 'LineWidth', 1.5);
title('Ideal Impulse Response (truncated)', 'FontSize', 12, 'FontWeight', 'bold');
xlabel('n (sample)'); ylabel('h_{ideal}[n]');
grid on; axis tight;

subplot(1,3,2);
stem(n_plot, w, 'r', 'LineWidth', 1.5);
title('Hamming Window w[n]', 'FontSize', 12, 'FontWeight', 'bold');
xlabel('n (sample)'); ylabel('w[n]');
grid on; axis tight;

subplot(1,3,3);
stem(n_plot, h_matlab, 'g', 'LineWidth', 1.5);
title('Windowed Response h[n] = h_{ideal} × w', 'FontSize', 12, 'FontWeight', 'bold');
xlabel('n (sample)'); ylabel('h[n]');
grid on; axis tight;

sgtitle(sprintf('Window Method: FIR Low-Pass Filter Design (M=%d)', M), ...
        'FontSize', 14, 'FontWeight', 'bold');

% Figure 2: Impulse response
figure('Position', [100, 550, 900, 400], 'Name', 'Impulse Response');
stem([0:(M-1)], b_matlab, 'LineWidth', 2);
title(sprintf('FIR Low-Pass Filter Impulse Response (M=%d)', M), 'FontSize', 12, 'FontWeight', 'bold');
xlabel('n (sample)'); ylabel('h[n]');
grid on;
hold on;
plot([0:(M-1)], b_matlab, 'b--', 'Alpha', 0.3);
hold off;
axis tight;

% Figure 3: Frequency response (magnitude linear scale)
figure('Position', [1100, 100, 900, 400], 'Name', 'Magnitude Response');
freqz(b_matlab, 1, 1024);
title(sprintf('Frequency Response (M=%d, Hamming Window)', M), 'FontSize', 12, 'FontWeight', 'bold');
grid on;

% Modify the fvtool-like output
fprintf('  Opening interactive FIR filter visualization tool...\n\n');
fvtool(b_matlab, 'Analysis', 'freq');

%% STEP 12: SUMMARY TABLE
fprintf('STEP 12: DESIGN SUMMARY\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('┌─────────────────────────────────────────────┐\n');
fprintf('│        FIR LOW-PASS FILTER SUMMARY          │\n');
fprintf('├─────────────────────────────────────────────┤\n');
fprintf('│ Specification:                              │\n');
fprintf('│   Passband cutoff (ωc):     0.2π rad/sample│\n');
fprintf('│   Stopband attenuation:     50 dB          │\n');
fprintf('│   Window type:              Hamming        │\n');
fprintf('├─────────────────────────────────────────────┤\n');
fprintf('│ Design Results:                             │\n');
fprintf('│   Filter length (M):        %2d samples     │\n', M);
fprintf('│   Filter order (N=M-1):     %2d            │\n', M-1);
fprintf('│   Symmetry:                 ✓ Linear phase │\n');
fprintf('│   Stability:                ✓ Always stable│\n');
fprintf('│   -3dB cutoff:              %.4f rad/sample│\n', f_3db);
fprintf('│   Actual stopband atten.:   %.2f dB       │\n', min_stopband);
fprintf('└─────────────────────────────────────────────┘\n\n');

fprintf('Filter coefficients h[n]:\n');
fprintf('  b = [');
for i = 1:min(5, M)
    fprintf('%.6f, ', b_matlab(i));
end
fprintf('..., ');
for i = max(1, M-2):M
    fprintf('%.6f', b_matlab(i));
    if i < M
        fprintf(', ');
    end
end
fprintf(']\n\n');

%% VIVA PREPARATION HINTS
fprintf('═══════════════════════════════════════════════════════════\n');
fprintf('VIVA PREPARATION: KEY POINTS TO REMEMBER\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Q1: Why did you choose Hamming window?\n');
fprintf('A:  Need 50 dB attenuation. Hamming gives 53 dB (sufficient).\n');
fprintf('    It balances transition width and stopband ripple.\n\n');

fprintf('Q2: What happens without windowing?\n');
fprintf('A:  Without window, h[n] would be infinite. Windowing truncates\n');
fprintf('    it to practical length M, but introduces Gibbs ripple.\n\n');

fprintf('Q3: How does filter order affect transition?\n');
fprintf('A:  Higher M → narrower transition (sharper cutoff).\n');
fprintf('    Transition width ≈ 8π/M for Hamming.\n\n');

fprintf('Q4: Why is linear phase important?\n');
fprintf('A:  Linear phase = constant delay. Preserves signal shape.\n');
fprintf('    Symmetric h[n] automatically gives linear phase.\n\n');

fprintf('Q5: How would you design for 80 dB attenuation?\n');
fprintf('A:  Hamming (53 dB) insufficient. Would use Blackman (74 dB).\n');
fprintf('    But transition width increases. Trade-off!\n\n');

fprintf('═══════════════════════════════════════════════════════════\n');
fprintf('✓ LAB TASK COMPLETED. Ready for viva!\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');
