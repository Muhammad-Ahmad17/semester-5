clc 
clear 
close all 

R = 113 ;
n = round (R/2 + 5) ; 

t= -n:n;

% ----- a
x1 = 2 * gauspuls (t+2) - gauspuls (t-4);
stem (t,x1)
xlabel ('x-axis:n')
ylabel('y-axis:x_1(n)')

title ('Lab1-Tak1-a')

% ----- b
figure;

x2 = t .* (heaviside(t) - heaviside(t-10)) + 10 * exp(-0.3*(t-10)) .* (heaviside(t-10) - heaviside(t-20));

stem(t, x2);
xlabel('n');
ylabel('x_2(n)');
title('Lab1-Tak1-b');

% ----- c

figure;
x3 = x1 .* x2 + flip(x1) .* flip(x2);
stem(t, x3);
xlabel('n');
ylabel('x_3(n)');
title('Lab1-Tak1-c');

