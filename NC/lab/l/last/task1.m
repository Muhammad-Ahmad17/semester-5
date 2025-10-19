g1 = @(x) x^3 + x^2 -1
g2 = @(x) 1 / sqrt(x+1)
g3 = @ (x) 1/ x^2 + x


a = 0;
b = 1;
x0 = 0;
tol = 1e-2;
max_iter = 10;
                                                                           
fixed_point(g2, a, b, x0, tol, max_iter);
