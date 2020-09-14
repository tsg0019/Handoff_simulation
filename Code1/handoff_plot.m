function handoff_plot(arr)
%HANDOFF_PLOT 이 함수의 요약 설명 위치
%   자세한 설명 위치
M = max(arr);
cumulation = ones(1,M+1);   %size (1XM+1) cumulation이라는 배열생성 

for j = 1:1:10000
    cumulation(arr(j)+1) = cumulation(arr(j)+1) + 1 ;
end

x_axis = (0:1:M);
bar(x_axis,cumulation);