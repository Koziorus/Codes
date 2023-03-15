clear all;
close all;

a = 2;

n_max = 100;
circles = zeros(n_max, 3);
tries = linspace(0,0,n_max);
areas = 1:n_max;

tries(1) = 1;
r = rand(1)*a/2;
x = r + rand(1)*(a-2*r);
y = r + rand(1)*(a-2*r);
circles(1,1:3) = [x y r];
areas(1) = pi*r*r;
plot_circle(x,y,r,a);
hold on


for i = 2:n_max
    not_placed = true;
    while not_placed

    r = rand(1)*a/2;
    x = r + rand(1)*(a-2*r);
    y = r + rand(1)*(a-2*r);
    tries(i) = tries(i) + 1;

    is_intersecting = false;

    for j = 1:i-1
        if norm([x y] - circles(j,1:2)) < r + circles(j,3)
            is_intersecting = true;
            break;
        end
    end

    if is_intersecting
        continue;
    end

    not_placed = false;
    circles(i, 1:3) = [x y r];
    areas(i) = pi*r*r;
    plot_circle(x,y,r,a);
    pause(0.01);
    
    end
end

hold off
plot(0:n_max-1,cumsum(areas))
title("Powierzchnie");
xlabel("Liczba narysowanych okręgów")
ylabel("Powierzchnia")
% figure
print -dpng zadanie1a;

avg = cumsum(tries)./(1:n_max);

plot(0:n_max-1,avg)
title("Średnia liczba losowań");
xlabel("Liczba narysowanych okręgów")
ylabel("Liczba losowań")
% figure
print -dpng zadanie1b;


Edges = [1 1 2 2 2 3 3 3 4 4 5 5 6 6 7;
         6 4 4 5 3 6 5 7 6 5 4 6 4 7 6];

N = 7;
d = 0.85;

B = sparse(Edges(2,:),Edges(1,:), linspace(1,1,size(Edges,2)));

L = 1./sum(B);

A = diag(L);
A = sparse(A);

I = speye(N);

M = I - d*B*A;

b = linspace((1-d)/N,(1-d)/N,N)';

List = whos("A","B","I","M","b");
whos A B I M b
writetable(struct2table(List), "sparse_test.txt")

spy(B)
title("Podgląd odnośników");
print -dpng spy_b;

r = M\b;

bar(r);
title("PageRank");
xlabel("Strony");
ylabel("Wartości PR");
print -dpng bar_r



function plot_circle(X,Y,R,a)
    theta = linspace(0,2*pi);
    x = R*cos(theta) + X;
    y = R*sin(theta) + Y;
    plot(x,y);
    axis equal
    axis([0 a 0 a])
end

