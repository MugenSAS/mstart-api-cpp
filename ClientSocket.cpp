#include <ClientSocket.h>
#include <QHostAddress>
#include <osc/composer/OscMessageComposer.h>
#include <tools/ByteBuffer.h>

ClientSocket::ClientSocket(QObject* parent)
: QObject(parent)
, mRawMessage(NULL)
, mSocket(NULL)
, mMessageSize(0)
, mMessageSizeLeft(0)
, mRBytes(0)
, mSBytes(0)
{
    mSocket = new QTcpSocket( parent );
    mSocket->setSocketOption( QTcpSocket::LowDelayOption, 1 );
    this->connect( mSocket, SIGNAL(connected()), this, SLOT(connected()) );
    this->connect( mSocket, SIGNAL(readyRead()), this, SLOT(receive()) );
    this->connect( mSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)) );
}

ClientSocket::~ClientSocket()
{
  mSocket->close();
  delete mSocket;
}

void
ClientSocket::start(QString address, quint16 port)
{
  QHostAddress addr(address);
qDebug()<<"host : "<<address;
  mSocket->connectToHost(addr, port);
}

void
ClientSocket::send( QByteArray* bytes )
{
    qDebug() << "ClientSocket::send";
    mSocket->write( bytes->data(), bytes->size() );
}

QByteArray*
ClientSocket::receive()
{
    if ( mSocket->state() != QAbstractSocket::ConnectedState )
        return NULL;

    while( true )
    {
        int bytesToWrite = mSocket->bytesAvailable();
        if( bytesToWrite <= 4 )
            return NULL;

        // Get the size of the bloc and initialize a new raw message
        if( mMessageSizeLeft == 0 )
        {
            QDataStream in( mSocket );
            in.setVersion( QDataStream::Qt_4_0 );
            in >> mMessageSizeLeft;
            bytesToWrite -= 4;
            mMessageSize = 0;
            mRawMessage = new QByteArray();
            mRawMessage->resize( mMessageSizeLeft ); // Resize to receive data
        }

        int maxBytesToWrite = (mMessageSizeLeft < bytesToWrite) ? mMessageSizeLeft : bytesToWrite;

        if( maxBytesToWrite > 0 )
        {
            mSocket->read( (char*) mRawMessage->constData() + mMessageSize, maxBytesToWrite );
            mMessageSize += maxBytesToWrite;
            mMessageSizeLeft -= maxBytesToWrite;

            // The raw message is fully received, notify and exit loop
            if( mMessageSizeLeft == 0 )
            {
                mRBytes += mMessageSize;
                emit messageReady( mRawMessage );
            }
        }
    }

    return mRawMessage;
}

void
ClientSocket::connected()
{
    qDebug() << "Client: connected to TCP server: " << mSocket->peerAddress();
    emit socketConnected();
}

void
ClientSocket::error(QAbstractSocket::SocketError err)
{
    qDebug() << "Client: ERROR: " << err;
}
