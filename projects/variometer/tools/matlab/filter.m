% MS5611 barometric pressure sensor measurement data processing
clear;
close all;
data = load('./p_raw.dat');
t = data(:,1);
p_raw = data(:,2);
t_raw = data(:,3);
dt = (t(2)-t(1))/1000;

% Scale and offset time vector
t_off = t(1)
for i = 1:length(t)
    t(i) -= t_off;
    t(i) /= 1000;
end

% Alpha-beta filtering
alpha = 0.2;
beta = 0.005;
xk_1 = p_raw(1);
vk_1 = 0;
p_fil = zeros(1,length(t));

for i = 1:length(t)
    xk = xk_1 + (vk_1 * dt);
    vk = vk_1;
    
    rk = p_raw(i) - xk;
    
    xk = xk + alpha * rk;
    vk = vk + (beta * rk)/dt;
    
    xk_1 = xk;    
    vk_1 = vk;
    
    p_fil(i) = xk;
end    

% Calculate height
h = zeros(1,length(t));
for i = 1:length(t)
    h(i) = pressure_to_height(p_fil(i));
end

% Calculate slope using linear regression
window_size = 1/dt;
h_window = zeros(1,window_size);
vario_1s = zeros(1,length(t));
for i = window_size:length(t)
    for j = 1:window_size
       h_window(j) = h(i-window_size+j);
    end
    vario_1s(i) = linfit(h_window, dt);
end

% Calculate slope using linear regression
window_size = 2/dt;
h_window = zeros(1,window_size);
vario_2s = zeros(1,length(t));
for i = window_size:length(t)
    for j = 1:window_size
       h_window(j) = h(i-window_size+j);
    end
    vario_2s(i) = linfit(h_window, dt);
end

% Calculate slope using linear regression
window_size = 3/dt;
h_window = zeros(1,window_size);
vario_3s = zeros(1,length(t));
for i = window_size:length(t)
    for j = 1:window_size
       h_window(j) = h(i-window_size+j);
    end
    vario_3s(i) = linfit(h_window, dt);
end

% Plot results
figure(1);
title ("Height");
plot(t,h,'b-');
xlabel("time (s)");
ylabel("height (m)");
grid on;
grid minor;
hold on;

figure(2);
title ("Vario");
plot(t, vario_1s,'r-;1s;');
hold on;
plot(t,vario_2s,'b-;2s;');
hold on;
plot(t,vario_3s,'g-;3s;');
xlabel("time (s)");
ylabel("vario (m/s)");
grid on;
grid minor;
hold on;
