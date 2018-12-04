/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSurfaceTagInfosDialog_h
#define _mqSurfaceTagInfosDialog_h


#include <QDialog>

namespace Ui
{
class mqSurfaceTagInfosDialog;
}




/**
* mqSurfaceTagInfosDialog is the dialog used by MorphoDig to compute infos (area, volume etc. ) for active Surface, and possibly for each tagged region
*/
class  mqSurfaceTagInfosDialog : public QDialog
{
  Q_OBJECT

public:
  mqSurfaceTagInfosDialog(QWidget* Parent);
  
  ~mqSurfaceTagInfosDialog();
  void editInfos();
  void PopulateComboTags();
  public slots:
  
  
    
  virtual void slotIncludeTags();
  virtual void sloteditInfos();
  virtual void slotClose();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqSurfaceTagInfosDialog)
  Ui::mqSurfaceTagInfosDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
