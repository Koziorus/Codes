clc
clear all
close all


a = 0;   
b = 50;

[xvect, xdif, fx, it_cnt] = bisection(@compute_time, 1, 60000, 1e-3);

x = xvect(fx ~= 0);
x(end)

plot(1:it_cnt, xvect(1:it_cnt))


%%
% [xvect, xdif, fx, it_cnt] = bisect(a,b,1e-12,@compute_impedance);