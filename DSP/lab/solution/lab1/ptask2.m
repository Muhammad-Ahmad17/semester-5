clc;
clear;
close all;

R = 113;
n = round(R/2 + 5);   
t = -n:n;             

x4 = exp(-0.1+j*0.3) * t;

% magnitude
subplot(411)
stem (t,abs(x4))
title('magnitude')
% phase
subplot(412)
stem (t,angle(x4))
title('phase')
% real 
subplot(413)
stem (t,real(x4))
title('real')
% imag
subplot(414)
stem (t,imag(x4))
title('imag')


