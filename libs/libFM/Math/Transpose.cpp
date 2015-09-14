#include "Transpose.hpp"
#include "PODType.hpp"
#include "SingleType.hpp"
#include "Complex.hpp"

using namespace FM;

const int BLOCKSIZE = 100; // Optimize?

template <class T, int block>
static void blocked_transpose(const T *A, T *B, ndx_t N, ndx_t M)
{
  for (ndx_t i=0;i<N;i+=block)
    for (ndx_t j=0;j<M;j+=block)
      for (int k=0;k<block;k++)
	for (int n=0;n<block;n++)
	  if (((j+n) < M) && ((i+k) < N))
	    B[(j+n)+M*(i+k)] = A[(i+k)+N*(j+n)];
}

template <class ElementType>
Object FM::MatrixTranspose(const Object &a) {
  if (a.isScalar()) return a;
  if (!a.is2D()) throw Exception("Cannot transpose multidimensional arrays");
  ArrayType<ElementType> *type = a.asType<ArrayType<ElementType> >();
  Object ret = type->makeMatrix(a.cols(),a.rows());
  blocked_transpose<ElementType,BLOCKSIZE>(type->ro(a),type->rw(ret),a.rows(),a.cols());
  return ret;
}

template Object FM::MatrixTranspose<Object>(const Object &a);
template Object FM::MatrixTranspose<bool>(const Object &a);
template Object FM::MatrixTranspose<char>(const Object &a);
template Object FM::MatrixTranspose<double>(const Object &a);
template Object FM::MatrixTranspose<float>(const Object &a);
template Object FM::MatrixTranspose<uint8_t>(const Object &a);
template Object FM::MatrixTranspose<int8_t>(const Object &a);
template Object FM::MatrixTranspose<uint16_t>(const Object &a);
template Object FM::MatrixTranspose<int16_t>(const Object &a);
template Object FM::MatrixTranspose<uint32_t>(const Object &a);
template Object FM::MatrixTranspose<int32_t>(const Object &a);
template Object FM::MatrixTranspose<uint64_t>(const Object &a);
template Object FM::MatrixTranspose<int64_t>(const Object &a);
template Object FM::MatrixTranspose<Complex<double> >(const Object &a);
template Object FM::MatrixTranspose<Complex<float> >(const Object &a);
template Object FM::MatrixTranspose<Complex<uint8_t> >(const Object &a);
template Object FM::MatrixTranspose<Complex<int8_t> >(const Object &a);
template Object FM::MatrixTranspose<Complex<uint16_t> >(const Object &a);
template Object FM::MatrixTranspose<Complex<int16_t> >(const Object &a);
template Object FM::MatrixTranspose<Complex<uint32_t> >(const Object &a);
template Object FM::MatrixTranspose<Complex<int32_t> >(const Object &a);
template Object FM::MatrixTranspose<Complex<uint64_t> >(const Object &a);
template Object FM::MatrixTranspose<Complex<int64_t> >(const Object &a);
