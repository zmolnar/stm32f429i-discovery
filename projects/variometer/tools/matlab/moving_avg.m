p_raw = load('C:\Users\Zoltan Molnar\Desktop\p_raw.dat');
p_raw_t=p_raw';
windowSize1=10;
windowSize2=25;
windowSize3=50;
windowSize4=100;

a=1;
b1=(1/windowSize1)*ones(1,windowSize1);
p_avg_1=filter(b1,a,p_raw_data);

a=1;
b2=(1/windowSize2)*ones(1,windowSize2);
p_avg_2=filter(b2,a,p_raw_data);

a=1;
b3=(1/windowSize3)*ones(1,windowSize3);
p_avg_3=filter(b3,a,p_raw_data);

a=1;
b4=(1/windowSize4)*ones(1,windowSize4);
p_avg_4=filter(b4,a,p_raw_data);

plot(p_avg_1,'b-');
hold on;
plot(p_avg_2,'y-');
hold on;
plot(p_avg_3,'g-');
hold on;
plot(p_avg_4,'r-');
