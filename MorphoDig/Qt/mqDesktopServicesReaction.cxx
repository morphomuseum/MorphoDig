/*=========================================================================

   Program: MorphoDig
   Module:  mqDesktopServicesReaction.cxx


========================================================================*/
#include "mqDesktopServicesReaction.h"
#include "mqCoreUtilities.h"

#include <QDesktopServices>
#include <QFileInfo>
#include <QMessageBox>
#include <QtDebug>

#include <iostream>
//-----------------------------------------------------------------------------
mqDesktopServicesReaction::mqDesktopServicesReaction(const QUrl& url, QAction* parentObject)
  : Superclass(parentObject)
  , URL(url)
{
}

//-----------------------------------------------------------------------------
mqDesktopServicesReaction::~mqDesktopServicesReaction()
{
}

//-----------------------------------------------------------------------------
bool mqDesktopServicesReaction::openUrl(const QUrl& url)
{
  if (url.isLocalFile() && !QFileInfo(url.toLocalFile()).exists())
  {
    QString filename = QFileInfo(url.toLocalFile()).absoluteFilePath();
    QString msg =
      QString("The requested file is not available in your installation. "
              "You can manually obtain and place the file (or ask your administrators) at the "
              "following location for this to work.\n\n'%1'")
        .arg(filename);
    // dump to cout for easy copy/paste.
    std::cout << msg.toUtf8().data() << std::endl;
    QMessageBox::warning(mqCoreUtilities::mainWidget(), "Missing file", msg, QMessageBox::Ok);
    return false;
  }
  if (!QDesktopServices::openUrl(url))
  {
    qCritical() << "Failed to open '" << url << "'";
    return false;
  }
  return true;
}
