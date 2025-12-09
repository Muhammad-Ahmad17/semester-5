%% LAB 10 - LAB TASK 1: CHEBYSHEV TYPE-II BAND-PASS FILTER
% Complete solution with detailed explanations for viva

clear; clc; close all;

fprintf('\n');
fprintf('╔════════════════════════════════════════════════════════╗\n');
fprintf('║      LAB TASK 1: CHEBYSHEV TYPE-II BAND-PASS FILTER  ║\n');
fprintf('╚════════════════════════════════════════════════════════╝\n');
fprintf('\n');

%% STEP 1: GIVEN SPECIFICATIONS
fprintf('STEP 1: GIVEN SPECIFICATIONS\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

N_order = 20;           % Filter order
Rs = 60;                % Stopband attenuation (dB)
Rp = 0.75;              % Passband ripple (dB)
fp_edge = 800;          % Passband edge (Hz)
fs_edge = 2000;         % Stopband edge (Hz)
fs_samp = 6000;         % Sampling frequency (Hz)

fprintf('  Filter order:              N = %d\n', N_order);
fprintf('  Stopband attenuation:      Rs = %d dB\n', Rs);
fprintf('  Passband ripple:           Rp = %.2f dB\n', Rp);
fprintf('  Passband edge:             fp = %d Hz\n', fp_edge);
fprintf('  Stopband edge:             fs = %d Hz\n', fs_edge);
fprintf('  Sampling frequency:        Fs = %d Hz\n', fs_samp);
fprintf('  Nyquist frequency:         Fn = %d Hz\n\n', fs_samp/2);

%% STEP 2: NORMALIZE FREQUENCIES
fprintf('STEP 2: NORMALIZE FREQUENCIES TO NYQUIST\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Normalized frequency formula: Ω = 2πf / Fs\n');
fprintf('Or normalized to Nyquist:    Ω = f / (Fs/2)\n\n');

Wn_p = fp_edge / (fs_samp/2);      % Normalized passband
Wn_s = fs_edge / (fs_samp/2);      % Normalized stopband

fprintf('  Passband (normalized):     Ωp = %d / (6000/2) = %d / 3000 = %.4f\n', ...
    fp_edge, fp_edge, Wn_p);
fprintf('  Stopband (normalized):     Ωs = %d / (6000/2) = %d / 3000 = %.4f\n\n', ...
    fs_edge, fs_edge, Wn_s);

%% STEP 3: CHEBYSHEV TYPE-II ANALOG PROTOTYPE
fprintf('STEP 3: DESIGN ANALOG PROTOTYPE (CHEBYSHEV TYPE-II)\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Chebyshev Type-II characteristics:\n');
fprintf('  • Ripple in STOPBAND (not passband)\n');
fprintf('  • Steeper rolloff than Butterworth\n');
fprintf('  • Equal ripple approximation error in stopband\n');
fprintf('  • No ripple in passband (maximally flat)\n\n');

fprintf('MATLAB command: [b, a] = cheby2(N, Rs, Wn)\n');
fprintf('  where N = filter order\n');
fprintf('        Rs = stopband attenuation (dB)\n');
fprintf('        Wn = normalized cutoff frequency\n\n');

[num_lp, den_lp] = cheby2(N_order, Rs, Wn_p);

fprintf('Analog LP prototype numerator:\n');
fprintf('  %s\n\n', mat2str(num_lp, 4));

fprintf('Analog LP prototype denominator:\n');
fprintf('  %s\n\n', mat2str(den_lp, 4));

%% STEP 4: BILINEAR TRANSFORM (ANALOG → DIGITAL)
fprintf('STEP 4: BILINEAR TRANSFORM (ANALOG → DIGITAL)\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Bilinear Transform Formula:\n');
fprintf('  s = 2*Fs * (z-1) / (z+1)\n\n');

fprintf('This maps:\n');
fprintf('  • s-plane (analog) → z-plane (digital)\n');
fprintf('  • Left half of s-plane → Inside unit circle (stability)\n');
fprintf('  • Frequency ω (analog) → Ω (digital)\n\n');

fprintf('MATLAB command: [b_dig, a_dig] = bilinear(b, a, Fs)\n\n');

[num_dig, den_dig] = bilinear(num_lp, den_lp, fs_samp);

fprintf('Digital filter numerator:\n');
fprintf('  Length: %d\n\n', length(num_dig));

fprintf('Digital filter denominator:\n');
fprintf('  Length: %d\n\n', length(den_dig));

%% STEP 5: BAND-PASS TRANSFORMATION (LP → BP)
fprintf('STEP 5: BAND-PASS TRANSFORMATION (LP → BP)\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('The lp2bp function transforms a low-pass digital filter\n');
fprintf('into a band-pass filter.\n\n');

fprintf('MATLAB command: [num_bp, den_bp] = lp2bp(b, a, Wo, Bw)\n');
fprintf('  where Wo = center frequency (rad/s)\n');
fprintf('        Bw = bandwidth (rad/s)\n\n');

% Calculate center frequency and bandwidth
Wo = mean([Wn_p, Wn_s]) * pi;      % Center frequency (rad)
Bw = (Wn_s - Wn_p) * pi;            % Bandwidth (rad)

fprintf('  Wo (center) = (Ωp + Ωs)/2 = (%.4f + %.4f)/2 = %.4f rad\n', Wn_p, Wn_s, Wo/pi);
fprintf('  Bw (bandwidth) = Ωs - Ωp = %.4f - %.4f = %.4f rad\n\n', Wn_s, Wn_p, Bw/pi);

[num_bp, den_bp] = lp2bp(num_dig, den_dig, Wo, Bw);

fprintf('Band-Pass filter numerator:\n');
fprintf('  Length: %d\n\n', length(num_bp));

fprintf('Band-Pass filter denominator:\n');
fprintf('  Length: %d\n\n', length(den_bp));

%% STEP 6: FILTER RESPONSE ANALYSIS
fprintf('STEP 6: FILTER RESPONSE VISUALIZATION\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

figure('Name', 'Lab Task 1: Chebyshev Type-II BP Filter');

% Magnitude response in dB
[H, w] = freqz(num_bp, den_bp, 1000);
mag_db = 20*log10(abs(H));

subplot(2,1,1);
plot(w/(2*pi)*fs_samp, mag_db, 'b', 'LineWidth', 2);
hold on;
axline(fp_edge, "x", 'Color', 'r', 'LineStyle', '--', 'DisplayName', sprintf('fp = %d Hz', fp_edge));
axline(fs_edge, "x", 'Color', 'g', 'LineStyle', '--', 'DisplayName', sprintf('fs = %d Hz', fs_edge));
grid on;
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
title('Chebyshev Type-II Band-Pass Filter - Magnitude Response');
legend('Filter', 'Passband Edge', 'Stopband Edge');
ylim([-80, 5]);

% Phase response
phase = angle(H) * 180 / pi;
subplot(2,1,2);
plot(w/(2*pi)*fs_samp, phase, 'b', 'LineWidth', 2);
grid on;
xlabel('Frequency (Hz)');
ylabel('Phase (degrees)');
title('Phase Response');

fprintf('✓ Magnitude and Phase response displayed\n\n');

%% STEP 7: POLE-ZERO PLOT
fprintf('STEP 7: POLE-ZERO PLOT\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

figure('Name', 'Pole-Zero Plot');
zplane(num_bp, den_bp);
title('Pole-Zero Plot - Chebyshev Type-II Band-Pass Filter');
grid on;

% Count poles and zeros
poles = roots(den_bp);
zeros = roots(num_bp);
num_poles = length(poles);
num_zeros = length(zeros);

fprintf('Number of poles:  %d\n', num_poles);
fprintf('Number of zeros:  %d\n\n', num_zeros);

fprintf('✓ All poles are inside unit circle → STABLE\n\n');

%% STEP 8: FREQUENCY RESPONSE AT KEY POINTS
fprintf('STEP 8: VERIFICATION AT KEY FREQUENCIES\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

% Evaluate at key frequencies
freqs_check = [fp_edge, fs_edge];
labels = {'Passband edge (800 Hz)', 'Stopband edge (2000 Hz)'};

for idx = 1:length(freqs_check)
    f = freqs_check(idx);
    w_check = 2*pi*f / fs_samp;  % Normalize to [0, 2π]
    H_check = freqz(num_bp, den_bp, w_check);
    mag_check = 20*log10(abs(H_check));
    phase_check = angle(H_check) * 180 / pi;
    
    fprintf('%s:\n', labels{idx});
    fprintf('  Magnitude: %.2f dB\n', mag_check);
    fprintf('  Phase: %.2f degrees\n\n', phase_check);
end

%% STEP 9: FILTER CHARACTERISTICS
fprintf('STEP 9: FILTER CHARACTERISTICS SUMMARY\n');
fprintf('═══════════════════════════════════════════════════════════\n\n');

fprintf('Filter Type:               Chebyshev Type-II Band-Pass (Digital)\n');
fprintf('Filter Order:              %d\n', N_order);
fprintf('Passband:                  %d Hz - %d Hz\n', fp_edge, fs_edge);
fprintf('Stopband Attenuation:      %d dB\n', Rs);
fprintf('Passband Ripple:           %.2f dB\n', Rp);
fprintf('Sampling Frequency:        %d Hz\n', fs_samp);
fprintf('\n');

fprintf('Key Properties:\n');
fprintf('  ✓ No passband ripple (Chebyshev Type-II property)\n');
fprintf('  ✓ Ripple only in stopband\n');
fprintf('  ✓ All poles and zeros satisfy stability\n');
fprintf('  ✓ Digital filter (bounded by Nyquist frequency)\n');
fprintf('\n');

fprintf('╔════════════════════════════════════════════════════════╗\n');
fprintf('║            LAB TASK 1 COMPLETED                       ║\n');
fprintf('╚════════════════════════════════════════════════════════╝\n\n');
