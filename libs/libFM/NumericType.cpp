#include "NumericType.hpp"
#include "BaseTypes.hpp"

using namespace FM;

template <class T, FM::DataCode codeNum>
Object NumericType<T,codeNum>::asIndexNoBoundsCheck(const Object &a)
{
  if (!a.isComplex() && a.isScalar())
    {
      T aval = this->scalarValue(a);
      if (aval < 1) throw Exception("Index out of range");
      return Type::_base->_index->makeScalar(static_cast<ndx_t>(aval)-1);
    }
  if (a.isComplex())
    {
      // TODO - add message catalog with numbers and translations
      std::cout << "WARNING: Complex part of index ignored\r\n";
    }
  dim_t len = a.dims().elementCount();
  Object output = Type::_base->_index->makeMatrix(len,1);
  ndx_t *op = Type::_base->_index->readWriteData(output);
  const T *ip = this->readOnlyData(a);
  dim_t incr = (a.isComplex() ? 2 : 1);
  for (dim_t i=0;i<len;i++)
    {
      op[i] = static_cast<ndx_t>(*ip)-1;
      if (op[i] < 0) throw Exception("Index values must be >= 1");
      ip += incr;
    }
  return output;  
}

template <class T, FM::DataCode codeNum>
Object NumericType<T,codeNum>::asLogical(const Object &a)
{
  if (codeNum == TypeBool) return a;
  Object output = Type::_base->_bool->zeroArrayOfSize(a.dims(),false);
  bool *op = Type::_base->_bool->readWriteData(output);
  dim_t len = a.dims().elementCount();
  if (a.isComplex())
    {
      const FM::Complex<T> *ip = reinterpret_cast<const FM::Complex<T> *>(this->readOnlyData(a));
      for (dim_t i=0;i<len;i++)	
	op[i] = ((ip[i].r != 0) || (ip[i].i != 0));
    }
  else
    {
      const T* ip = static_cast<const T*>(this->readOnlyData(a));
      for (dim_t i=0;i<len;i++)
	op[i] = (ip[i] != 0);
    }
  return output;
}

template <class T, FM::DataCode codeNum>
Object NumericType<T,codeNum>::asIndex(const Object &a, dim_t max)
{
  if (!a.isComplex() && a.isScalar())
    {
      T aval = this->scalarValue(a);
      if ((aval < 1) || (aval > max)) throw Exception("Index out of range");
      return Type::_base->_index->makeScalar(static_cast<ndx_t>(aval)-1);
    }
  if (a.isComplex())
    {
      // TODO - add message catalog with numbers and translations
      std::cout << "WARNING: Complex part of index ignored\r\n";
    }
  dim_t len = a.dims().elementCount();
  Object output = Type::_base->_index->makeMatrix(len,1);
  ndx_t *op = Type::_base->_index->readWriteData(output);
  const T *ip = this->readOnlyData(a);
  dim_t incr = (a.isComplex() ? 2 : 1);
  for (dim_t i=0;i<len;i++)
    {
      op[i] = static_cast<ndx_t>(*ip)-1;
      if (op[i] < 0) throw Exception("Index values must be >= 1");
      if (op[i] >= max) throw Exception("Out of range");
      ip += incr;
    }
  return output;  
}

template class NumericType<float,TypeSingle>;
template class NumericType<double,TypeDouble>;
template class NumericType<int32_t,TypeInt32>;
template class NumericType<uint32_t,TypeUInt32>;
template class NumericType<int64_t,TypeInt64>;
template class NumericType<uint64_t,TypeUInt64>;
template class NumericType<bool,TypeBool>;
template class NumericType<ndx_t,TypeIndex>;
template class NumericType<FMChar,TypeString>;