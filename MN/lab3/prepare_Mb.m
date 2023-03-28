function [output_M,output_b] = prepare_Mb(N, density)

[Edges] = generate_network(N, density);

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

output_M = M;
output_b = b;
end

