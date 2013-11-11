#ifndef __itkObjectnessMeasurementFunction_h
#define __itkObjectnessMeasurementFunction_h

#include "itkLightObject.h"

namespace itk
{

class ObjectnessMeasurementFunction:public LightObject
{
public:
  /** Standard class typedefs. */
  typedef ObjectnessMeasurementFunction   Self;
  typedef LightObject                Superclass;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Run-time type information (and related methods) */
  itkTypeMacro(ObjectnessMeasurementFunction, LightObject);

  virtual double ComputeObjectProperty(const FixedArray< double,3> & eigenvalues)=0;

protected:
  ObjectnessMeasurementFunction(){

  }
  ~ObjectnessMeasurementFunction(){

  }
  void PrintSelf(std::ostream & os, Indent indent) const{

  }

private:
  ObjectnessMeasurementFunction(const Self &); //purposely not implemented
  void operator=(const Self &);           //purposely not implemented
};

} // end namespace itk


#endif
