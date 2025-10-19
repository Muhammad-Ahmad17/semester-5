% M = [4 1 0 0 0 0 0 0 0 ;
%      1 4 1 0 0 0 0 0 0 ; 
%      0 1 4 1 0 0 0 0 0 ; 
%      0 0 1 4 1 0 0 0 0 ; 
%      0 0 0 1 4 1 0 0 0 ; 
%      0 0 0 0 1 4 1 0 0 ; 
%      0 0 0 0 0 1 4 1 0 ; 
%      0 0 0 0 0 0 1 4 1 ; 
%      0 0 0 0 0 0 0 1 4 ;
%      ]
% 
% n = 9; 
% A= zeros (n)
% 
% for i = 1:n 
%     A(i,i) = 4
%     if i>1
%         A(i,i-1) = 1
%     end
%     if i<n
%         A(i,i+1) = 1
%     end 
% end


n = 9;
A = zeros(n)

for i = 1:n
    A(i, i) = 4  % Main diagonal
    
    % lower diagonals
    if i > 1
        A(i, i-1) = 1  
    end
    if i > 3
        A(i, i-3) = 1  
    end
    if i > 5
        A(i, i-5) = 1  
    end
    if i > 7
        A(i, i-7) = 1  
    end

    % upper diagonal
    if i < n
        A(i, i+1) = 1 
    end
    
    if i <= n - 3
        A(i, i+3) = 1  
    end
    
    if i <= n - 5
        A(i, i+5) = 1  
    end
    
    if i <= n - 7
        A(i, i+7) = 1 
    end
end
