clc;clear;

Cell_tendency = load('Cellsize_tendency.txt');
v_tendency = load('Velocity_tendency.txt');
d_tendency = load('Distance_tendency.txt');
Call_tendency = load('Calltime_tendency.txt');

handoff_tendency(Cell_tendency,v_tendency,d_tendency,Call_tendency);

