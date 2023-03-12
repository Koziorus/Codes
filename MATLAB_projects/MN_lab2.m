%% code

clear all;
close all;

plot_range = 2; 
n_max = 200; % change to 200

circles = zeros(n_max, 3);

% first circle
r = rand(1) * (plot_range/2);
y = r + rand(1)*(plot_range - 2*r);
x = r + rand(1)*(plot_range - 2*r);
circles(1, 1:3) = [x y r];
plot_circle(x,y,r, plot_range);
hold on

for i = 2:n_max
    placed = false;
    while placed == false
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

        placed = true;
    end
    pause(0.001);
end   
%% functions
function plot_circle(X, Y, R, plot_range)
    theta = linspace(0, 2*pi);
    x = R*cos(theta) + X;
    y = R*sin(theta) + Y;
    plot(x,y);
    axis equal; % axis have the same screen length
    axis([0 plot_range 0 plot_range]); % domains <0,2>
end