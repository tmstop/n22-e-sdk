#ifndef __FILTER_H
#define __FILTER_H
  /**************************************************************************

**************************************************************************/
extern float angle, angle_dot; 	
extern  float angle_X,angle_Y;
void Kalman_Filter(float Accel,float Gyro);		
void Yijielvbo_X(float angle_m, float gyro_m);
void Yijielvbo_Y(float angle_m, float gyro_m);
#endif

