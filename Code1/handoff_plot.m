function handoff_plot(arr)
%HANDOFF_PLOT �� �Լ��� ��� ���� ��ġ
%   �ڼ��� ���� ��ġ
M = max(arr);
cumulation = ones(1,M+1);   %size (1XM+1) cumulation�̶�� �迭���� 

for j = 1:1:10000
    cumulation(arr(j)+1) = cumulation(arr(j)+1) + 1 ;
end

x_axis = (0:1:M);
bar(x_axis,cumulation);