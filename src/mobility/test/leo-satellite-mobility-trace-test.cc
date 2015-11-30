/**
 * This test verifies the accuracy of the position of terminal after a predefined time interval
 * Command to Run this test: ./test.py -v --suite=terminal-mobility --text=result.txt
**/
#include <ns3/test.h>
#include <ns3/log.h>
#include "ns3/simulator.h"
#include <ns3/terminal-mobility-model.h>

NS_LOG_COMPONENT_DEFINE ("VerifyCurrentLEOSatellitePosition");

using namespace ns3;
class LEOSatelliteMobilityTestCase: public TestCase
{
public:
  LEOSatelliteMobilityTestCase();
  virtual ~LEOSatelliteMobilityTestCase();

private:
  void TestPosition (LEOSatelliteSphericalPos expectedPos);
  virtual void DoRun (void);
  Ptr<LEOSatelliteMobilityModel> m_mob;
};


LEOSatelliteMobilityTestCase::LEOSatelliteMobilityTestCase()
 : TestCase ("Test behavior for position after 5 seconds")
{
}

LEOSatelliteMobilityTestCase::~LEOSatelliteMobilityTestCase()
{
}

void LEOSatelliteMobilityTestCase::TestPosition(LEOSatSphericalPos expectedPos)
{
   LEOSatSphericalPos pos= m_mob->DoGetSatSphericalPos();
   NS_TEST_ASSERT_MSG_EQ_TOL (pos.r,
                              expectedPos.r,
                              0.001,
                              "r for (" << pos.r << ") is incorrect "
                              );

   NS_TEST_ASSERT_MSG_EQ_TOL (pos.theta,
                              expectedPos.theta,
                              0.001,
                              "theta for (" << pos.theta << ") is incorrect "
                              );

   NS_TEST_ASSERT_MSG_EQ_TOL (pos.phi,
                              expectedPos.phi,
                              0.001,
                              "phi for (" << pos.phi << ") is incorrect "
                              );

}

void LEOSatelliteMobilityTestCase::DoRun(void)
{
  struct LEOSatPolarPos initialpos;
  struct LEOSatSphericalPos expectedPos;
  initialpos.latitude=58.4;
  initialpos.longitude=58.4;
  expectedPos.r=6378;
  expectedPos.theta=0.551524;
  expectedPos.phi=1.019636606;
  expectedPos.period=86164;
  m_mob = CreateObject<LEOSatelliteMobilityModel> ();
  m_mob->DoSetSatSphericalPos(initialpos);
  Simulator::Schedule (Seconds (5.0), &LEOSatelliteMobilityTestCase::TestPosition, this, expectedPos);
  Simulator::Run ();
  Simulator::Destroy ();
}

class LEOSatelliteMobilityTestSuite : public TestSuite
{
public:
  LEOSatelliteMobilityTestSuite ();
};


TerminalMobilityTestSuite::TerminalMobilityTestSuite()
  : TestSuite ("terminal-mobility", UNIT)
{
  //NS_LOG_INFO ("creating TerminalMobilityTestSuite");
  //int i = 0; iteration number
  AddTestCase (new TerminalMobilityTestCase, TestCase::QUICK);
}

static TerminalMobilityTestSuite g_TerminalMobilityTestSuite;
