#include <Robot.h>

#include <osc/composer/OscMessageComposer.h>
#include <osc/reader/OscMessage.h>

#include <DataManager.h>

#include <QString>

Robot::Robot( DataManager* dm, QObject* parent )
: mDataManager( dm )
{
	// Initialize here everything needed to establish the dialog between your connected system and the M-START Server

	connect( mDataManager, SIGNAL(messageReceived(OscMessage*)), this, SLOT(manageMessage(OscMessage*)) );
}

Robot::~Robot()
{
	// Clean anything that needs to be cleaned
}

void 
Robot::manageMessage( OscMessage* msg )
{
	if ( msg == NULL )
        return;
		
	if( msg->getAddress().startsWith("/client-send/...") )
	{
		// Process the message
	}
	else if( msg->getAddress().startsWith("/client-send/....") )
	{
		// Process the message a different way
	}
	else
		// And so on
}
