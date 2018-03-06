/*=========================================================================

   Program: MorphoDig
   Module:    mqReaction.h

  
========================================================================*/
#ifndef mqReaction_h
#define mqReaction_h


#include <QAction>
#include <QObject>
#include <QMainWindow>

/**
* @defgroup Reactions ParaView Reactions
* ParaView client relies of a collection of reactions that autonomous entities
* that use mqApplicationCore and other core components to get their work done which
* keeping track for their own enabled state without any external input. To
* use, simple create this reaction and assign it to a menu
* and add it to menus/toolbars whatever.
*/

/**
* @ingroup Reactions
* This is a superclass just to make it easier to collect all such reactions.
*/
class mqReaction : public QObject
{
  Q_OBJECT
  typedef QObject Superclass;

public:
  /**
  * Constructor. Parent cannot be NULL.
  */
  //mqReaction(QAction* parent, Qt::ConnectionType type = Qt::AutoConnection, QMainWindow* mainWindow = 0);
	mqReaction(QAction* parent, Qt::ConnectionType type = Qt::AutoConnection);
  virtual ~mqReaction();

  /**
  * Provides access to the parent action.
  */
  QAction* parentAction() const { return qobject_cast<QAction*>(this->parent()); }

protected slots:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() {}

  virtual void updateEnableState() {}
  virtual void updateMasterEnableState(bool);

protected:
  bool IsMaster;

private:
  Q_DISABLE_COPY(mqReaction)
};

#endif
