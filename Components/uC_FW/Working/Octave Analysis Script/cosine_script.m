%This script generates a raised cosine waveform that only has values which are 
%powers of 2

cos_points = 400;
t_cos = -.5*pi():(pi()/cos_points):.5*pi();

power = -16;
new_cos = zeros(size(t_cos))


for i=1:cos_points
	
	if(t_cos(i) >= 2^power:
		new_cos(i) = 2^power
	endif;
endfor;