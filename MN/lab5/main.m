%% Zadanie 1

close all
clear

for K = [5 15 25 35]
    [prob_x, prob_y, wart_prob, tor_x, tor_y] = lazik(K);
 
    [XX, YY] = meshgrid(linspace(0, 1, 101), linspace(0, 1, 101));

    [p] = polyfit2d(prob_x, prob_y, wart_prob);
    [FP] = polyval2d(XX, YY, p);

    [t] = trygfit2d(prob_x, prob_y, wart_prob);
    [FT] = trygval2d(XX, YY, t);
   
    figure
    subplot(2,2,1);
    plot(tor_x, tor_y, '-o', 'linewidth', 2);

    subplot(2,2,2);
    
    surf(reshape(prob_x, K, K), reshape(prob_y, K, K), reshape(wart_prob, K, K))
    shading flat

    subplot(2,2,3);
    surf(XX, YY, FP);
    shading flat

    subplot(2,2,4);
    surf(XX, YY, FT);
    shading flat
    
end


%% Zadanie 2
close all
clear

min_K = 5;
max_K = 45;

Div_poly = zeros(1, max_K);
Div_tryg = zeros(1, max_K);

for K = min_K:max_K
    [prob_x, prob_y, wart_prob, tor_x, tor_y] = lazik(K);
 
    [XX, YY] = meshgrid(linspace(0, 1, 101), linspace(0, 1, 101));

    [p] = polyfit2d(prob_x, prob_y, wart_prob);
    [FP1] = polyval2d(XX, YY, p);

    [t] = trygfit2d(prob_x, prob_y, wart_prob);
    [FT1] = trygval2d(XX, YY, t);
    
    if K > min_K
        Div_poly(K) = max(max(abs(FP2 - FP1)));
        Div_tryg(K) = max(max(abs(FT2 - FT1)));
    end

    % fprintf('%g ', Div_poly(K))
 
    FP2 = FP1;
    FT2 = FT1;
end

figure
plot(min_K:max_K, Div_poly(min_K:max_K))
title("tryg")

figure
plot(min_K:max_K, Div_tryg(min_K:max_K))
title("poly")






