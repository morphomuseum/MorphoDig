/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqMoveCurveHandlesDialog.h"
#include "ui_mqMoveCurveHandlesDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QHeaderView>


#include <sstream>

#define NORMAL_NODE 0
#define STARTING_NODE 1
#define MILESTONE_NODE 2
#define CONNECT_NODE 3


// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT "64"
#else
#define ENVIRONMENT "32"
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT "64"
#else
#define ENVIRONMENT "32"
#endif
#endif

//-----------------------------------------------------------------------------
mqMoveCurveHandlesDialog::mqMoveCurveHandlesDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqMoveCurveHandlesDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqMoveCurveHandlesDialog");
	
	
	// This is where we 
  //
 

 this->Ui->Intensity->setValue(25);  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotMoveCurveHandles()));

}




//-----------------------------------------------------------------------------
mqMoveCurveHandlesDialog::~mqMoveCurveHandlesDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqMoveCurveHandlesDialog::slotMoveCurveHandles()
{
	cout << "Move Curve Handles!" << endl;
	double intensity = (double)this->Ui->Intensity->value();
	cout << intensity << endl;
	vtkSmartPointer<vtkLMActorCollection> Handles = vtkSmartPointer<vtkLMActorCollection>::New();
	Handles = mqMorphoDigCore::instance()->getHandleLandmarkCollection();
	vtkSmartPointer<vtkLMActorCollection> Nodes = vtkSmartPointer<vtkLMActorCollection>::New();
	Nodes = mqMorphoDigCore::instance()->getNodeLandmarkCollection();


	int cpt;
	
	double vv1[3], vvprec[3], vvnext[3], oldcoords[3], newcoords[3], vdir[3];
	double dist, length;
	int nh = Handles->GetNumberOfItems();
	int nn = Nodes->GetNumberOfItems();
	int start = 0;
	int selected = 0;
	vtkLMActor *k, *l, *ll = NULL; 
	
	if (nh == nn)
	{
		int handles_modified = 0;
		int ind = 0;
		int ind2 = 0;
		vvprec[0] = 0;
		vvprec[1] = 0;
		vvprec[2] = 0;
		cpt = 0;
		k = Nodes->GetLandmarkAfter(ind); // k = normal landmark 0
		l = Handles->GetLandmarkAfter(ind); // l = target landmark 0

		while (k != NULL)
		{
			ll = l; // ll =  handle landmark of node k
			selected = l->GetSelected(); // current target landmark is selected?
			if (selected == 1)
			{
				//cout << "precnode:" << vvprec[0] << "," << vvprec[1] << "," << vvprec[2] << "," << endl;
			}


			vtkMatrix4x4 *Matl = l->GetMatrix();
			oldcoords[0] = Matl->GetElement(0, 3);
			oldcoords[1] = Matl->GetElement(1, 3);
			oldcoords[2] = Matl->GetElement(2, 3);
			if (selected == 1)
			{
				//cout << "oldcoords:" << oldcoords[0] << "," << oldcoords[1] << "," << oldcoords[2] << "," << endl;
			}
			
			
			vtkMatrix4x4 *Matk = k->GetMatrix();
			vv1[0] = Matk->GetElement(0, 3);
			vv1[1] = Matk->GetElement(1, 3);
			vv1[2] = Matk->GetElement(2, 3);
			if (selected == 1)
			{
				
				//cout << "node:" << vv1[0] << "," << vv1[1] << "," << vv1[2] << "," << endl;
			
			}
			start = k->GetLMNodeType();
			if (ind == 0) { start = STARTING_NODE; }
			if (start != STARTING_NODE) { start = 0; }
			cpt++;
			ind = k->GetLMNumber();
			ind2 = l->GetLMNumber();


			k = Nodes->GetLandmarkAfter(ind); // now is normal landmark n+1
			l = Handles->GetLandmarkAfter(ind2);// now is target landmark n+1
			vvnext[0] = 0;
			vvnext[1] = 0;
			vvnext[2] = 0;
			if (k != NULL)
			{

				vtkMatrix4x4 *Matk2 = k->GetMatrix();
				vvnext[0] = Matk2->GetElement(0, 3);
				vvnext[1] = Matk2->GetElement(1, 3);
				vvnext[2] = Matk2->GetElement(2, 3);
				if (selected == 1)
				{

					//cout << "nextnode:" << vvnext[0] << "," << vvnext[1] << "," << vvnext[2] << "," << endl;
				}

			}
			if (start == 1) //current handle is on a curve start!
			{
				// only need vv1[] and vvnext[]
				if (k == NULL || (k != NULL && k->GetLMNodeType()==STARTING_NODE))
				{
					//do nothing! means is a start and no next!
				}
				else
				{
					vdir[0] = vvnext[0] - vv1[0];
					vdir[1] = vvnext[1] - vv1[1];
					vdir[2] = vvnext[2] - vv1[2];
					length = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
					if (length != 0)
					{
						vdir[0] /= length;
						vdir[1] /= length;
						vdir[2] /= length;
					}
					dist = sqrt((vvnext[0] - vv1[0])*(vvnext[0] - vv1[0])
						+ (vvnext[1] - vv1[1])*(vvnext[1] - vv1[1]) +
						(vvnext[2] - vv1[2])*(vvnext[2] - vv1[2]));
					newcoords[0] = vv1[0] + intensity*dist*vdir[0] / 100;
					newcoords[1] = vv1[1] + intensity*dist*vdir[1] / 100;
					newcoords[2] = vv1[2] + intensity*dist*vdir[2] / 100;

				}
			}
			else // current handle is not on a curve start
			{
				//if next is null or next is a curve start						
				if (k == NULL || (k != NULL && k->GetLMNodeType()==STARTING_NODE))
				{
					// then need vvprec and vv
					vdir[0] = vv1[0] - vvprec[0];
					vdir[1] = vv1[1] - vvprec[1];
					vdir[2] = vv1[2] - vvprec[2];
					length = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
					if (length != 0)
					{
						vdir[0] /= length;
						vdir[1] /= length;
						vdir[2] /= length;
					}
					dist = sqrt((vv1[0] - vvprec[0])*(vv1[0] - vvprec[0])
						+ (vv1[1] - vvprec[1])*(vv1[1] - vvprec[1]) +
						(vv1[2] - vvprec[2])*(vv1[2] - vvprec[2]));
					newcoords[0] = vv1[0] + intensity*dist*vdir[0] / 100;
					newcoords[1] = vv1[1] + intensity*dist*vdir[1] / 100;
					newcoords[2] = vv1[2] + intensity*dist*vdir[2] / 100;
				}
				else
				{
					//
					vdir[0] = vvnext[0] - vvprec[0];
					vdir[1] = vvnext[1] - vvprec[1];
					vdir[2] = vvnext[2] - vvprec[2];
					length = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
					if (length != 0)
					{
						vdir[0] /= length;
						vdir[1] /= length;
						vdir[2] /= length;
					}

					dist = sqrt((vvnext[0] - vvprec[0])*(vvnext[0] - vvprec[0])
						+ (vvnext[1] - vvprec[1])*(vvnext[1] - vvprec[1]) +
						(vvnext[2] - vvprec[2])*(vvnext[2] - vvprec[2]))


						;
					newcoords[0] = vv1[0] + intensity*dist*vdir[0] / 100;
					newcoords[1] = vv1[1] + intensity*dist*vdir[1] / 100;
					newcoords[2] = vv1[2] + intensity*dist*vdir[2] / 100;

					if (selected == 1)
					{

						//cout << "newcoords:" << newcoords[0] << "," << newcoords[1] << "," << newcoords[2] << "," << endl;
					
					}

				}

			}
			vvprec[0] = vv1[0];
			vvprec[1] = vv1[1];
			vvprec[2] = vv1[2];


			if (selected == 1)
			{
				// only in this case do we move the landmark
				//M2[3][0]=x;M2[3][1]=y;M2[3][2]=z;
				
				ll->SetLMOrigin(newcoords);
				handles_modified = 1;
				ll->SetChanged(1);
				
			}
		}


		if (handles_modified == 1)
		{
			Handles->Modified();
		}
	}
	
	

	

	

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

