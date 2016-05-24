#include <DataManager.h>
#include <ClientSocket.h>
#include <osc/reader/OscReader.h>
#include <osc/reader/OscMessage.h>
#include <osc/composer/OscMessageComposer.h>
#include <tools/ByteBuffer.h>

DataManager::DataManager( QObject* parent, QString serverIP, QString activity )
    : QObject(parent)
    , mOscMessage(NULL)
    , mActivity(activity)
{
    mClient = new ClientSocket( parent );
    mClient->start( serverIP, 59900 );
    this->connect(mClient, SIGNAL(socketConnected()), this, SLOT(tcpSocketConnected()) );
    this->connect(mClient, SIGNAL(messageReady(QByteArray*)), this, SLOT(manageReceived(QByteArray*)));
}

DataManager::~DataManager()
{
    delete mClient;
}

OscMessage*
DataManager::toOscMessage( QByteArray* rawMessage )
{
    try {
        OscReader reader(rawMessage);
        return reader.getMessage();
    } catch(...) {
        return NULL;
    }
}

//-------------------------------------------------------------------------------
void
DataManager::tcpSocketConnected()
{
    qDebug() << "Connected";
    sendActivityLink(this->mActivity);
}

void
DataManager::manageReceived( QByteArray* bytes )
{
    if (mOscMessage)
        delete mOscMessage;
    mOscMessage = toOscMessage(bytes);
    emit messageReceived( mOscMessage );
}

void
DataManager::send( QByteArray* bytes )
{
    ByteBuffer* bb = ByteBuffer::allocate( 4 );
    bb->putInt( bytes->size() );
    mClient->send( bb->getByteArray() );
    mClient->send( bytes );
}

void
DataManager::sendFloat(QString address, float v){
    OscMessageComposer msg( address );
    msg.pushFloat(v);
    send(msg.getBytes());
}

void
DataManager::sendDouble(QString address, double d){
    OscMessageComposer msg( address );
    msg.pushDouble(d);
    send(msg.getBytes());
}

void
DataManager::sendBool(QString address, bool b){
  OscMessageComposer msg(address);
  msg.pushBool(b);
  send(msg.getBytes());
}

void
DataManager::sendActivityLink(QString activity)
{
    QString addr = "/set-link-with-activity";
    OscMessageComposer msg(addr);
    msg.pushString(activity);
    send(msg.getBytes());
}

