#ifndef LEO_SATELLITE_HELPER_H
#define LEO_SATELLITE_HELPER_H
 
#include "ns3/nstime.h"
#include "ns3/vector.h"
#include "leo-satellite-helper.h"
#include "mobility-model.h"

namespace ns3 {

//struct LEOSatPolarPos;
//struct  LEOSatSphericalPos;

class LEOSatelliteHelper
{
public:
   LEOSatelliteHelper();
   
   // Take the polar coordinate values as input and set the position in spherical system.
   LEOSatelliteHelper(const LEOSatPolarPos &polarPos);
   
   void SetPos(const LEOSatPolarPos &polarPos);
      
   // Returns the position in spherical system
   LEOSatSphericalPos GetCurrentPos(void) const;
 
   Vector GetVelocity (void) const;
   
   // Pause mobility at current position
   void Pause (void);
   
   // Pause mobility at current position
   void Unpause (void);

   // Pause mobility at current position
   void Update (void) const;
   LEOSatSphericalPos computeCurPos(double timeAdvance) const;
   LEOSatSphericalPos convertPolarToSpherical(const LEOSatPolarPos &polarPos);
private:
   mutable Time m_lastUpdate; //!< time of last update
   mutable LEOSatSphericalPos m_pos; //!< The position of the satellite in Spherical coordinates.
   Vector m_velocity; //!< state variable for velocity
   bool m_paused;  //!< state variable for paused
};
 
} // namespace ns3
 
#endif /* LEO_SATELLITE_HELPER_H */
