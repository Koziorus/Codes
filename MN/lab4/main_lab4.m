clc
clear all
close all


a = 1;   
b = 60000;
eps = 1e-3;

[xvect, xdif, fx, it_cnt] = bisection(@compute_time, a, b, eps);

x = xvect(fx ~= 0);
%x(end)

plot(1:size(x,2), x)

semilogy(1:size(x,2) - 1, xdif(2:(size(x,2))))



%%

a = 0;
b = 50;
eps = 1e-12;

[xvect, xdif, fx, it_cnt] = bisection(@compute_impedance, a, b, eps);

x = xvect(fx ~= 0);
x(end)

%%

a = 0;
b = 50;
eps = 1e-12;

[xvect, xdif, fx, it_cnt] = bisection(@compute_rocket_time, a, b, eps);

x = xvect(fx ~= 0);
x(end)

%%

a = 1;   
b = 60000;
eps = 1e-3;

[xvect, xdif, fx, it_cnt] = secant(@compute_time, a, b, eps);

x = xvect(fx ~= 0);
x(end)

%%
options = optimset('Display', 'iter');
fzero(@tan, 4.5, options);


