function x = fact (n)
  if n ==0 || n ==1
    x = 1;
  else
    x = n * fact(n-1);
  end
endfunction
