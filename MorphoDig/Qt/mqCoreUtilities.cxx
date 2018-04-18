/*=========================================================================

   Program: MorphoDig
   Module:    mqCoreUtilities.cxx

   

========================================================================*/
#include "mqCoreUtilities.h"

#include <QAbstractButton>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <vtkWeakPointer.h>
//#include "mqApplicationCore.h"
//#include "mqSettings.h"


#include <cstdlib>

QPointer<QWidget> mqCoreUtilities::MainWidget = 0;

//-----------------------------------------------------------------------------
QWidget* mqCoreUtilities::findMainWindow()
{
  foreach (QWidget* widget, QApplication::topLevelWidgets())
  {
    if (widget->isWindow() && widget->isVisible() && qobject_cast<QMainWindow*>(widget))
    {
      return widget;
    }
  }

  // Find any window (even if not visible).
  foreach (QWidget* widget, QApplication::topLevelWidgets())
  {
    if (widget->isWindow() && qobject_cast<QMainWindow*>(widget))
    {
      return widget;
    }
  }

  return NULL;
}

class mqCoreUtilitiesEventHelper::mqInternal
{
public:
	vtkWeakPointer<vtkObject> EventInvoker;
	unsigned long EventID;
	mqInternal()
		: EventID(0)
	{
	}

	~mqInternal()
	{
		if (this->EventInvoker && this->EventID > 0)
		{
			this->EventInvoker->RemoveObserver(this->EventID);
		}
	}
};

//-----------------------------------------------------------------------------
mqCoreUtilitiesEventHelper::mqCoreUtilitiesEventHelper(QObject* object)
	: Superclass(object)
	, Interal(new mqCoreUtilitiesEventHelper::mqInternal())
{
}

//-----------------------------------------------------------------------------
mqCoreUtilitiesEventHelper::~mqCoreUtilitiesEventHelper()
{
	delete this->Interal;
}

//-----------------------------------------------------------------------------
void mqCoreUtilitiesEventHelper::executeEvent(vtkObject* obj, unsigned long eventid, void* calldata)
{
	emit this->eventInvoked(obj, eventid, calldata);
}

unsigned long mqCoreUtilities::connect(vtkObject* vtk_object, int vtk_event_id, QObject* qobject,
	const char* signal_or_slot, Qt::ConnectionType type /* = Qt::AutoConnection*/)
{
	Q_ASSERT(vtk_object != NULL);
	Q_ASSERT(qobject != NULL);
	Q_ASSERT(signal_or_slot != NULL);
	if (vtk_object == NULL || qobject == NULL || signal_or_slot == NULL)
	{
		// qCritical is Qt's 'print error message' stream
		qCritical() << "Error: Cannot connect to or from NULL.";
		return 0;
	}

	mqCoreUtilitiesEventHelper* helper = new mqCoreUtilitiesEventHelper(qobject);
	unsigned long eventid =
		vtk_object->AddObserver(vtk_event_id, helper, &mqCoreUtilitiesEventHelper::executeEvent);
	helper->Interal->EventID = eventid;
	helper->Interal->EventInvoker = vtk_object;

	QObject::connect(
		helper, SIGNAL(eventInvoked(vtkObject*, unsigned long, void*)), qobject, signal_or_slot, type);

	// * When qobject is deleted, helper is deleted. pqCoreUtilitiesEventHelper in
	// its destructor ensures that the observer is removed from the vtk_object if
	// it exists.
	// * When VTK-object is deleted, it removes the observer, but cannot delete
	// helper. Since pqCoreUtilitiesEventHelper::Interal keeps a weak-pointer to
	// the vtk_object, that gets cleared. So eventually when qobject is destroyed,
	// the pqCoreUtilitiesEventHelper is deleted, but since the vtk_object is
	// already deleted, it doesnt' do anything special.
	return eventid;
}

//-----------------------------------------------------------------------------
QString mqCoreUtilities::getMorphoDigUserDirectory()
{
  //mqSettings* settings = mqApplicationCore::instance()->settings();
  //return QFileInfo(settings->fileName()).path();
	QString mystr = "Not implemented yet";
	return mystr;
}

//-----------------------------------------------------------------------------
QString mqCoreUtilities::getMorphoDigApplicationDirectory()
{
  return QApplication::applicationDirPath();
}

//-----------------------------------------------------------------------------






