%% 1

clear all;
close all;

plot_range = 2; 
n_max = 200; % change to 200

circles = zeros(n_max, 3);
areas = zeros(n_max);
tries = zeros(n_max);

% first circle
r = rand(1) * (plot_range/2);
y = r + rand(1)*(plot_range - 2*r);
x = r + rand(1)*(plot_range - 2*r);
circles(1, 1:3) = [x y r];
areas(1) = pi*r*r;
tries(1) = 1;

plot_circle(x,y,r, plot_range);
hold on

for i = 2:n_max
    placed = false;
    number_of_tries = 0;
    while placed == false
        number_of_tries = number_of_tries + 1;
        r = rand(1) * (plot_range/2);
        
        % y = r; % minimum y for the given r
        % y = y + rand(1)*(plot_range - y - r);
        % x = r; % minimum x for the given r
        % x = x + rand(1)*(plot_range - x - r);
        
        y = r + rand(1)*(plot_range - 2*r);
        x = r + rand(1)*(plot_range - 2*r);
    
        intersecting = false;
       
        for j = 1:(i-1)
            distance = norm([x y] - circles(j, 1:2));
            if distance < r + circles(j, 3)
                intersecting = true;
                break;
            end
        end
    
        if intersecting == true
            continue;
        end
        
        plot_circle(x,y,r, plot_range);
        
        circles(i, 1:3) = [x y r];
        areas(i) = pi*r*r;
        tries(i) = number_of_tries;

        placed = true;
    end
    pause(0.001);
end   
hold off

figure
plot(1:n_max, cumsum(areas));

figure
plot(1:n_max, cumsum(tries))
%% 2
Edges = [1 1 2 2 2 3 3 3 4 4 5 5 6 6 7;
         4 6 4 3 5 6 5 7 6 5 4 6 4 7 6];
N = 7;
d = 0.85;

vert_num = size(Edges,2);

b = linspace((1 - d)/N,(1 - d)/N, N)';

B = zeros(N, N);

for i = 1:vert_num
    B(Edges(2,i), Edges(1,i)) = 1;
end

B = sparse(B);

L = 1./sum(B);
A = diag(L);

A = sparse(A);

I = speye(N);
M = I - d * B * A;

List = whos("A", "B", "I", "M", "b");
writetable(struct2table(List),'result1.txt');

spy(B);
title("Odnosniki stron");
print -dpng spy_b

r = M\b;

bar(r);
title("PageRank");
xlabel("Strona")
ylabel("Wartosc PR");
print -dpng bar_r

%% functions
function plot_circle(X, Y, R, plot_range)
    theta = linspace(0, 2*pi);
    x = R*cos(theta) + X;
    y = R*sin(theta) + Y;
    plot(x,y);
    axis equal; % axis have the same screen length
    axis([0 plot_range 0 plot_range]); % domains <0,2>
end