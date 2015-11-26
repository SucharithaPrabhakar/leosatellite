#include "ns3/simulator.h"
#include "ns3/log.h"
#include "leo-satellite-mobility-model.h"

namespace ns3 {

//NS_LOG_COMPONENT_DEFINE("LEOSatelliteHelper");

LEOSatSphericalPos
LEOSatelliteHelper::computeCurPos(double timeAdvance)
{
  double partial; // fraction of orbit period completed
  LEOSatSphericalPos initialSphericalPos =  m_pos;
  //partial = (fmod(NOW() + timeAdvance, period)/period) * 2*PI; //rad

 //Sucharitha: timeadvance is in seconds. Verify if that is correct.

  LEOSatSphericalPos curSphericalPos, newSphericalPos;
  curSphericalPos.theta = fmod(initialSphericalPos.theta + partial, 2*PI);
  curSphericalPos.phi = initialSphericalPos.phi;
  //assert(inclination < PI);
  newSphericalPos.theta = PI/2 - asin(sin(inclination) * sin(curSphericalPos.theta));
  /*if  ((curSphericalPos.theta > PI/2) && (curSphericalPos.theta < 3*PI/2))
  {
    newSphericalPos.phi = atan(cos(inclination) * tan(curSphericalPos.theta)) + curSphericalPos.phi + PI; 
  }
  else
  {
    newSphericalPos.phi = atan(cos(inclination) * tan(curSphericalPos.theta)) + curSphericalPos.phi;  
  }
  */
  //newSphericalPos.phi = fmod(newSphericalPos.phi + 2*PI, 2*PI);
  curSphericalPos.r = initialSphericalPos.r;
  curSphericalPos.theta = newSphericalPos.theta;
  curSphericalPos.phi = newSphericalPos.phi;
  return curSphericalPos;
}

LEOSatelliteHelper::LEOSatelliteHelper()
     : m_paused (true)
{
   //NS_LOG_FUNCTION (this);
}

// Sucharitha: Make sure that function aborts with invalid values
LEOSatelliteHelper::LEOSatelliteHelper (const LEOSatPolarPos &polarPos)
{
   NS_LOG_FUNCTION (this << polarPos.altitude << polarPos.longitude <<    polarPos.alpha << polarPos.inclination);

   // Check validity of passed values.
   if (polarPos.altitude < 0)
   {
      //NS_LOG_FUNCTION(this << "Altitude out of bounds");
   }
   if ((polarPos.longitude < -180) || (polarPos.longitude) > 180) 
   {
     //NS_LOG_FUNCTION(this << "Longitude out of bounds");
   }
   if ((polarPos.alpha < 0) || polarPos.alpha >= 360))
   {
      //NS_LOG_FUNCTION(this << "Alpha out of bounds");
   }
   if ((polarPos.Incl < 0) || (polarPos.Incl > 180))
   {
     //NS_LOG_FUNCTION(this << "Inclination out of bounds");
   }

  m_paused = true;
  LEOSatSphericalPos m_pos;
  //m_pos.r = polarPos.altitude + EARTH_RADIUS;
  //m_pos.theta = DEG_TO_RAD(polarPos.alpha);
  if (polarPos.longitude < 0)
  {
       //m_pos.phi = DEG_TO_RAD(360 + polarPos.longitude);
  }
  else
  {
       //m_pos.phi = DEG_TO_RAD(polarPos.longitude);
  }
  //inclination = DEG_TO_RAD(polarPos.inclination);
  double num = m_pos.r * m_pos.r * m_pos.r;
  //period = 2 * PI * sqrt(num/MU);
}


void
LEOSatelliteHelper::SetPos(const LEOSatPolarPos& pos)
{
   //NS_LOG_FUNCTION (this << pos.altitude << pos.r << pos.theta << pos.phi);
   m_pos = convertPolarToSpherical(pos);
   m_lastUpdate = Simulator::Now ();
}

LEOSatPolarPos
LEOSatelliteHelper::GetCurrentPos(void) const
{
  //NS_LOG_FUNCTION (this);
  return m_pos;
}

Vector
ConstantVelocityHelper::GetVelocity (void) const
{
  //NS_LOG_FUNCTION (this);
  return m_paused ? Vector (0.0, 0.0, 0.0) : m_velocity;
}

void
LEOSatelliteHelper::Update (void) const
{
  NS_LOG_FUNCTION (this);
  Time now = Simulator::Now ();
  NS_ASSERT (m_lastUpdate <= now);
  Time deltaTime = now - m_lastUpdate;
  m_lastUpdate = now;
  if (m_paused)
  {
    return;
  }
  double deltaS = deltaTime.GetSeconds ();
  m_pos = computeCurPos(deltaS);
}

void
LEOSatelliteHelper::Pause (void)
{
   //NS_LOG_FUNCTION (this);
   m_paused = true;
}


void
LEOSatelliteHelper::Unpause (void)
{
   //NS_LOG_FUNCTION (this);
   m_paused = false;
}
 
} // namespace ns3

