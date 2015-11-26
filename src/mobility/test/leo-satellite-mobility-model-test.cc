#include <ns3/test.h>
#include <ns3/log.h>

/**
 * This test verifies the accuracy of the conversion from Polar coordinates to
 * spherical coordinates
**/

NS_LOG_COMPONENT_DEFINE ("PolarToSphericalTest");

using namespace ns3;

// Some ammount tolerance for testing
const double TOLERANCE = 10;

const double altitude = 100;
const double longitude = 54.8;
const double alpha = 60;
const double inclination = 45; 
const int plane = 2;

const double expected_r = 0;
const double expected_theta = 0;
const double expected_phi = 0;


class LEOSatellitePolarToSPhericalTestCase: public TestCase
{
public:
  LEOSatellitePolarToSPhericalTestCase (double latitude,
                                        double longitude,
                                        double alpha,
                                        double inclination,
					double plane,
                                        int i);
  virtual ~LEOSatellitePolarToSPhericalTestCase();

private:
  virtual void DoRun (void);
  static std::string Name (double latitude,
                           double longitude,
			   double alpha,
                           double inclination,
                           double plane);
  double m_latitude;
  double m_longitude;
  double m_alpha;
  double m_inclination;
  double m_plane;
  int m_i;
};

std::string
LEOSatellitePolarToSPhericalTestCase::Name (double latitude,
                                            double longitude,
                                            double alpha,
                                            double inclination,
                                            double plane)
{
  std::ostringstream oss;
  oss << "latitude = " << latitude << " degrees, "
      << "longitude = " << longitude << " degrees, "
      << "alpha = " << altpha << " meters, "
      << "inclination = " << inclination << " degrees, "
      << "plane = " << plane << " number, "
  return oss.str();
}

LEOSatellitePolarToSPhericalTestCase::LEOSatellitePolarToSPhericalTestCase (double latitude,
                                            double longitude,
                                            double alpha,
                                            double inclination,
                                            double plane,
					    int i)
    : TestCase (Name (latitude, longitude, alpha, inclination, plane)),
    m_latitude (latitude),
    m_longitude (longitude),
    m_alpha (alpha),
    m_inclination (inclination)
    m_plane (plane),
    m_i (i)
{
}

LEOSatellitePolarToSPhericalTestCase::~LEOSatellitePolarToSPhericalTestCase()
{
}

void
LEOSatellitePolarToSPhericalTestCase::DoRun (void)
{
   struct LEOSatPolarPos pPos(m_latitude, m_longitude, m_alpha, m_inclination, m_plane);
   LEOSatelliteMobilityModel::DoSetPos(pPos);
   struct LEOSatSphericalPos spPos= LEOSatelliteMobilityModel::GetPos();

   NS_TEST_ASSERT_MSG_EQ_TOL (spPos.r,
                              expected_r,
                              TOLERANCE,
                              "r for (" << spPos.r << ") is incorrect "
                              "in iteration" << m_i);

   NS_TEST_ASSERT_MSG_EQ_TOL (spPos.theta,
                              expected_theta,
                              TOLERANCE,
                              "r for (" << spPos.theta << ") is incorrect "
                              "in iteration" << m_i);
 
   NS_TEST_ASSERT_MSG_EQ_TOL (spPos.phi,
                              expected_phi,
                              TOLERANCE,
                              "r for (" << spPos.phi << ") is incorrect "
                              "in iteration" << m_i);
}


class LEOSatellitePolarToSPhericalTestSuite : public TestSuite
{
public:
  LEOSatellitePolarToSPhericalTestSuite ();
};


LEOSatellitePolarToSPhericalTestSuite::LEOSatellitePolarToSPhericalTestSuite()
  : TestSuite ("polar-to-spherical", UNIT)
{
  NS_LOG_INFO ("creating PolarToSPhericalTestSuite");
  int i = 0; // iteration number
  AddTestCase (new LEOSatellitePolarToSPhericalTestCase (latitude,
                                                         longitude,
                                                         alpha,
                                                         inclination,
                                                         plane
                                                         i),
                           TestCase::QUICK);
}

static LEOSatellitePolarToSPhericalTestSuite g_LEOSatellitePolarToSPhericalTestSuite;

