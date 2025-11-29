function [f] = factorial(x)
if (x <= 1)
    f=1
else
    f=x*factorial(x-1)
end