/*=========================================================================

   Program: MorphoDig
   Module:    pqSetName.h

 

=========================================================================*/

#ifndef _mqSetName_h
#define _mqSetName_h


#include <QString>

/**
  Using mqSetName, you can create and initialize Qt objects without having to create a bunch of
  temporaries:

  \code
  menu->addAction("Open") << mqSetName("FileOpenMenu");
  \endcode

  \sa mqSetData, mqConnect
*/

struct  mqSetName
{
  mqSetName(const QString& Name);
  const QString Name;

private:
  void operator=(const mqSetName&);
};

/**
* Sets a Qt object's name
*/
template <typename T>
T* operator<<(T* LHS, const mqSetName& RHS)
{
  LHS->setObjectName(RHS.Name);
  return LHS;
}

#endif // !_mqSetName_h
