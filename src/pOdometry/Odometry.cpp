/************************************************************/
/*    NAME: William Rodriguez                                              */
/*    ORGN: MIT                                             */
/*    FILE: Odometry.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Odometry.h"

using namespace std;

//---------------------------------------------------------
// Constructor

Odometry::Odometry()
{
    m_first_reading = true;
    // All values are set to zero at beginning since heading from origin
    m_current_x = 0;
    m_current_y = 0;
    m_previous_x = 0;
    m_previous_y = 0;
    m_total_distance = 0;
}

//---------------------------------------------------------
// Destructor

Odometry::~Odometry()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Odometry::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);        // Add this line
  MOOSMSG_LIST::iterator p;
    
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

//#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
      
      if (key == "NAV_X"){
          //reportEvent("NAV_X message read: " + msg.GetDouble());
          m_previous_x = m_current_x;
          m_current_x = msg.GetDouble();
      }
      else if (key == "NAV_Y"){
          //reportEvent("NAV_Y message read: " + msg.GetDouble());
          m_previous_y = m_current_y;
          m_current_y = msg.GetDouble();
      }
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Odometry::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
    
    // Registers variables on connection to server
   RegisterVariables();


   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Odometry::Iterate()
{
    AppCastingMOOSApp::Iterate(); //Added this line for AppCast
  // first reading, don't do anything out of recognizing this
    if (m_first_reading){
        m_first_reading = false;
    } else{
        double xDisplacement = (m_previous_x - m_current_x);
        double yDisplacement = (m_previous_y - m_current_y);
        
        m_total_distance += sqrt((xDisplacement*xDisplacement)+(yDisplacement*yDisplacement));
        Notify("ODOMETRY_DIST",m_total_distance);
    }
  AppCastingMOOSApp::PostReport(); //added this for AppCast
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Odometry::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();   //added this line
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      if(param == "foo") {
        //handled
      }
      else if(param == "bar") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Odometry::RegisterVariables()
{
    AppCastingMOOSApp::RegisterVariables();      // Add this line
  // Register variables here, second input is optional;
    Register("NAV_X",0);
    Register("NAV_Y",0);

}

 bool Odometry::buildReport()
{
    m_msgs << "Total distance traveled: " << m_total_distance << endl;

    return(true);
}

