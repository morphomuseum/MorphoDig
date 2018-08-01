/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqMergeTagsDialog_h
#define _mqMergeTagsDialog_h


#include "vtkMDActor.h"
#include <QDialog>

namespace Ui
{
class mqMergeTagsDialog;
}




/**
* mqMergeTagsDialog : merge to tags (source => target)
*/
class  mqMergeTagsDialog : public QDialog
{
  Q_OBJECT

public:
  mqMergeTagsDialog(QWidget* Parent);
  
  ~mqMergeTagsDialog();
  void launchMergeTags();
  
  public slots:
  
  
  
  virtual void slotLaunchMergeTags();
  virtual void slotClose();
  
  

protected:
	
private:
	
	int idSourceTags;
	int idTargetTags;	
  Q_DISABLE_COPY(mqMergeTagsDialog)
  Ui::mqMergeTagsDialog* const Ui;
 

};

#endif // !_mqCURDialog_h
