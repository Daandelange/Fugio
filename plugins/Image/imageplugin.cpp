#include "imageplugin.h"

#include <fugio/image/uuid.h>

#include "imageloadernode.h"
#include "imagepreviewnode.h"
#include "grabscreennode.h"
#include "scaleimagenode.h"
#include "imagenode.h"
#include "colourmasknode.h"
#include "imagesizenode.h"
#include "imagefilternode.h"
#include "imagesavenode.h"

#include "painterwindownode.h"

#include "imagepin.h"

QList<QUuid>				NodeControlBase::PID_UUID;

using namespace fugio;

ClassEntry		mNodeClasses[] =
{
	ClassEntry( "Colour Mask", "Image", NID_COLOUR_MASK, &ColourMaskNode::staticMetaObject ),
	ClassEntry( "Filter", "Image", NID_IMAGE_FILTER, &ImageFilterNode::staticMetaObject ),
	ClassEntry( "Grab Screen", "Image", NID_GRAB_SCREEN, &GrabScreenNode::staticMetaObject ),
	ClassEntry( "Image", "Image", NID_IMAGE, &ImageNode::staticMetaObject ),
	ClassEntry( "Image Loader", "Image", NID_IMAGE_LOADER, &ImageLoaderNode::staticMetaObject ),
	ClassEntry( "Image Preview", "Image", NID_IMAGE_PREVIEW, &ImagePreviewNode::staticMetaObject ),
	ClassEntry( "Painter Window", "Image", NID_PAINTER_WINDOW, &PainterWindowNode::staticMetaObject ),
	ClassEntry( "Scale", "Image", NID_SCALE_IMAGE, &ScaleImageNode::staticMetaObject ),
	ClassEntry( "Save", "Image", NID_IMAGE_SAVE, &ImageSaveNode::staticMetaObject ),
	ClassEntry( "Size", "Image", ClassEntry::Deprecated, NID_IMAGE_SIZE, &ImageSizeNode::staticMetaObject ),
	ClassEntry()
};

ClassEntry		mPinClasses[] =
{
	ClassEntry( "Image", PID_IMAGE, &ImagePin::staticMetaObject ),
	ClassEntry()
};

ImagePlugin::ImagePlugin()
	: mApp( 0 )
{

}

PluginInterface::InitResult ImagePlugin::initialise( fugio::GlobalInterface *pApp, bool pLastChance )
{
	Q_UNUSED( pLastChance )

	mApp = pApp;

	mApp->registerNodeClasses( mNodeClasses );

	mApp->registerPinClasses( mPinClasses );

	return( INIT_OK );
}

void ImagePlugin::deinitialise()
{
	mApp->unregisterPinClasses( mPinClasses );

	mApp->unregisterNodeClasses( mNodeClasses );

	mApp = 0;
}
