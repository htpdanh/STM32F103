#include "KalmanFilter.h"


void SimpleKalmanFilter_init(SimpleKalmanFilter* klm,float mea_e, float est_e, float q,float last_estm)
{
  klm->_err_measure=mea_e;
  klm->_err_estimate=est_e;
  klm->_q = q;
	klm->_current_estimate = 0;
	klm->_last_estimate = last_estm;
	klm->_kalman_gain = 0;
}

float updateEstimate(SimpleKalmanFilter* klm,float mea)
{
  klm->_kalman_gain = klm->_err_estimate/(klm->_err_estimate + klm->_err_measure);
  klm->_current_estimate =klm->_last_estimate + klm->_kalman_gain * (mea - klm->_last_estimate);
  //klm->_err_estimate =  (1.0f - klm->_kalman_gain)*klm->_err_estimate + fabsf(klm->_last_estimate-klm->_current_estimate)*klm->_q;
  klm->_err_estimate =  (1.0f - klm->_kalman_gain)*klm->_err_estimate + klm->_q;
  klm->_last_estimate = klm->_current_estimate;
  return klm->_current_estimate;
}

void setMeasurementError(SimpleKalmanFilter* klm, float mea_e)
{
  klm->_err_measure=mea_e;
}

void setEstimateError(SimpleKalmanFilter* klm, float est_e)
{
  klm->_err_estimate=est_e;
}

void setProcessNoise(SimpleKalmanFilter* klm, float q)
{
  klm->_q=q;
}

float getKalmanGain(SimpleKalmanFilter* klm) {
  return klm->_kalman_gain;
}

float getEstimateError(SimpleKalmanFilter* klm) {
  return klm->_err_estimate;
}