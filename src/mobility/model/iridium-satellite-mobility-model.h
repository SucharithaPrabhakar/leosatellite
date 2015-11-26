#ifndef IRIDIUM_SATELLITE_MOBILITY_MODEL_H
#define IRIDIUM_SATELLITE_MOBILITY_MODEL_H

#include "mobility-model.h"
#include "ns3/nstime.h"

struct coordinate {
        double r;        // km
        double theta;    // radians
        double phi;      // radians
};

class IridiumSatelliteMobilityModel : public MobilityModel
{

public:
  /**
   * Register this type with the TypeId system.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  IridiumSatelliteMobilityModel();
  virtual ~IridiumSatelliteMobilityModel();

  void SetSatPosition (int satellitenum, int altitude, int azimuth, Time m_lastUpdate);
  void SetCurPosition(Time m_curTime);
private:
  // Satellite num should indicate the orbital plane, and also enable recognition of it's neighbours.
  int m_satellitenum;
  //Altitude (Alt), sometimes referred to as elevation, is the angle
  //between the object and the observer's local horizon. For visible objects 
  //it is an angle between 0 degrees to 90 degrees.
  int m_altitude;
  //Azimuth (Az), that is the angle of the object around the horizon, usually 
  //measured from the north increasing towards the east
  int m_azimuth;
  //Time at which this position was updated
  Time m_lastUpdate;
};

} // namenamespace ns3


