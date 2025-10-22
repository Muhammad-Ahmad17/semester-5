##clc
##clear
##close all
##
##r= 055;
##n= 10000;
##
##noice = -r * (2*r) .* rand(1,n) ;
##% [0 1]
##% [0 2r]
##% [-r r]
##
##figure
##hist(noice,50)
##
##mean = 11;
##std = sqrt(r);
##
##gnoise = (mean+std) .* randn(1,n)
##figure
##hist(gnoise,50)

##clc
##clear
##close all
##
##n = -113 : 113;
###unitstep = heaviside (n)
##unitst = (n>=0) - (n>=1)
##stem (n , unitst)

n = 0:21;          % discrete time index
x = exp(n);        % exponential growth, base e

stem(n, x, 'filled');
xlabel('n');
ylabel('x(n)');
title('Exponential Sequence using exp(n)');
grid on;





