
#include "texteditornode.h"

#include <QMainWindow>
#include <QPushButton>
#include <QSettings>

#include "fugio/global_interface.h"
#include "fugio/context_interface.h"
#include <fugio/context_widget_interface.h>
#include <fugio/text/syntax_highlighter_interface.h>
#include <fugio/node_signals.h>
#include <fugio/pin_signals.h>
#include <fugio/global_interface.h>
#include <fugio/context_signals.h>

#include "texteditorform.h"
#include "cmdtexteditorupdate.h"
#include "textplugin.h"

TextEditorNode::TextEditorNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode ), mDockWidget( 0 ), mTextEdit( 0 ), mDockArea( Qt::BottomDockWidgetArea ), mDockVisible( true ), mHighlighter( 0 )
{
	mPinStringInterface = pinOutput<fugio::VariantInterface *>( "Text", mPinString, PID_STRING );

	connect( mPinString->qobject(), SIGNAL(updated()), this, SLOT(onTextPinUpdated()) );
}

TextEditorNode::~TextEditorNode( void )
{
	QMainWindow		*MainWindow = mNode->context()->global()->mainWindow();

	if( mDockWidget != 0 )
	{
		MainWindow->removeDockWidget( mDockWidget );

		delete mDockWidget;

		mDockWidget = 0;
	}

	if( mHighlighter != 0 )
	{
		delete mHighlighter;

		mHighlighter = 0;
	}
}

QWidget *TextEditorNode::gui()
{
	QPushButton		*GUI = new QPushButton( "Edit..." );

	connect( GUI, SIGNAL(clicked()), this, SLOT(onEditClicked()) );

	return( GUI );
}

void TextEditorNode::checkHighlighter()
{
	if( mPinString->isConnected() )
	{
		if( mPinString->connectedPin()->globalId() != mPinUuid )
		{
			if( mHighlighter != 0 )
			{
				if( mTextEdit )
				{
					mTextEdit->setHighlighter( 0 );
				}

				delete mHighlighter;

				mHighlighter = 0;
			}

			if( mTextEdit != 0 )
			{
				fugio::SyntaxHighlighterInterface	*H = qobject_cast<fugio::SyntaxHighlighterInterface *>( mPinString->connectedPin()->findInterface( IID_SYNTAX_HIGHLIGHTER ) );

				if( H != 0 )
				{
					mHighlighter = H->highlighter( mTextEdit->textEdit()->document() );

					if( mHighlighter )
					{
						mHighlighter->moveToThread( TextPlugin::global()->thread() );

						if( mTextEdit )
						{
							mTextEdit->setHighlighter( H );

							connect( mHighlighter, SIGNAL(errorsUpdated()), mTextEdit, SLOT(errorsUpdated()) );
						}
					}
				}
			}
		}
	}
	else
	{
		if( mHighlighter != 0 )
		{
			delete mHighlighter;

			mHighlighter = 0;

			if( mTextEdit )
			{
				mTextEdit->setHighlighter( 0 );
			}
		}

		mPinUuid = QUuid();
	}
}

void TextEditorNode::inputsUpdated( qint64 pTimeStamp )
{
	NodeControlBase::inputsUpdated( pTimeStamp );

	checkHighlighter();

	mNode->context()->pinUpdated( mPinString );
}

bool TextEditorNode::initialise( void )
{
	QMainWindow		*MainWindow = mNode->context()->global()->mainWindow();

	if( !MainWindow )
	{
		return( false );
	}

	if( ( mDockWidget = new QDockWidget( "TextEditor", MainWindow ) ) == 0 )
	{
		return( false );
	}

	mDockWidget->setObjectName( mNode->uuid().toString() );

	connect( mDockWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(dockSetVisible(bool)) );

	if( ( mTextEdit = new TextEditorForm( mDockWidget ) ) == 0 )
	{
		return( false );
	}

	setupTextEditor( mTextEdit->textEdit() );

	connect( mTextEdit, SIGNAL(updateText()), this, SLOT(onTextUpdate()) );

	connect( mNode->qobject(), SIGNAL(nameChanged(QString)), mTextEdit, SLOT(updateNodeName(QString)) );

	mDockWidget->setWidget( mTextEdit );

	MainWindow->addDockWidget( mDockArea, mDockWidget );

	mTextEdit->updateNodeName( mNode->name() );

	checkHighlighter();

	if( !mDockVisible )
	{
		mDockWidget->hide();
	}

	return( true );
}

bool TextEditorNode::deinitialise( void )
{
	if( mDockWidget )
	{
		mDockWidget->deleteLater();

		mDockWidget = 0;

		mTextEdit = 0;
	}

	return( true );
}

void TextEditorNode::loadSettings( QSettings &pSettings )
{
	mPinStringInterface->setVariant( QString::fromUtf8( pSettings.value( "value" ).toByteArray() ) );

	mDockArea = (Qt::DockWidgetArea)pSettings.value( "dockarea", int( mDockArea ) ).toInt();

	mDockVisible = pSettings.value( "visible", mDockVisible ).toBool();
}

void TextEditorNode::saveSettings( QSettings &pSettings ) const
{
	pSettings.setValue( "value", mPinStringInterface->variant() );

	pSettings.setValue( "dockarea", int( mDockArea ) );

	pSettings.setValue( "visible", mDockVisible );
}

void TextEditorNode::setupTextEditor( QPlainTextEdit *pTextEdit )
{
	QFont font;
	font.setFamily("Courier");
	font.setFixedPitch(true);
	font.setPointSize(10);

	pTextEdit->setFont(font);

	pTextEdit->setTabStopWidth( pTextEdit->fontMetrics().width( QLatin1Char( ' ' ) ) * 4 );
	pTextEdit->setLineWrapMode( QPlainTextEdit::NoWrap );

	pTextEdit->document()->setPlainText( mPinStringInterface->variant().toString() );

	pTextEdit->document()->setModified( false );

	connect( pTextEdit, SIGNAL(modificationChanged(bool)), this, SLOT(onTextModified(bool)) );

	connect( this, SIGNAL(modified(bool)), pTextEdit->document(), SLOT(setModified(bool)) );
}

void TextEditorNode::onEditClicked( void )
{
	if( !mTextEdit )
	{
		return;
	}

	if( mDockWidget->isHidden() )
	{
		mDockWidget->show();
	}

	mTextEdit->textEdit()->activateWindow();
}

void TextEditorNode::onTextModified( bool pModified )
{
	if( mDockWidget )
	{
		if( pModified )
		{
			mDockWidget->setWindowTitle( QString( "TextEditor: %1*" ).arg( mNode->name() ) );
		}
		else
		{
			mDockWidget->setWindowTitle( QString( "TextEditor: %1" ).arg( mNode->name() ) );
		}
	}
}

void TextEditorNode::onTextUpdate()
{
	// We need to trigger the update within the context of the frame start so
	// pin processing takes place...

	connect( mNode->context()->qobject(), SIGNAL(frameStart()), this, SLOT(contextFrameStart()) );
}

void TextEditorNode::onTextPinUpdated()
{
	if( !mTextEdit )
	{
		return;
	}

	checkHighlighter();

	const QString	NewTxt = mPinStringInterface->variant().toString();

	if( NewTxt != mTextEdit->textEdit()->document()->toPlainText() && !NewTxt.isEmpty() )
	{
		mTextEdit->textEdit()->document()->setPlainText( NewTxt );

		emit modified( false );
	}
}

void TextEditorNode::contextFrameStart()
{
	if( mTextEdit->textEdit()->document()->isModified() )
	{
		fugio::ContextWidgetInterface	*ICW = qobject_cast<fugio::ContextWidgetInterface *>( mNode->context()->findInterface( IID_CONTEXT_WIDGET ) );

		if( ICW )
		{
			CmdTextEditorUpdate		*CMD = new CmdTextEditorUpdate( mPinString, mTextEdit->textEdit()->document()->toPlainText() );

			ICW->undoStack()->push( CMD );
		}

		emit modified( false );
	}
	else
	{
		pinUpdated( mPinString );
	}

	// disconnect the frame start again

	disconnect( mNode->context()->qobject(), SIGNAL(frameStart()), this, SLOT(contextFrameStart()) );
}

void TextEditorNode::dockSetVisible( bool pVisible )
{
	mDockVisible = pVisible;
}

