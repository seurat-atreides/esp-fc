#ifndef _ESPFC_ACTUATOR_H_
#define _ESPFC_ACTUATOR_H_

#include "Model.h"
#include "Math.h"

namespace Espfc {

class Actuator
{
  public:
    Actuator(Model& model): _model(model) {}
    int begin()
    {
      _model.state.armed = false;
    }

    int update()
    {
      _model.state.stats.start(COUNTER_ACTUATOR);
      updateFlightMode();
      updateScaler();
      updateArming();
      _model.state.stats.end(COUNTER_ACTUATOR);
    }

  private:
    void updateScaler()
    {
      for(size_t i = 0; i < 3; i++)
      {
        short mode = _model.config.actuatorConfig[i];
        if(!mode) continue;
        short c = _model.config.actuatorChannels[i];
        float v = _model.state.input[c];
        float min = _model.config.actuatorMin[i];
        float max = _model.config.actuatorMax[i];
        float scale = Math::map3(v, -1.f, 0.f, 1.f, min, min < 0 ? 0.f : 1.f, max);
        for(size_t x = 0; x < AXES; x++)
        {
          if(
            x == AXIS_ROLL   && (mode & ACT_AXIS_ROLL)  ||
            x == AXIS_PITCH  && (mode & ACT_AXIS_PITCH) ||
            x == AXIS_YAW    && (mode & ACT_AXIS_YAW)   ||
            x == AXIS_THRUST && (mode & ACT_AXIS_THRUST)
          )
          {

            if(mode & ACT_INNER_P) _model.state.innerPid[x].pScale = scale;
            if(mode & ACT_INNER_I) _model.state.innerPid[x].iScale = scale;
            if(mode & ACT_INNER_D) _model.state.innerPid[x].dScale = scale;

            if(mode & ACT_OUTER_P) _model.state.outerPid[x].pScale = scale;
            if(mode & ACT_OUTER_I) _model.state.outerPid[x].iScale = scale;
            if(mode & ACT_OUTER_D) _model.state.outerPid[x].dScale = scale;

          }
        }
        if(mode & ACT_GYRO_THRUST) _model.state.gyroThrustScale = scale;
      }
    }

    void updateFlightMode()
    {
      float v = _model.state.input[_model.config.flightModeChannel];
      int i = v > 0.33f ? 2 : (v > -0.33f ? 1 : 0);
      _model.state.flightMode = _model.config.flightModes[i];
    }

    void updateArming()
    {
      if(!_model.state.gyroBiasValid) return;
      _model.state.armed = _model.state.flightMode != MODE_OFF;
    }

    Model& _model;
};

}

#endif
