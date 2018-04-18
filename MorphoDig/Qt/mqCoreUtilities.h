/*=========================================================================

   Program: MorphoDig
   Module:    mqCoreUtilities.h

   

========================================================================*/
#ifndef mqCoreUtilities_h
#define mqCoreUtilities_h


#include <vtkObject.h>

#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QWidget>

/**
* pqCoreUtilities is a collection of arbitrary utility functions that can be
* used by the application.
*/

class mqCoreUtilitiesEventHelper : public QObject
{
	Q_OBJECT;
	typedef QObject Superclass;

public:
	mqCoreUtilitiesEventHelper(QObject* parent);
	~mqCoreUtilitiesEventHelper() override;

signals:
	void eventInvoked(vtkObject*, unsigned long, void*);

private:
	Q_DISABLE_COPY(mqCoreUtilitiesEventHelper)

		void executeEvent(vtkObject*, unsigned long, void*);
	class mqInternal;
	mqInternal* Interal;
	friend class mqCoreUtilities;
};

class  mqCoreUtilities
{
public:
  /**
  * When popuping up dialogs, it's generally better if we set the parent
  * widget for those dialogs to be the QMainWindow so that the dialogs show up
  * centered correctly in the application. For that purpose this convenience
  * method is provided. It locates a QMainWindow and returns it.
  */
  static void setMainWidget(QWidget* widget) { mqCoreUtilities::MainWidget = widget; }
  static QWidget* mainWidget()
  {
    if (!mqCoreUtilities::MainWidget)
    {
      mqCoreUtilities::MainWidget = mqCoreUtilities::findMainWindow();
    }
    return mqCoreUtilities::MainWidget;
  }

 
  /**
  * Return the path of the root ParaView user specific configuration directory
  */
  static QString getMorphoDigUserDirectory();

  /**
  * Method used to connect VTK events to Qt slots (or signals).
  * This is an alternative to using vtkEventQtSlotConnect. This method gives a
  * cleaner API to connect vtk-events to Qt slots. It manages cleanup
  * correctly i.e. either vtk-object or the qt-object can be deleted and the
  * observers will be cleaned up correctly. One can disconnect the connection
  * made explicitly by vtk_object->RemoveObserver(eventId) where eventId is
  * the returned value.
  */
  static unsigned long connect(vtkObject* vtk_object, int vtk_event_id, QObject* qobject,
	  const char* signal_or_slot, Qt::ConnectionType type = Qt::AutoConnection);

  /**
  * Return the path of the launched application
  */
  static QString getMorphoDigApplicationDirectory();

  

private:
  static QWidget* findMainWindow();
  static QPointer<QWidget> MainWidget;
};

#endif
