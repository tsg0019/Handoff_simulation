#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

typedef struct Point {
	double x;
	double y;
} Point;

typedef struct Node {
	double x, y;
	double distance;
	double angle;
	double velocity;
} Node;
double operate10000(Node* node, double Cell_length, double avg_call_time, double avg_turn_distance, double vel_min, double vel_MAX, Point* point);
void Cellsize_tendency(Node* node, FILE* fp);
void velocity_tendency(Node* node, FILE* fp);
void distance_tendency(Node* node, FILE* fp);
void Calltime_tendency(Node* node, FILE* fp);
//void set_base(double* Cell_length, double* vel_min, double* vel_MAX, double* avg_turn_distance, double* avg_call_time);
double Rand_velocity();
double Rand_angle();
double exponential(double lambda);
double Rand_0to1();
double Rand_uniform(double start, double end);
double Rand_x();
double Rand_y();
int Nodecheck(Node* ptrx, double Cell_length);
void move_node(Node* node);
int onSegment(Point p, Point q, Point r);
int orientation(Point p, Point q, Point r);
int doIntersect(Point p1, Point q1, Point p2, Point q2);
int which_line_inter(int* line, double before_x, double before_y, Node* node, Point* point);
void parr_move(int* line, double* before_x, double* before_y, Node* node, double Cell_length);

int main() {
	FILE* fp = NULL;
	int buffer = 0;
	srand(time(NULL)); //or srand(time(NULL))
	struct Node node; //node구조체 선언

	double Cell_length;
	double vel_min, vel_MAX;
	double avg_turn_distance;
	double avg_call_time;



	Cellsize_tendency(&node, fp);
	velocity_tendency(&node, fp);
	distance_tendency(&node, fp);
	Calltime_tendency(&node, fp);

	return 0;
}

void Cellsize_tendency(Node* node, FILE* fp) {
	//fp = fopen("C:\\Users\\이정훈\\Documents\\Cellsize_tendency.txt", "w");
	fp = fopen("Cellsize_tendency.txt", "w");
	double vel_min = 2 * 1000 / 60.0; double vel_MAX = 6 * 1000 / 60.0;
	double avg_turn_distance = 100.0;
	double avg_call_time = 2.0;
	for (double Cell_size = 100; Cell_size <= 800; Cell_size += 10) {
		Point point[6] = {
		{Cell_size / 2 * sqrt(3) ,Cell_size / 2},
		{0,Cell_size},
		{ -Cell_size / 2 * sqrt(3) , Cell_size / 2},
		{ -Cell_size / 2 * sqrt(3) , -Cell_size / 2},
		{0, -Cell_size},
		{Cell_size / 2 * sqrt(3) ,  -Cell_size / 2}
		};
		fprintf(fp, "%lf,", operate10000(node, Cell_size, avg_call_time, avg_turn_distance, vel_min, vel_MAX, point));
	}
	fclose(fp);
}
void velocity_tendency(Node* node, FILE* fp) {
	fp = fopen("Velocity_tendency.txt", "w");
	Point point[6] = {
		{500.0 / 2 * sqrt(3) ,500.0 / 2},
		{0,500.0},
		{ -500.0 / 2 * sqrt(3) , 500.0 / 2},
		{ -500.0 / 2 * sqrt(3) , -500.0 / 2},
		{0, -500.0},
		{500.0 / 2 * sqrt(3) ,  -500.0 / 2}
	};
	double avg_turn_distance = 100.0;
	double avg_call_time = 2.0;
	for (double vel = 2; vel <= 10; vel += 0.1) {
		fprintf(fp, "%lf,", operate10000(node, 500.0, avg_call_time, avg_turn_distance, vel * 1000 / 60.6, (4 + vel) * 1000 / 60.6, point));
	}
	fclose(fp);
}
void distance_tendency(Node* node, FILE* fp) {
	fp = fopen("Distance_tendency.txt", "w");
	Point point[6] = {
		{500.0 / 2 * sqrt(3) ,500.0 / 2},
		{0,500.0},
		{ -500.0 / 2 * sqrt(3) , 500.0 / 2},
		{ -500.0 / 2 * sqrt(3) , -500.0 / 2},
		{0, -500.0},
		{500.0 / 2 * sqrt(3) ,  -500.0 / 2}
	};

	double vel_min = 2 * 1000 / 60.0; double vel_MAX = 6 * 1000 / 60.0;
	double avg_call_time = 2.0;
	for (double avg_turn_distance = 100; avg_turn_distance <= 1500; avg_turn_distance += 10) {
		fprintf(fp, "%lf,", operate10000(node, 500.0, avg_call_time, avg_turn_distance, vel_min, vel_MAX, point));
	}
	fclose(fp);
}
void Calltime_tendency(Node* node, FILE* fp) {
	fp = fopen("Calltime_tendency.txt", "w");
	Point point[6] = {
		{500.0 / 2 * sqrt(3) ,500.0 / 2},
		{0,500.0},
		{ -500.0 / 2 * sqrt(3) , 500.0 / 2},
		{ -500.0 / 2 * sqrt(3) , -500.0 / 2},
		{0, -500.0},
		{500.0 / 2 * sqrt(3) ,  -500.0 / 2}
	};
	double vel_min = 2 * 1000 / 60.0; double vel_MAX = 6 * 1000 / 60.0;
	double avg_turn_distance = 100;
	for (double avg_call_time = 2.0; avg_call_time <= 10.0; avg_call_time += 0.1) {
		fprintf(fp, "%lf,", operate10000(node, 500.0, avg_call_time, avg_turn_distance, vel_min, vel_MAX, point));
	}
	fclose(fp);
}
double operate10000(Node* node, double Cell_length, double avg_call_time, double avg_turn_distance, double vel_min, double vel_MAX, Point* point) {	//10000번 실행 후 평균값을 return
	double h_o_num = 0;
	for (int i = 0; i < 10000; i++) {
		do {									//균일하게 육각형 내에서 generation 하는 do while문
			node->x = Rand_x();
			node->y = Rand_y();
		} while (Nodecheck(node, Cell_length) == 0);
		double Tc = exponential(1.0 / avg_call_time);									//Tc is exponential R.V.
		int num_of_move = 1;
		do {
			node->distance = exponential(1.0 / avg_turn_distance);							//exponential R.V
			node->velocity = Rand_uniform(vel_min, vel_MAX);					//uniform R.V.	[2000m/60min,6000m/60min]
			node->angle = Rand_uniform(0.0, (double)2 * M_PI);				//uniform R.V.
			double Tc_before = Tc;	//Tc값 임시 저장
			Tc -= node->distance / node->velocity;							//Tc값 감소시킴
			if (Tc < 0) {
				double distance_ratio = Tc_before / (node->distance / node->velocity);	//임시저장 해 뒀던 Tc와 (distance/velocity)사이 관계를 구해서 이동하는 거리를 계산 후 Nodecheck
				node->distance *= distance_ratio;
				move_node(node);
				if (Nodecheck(node, Cell_length) == 1) {
					h_o_num++;
				}
				else {
					h_o_num++;
				}
			}
			else {
				double before_x = node->x;
				double before_y = node->y;
				move_node(node);												//R.V.에 따라 노드위치 이동
				int line = -1;													//처음에 닿는 line=-1로 초기화
				while (Nodecheck(node, Cell_length) == 0) {									//도착점이 CELL 밖에 있으면 
					which_line_inter(&line, before_x, before_y, node, point);	//어떤 변과 만나는지 체크
					parr_move(&line, &before_x, &before_y, node, Cell_length);				//평행이동시킴
					h_o_num++;
				}
			}
		} while (Tc >= 0);
	}
	// printf("평균 handoff 횟수 : %lf", (double)h_o_num / 10000);

	return h_o_num / 10000;
}
/*void set_base(double* Cell_length, double* vel_min, double* vel_MAX, double* avg_turn_distance, double* avg_call_time) {
	*Cell_length = *Cell_length;
	*vel_min = *vel_min * 1000 / 60.0;
	*vel_MAX = *vel_MAX * 1000 / 60.0;
	*avg_turn_distance
}*/
double Rand_0to1() {						// 0~1 Randomly generation
	return (double)rand() / 32767;
}
double Rand_uniform(double start, double end) {
	double uniform = Rand_0to1() * (end - start) + start;
	return uniform;
}
double Rand_velocity() {					// 속도 2~6km/h uniformly dist
	double velocity = Rand_0to1() * 4 + 2;
	return velocity;
}
double Rand_angle() {						// 0 ~ 2PI Randomly generation
	double angle = Rand_0to1() * 2 * M_PI;
	return angle;
}
double Rand_x() {
	double x = Rand_0to1() * 500 * sqrt(3) - 250 * sqrt(3);
	return x;
}
double Rand_y() {
	double y = Rand_0to1() * 1000 - 500;
	return y;
}
double exponential(double lambda) {
	double first = log(1.0 - 0.9999999999999 * Rand_0to1());
	double time = first / (-lambda);
	return time;
}
int Nodecheck(Node* ptr, double Cell_length) {
	if (ptr->x < (-Cell_length / 2) * sqrt(3)) { return 0; }
	else if (ptr->x > (Cell_length / 2) * sqrt(3)) { return 0; }
	else if ((ptr->y) >= ((ptr->x) / (-sqrt(3)) + Cell_length)) { return 0; }
	else if ((ptr->y) <= ((ptr->x) / (-sqrt(3)) - Cell_length)) { return 0; }
	else if ((ptr->y) >= ((ptr->x) / (sqrt(3)) + Cell_length)) { return 0; }
	else if ((ptr->y) <= ((ptr->x) / (sqrt(3)) - Cell_length)) { return 0; }
	else { return 1; }
	return 1;
}
void move_node(Node* node) {
	double x_move = node->distance * cos(node->angle);
	double y_move = node->distance * sin(node->angle);
	node->x += x_move;
	node->y += y_move;
}
int onSegment(Point p, Point q, Point r) {
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return 1;

	return 0;
}
int orientation(Point p, Point q, Point r)
{
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
	// for details of below formula. 
	double val = (q.y - p.y) * (r.x - q.x) -		//double로 내가 바꿈
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear 

	return (val > 0) ? 1 : 2; // clock or counterclock wise 
}
int doIntersect(Point p1, Point q1, Point p2, Point q2)
{
	// Find the four orientations needed for general and 
	// special cases 
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case 
	if (o1 != o2 && o3 != o4)
		return 1;

	// Special Cases 
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1 
	if (o1 == 0 && onSegment(p1, p2, q1)) return 1;

	// p1, q1 and q2 are colinear and q2 lies on segment p1q1 
	if (o2 == 0 && onSegment(p1, q2, q1)) return 1;

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2 
	if (o3 == 0 && onSegment(p2, p1, q2)) return 1;

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2 
	if (o4 == 0 && onSegment(p2, q1, q2)) return 1;

	return 0; // Doesn't fall in any of the above cases 
}
int which_line_inter(int* line, double before_x, double before_y, Node* node, Point* point) {
	Point before = { before_x, before_y };
	Point after = { node->x, node->y };
	if (*line == -1) {
		for (int i = 0; i < 6; i++) {
			if (doIntersect(before, after, point[i], point[(i + 1) % 6]) == 1) {
				*line = i;
				return *line;
			}
		}
	}
	else {
		for (int i = 0; i < (*line + 3) % 6; i++) {
			if (doIntersect(before, after, point[i], point[(i + 1) % 6])) {
				*line = i;
				return *line;
			}
		}
		for (int i = (*line + 3) % 6 + 1; i < 6; i++) {
			if (doIntersect(before, after, point[i], point[(i + 1) % 6])) {
				*line = i;
				return *line;
			}
		}
	}
	return *line;
}
void parr_move(int* line, double* before_x, double* before_y, Node* node, double Cell_length) {
	switch (*line) {
	case 0:
		//printf("line0와 닿은 것 확인\n");
		*before_x -= (Cell_length / 2) * sqrt(3);
		*before_y -= (Cell_length / 2) * 3;
		node->x -= (Cell_length / 2) * sqrt(3);
		node->y -= (Cell_length / 2) * 3;
		break;
	case 1:
		//printf("line1와 닿은 것 확인\n");
		*before_x += (Cell_length / 2) * sqrt(3);
		*before_y -= (Cell_length / 2) * 3;
		node->x += (Cell_length / 2) * sqrt(3);
		node->y -= (Cell_length / 2) * 3;
		break;
	case 2:
		//printf("line2와 닿은 것 확인\n");
		*before_x += Cell_length * sqrt(3);
		node->x += Cell_length * sqrt(3);
		break;
	case 3:
		//printf("line3와 닿은 것 확인\n");
		*before_x += (Cell_length / 2) * sqrt(3);
		*before_y += (Cell_length / 2) * 3;
		node->x += (Cell_length / 2) * sqrt(3);
		node->y += (Cell_length / 2) * 3;
		break;
	case 4:
		//printf("line4와 닿은 것 확인\n");
		*before_x -= (Cell_length / 2) * sqrt(3);
		*before_y += (Cell_length / 2) * 3;
		node->x -= (Cell_length / 2) * sqrt(3);
		node->y += (Cell_length / 2) * 3;
		break;
	case 5:
		//printf("line5와 닿은 것 확인\n");
		*before_x -= Cell_length * sqrt(3);
		node->x -= Cell_length * sqrt(3);
		break;
	}
}