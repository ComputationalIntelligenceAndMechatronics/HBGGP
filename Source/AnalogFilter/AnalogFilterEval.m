function obj = AnalogFilterEval(A,B,C,D)

sys = ss(A,B,C,D);

%Determine the cutoff frequency
cutoff = 2*pi*1000;

%Create evaluation point array
a1 = 2;
b1 = 5;
a2 = 3;
b2 = 4;
n1 = 200;
n2 = 200;
w1 = logspace(a1,b1,n1);
%w2 = logspace(a2,b2,n2);
%w = sort([w1,w2]);
%n = n1+n2;
w = w1;
n = n1;
 
% Compute the frequency response
[mag,phase] = bode(sys,w);
mag = shiftdim(mag,1);
 
% Compute ideal response
idx = find(w > cutoff);
imag = zeros(size(w));
imag(idx) = 1;
 
% Compute the error in magnitude with the ideal response
error = imag - mag;
 
error2 = abs(error);
rmserror = sum(error2);
 
obj = n/(n+rmserror);
 

