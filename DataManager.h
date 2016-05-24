#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include <QObject>

class ClientSocket;
class OscMessage;

// Convinient class to manage connection, sending and receiving messages from an M-START Server
class DataManager : public QObject
{
    Q_OBJECT
public:
	// Constructor
    DataManager( QObject* parent = NULL, QString serverIP="localhost", QString activty="demo" );
    virtual ~DataManager();

	// Convert a raw byte message to an OSC message
    OscMessage* toOscMessage( QByteArray* rawMessage );
	// Sends a raw message
    void send( QByteArray* bytes );
	// Sends a boolean
    void sendBool(QString address, bool b);
	// Sends a float
    void sendFloat(QString address, float v);
	// Sends a double
    void sendDouble(QString address, double d);
	// Sends the activity link to the server
    Q_INVOKABLE void sendActivityLink(QString activity);

public Q_SLOTS:
	// Called each time a new OSC message is received. You either re-implement this method to directly deal with the messages or use the signal in your own class to do so
    virtual void manageReceived( QByteArray* );
	// Called when the connection is set with the M-START Server. Sends the activity link.
    void tcpSocketConnected();

Q_SIGNALS:
	// Signal emitted each a new message is received.
    void messageReceived( OscMessage* );

private:
    ClientSocket* mClient;
    OscMessage* mOscMessage;
    QString mActivity;
};

#endif // _DATA_MANAGER_H_
