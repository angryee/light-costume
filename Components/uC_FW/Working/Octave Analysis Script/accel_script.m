%Script to load and analyze accelerometer data
printf "Running acceleration data analysis script\r\n"

%This script needs the control toolbox
clear *
pkg load signal
pkg load control

%clear all variables
clear *

t_trans =1000;

filter_order = 2;

ts = (1/1000);	%800Hz sampling rate
accel_data_path = "accel_data.txt";

file_accel_data = fopen(accel_data_path,"rb");
[val,count] = fread(file_accel_data,Inf,"uint16");
fclose(file_accel_data);

%Generate a time vector from t=0 to the end determined by count and sampling time
tmax = (count-1)*ts;
t=0:ts:tmax;

figure(1)
plot(t,val','1')
title("Raw Sampled Accelerometer Data")
xlabel("Time (s)")
ylabel("Accelerometer Data")
print("plots/raw_accel_data.png")
orig_axis = axis();
%------------------------------------
%Step 2 - filter the data

f_lp = 400; %Hz
f_hp = 10; %Hz
fz_lp = f_lp*ts;
fz_hp = f_hp*ts;


[bl,al] = butter(filter_order,fz_hp,'low');
[bh,ah] = butter(filter_order,fz_hp,'high');
accel_filt = filter(bl,al,val);
figure(2)
plot(t,accel_filt,'2')
title("Low-pass filtered acceleration data")

accel_filt = filter(bh,ah,accel_filt);

figure(3)

plot(t,accel_filt)
title("High+Low pass filtered acceleration data")


figure(4)
hold off
accel_filt_sq = ((accel_filt.-mean(accel_filt)).^2).^.5;
plot(t(t_trans:count),accel_filt_sq(t_trans:count))
title("RMS Filtered acceleration data")
cos_points = 250;

t_cos = -.5*pi():(pi()/cos_points):.5*pi();

cos_filt = cos(t_cos);
figure(5)
plot(t_cos,cos_filt)

h = conv(cos_filt,accel_filt_sq);
count = size(h);
i = h((cos_points/2)+1:count-(cos_points/2))';

figure(6)
hold off
plot(t,i)



%axis([0,tmax,0,500])
threshold =.10*cos_points;
thresh_wvfm = threshold * ones(size(i));
hold on
%plot(t,thresh_wvfm)
threshold =.35*cos_points;
thresh_wvfm = threshold * ones(size(i));
%plot(t,thresh_wvfm)
title("Raised cosine convolved acceleration data")
