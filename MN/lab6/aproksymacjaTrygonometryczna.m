function [x_approx] = aproksymacjaTrygonometryczna(n, x, N)

n = n*(pi/max(n));
S = zeros(N+1,N+1);

% generacja macierzy S
%...
%...

t = zeros(N+1,1);
for k = 1:N+1
    t(k,1) = sum(x.*cos((k-1)*n));
end


% Rozwiazanie ukladu rownan Sc = t
%...
%...

c1 = cos((0:N)'*n);

x_approx = (c1' * c).';

end
