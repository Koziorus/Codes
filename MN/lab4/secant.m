function [xvect,xdif,fx,it_cnt] = secant(fun,a,b,eps)
    % fun - funkcja, ktorej miejsce zerowe bedzie poszukiwane
    % [a,b] - przedzial poszukiwania miejsca zerowego
    % eps - prog dokladnosci obliczen
    % 
    % xvect - wektor kolejnych wartosci przyblizonego rozwiazania
    % xdif - wektor roznic pomiedzy kolejnymi wartosciami przyblizonego rozwiazania
    % fx - wektor wartosci funkcji dla kolejnych elementow wektora xvect
    % it_cnt - liczba iteracji wykonanych przy poszukiwaniu miejsca zerowego
    
    n_max = 1000;
    
    xvect = zeros(1, n_max);
    xdif = zeros(1, n_max);
    fx = zeros(1, n_max);
    it_cnt = 0;
    
    xvect(1) = a;
    xvect(2) = b;
    
    xdif(2) = xvect(1) - xvect(2);
    
    for i = 3:n_max
        xvect(i) = xvect(i - 1) - (fun(xvect(i - 1)) * (xvect(i - 1) - xvect(i - 2))) / (fun(xvect(i - 1)) - fun(xvect(i - 2))); 
        
        xdif(i) = xvect(i-1) - xvect(i);
        fx(i) = fun(xvect(i));

        if (abs(fx(i)) < eps) || (abs(xvect(i-1) - xvect(i)) < eps)
            it_cnt = i;
            return
        end
    end
    
end

