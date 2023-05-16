%% Zadanie 2
clc
clear all
close all

warning('off','all')

trajektoria1 = load('trajektoria1.mat');

figure
plot3(trajektoria1.x, trajektoria1.y, trajektoria1.z, 'o')
grid on
axis equal

%% Zadanie 4

close all

trajektoria1 = load('trajektoria1.mat');

N = 60;
xa = aproksymacjaWielomianowa(trajektoria1.n, trajektoria1.x, N);
ya = aproksymacjaWielomianowa(trajektoria1.n, trajektoria1.y, N);
za = aproksymacjaWielomianowa(trajektoria1.n, trajektoria1.z, N);

figure
plot3(trajektoria1.x, trajektoria1.y, trajektoria1.z, 'o')
hold on
plot3(xa, ya, za, 'g', 'lineWidth', 4)
title('Trajektoria właściwa i aproksymowana')
xlabel('Współrzędna X [m]')
ylabel('Współrzędna Y [m]')
zlabel('Współrzędna Z [m]')

saveas(gcf,'zadanie4.png')

%% Zadanie 5

% 5a
close all

trajektoria2 = load('trajektoria2.mat');

N = 60;
xa = aproksymacjaWielomianowa(trajektoria2.n, trajektoria2.x, N);
ya = aproksymacjaWielomianowa(trajektoria2.n, trajektoria2.y, N);
za = aproksymacjaWielomianowa(trajektoria2.n, trajektoria2.z, N);

figure
plot3(trajektoria2.x, trajektoria2.y, trajektoria2.z, 'o')
hold on
plot3(xa, ya, za, 'g', 'lineWidth', 4)
title('Trajektoria właściwa i aproksymowana')
xlabel('Współrzędna X [m]')
ylabel('Współrzędna Y [m]')
zlabel('Współrzędna Z [m]')

saveas(gcf,'zadanie5a.png')

% 5b
sigma = [trajektoria2.x; trajektoria2.y; trajektoria2.z];
M = size(trajektoria2.n, 2);
sigma_apr = zeros(3, M);

err_mat = zeros(3,1);
err = zeros(1, 71);

for N = 1:71
    sigma_apr(1,:) = aproksymacjaWielomianowa(trajektoria2.n, trajektoria2.x, N);
    sigma_apr(2,:) = aproksymacjaWielomianowa(trajektoria2.n, trajektoria2.y, N);
    sigma_apr(3,:) = aproksymacjaWielomianowa(trajektoria2.n, trajektoria2.z, N);
    
    err_mat = sqrt(sum((sigma - sigma_apr).^2, 2))/M;

    err(N) = sum(err_mat);
end

figure
semilogy(1:71, err)
title('Błąd aproksymacji')
xlabel('Rząd aproksymacji')
ylabel('Wartość błędu')

saveas(gcf,'zadanie5b.png')

%% Zadanie 6

% 6a
close all

trajektoria2 = load('trajektoria2.mat');

N = 60;
xa = aproksymacjaTrygonometryczna(trajektoria2.n, trajektoria2.x, N);
ya = aproksymacjaTrygonometryczna(trajektoria2.n, trajektoria2.y, N);
za = aproksymacjaTrygonometryczna(trajektoria2.n, trajektoria2.z, N);

figure
plot3(trajektoria2.x, trajektoria2.y, trajektoria2.z, 'o')
hold on
plot3(xa, ya, za, 'g', 'lineWidth', 4)
title('Trajektoria właściwa i aproksymowana')
xlabel('Współrzędna X [m]')
ylabel('Współrzędna Y [m]')
zlabel('Współrzędna Z [m]')

saveas(gcf,'zadanie6a.png')

% 6b

sigma = [trajektoria2.x; trajektoria2.y; trajektoria2.z];
M = size(trajektoria2.n, 2);
sigma_apr = zeros(3, M);

err_mat = zeros(3,1);
err = zeros(1, 71);

for N = 1:71
    sigma_apr(1,:) = aproksymacjaTrygonometryczna(trajektoria2.n, trajektoria2.x, N);
    sigma_apr(2,:) = aproksymacjaTrygonometryczna(trajektoria2.n, trajektoria2.y, N);
    sigma_apr(3,:) = aproksymacjaTrygonometryczna(trajektoria2.n, trajektoria2.z, N);
    
    err_mat = sqrt(sum((sigma - sigma_apr).^2, 2))/M;

    err(N) = sum(err_mat);
end

figure
semilogy(1:71, err)
title('Błąd aproksymacji')
xlabel('Rząd aproksymacji')
ylabel('Wartość błędu')

saveas(gcf,'zadanie6b.png')

% 6c

N = 150;
xa = aproksymacjaTrygonometryczna(trajektoria2.n, trajektoria2.x, N);
ya = aproksymacjaTrygonometryczna(trajektoria2.n, trajektoria2.y, N);
za = aproksymacjaTrygonometryczna(trajektoria2.n, trajektoria2.z, N);

figure
plot3(trajektoria2.x, trajektoria2.y, trajektoria2.z, 'o')
hold on
plot3(xa, ya, za, 'g', 'lineWidth', 4)
title('Trajektoria właściwa i aproksymowana')
xlabel('Współrzędna X [m]')
ylabel('Współrzędna Y [m]')
zlabel('Współrzędna Z [m]')

saveas(gcf,'zadanie6c.png')


