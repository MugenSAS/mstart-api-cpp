#include <QGuiApplication>

#include <DataManager.h>
#include <Robot.h>

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);
  
  QString serverIP, activity; 
  
  serverIP = "192.168.0.1"; // The M-START Server IP address
  activity = "RobotHMI"; // The activity name
    
  DataManager* dm = new DataManager(NULL, serverIP, activity); // Initialize the data management
  Robot = new Robot(dm); // Initialize the class which will handle the interactions with the server with the data manager
    
  return app.exec(); // Run the application main loop
}
