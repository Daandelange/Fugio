#include "fileplugin.h"

#include <fugio/file/uuid.h>

#include "filewatchernode.h"
#include "filenamenode.h"
#include "loadnode.h"

#include "filenamepin.h"

QList<QUuid>				NodeControlBase::PID_UUID;

ClassEntry		mNodeClasses[] =
{
	ClassEntry( "Filename", "GUI", NID_FILENAME, &FilenameNode::staticMetaObject ),
	ClassEntry( "File Load", "File", NID_FILE_LOAD, &LoadNode::staticMetaObject ),
	ClassEntry( "File Watcher", "File", NID_FILE_WATCH, &FileWatcherNode::staticMetaObject ),
	ClassEntry()
};

ClassEntry		mPinClasses[] =
{
	ClassEntry( "Filename", PID_FILENAME, &FilenamePin::staticMetaObject ),
	ClassEntry()
};

FilePlugin::FilePlugin()
	: mApp( 0 )
{

}

PluginInterface::InitResult FilePlugin::initialise( fugio::GlobalInterface *pApp, bool pLastChance )
{
	mApp = pApp;

	mApp->registerNodeClasses( mNodeClasses );

	mApp->registerPinClasses( mPinClasses );

	return( INIT_OK );
}

void FilePlugin::deinitialise()
{
	mApp->unregisterPinClasses( mPinClasses );

	mApp->unregisterNodeClasses( mNodeClasses );

	mApp = 0;
}
