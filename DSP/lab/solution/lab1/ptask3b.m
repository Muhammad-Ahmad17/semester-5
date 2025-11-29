clc
clear
close all

fp = 2/3 

% 4 periods
t =  0 :.01 : 4*fp 

% x(t)
x_t = 3 * cos( (3*pi*t) + (pi/3) )

plot (t , x_t)
