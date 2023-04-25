function [ x,y,f,xp,yp ] = lazik( K )
% K - funkcja zostanie sprobkowana w punktach na plaszczyznie ulozonych w siatke K x K punktow
% x - wspolrzedne x punktow probkowania
% y - wspolrzedne y punktow probkowania
% f - wartosc funkcji w punktach probkowania
% xp - wspolrzedne x punktow probkowania zgodne z torem jazdy lazika
% yp - wspolrzedne y punktow probkowania zgodne z torem jazdy lazika

% generacja polozenia probek
[x,y] = meshgrid(linspace(0,1,K),linspace(0,1,K));

s = 1.60;
xs = 0.5 - 0.5/s;
ys = 0.5 - 0.5/s;

w1=-5*1; a1=30; x1=xs+0.4/s; y1=ys+1/s*0.3;
w2= 6*1; a2=40; x2=xs+0.6/s; y2=ys+1/s*1.0;
w3= 7*1; a3=50; x3=xs+0.9/s; y3=ys+1/s*0.5;
w4= 6*1; a4=40; x4=xs+0.6/s; y4=ys+1/s*0.1;
w5= 7*1; a5=70; x5=xs+0.1/s; y5=ys+1/s*0.95;
w6=-5*1; a6=10; x6=xs+0.5/s; y6=ys+1/s*0.5;

f = 10;
f = f + w1*exp(-a1*((x-x1).^2+(y-y1).^2));
f = f + w2*exp(-a2*((x-x2).^2+(y-y2).^2));
f = f + w3*exp(-a3*((x-x3).^2+(y-y3).^2));
f = f + w4*exp(-a4*((x-x4).^2+(y-y4).^2));
f = f + w5*exp(-a5*((x-x5).^2+(y-y5).^2));
f = f + w6*exp(-a6*((x-x6).^2+(y-y6).^2));

% utworzenie wektora toru ruchu lazika
xp = x;
xp(2:2:end,:) = xp(2:2:end,end:-1:1);
xp = xp';
yp = y';

% macierz -> wektor
x = reshape(x,K*K,1);
y = reshape(y,K*K,1);
f = reshape(f,K*K,1);
xp = reshape(xp,K*K,1);
yp = reshape(yp,K*K,1);


end

