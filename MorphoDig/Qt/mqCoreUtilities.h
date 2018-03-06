/*=========================================================================

   Program: MorphoDig
   Module:    mqCoreUtilities.h

   

========================================================================*/
#ifndef mqCoreUtilities_h
#define mqCoreUtilities_h




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
  * Return the path of the launched application
  */
  static QString getMorphoDigApplicationDirectory();

  

private:
  static QWidget* findMainWindow();
  static QPointer<QWidget> MainWidget;
};

#endif
