#ifndef TRIGGERPIN_H
#define TRIGGERPIN_H

#include <fugio/core/uuid.h>

#include <QObject>

#include <fugio/pin_interface.h>
#include <fugio/pin_control_interface.h>

#include <fugio/pincontrolbase.h>

#include <fugio/serialise_interface.h>

class TriggerPin : public fugio::PinControlBase, public fugio::SerialiseInterface
{
	Q_OBJECT
	Q_INTERFACES( fugio::SerialiseInterface )

public:
	Q_INVOKABLE explicit TriggerPin( QSharedPointer<fugio::PinInterface> pPin );

	virtual ~TriggerPin( void );

	//-------------------------------------------------------------------------
	// fugio::PinControlInterface

	virtual QString toString( void ) const Q_DECL_OVERRIDE
	{
		return( "" );
	}

	virtual QString description( void ) const Q_DECL_OVERRIDE
	{
		return( "Trigger" );
	}

	//-------------------------------------------------------------------------
	// fugio::SerialiseInterface

	virtual void serialise( QDataStream &pDataStream ) Q_DECL_OVERRIDE
	{
		Q_UNUSED( pDataStream )
	}

	virtual void deserialise( QDataStream &pDataStream ) Q_DECL_OVERRIDE
	{
		Q_UNUSED( pDataStream )
	}
};

#endif // TRIGGERPIN_H
