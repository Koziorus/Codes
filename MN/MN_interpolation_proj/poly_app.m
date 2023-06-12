function [X_app, Y_app] = poly_app(X, Y, all_points, n)
    A = zeros(n, n);
    A(:, 1) = 1;
    
    for i = 2:n
        A(:,i) = X.^(i-1);
    end
    
    b = Y;
    a = A\b;
    
    m = n * 100; % points in the interpolation
    X_app = all_points(floor(linspace(1, size(all_points, 1), m)), 1);
    
    X_pow = zeros(m, n); % 1 x1 x1^2 ... \ ... 1 x2 x2^2 ... \ ...
    X_pow(:, 1) = 1;
    
    for i = 2:n
        X_pow(:,i) = X_app.^(i-1);
    end
    
    Y_app = X_pow * a; 
end