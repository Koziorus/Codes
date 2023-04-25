function [t] = trygfit2d(x,y,f)
%
%   A*t = f
%

M = sqrt(size(x,1))-1;
N=M;

xx = x * ones(1,M+1);
mm = ones(size(x,1),1)*(0:M);
yy = y * ones(1,N+1);
nn = ones(size(y,1),1)*(0:N);

a = max(x);
b = max(y);
xm = cos(xx.*mm*pi/a);
yn = cos(yy.*nn*pi/b);

MN = (M+1)*(N+1);
A = zeros(MN,MN);

for k = 1:MN
    xmk = xm(k,:);
    ynk = yn(k,:);
    A(k,:) = kron(xmk,ynk);
end

t = A\f;

end

