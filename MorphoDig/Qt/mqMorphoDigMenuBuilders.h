/*=========================================================================

   Program: ISE-MorphoDig
   Module:    pqMorphoDigMenuBuilders.h

   

========================================================================*/
#ifndef mqMorphoDigMenuBuilders_h
#define mqMorphoDigMenuBuilders_h



class QMenu;
class QWidget;
class QMainWindow;


class mqMorphoDigMenuBuilders
{
public:


	
  /**
  * Builds standard File menu.
  */
  //static void buildFileMenu(QMenu& menu, QMainWindow* mainWindow = 0);
  static void buildFileMenu(QMenu& menu);

  static void buildLandmarksMenu(QMenu& menu);

  static void buildEditSelectedSurfacesMenu(QMenu& menu);
  /**
  * Builds the standard Edit menu.
  */
  //static void buildEditMenu(QMenu& menu, QMainWindow* mainWindow = 0);
  static void buildEditMenu(QMenu& menu);
  //static void buildViewMenu(QMenu& menu);
  static void  buildViewMenu(QMenu& menu, QMainWindow& window, QMainWindow& projectwindow);

  /**
  * Builds the help menu.
  */
  //static void buildHelpMenu(QMenu& menu, QMainWindow* mainWindow = 0);
  static void buildHelpMenu(QMenu& menu);
  static void buildProjectDocks(QMainWindow& projectWindow);
  /**
  * Builds and adds all standard MorphoDig toolbars.
  */
  static void buildToolbars(QMainWindow& mainWindow);

  static void buildStatusBar(QMainWindow& mainWindow);

  /*public slots:
  virtual void slotLandmarkMoveUp();
  virtual void slotLandmarkMoveDown();*/

  
};

#endif
