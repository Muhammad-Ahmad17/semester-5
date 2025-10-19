% CREATE A MATRIX  2 BY 2
A = [1,2;3,4];
% APPEND THE ABOVE SO THAT IT BECOME 2 BY 3 
% ; AGLI ROW ME JANA HA
C = [2;2];
B = [A ,C];
% APPEND THE ABOVE SO THAT IT BECOME 3 BY 3 
E = [1,1,1];
D = [B ; E];
% x=(A^-1)*B 
F = [1,2,3];
x = inv (D) .* F;




