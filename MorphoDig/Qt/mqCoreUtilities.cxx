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






