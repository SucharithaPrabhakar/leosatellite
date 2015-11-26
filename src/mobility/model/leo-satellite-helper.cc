#include "ns3/simulator.h"
#include "ns3/log.h"
#include "leo-satellite-mobility-model.h"

#define PI 3.1415926535897
#define MU 398601.2 // Greek Mu (km^3/s^2)
#define LIGHT 299793 // km/s
#define EARTH_PERIOD 86164 // seconds
#define EARTH_RADIUS 6378  // km
#define GEO_ALTITUDE 35786 // km
#define ATMOS_MARGIN 150 // km

#define DEG_TO_RAD(x) ((x) * PI/180)
#define RAD_TO_DEG(x) ((x) * 180/PI)
#define DISTANCE(s_x, s_y, s_z, e_x, e_y, e_z) (sqrt((s_x - e_x) * (s_x - e_x) \
                + (s_y - e_y) * (s_y - e_y) + (s_z - e_z) * (s_z - e_z)))
namespace ns3 {

NS_LOG_COMPONENT_DEFINE("LEOSatelliteHelper");

LEOSatSphericalPos
LEOSatelliteHelper::computeCurPos(double timeAdvance) const
{
  double partial; // fraction of orbit period completed
  LEOSatSphericalPos initialSphericalPos =  m_pos;
  partial = (fmod(timeAdvance, initialSphericalPos.period)/initialSphericalPos.period) * 2*PI; //rad

  //Sucharitha: timeadvance is in seconds. Verify if that is correct.

  LEOSatSphericalPos curSphericalPos, newSphericalPos;
  curSphericalPos.theta = fmod(initialSphericalPos.theta + partial, 2*PI);
  curSphericalPos.phi = initialSphericalPos.phi;
  NS_ASSERT(initialSphericalPos.inclination < PI);
  newSphericalPos.theta = PI/2 - asin(sin(initialSphericalPos.inclination) * sin(curSphericalPos.theta));
  if  ((curSphericalPos.theta > PI/2) && (curSphericalPos.theta < 3*PI/2))
  {
    newSphericalPos.phi = atan(cos(initialSphericalPos.inclination) * tan(curSphericalPos.theta)) + curSphericalPos.phi + PI; 
  }
  else
  {
    newSphericalPos.phi = atan(cos(initialSphericalPos.inclination) * tan(curSphericalPos.theta)) + curSphericalPos.phi;  
  }
  
  newSphericalPos.phi = fmod(newSphericalPos.phi + 2*PI, 2*PI);
  curSphericalPos.r = initialSphericalPos.r;
  curSphericalPos.theta = newSphericalPos.theta;
  curSphericalPos.phi = newSphericalPos.phi;
  return curSphericalPos;
}

LEOSatSphericalPos
LEOSatelliteHelper::convertPolarToSpherical(const LEOSatPolarPos &polarPos)
{
   NS_LOG_FUNCTION (this << polarPos.altitude << polarPos.longitude <<    polarPos.alpha << polarPos.inclination);
   LEOSatSphericalPos sphericalPos;
   // Check validity of passed values.
   if (polarPos.altitude < 0)
   {
      NS_LOG_FUNCTION(this << "Altitude out of bounds");
   }
   if ((polarPos.longitude < -180) || (polarPos.longitude) > 180)
   {
      NS_LOG_FUNCTION(this << "Longitude out of bounds");
   }
   if ((polarPos.alpha < 0) || (polarPos.alpha >= 360))
   {
      NS_LOG_FUNCTION(this << "Alpha out of bounds");
   }
   if ((polarPos.inclination < 0) || (polarPos.inclination > 180))
   {
      NS_LOG_FUNCTION(this << "Inclination out of bounds");
   }

   m_paused = true;
   sphericalPos.r = polarPos.altitude + EARTH_RADIUS;
   sphericalPos.theta = DEG_TO_RAD(polarPos.alpha);
   if (polarPos.longitude < 0)
   {
     sphericalPos.phi = DEG_TO_RAD(360 + polarPos.longitude);
   }
   else
   {
      sphericalPos.phi = DEG_TO_RAD(polarPos.longitude);
   }
   sphericalPos.inclination = DEG_TO_RAD(polarPos.inclination);
   double num = sphericalPos.r * sphericalPos.r * sphericalPos.r;
   sphericalPos.period = 2 * PI * sqrt(num/MU);
   return sphericalPos;
}

LEOSatelliteHelper::LEOSatelliteHelper()
     : m_paused (true)
{
   NS_LOG_FUNCTION (this);
}

// Sucharitha: Make sure that function aborts with invalid values
LEOSatelliteHelper::LEOSatelliteHelper (const LEOSatPolarPos &polarPos)
{
   NS_LOG_FUNCTION (this << polarPos.altitude << polarPos.longitude <<    polarPos.alpha << polarPos.inclination);

   // Check validity of passed values.
   if (polarPos.altitude < 0)
   {
      NS_LOG_FUNCTION(this << "Altitude out of bounds");
   }
   if ((polarPos.longitude < -180) || (polarPos.longitude) > 180) 
   {
      NS_LOG_FUNCTION(this << "Longitude out of bounds");
   }
   if ((polarPos.alpha < 0) ||(polarPos.alpha >= 360))
   {
      NS_LOG_FUNCTION(this << "Alpha out of bounds");
   }
   if ((polarPos.inclination < 0) || (polarPos.inclination > 180))
   {
      NS_LOG_FUNCTION(this << "Inclination out of bounds");
   }

  m_paused = true;
  m_pos.r = polarPos.altitude + EARTH_RADIUS;
  m_pos.theta = DEG_TO_RAD(polarPos.alpha);
  if (polarPos.longitude < 0)
  {
     m_pos.phi = DEG_TO_RAD(360 + polarPos.longitude);
  }
  else
  {
      m_pos.phi = DEG_TO_RAD(polarPos.longitude);
  }
  m_pos.inclination = DEG_TO_RAD(polarPos.inclination);
  double num = m_pos.r * m_pos.r * m_pos.r;
  m_pos.period = 2 * PI * sqrt(num/MU);
}


void
LEOSatelliteHelper::SetPos(const LEOSatPolarPos& pos)
{
   NS_LOG_FUNCTION (this << pos.altitude << pos.longitude << pos.alpha << pos.inclination << pos.plane);
   m_pos = convertPolarToSpherical(pos);
   m_lastUpdate = Simulator::Now();
}

LEOSatSphericalPos
LEOSatelliteHelper::GetCurrentPos(void) const
{
  NS_LOG_FUNCTION (this);
  return m_pos;
}

Vector
LEOSatelliteHelper::GetVelocity (void) const
{
  NS_LOG_FUNCTION (this);
  return m_paused ? Vector (0.0, 0.0, 0.0) : m_velocity;
}

void
LEOSatelliteHelper::Update (void) const
{
  NS_LOG_FUNCTION (this);
  Time now = Simulator::Now();
  NS_ASSERT(m_lastUpdate <= now);
  Time deltaTime = now - m_lastUpdate;
  m_lastUpdate = now;
  if (m_paused)
  {
    return;
  }
  double deltaS = deltaTime.GetSeconds();
  m_pos = computeCurPos(deltaS);
}

void
LEOSatelliteHelper::Pause (void)
{
   NS_LOG_FUNCTION (this);
   m_paused = true;
}


void
LEOSatelliteHelper::Unpause (void)
{
   NS_LOG_FUNCTION (this);
   m_paused = false;
}
 
} // namespace ns3

