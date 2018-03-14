/*=========================================================================

   Program: MorphoDig
 
  
========================================================================*/
#ifndef mqActorTreePanel_h
#define mqActorTreePanel_h


#include <QWidget>
class Ui_mqActorTreePanel;


class  mqActorTreePanel : public QWidget
{
  Q_OBJECT
  typedef QWidget Superclass;

public:
	mqActorTreePanel(const QString& title, QWidget* parentObject = 0)
    : Superclass(parentObject)
  {
    this->constructor();
  }
	mqActorTreePanel(QWidget* parentObject = 0)
    : Superclass(parentObject)
  {
    this->constructor();
  }
  

  public slots :
 virtual void slotSomething();
  
  
private:
	Q_DISABLE_COPY(mqActorTreePanel)
  
  Ui_mqActorTreePanel *ui;
  void constructor();
};

#endif
