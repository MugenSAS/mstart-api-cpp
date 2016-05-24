#ifndef _ROBOT_H_
#define _ROBOT_H_

#include <QObject>

class DataManager;

// Class stub to show how to connect to an M-START Server
class Robot
{
    Q_OBJECT
public:
	// Constructor
    Robot( DataManager* dm, QObject* parent = NULL );
    virtual ~Robot();

public Q_SLOTS:
	// Handles received messages from the M-START Server
    virtual void manageMessage( OscMessage* );

private:
    DataManager* mDataManager;
};

#endif // _ROBOT_H_
