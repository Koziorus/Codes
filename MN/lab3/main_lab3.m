clc
clear all
close all

% odpowiednie fragmenty kodu mozna wykonac poprzez zaznaczenie i wcisniecie F9 w Matlabie
% komentowanie/odkomentowywanie: ctrl+r / ctrl+t

%% Zadanie A
N = 10;
density = 3; % parametr decydujacy o gestosci polaczen miedzy stronami
[Edges] = generate_network(N, density);

%% Zadanie B

I = speye(N,N);

B = sparse(Edges(2,:), Edges(1,:), ones(1,size(Edges,2)), N, N);

L = sum(B);
d = 0.85;
A = spdiags((1./L)', 0, N, N);
M = I - d*B*A;

if issparse(M) == false
    fprintf('Error: M not sparse')
    return;
end

b = linspace((1-d)/N, (1-d)/N, N)';
%% Zadanie C
r = M\b;


%% Zadanie D
%------------------
clc
clear all
close all

N = [500, 1000, 3000, 6000, 12000];


for i = 1:5
    [M, b] = prepare_Mb(N(i), 10);
    tic
    % obliczenia start
    r = M\b;
    % obliczenia stop
    czas_Gauss(i) = toc;
end
plot(N, czas_Gauss)

%% Zadanie E

clc
clear all
close all

N = [500, 1000, 3000, 6000, 12000];
residuals = zeros(1,1000);

for i = 1:5
    display(N(i))
    [M, b] = prepare_Mb(N(i), 10);
    L = tril(M,-1);
    U = triu(M,1);
    D = diag(diag(M));
    r = ones(N(i), 1);
    X = -D\(L+U);
    Y = D\b;
    r = X * r + Y;
    res = M*r - b;

    iterations(i) = 0;
    
    tic
    % obliczenia start
    while norm(res) > 10^(-14)
        r = X * r + Y;

        res = M*r - b;
        
        iterations(i) = iterations(i) + 1;

        if N(i) == 1000
            residuals(iterations(i)) = norm(res);
        end
    end
    % obliczenia stop
    czas_Jacobi(i) = toc;
end
plot(N, czas_Jacobi)
figure
plot(N, iterations)
figure
semilogy(residuals); % pod 2 indeksem jest liczba iteracji dla N = 1000

%% Zadanie F


%% Zadanie G




