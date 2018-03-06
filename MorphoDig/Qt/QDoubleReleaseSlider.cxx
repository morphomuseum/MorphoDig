#include "QDoubleReleaseSlider.h"
#include <cmath>

QDoubleReleaseSlider::QDoubleReleaseSlider(QWidget *parent) :
	QDoubleSlider(parent)
{
	connect(this, SIGNAL(sliderReleased()), this, SLOT(slotRefresh()));

}
void QDoubleReleaseSlider::slotRefresh()
{
	this->Refresh();
}


void QDoubleReleaseSlider::Refresh()
{
	//std::cout << "Refresh to 0!" << endl;
	this->setDoubleValue(0);
}

