##clc
##clear
##close all
##
##% t = 0 : : 2;
##n = 0 : 19;
##fs = 100;
##f = 10;
####x = cos*2*pif*t
##T=n/fs
##x = cos(2*pi*f*(T))
##
##
##plot(n,x)
##hold on
##stem (T ,x)


f = 10;
b = 4;   #bits
ql= 2^b;  #quantization-levels
fs = 100;
n = 0:19;
t = n*(1/fs); # a to d conver of signal
x = cos(2*pi*f*t);
d = ( max(x) - min(x) ) / ql; # difference , step size
#  xq = quant (d ,x) # quantiz signal
xq = d * round (x/d);
err = xq - x

sqnr = snr (x, err)

stem (t , x , "-b");
hold on
stairs (t , xq , "-g")
hold on
stem (t, err , "-r")

legend('sampled' , 'quant' , 'err')
sqnr = snr (x, err)

