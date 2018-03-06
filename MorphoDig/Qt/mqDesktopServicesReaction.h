/*=========================================================================

   Program: MorphoDig
   
========================================================================*/
#ifndef mqDesktopServicesReaction_h
#define mqDesktopServicesReaction_h

#include "mqReaction.h"

#include <QUrl>

/**
* @ingroup Reactions
* pqDesktopServicesReaction can be used to open a file (or URL) using
* QDesktopServices. e.g. if your application wants to launch a PDF viewer to
* open the application's User Guide, you can hookup a menu QAction to the
* pqDesktopServicesReaction. e.g.
* @code
* QAction* action = ...
* new mqDesktopServicesReaction(QUrl("file:///..../doc/UsersGuide.pdf"), action);
* @endcode
*/
class  mqDesktopServicesReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqDesktopServicesReaction(const QUrl& url, QAction* parent);
  virtual ~mqDesktopServicesReaction();

  /**
  * Attempt to open a file (local or on the Web) using QDesktopServices.
  * Returns false if failed to open for some reason, other returns true.
  */
  static bool openUrl(const QUrl& url);

protected:
  virtual void onTriggered() { mqDesktopServicesReaction::openUrl(this->URL); }

private:
  Q_DISABLE_COPY(mqDesktopServicesReaction)
  QUrl URL;
};

#endif
