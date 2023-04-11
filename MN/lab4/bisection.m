function [xvect,xdif,fx,it_cnt] = bisection(fun,a,b,eps)
    % fun - funkcja, ktorej miejsce zerowe bedzie poszukiwane
    % [a,b] - przedzial poszukiwania miejsca zerowego
    % eps - prog dokladnosci obliczen
    % 
    % xvect - wektor kolejnych wartosci przyblizonego rozwiazania
    % xdif - wektor roznic pomiedzy kolejnymi wartosciami przyblizonego rozwiazania
    % fx - wektor wartosci funkcji dla kolejnych elementow wektora xvect
    % it_cnt - liczba iteracji wykonanych przy poszukiwaniu miejsca zerowego
    max_n = 1000;
    
    xvect = zeros(1,max_n);
    fx = zeros(1,max_n);
    xdif = zeros(1,max_n);
    it_cnt = 0;
    
    for i = 1:max_n
        c = (a + b)/2;
        fc = fun(c);
    
        xvect(i) = c;
        fx(i) = fc;
        if i >= 2
            xdif(i) = xvect(i-1) - xvect(i);
        end

        if (abs(b-a) < eps) || (abs(fc) < eps)
            it_cnt = i;
            return
        elseif fc * fun(a) < 0
            b = c;
        else
            a = c;
        end
    end
end

