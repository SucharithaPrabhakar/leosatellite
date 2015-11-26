#include "leo-satellite-mobility-model.h"
#include "ns3/simulator.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LEOSatelliteMobilityModel);

TypeId LEOSatelliteMobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LEOSatelliteMobilityModel")
      .SetParent<MobilityModel> ()
      .SetGroupName ("Mobility")
      .AddConstructor<LEOSatelliteMobilityModel> ();
      return tid;
}

LEOSatelliteMobilityModel::LEOSatelliteMobilityModel ()
{
}
 
LEOSatelliteMobilityModel::~LEOSatelliteMobilityModel ()
{
}

LEOSatSphericalPos
LEOSatelliteMobilityModel::DoGetSphericalPos(void) const
{
   m_helper.Update();
   return m_helper.GetCurrentPos();
}

void
LEOSatelliteMobilityModel::DoSetSphericalPos(const LEOSatPolarPos& pos)
{
  m_helper.SetPos(pos);
  NotifyCourseChange();
}

Vector
LEOSatelliteMobilityModel::DoGetPosition (void) const
{
  m_helper.Update ();
  return Vector(0.0, 0.0, 0.0); //m_helper.GetCurrentPos();
}
void
LEOSatelliteMobilityModel::DoSetPosition (const Vector &position)
{
  //m_helper.SetPos(position);
  //NotifyCourseChange();
}

Vector
LEOSatelliteMobilityModel::DoGetVelocity (void) const
{
  return m_helper.GetVelocity ();
}


} // namespace ns3

