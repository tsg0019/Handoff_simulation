function handoff_tendency(Cell_tendency,v_tendency,d_tendency,Call_tendency)
%HANDOFF_TENDENCY 이 함수의 요약 설명 위치
%   자세한 설명 위치
ax1 = subplot(2,2,1);
Cell_x = [100:10:800];
plot(ax1,Cell_x,Cell_tendency);
title('<CellSize Tendency>');
xlim([100 800]);
xlabel('CellSize');

ax2 = subplot(2,2,2);
v_x = [2:0.1:10];
plot(ax2,v_x,v_tendency);
title('<Velocity Tendency>');
xlim([2 10]);
xlabel('Velocity');

ax3 = subplot(2,2,3);
d_x = [100:10:7500];
plot(ax3, d_x,d_tendency);
title('<Avg Distance Tendency>');
xlim([100 7500]);
xlabel('Avg Distance before turn');

ax4 = subplot(2,2,4);
Call_x = [2:0.1:10];
plot(ax4, Call_x,Call_tendency);
title('<Avg Calltime Tendency>');
xlim([2 10]);
xlabel('Avg Calltime');

end

