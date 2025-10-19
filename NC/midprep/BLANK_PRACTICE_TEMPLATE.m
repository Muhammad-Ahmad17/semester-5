%═════════════════════════════════════════════════════════════════════════════
% BLANK TEMPLATE FOR PRACTICE - Print this and write by hand!
%═════════════════════════════════════════════════════════════════════════════

%─ Clean up workspace
clc
clear
close all

%─ Function definition
f = @(x) _________________;

%─ (Optional) Derivative
% df = @(x) _________________;

%─ (Optional) Second derivative  
% d2f = @(x) _________________;

%─ INPUT SECTION
%─ For bracket methods [a,b]:
% a = input('Enter a: ');
% b = input('Enter b: ');

%─ For open methods x0:
x0 = input('Enter x0: ');

%─ Validation (if bracket method)
% while f(a)*f(b) > 0
%     disp('Invalid bracket!');
%     a = input('Enter new a: ');
%     b = input('Enter new b: ');
% end

%─ INITIALIZE
tol = 1e-6;
maxIter = 50;
i = 0;
err = inf;

%─ PRINT HEADER
fprintf('%s\n', '═══════════════════════════════════════════════════════════');
fprintf('%-5s %-15s %-15s %-15s\n', 'Iter', 'x', 'f(x)', 'Error(%)');
fprintf('%s\n', '═══════════════════════════════════════════════════════════');

%─ MAIN ITERATION LOOP
while (err > tol) && (i < maxIter)
    i = i + 1;
    
    %═════════════════════════════════════════════════════════════════════════
    % *** PLUG YOUR FORMULA HERE ***
    % (Choose based on method type)
    %
    % Bisection:
    %   Xm = (a + b) / 2;
    %   if f(a)*f(Xm) < 0
    %       b = Xm;
    %   else
    %       a = Xm;
    %   end
    %   x_new = Xm;
    %
    % Newton-Raphson:
    %   f0 = f(x0);
    %   f1 = df(x0);
    %   x_new = x0 - f0/f1;
    %
    % Secant:
    %   x2 = x1 - f(x1)*(x1-x0)/(f(x1)-f(x0));
    %   x0 = x1;
    %   x1 = x2;
    %   x_new = x2;
    %
    % Regula Falsi:
    %   x_new = (a*f(b) - b*f(a))/(f(b) - f(a));
    %   if f(a)*f(x_new) < 0
    %       b = x_new;
    %   else
    %       a = x_new;
    %   end
    %
    % Fixed Point:
    %   x_new = g(x0);
    %
    % Modified Newton-Raphson:
    %   f0 = f(x0);
    %   f1 = df(x0);
    %   f2 = d2f(x0);
    %   x_new = x0 - (f0*f1)/(f1^2 - f0*f2);
    %═════════════════════════════════════════════════════════════════════════
    
    x_new = _________________;  % YOUR FORMULA HERE
    
    
    %─ Calculate error
    if i == 1
        err = inf;  % First iteration - no previous value
    else
        err = abs((x_new - x_old) / x_new) * 100;
    end
    
    
    %─ Print iteration results
    fprintf('%-5d %-15.8f %-15.8f %-15.6f\n', i, x_new, f(x_new), err);
    
    
    %─ Update variables for next iteration
    x_old = x_new;
    % OR depending on method:
    % x0 = x_new;
    % x0 = x1; x1 = x2;
    % etc.
end

%─ PRINT FINAL RESULT
fprintf('%s\n', '═══════════════════════════════════════════════════════════');
fprintf('\n✅ Root = %.8f\n', x_new);
fprintf('✅ Iterations: %d\n', i);
fprintf('✅ Final Error: %.6e\n\n', err);


%═════════════════════════════════════════════════════════════════════════════
% QUIZ REFERENCE FORMULAS - COPY EXACTLY!
%═════════════════════════════════════════════════════════════════════════════
%
% 1. BISECTION:
%    Xm = (a + b) / 2
%    Check: if f(a)*f(Xm) < 0 → b = Xm, else a = Xm
%
% 2. NEWTON-RAPHSON:
%    x_new = x0 - f(x0) / df(x0)
%
% 3. SECANT:
%    x2 = x1 - f(x1)*(x1-x0)/(f(x1)-f(x0))
%    Update: x0=x1, x1=x2
%
% 4. REGULA FALSI:
%    x_new = (a*f(b) - b*f(a)) / (f(b) - f(a))
%    Check: if f(a)*f(x_new) < 0 → b = x_new, else a = x_new
%
% 5. FIXED POINT:
%    x_new = g(x0)
%    (Need to rearrange: x = g(x))
%
% 6. MODIFIED NEWTON-RAPHSON:
%    x_new = x0 - (f0*f1)/(f1^2 - f0*f2)
%    where f0=f(x0), f1=df(x0), f2=d2f(x0)
%
%═════════════════════════════════════════════════════════════════════════════

