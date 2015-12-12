#ifndef LEO_SATELLITE_MOBILITY_MODEL_H
#define LEO_SATELLITE_MOBILITY_MODEL_H

#include <stdint.h>
#include "ns3/nstime.h"
#include "mobility-model.h"
#include "leo-satellite-helper.h"
 
namespace ns3 {

class LEOSatelliteMobilityModel : public MobilityModel
{

public:
   static TypeId GetTypeId (void);
   LEOSatelliteMobilityModel ();
   virtual ~LEOSatelliteMobilityModel ();
   LEOSatelliteHelper m_helper;
   void setSatSphericalPos(const LEOSatPolarPos& polarPos);
   virtual LEOSatSphericalPos DoGetSatSphericalPos(void) const;
   virtual void DoSetSatSphericalPos(const LEOSatPolarPos& polarPos);
private:
   virtual Vector DoGetPosition (void) const;
   virtual void DoSetPosition (const Vector &position);
   virtual Vector DoGetVelocity (void) const;
   //LEOSatelliteHelper m_helper;
};

} //namespace ns3

#endif /* LEO_SATELLITE_MOBILITY_MODEL*/

