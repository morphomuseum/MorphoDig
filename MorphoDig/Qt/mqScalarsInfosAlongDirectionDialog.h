/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqScalarsInfosAlongDirectionDialog_h
#define _mqScalarsInfosAlongDirectionDialog_h


#include <QDialog>

namespace Ui
{
class mqScalarsInfosAlongDirectionDialog;
}




/**
* mqScalarsInfosDialog is the dialog used by MorphoDig to compute infos (mean, variance etc. ) for active scalars
*/
class  mqScalarsInfosAlongDirectionDialog : public QDialog
{
  Q_OBJECT

public:
  mqScalarsInfosAlongDirectionDialog(QWidget* Parent);
  
  ~mqScalarsInfosAlongDirectionDialog();
  void editInfos();
  void PopulateComboTags();
  void updateAbsMinMax(int direction);
  public slots:
  
  
    
  virtual void slotIncludeTags();
  virtual void sloteditInfos();
  virtual void slotClose();
  virtual void slotrelMinMax();
  virtual void slotabsMinMax();
  virtual void slotrX();
  virtual void slotrY();
  virtual void slotrZ();

protected:
	
private:
	

  Q_DISABLE_COPY(mqScalarsInfosAlongDirectionDialog)
  Ui::mqScalarsInfosAlongDirectionDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
