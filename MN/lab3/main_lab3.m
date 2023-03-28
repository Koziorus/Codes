clc
clear all
close all

% odpowiednie fragmenty kodu mozna wykonac poprzez zaznaczenie i wcisniecie F9 w Matlabie
% komentowanie/odkomentowywanie: ctrl+r / ctrl+t

% Zadanie A
%------------------
N = 10;
density = 3; % parametr decydujacy o gestosci polaczen miedzy stronami
[Edges] = generate_network(N, density);
%-----------------

% Zadanie B
%------------------
% generacja macierzy I, A, B i wektora b
% macierze A, B i I musza byc przechowywane w formacie sparse (rzadkim)
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

r = M\b;

%-----------------



% Zadanie D
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
%------------------


%% Zadanie E
%------------------

clc
clear all
close all

N = [500, 1000, 3000, 6000, 12000];

for i = 1:5
    [M, b] = prepare_Mb(N(i), 10);
    L = tril(M);
    U = triu(M);
    D = diag(diag(M));
    r = ones(N(i), 1);
    res = M*r - b;
    iter = 0;
    tic
    % obliczenia start
    while norm(res) > 10^(-14)
        r = -D\(L+U)*r+D\b;

        res = M*r - b;
        display(res)
        iter = iter + 1;
    end
    % obliczenia stop
    czas_Jacobi(i) = toc;
    iterations(i) = iter;
end
plot(N, czas_Jacobi)
figure
plot(N, iterations)

%%------------------



%% Zadanie F
%------------------


% Zadanie G
%------------------









