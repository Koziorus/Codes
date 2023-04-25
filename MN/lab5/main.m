%% Zadanie 1

close all
clear

for K=[5 15 25 35]
    [prob_x, prob_y, wart_prob, tor_x, tor_y] = lazik(K);
 
    [p] = polyfit2d(prob_x, prob_y, wart_prob);
    [XX, YY] = meshgrid(linspace(0, 1, 101), linspace(0, 1, 101));
    [FP] = polyval2d(XX, YY, p);
    
    figure
    subplot(2,2,1);
    plot(tor_x, tor_y, '-o', 'linewidth', 2);
    
    pause;
end