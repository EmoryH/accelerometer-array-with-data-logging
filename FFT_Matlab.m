%Choose which log file to run
%Option to run all of them, but it will likely crash due to filesize

Logfile = 4;

%Pulls the data from a specified CSV file

%for i = 1:f
	if Logfile < 10
 		filename = compose("LOGGER0%01d.csv",f.') 
	elseif Logfile >= 10
		filename = compose("LOGGER%02d.csv",f.')
	end

%Creates a matrix M where each column is time or a sensor value (t,s1,s2,s3,s4)

M = csvread(filename);

%Prime the arrays for data

n = length(M);
t = zeros(n,1);
s1 = zeros(n,1);
s2 = zeros(n,1);
s3 = zeros(n,1);
s4 = zeros(n,1);

F_S = (1 / (30.0 / 1000.0));
dt = 1/F_S;
LL = n;
N = 1024;
f = F_S/2*linspace(0, 1, N/2+1);


%cycle through each row 1-n of matrix M and pulls each value into an an array

for i = 1:n

	t(i) = M(i,1);
	s1(i) = M(i,2);
	s2(i) = M(i,3);
	s3(i) = M(i,4);
	s4(i) = M(i,5);	

end

s1_fft = fft(s1,N)/LL;
s2_fft = fft(s2,N)/LL;
s3_fft = fft(s3,N)/LL;
s4_fft = fft(s4,N)/LL;


%%time domain plot

figure(1)
subplot(4,1,1)
plot(t,s1)
title('Sensor S1 in the time domain')
xlabel('time (ms)')
ylabel('Sens 1 Acceleration (G)')

subplot(4,1,2)
plot(t,s2)
title('Sensor S2 in the time domain')
xlabel('time (ms)')
ylabel('Sens 2 Acceleration (G)')

subplot(4,1,3)
plot(t,s3)
title('Sensor S3 in the time domain')
xlabel('time (ms)')
ylabel('Sens 3 Acceleration (G)')

subplot(4,1,4)
plot(t,s4)
title('Sensor S4 in the time domain')
xlabel('time (ms)')
ylabel('Sens 1 Acceleration (G)')


%frequency domain plot

figure(2)
subplot(4,1,1)
plot(f,2*abs(s1_fft(1:N/2+1)))
title('Sensor S1 in the Frequency Domain')
xlabel('Frequency (Hz)')
ylabel('Sens 1 Acceleration (G)')

subplot(4,1,2)
plot(f,2*abs(s2_fft(1:N/2+1)))
title('Sensor S2 in the Frequency Domain')
xlabel('Frequency (Hz)')
ylabel('Sens 2 Acceleration (G)')


subplot(4,1,3)
plot(f,2*abs(s3_fft(1:N/2+1)))
title('Sensor S3 in the Frequency Domain')
xlabel('Frequency (Hz)')
ylabel('Sens 3 Acceleration (G)')


subplot(4,1,4)
plot(f,2*abs(s4_fft(1:N/2+1)))
title('Sensor S4 in the Frequency Domain')
xlabel('Frequency (Hz)')
ylabel('Sens 4 Acceleration (G)')
