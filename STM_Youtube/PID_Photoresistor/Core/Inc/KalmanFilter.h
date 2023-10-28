#include "stdio.h"
#include "math.h"
#include "stm32f1xx_hal.h"

typedef struct{
	float _err_measure;
	float _err_estimate;
  float _q;
  float _current_estimate;
  float _last_estimate;
  float _kalman_gain;
}SimpleKalmanFilter;

void SimpleKalmanFilter_init(SimpleKalmanFilter* klm,float mea_e, float est_e, float q,float last_estm);
float updateEstimate(SimpleKalmanFilter* klm,float mea);
void setMeasurementError(SimpleKalmanFilter* klm,float mea_e);
void setEstimateError(SimpleKalmanFilter* klm,float est_e);
void setProcessNoise(SimpleKalmanFilter* klm,float q);
float getKalmanGain(SimpleKalmanFilter* klm);
float getEstimateError(SimpleKalmanFilter* klm);
