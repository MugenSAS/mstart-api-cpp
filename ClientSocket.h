#ifndef _CLIENT_SOCKET_H_
#define _CLIENT_SOCKET_H_

#include <QtNetwork>
#include <QObject>
#include <QString>

class QTcpSocket;

// A very basic class to create a client socket to connect to an M-START Server
class ClientSocket: public QObject
{
    Q_OBJECT
public:
    ClientSocket(QObject* parent = 0);
    virtual ~ClientSocket();
	// Starts the connection process with the M-START Server
    void start(QString address, quint16 port);

public slots:
	// Sends raw messages (already OSC formatted messages)
    void send( QByteArray* bytes );
	// Called each time a new message is ready to processed and emit the signal messageReady when the message is fully received
    QByteArray* receive();
	// Emits the signal socketConnected when the socket has completed its connection process with the M-START Server
    void connected();
	// Called each time an error occurs on the socket
    void error(QAbstractSocket::SocketError err);

Q_SIGNALS:
    // Signal emitted every time an OSC message is ready to be processed
    void messageReady( QByteArray* rawMessage );
	// Signal emitted when the socket is connected to the M-START Server
    void socketConnected();

private:
    QByteArray* mRawMessage;
    QTcpSocket* mSocket;
    qint32 mMessageSize; // The size of the received message
    qint32 mMessageSizeLeft; // The size left before the message is complete
    qint32 mRBytes; // The number of received bytes
    qint32 mSBytes; // The number of sent bytes
};

#endif // _CLIENT_SOCKET_H_
