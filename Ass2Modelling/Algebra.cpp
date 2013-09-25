// Algebra.cpp: implementation of the Algebra class.
// Copyright: (c) Burkhard Wuensche
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include <stdlib.h>
#include "Algebra.h"
#include "Utilities.h"
#include "Geometry.h"

// Prototypes of functions required for solving linear equations
void ludcmp(double *a, int n, int *indx, double *d);
void lubksb(double *a, int n, int *indx, double b[]);

//////////////////////////////////////////////////////////////////////
// CVector Class
//////////////////////////////////////////////////////////////////////

CVector::CVector()
{
	length=-1;
	v=NULL;
}

CVector::CVector(const int n, bool isInitialized):length(n)
{
	v=new float[length];
	if (isInitialized) for(int i=0;i<length;i++) v[i]=0.0;
}

CVector::~CVector()
{
	delete [] v;
}


CVector::CVector(const CVector& v1)
{
	if (v1.v == NULL) 
		displayMessage(ERROR_MESSAGE,"CVector::CVector(). source vector not allocated");	
	length=v1.length;
	v=new float[length];
	for(int i=0;i<length;i++) v[i]=v1.v[i];
}

CVector& CVector::operator=(const CVector& v1)
{
	if ((length==-1) && (v==NULL))
	{	
		length=v1.length;
		v=new float[length];
	}
	else if (length != v1.length) 
		displayMessage(ERROR_MESSAGE,"CVector::assignment. lengths not compatible");
	for(int i=0;i<length;i++) v[i]=v1.v[i];
	return *this;
}

CVector& CVector::operator=(float f)
{
	if (v == NULL) 
		displayMessage(ERROR_MESSAGE,"CVector::=. vector not allocated");	
	for(int i=0;i<length;i++) v[i]=f;
	return *this;
}

void CVector::setSize(int n, bool isInitialized)
{
	if (v != NULL) 
		displayMessage(ERROR_MESSAGE,"CVector::setSize. not empty");	
	if (n<=0)
		displayMessage(ERROR_MESSAGE,"CVector::setSize. not positive size");	
	else v = new float[n];
	length=n;
	if (isInitialized) for(int i=0;i<length;i++) v[i]=0.0;
}

float& CVector::operator[](int i) const
{
	if (v == NULL) 
		displayMessage(ERROR_MESSAGE,"CVector::[]. vector not allocated");	
	if ((i<0) || (i>=length))
		displayMessage(ERROR_MESSAGE,"CVector::[]. out of range");
	return v[i];
}

CVector& CVector::operator+=(const CVector& v1)
{
	if ((v == NULL) || (v1.v == NULL)) 
		displayMessage(ERROR_MESSAGE,"CVector::+=. vector not allocated");	
	if (length != v1.length) 
		displayMessage(ERROR_MESSAGE,"CVector::+=. lengths not compatible");
	for(int i=0;i<length;i++) v[i]+=v1.v[i];
	return *this;
}

CVector& CVector::operator-=(const CVector& v1)
{
	if ((v == NULL) || (v1.v == NULL)) 
		displayMessage(ERROR_MESSAGE,"CVector::-=. vector not allocated");	
	if (length != v1.length) 
		displayMessage(ERROR_MESSAGE,"CVector::-=. lengths not compatible");
	for(int i=0;i<length;i++) v[i]-=v1.v[i];
	return *this;
}

CVector& CVector::operator*=(float scalar)
{
	if (v == NULL) 
		displayMessage(ERROR_MESSAGE,"CVector::*=. vector not allocated");	
	for(int i=0;i<length;i++) v[i]*=scalar;
	return *this;
}

CVector& CVector::operator/=(float scalar)
{
	if (v == NULL) 
		displayMessage(ERROR_MESSAGE,"CVector::/=. vector not allocated");	
	for(int i=0;i<length;i++) v[i]/=scalar;
	return *this;
}

CVector operator+(const CVector& v1, const CVector& v2)
{
	if ((v1.v == NULL) || (v2.v == NULL)) 
		displayMessage(ERROR_MESSAGE,"CVector::+. vector not allocated");	
	if (v1.length != v2.length) 
		displayMessage(ERROR_MESSAGE,"CVector::+. lengths not compatible");
	CVector vector=v1;
	return vector+=v2;
}


CVector operator-(const CVector& v1, const CVector& v2)
{
	if ((v1.v == NULL) || (v2.v == NULL)) 
		displayMessage(ERROR_MESSAGE,"CVector::-. vector not allocated");	
	if (v1.length != v2.length) 
		displayMessage(ERROR_MESSAGE,"CVector::-. lengths not compatible");
	CVector vector=v1;
	return vector-=v2;
}


CVector operator*(float scalar, const CVector& v1)
{
	if (v1.v == NULL) 
		displayMessage(ERROR_MESSAGE,"CVector::*. vector not allocated");	
	CVector vector=v1;
	return vector*=scalar;
}

CVector operator*(const CVector& v1, const CVector& v2)
{
	if ((v1.v == NULL) || (v2.v == NULL))
		displayMessage(ERROR_MESSAGE,"CVector::*. vector not allocated");	
	if (v1.length != v2.length) 
		displayMessage(ERROR_MESSAGE,"CVector::*. lengths not compatible");
	CVector vector=v1;
	for(int j=0;j<vector.length;j++) vector.v[j]*=v2.v[j];
	return vector;
}

float dotProduct(const CVector& v1, const CVector& v2)
{ 
	float dotProduct=0.0;
	if ((v1.v == NULL) || (v2.v == NULL)) 
		displayMessage(ERROR_MESSAGE,"CVector::dotProduct. vector not allocated");	
	if (v1.length != v2.length) 
		displayMessage(ERROR_MESSAGE,"CVector::dotProduct. lengths not compatible");
	for(int i=0;i<v1.length;i++)
		dotProduct+=v1[i]*v2[i];
	return dotProduct;
}


ostream& operator<<(ostream& s, const CVector& v1)
{
	s << "n=" << v1.length << " ";
	for(int i=0;i<imin(v1.length,200);i++) 
		s << v1.v[i] << " ";
	return s;
}

istream& operator>>(istream& s, CVector& v1)
{
	s >> v1.length;
	for(int i=0;i<v1.length;i++) s >> v1.v[i];
	return s;
}


CVector operator*(const CMatrix& m1, const CVector& v1)
{
	if ((m1.m == NULL) || (v1.v == NULL)) 
		displayMessage(ERROR_MESSAGE,"CVector::CMatrix*CVector. matrix or vector not allocated");	
	if (v1.length != m1.ln) 
		displayMessage(ERROR_MESSAGE,"CMatrix::CMatrix*CVector. sizes not compatible");
	CVector v0(m1.lm);
	for(int i=0;i<v0.length;i++) 
		for(int j=0;j<v1.length;j++)
			v0.v[i]+=m1.m[i*m1.ln+j]*v1.v[j];
	return v0;
}

// left multiplication [ vector.transpose()*matrix ]
CVector operator*(const CVector& v1, const CMatrix& m1)
{
	if ((m1.m == NULL) || (v1.v == NULL)) 
		displayMessage(ERROR_MESSAGE,"CVector::CVector*CMatrix. matrix or vector not allocated");	
	if (v1.length != m1.lm)
		displayMessage(ERROR_MESSAGE,"CMatrix::CVector*CMatrix. sizes not compatible");
	CVector v0(m1.ln);
	for(int j=0;j<v0.length;j++) 
		for(int i=0;i<v1.length;i++)
			v0.v[j]+=m1.m[i*m1.ln+j]*v1.v[i];
	return v0;
}

//////////////////////////////////////////////////////////////////////
// CMatrix Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatrix::CMatrix()
{
	lm=-1;
	ln=-1;
	m=NULL;
}

CMatrix::CMatrix(int lm1, int ln1, bool isInitialized):lm(lm1),ln(ln1)
{
	m=new float[ln*lm];
	if (isInitialized) for(int i=0;i<ln*lm;i++) m[i]=0.0;
}

CMatrix::~CMatrix()
{
	delete [] m;
}

CMatrix::CMatrix(const CMatrix& m1)
{
	if (m1.m == NULL) 
		displayMessage(ERROR_MESSAGE,"CMatrix::CMatrix(). matrix not allocated");	
	lm=m1.lm;
	ln=m1.ln;
	m=new float[ln*lm];
	for(int i=0;i<ln*lm;i++) m[i]=m1.m[i];
}

CMatrix& CMatrix::operator=(const CMatrix& m1)
{
	if ((ln==-1) && (lm==-1) && (m==NULL))
	{	
		ln=m1.ln;
		lm=m1.lm;
		m=new float[ln*lm];
	}
	else if ((ln != m1.ln) || (lm != m1.lm)) 
		displayMessage(ERROR_MESSAGE,"CMatrix::assignment. sizes not compatible");
	for(int i=0;i<ln*lm;i++) m[i]=m1.m[i];
	return *this;
}

CMatrix& CMatrix::operator=(float f)
{
	for(int i=0;i<ln*lm;i++) m[i]=f;
	return *this;
}


void CMatrix::setColumn(int column, const CVector& v)
{
	if (lm != v.length) 
		displayMessage(ERROR_MESSAGE,"CMatrix::setColumn. sizes not compatible");	
	for(int i=0;i<v.length;i++)
		m[i*ln+column]=v[i];
}


void CMatrix::setRow(int row, const CVector& v)
{
	if (ln != v.length) 
		displayMessage(ERROR_MESSAGE,"CMatrix::setRow. sizes not compatible");	
	for(int i=0;i<v.length;i++)
		m[row*ln+i]=v[i];
}


CVector CMatrix::getColumn(int column) const
{
	CVector v1(lm);
	for(int i=0;i<lm;i++)
		v1.v[i]=m[i*ln+column];
	return v1;
}


CVector CMatrix::getRow(int row) const
{
	CVector v1(ln);
	for(int i=0;i<ln;i++)
		v1.v[i]=m[row*ln+i];
	return v1;
}


float& CMatrix::operator()(int i, int j) const
{
	if (m == NULL) 
		displayMessage(ERROR_MESSAGE,"CMatrix::(). matrix not allocated");	
	if ((i<0) || (i>=lm) || (j<0) || (j>=ln))
		displayMessage(ERROR_MESSAGE,"CMatrix::(). out of range");
	return m[i*ln+j];
}


void CMatrix::setSize(int lm1, int ln1, bool isInitialized)
{
	if (m != NULL) 
		displayMessage(ERROR_MESSAGE,"CMatrix::setSize. not empty");	
	if ((lm1<=0)||(ln1<=0))
		displayMessage(ERROR_MESSAGE,"CMatrix::setSize. not positive size");	
	else m = new float[lm1*ln1];
	lm=lm1;
	ln=ln1;
	if (isInitialized) for(int i=0;i<ln*lm;i++) m[i]=0.0;
}


CMatrix& CMatrix::operator+=(const CMatrix& m1)
{
	if ((m == NULL)||(m1.m == NULL)) 
		displayMessage(ERROR_MESSAGE,"CMatrix::+=. matrix not allocated");	
	if ((ln != m1.ln)||(lm != m1.lm))
		displayMessage(ERROR_MESSAGE,"CMatrix::+=. sizes not compatible");
	for(int i=0;i<lm*ln;i++) m[i]+=m1.m[i];
	return *this;
}

CMatrix& CMatrix::operator-=(const CMatrix& m1)
{
	if ((m == NULL)||(m1.m == NULL)) 
		displayMessage(ERROR_MESSAGE,"CMatrix::-=. matrix not allocated");	
	if ((ln != m1.ln)||(lm != m1.lm))
		displayMessage(ERROR_MESSAGE,"CMatrix::-=. sizes not compatible");
	for(int i=0;i<lm*ln;i++) m[i]-=m1.m[i];
	return *this;
}

CMatrix& CMatrix::operator*=(const CMatrix& m1)
{
	*this=(*this)*m1;
	return *this;
}

CMatrix& CMatrix::operator*=(float scalar)
{
	if (m == NULL) 
		displayMessage(ERROR_MESSAGE,"CMatrix::CMatrix*=scalar. matrix not allocated");	
	for(int i=0;i<lm*ln;i++) m[i]*=scalar;
	return *this;
}

CMatrix& CMatrix::operator/=(float scalar)
{
	if (m == NULL) 
		displayMessage(ERROR_MESSAGE,"CMatrix::CMatrix*=scalar. matrix not allocated");	
	for(int i=0;i<lm*ln;i++) m[i]/=scalar;
	return *this;
}

CMatrix operator*(float scalar, const CMatrix& m1)
{
	if (m1.m == NULL) 
		displayMessage(ERROR_MESSAGE,"CMatrix::(). matrix not allocated");	
	CMatrix m0=m1;
	for(int i=0;i<m0.ln*m0.lm;i++) m0.m[i]=scalar*m1.m[i];
	return m0;
}

CMatrix operator+(const CMatrix& m1, const CMatrix& m2)
{
	if ((m1.m == NULL)||(m2.m == NULL)) 
		displayMessage(ERROR_MESSAGE,"CMatrix::+. matrix not allocated");	
	if ((m1.ln != m2.ln) || (m1.lm != m2.lm))
		displayMessage(ERROR_MESSAGE,"CMatrix::+. sizes not compatible");
	CMatrix m0(m1.lm,m1.ln,false);
	for(int i=0;i<m0.ln*m0.lm;i++) m0.m[i]=m1.m[i]+m2.m[i];
	return m0;
}

CMatrix operator*(const CMatrix& m1, const CMatrix& m2)
{
	if ((m1.m == NULL)||(m2.m == NULL)) 
		displayMessage(ERROR_MESSAGE,"CMatrix::CMatrix*CMatrix. matrix not allocated");	
	if (m1.ln != m2.lm)
		displayMessage(ERROR_MESSAGE,"CMatrix::CMatrix*CMatrix. sizes not compatible");
	CMatrix m0(m1.lm,m2.ln);
	for(int i=0;i<m1.lm;i++) 
		for(int j=0;j<m2.ln;j++)
			for(int k=0;k<m1.ln;k++)
				m0.m[i*m0.ln+j]+=m1.m[i*m1.ln+k]*m2.m[k*m2.ln+j];
	return m0;
}

CMatrix CMatrix::transpose(void) const
{
	if (m == NULL) 
		displayMessage(ERROR_MESSAGE,"CMatrix::transpose. matrix not allocated");	
	CMatrix m0(ln,lm,false);
	for(int i=0;i<lm;i++)
		for(int j=0;j<ln;j++)
			m0.m[j*lm+i]=m[i*ln+j];
	return m0;
}


CMatrix CMatrix::inverse(void) const
{
	if (m == NULL) 
		displayMessage(ERROR_MESSAGE,"CMatrix::inverse. matrix not allocated");	
	if (lm != ln)
		displayMessage(ERROR_MESSAGE,"CMatrix::inverse. not a square matrix");
	int length=ln;
	CMatrix m0(length,length,false);
	double *matrix,*col;
	int *index,i,j;
	matrix=(double*) malloc (length*length*sizeof(double));
	col=(double*) malloc (length*sizeof(double));
	index=(int*) malloc (length*sizeof(int));
	for(i=0;i<length;i++)
		for(j=0;j<length;j++)
			matrix[Index2d(i,j,length,length)]=m[Index2d(i,j,length,length)];
	// do LU decomposition 
	double d;
    ludcmp(matrix,length,index,&d);
    // solve equations for RHS 
	for(i=0;i<length;i++)
	{
		for(j=0;j<length;j++) col[j]=0.0;
		col[i]=1.0;
	    lubksb(matrix,length,index,col);
		for(j=0;j<length;j++) m0.m[Index2d(j,i,length,length)]=(float) col[j];
	}
	free(index);
	free(col);
	free(matrix);

	return m0;
}


CVector SolveLinearSystem(const CMatrix& m0, const CVector& v0)
{ 
	return LUSolve(m0,v0);
}


CVector LUSolve(const CMatrix& m0, const CVector& v0)
{
	if ((m0.m == NULL)||(v0.v == NULL)) 
		displayMessage(ERROR_MESSAGE,"CMatrix::inverse. matrix or vector not allocated");	
	int length=v0.length;
	if ((m0.ln != m0.lm) || (m0.lm != length))
		displayMessage(ERROR_MESSAGE,"CMatrix::SolveLinearSystem. sizes not compatible");
	double *matrix,*vector;
	int *index,l,m;
	matrix=(double*) malloc(length*length*sizeof(double));
	vector=(double*) malloc(length*sizeof(double));
	index=(int*) malloc(length*sizeof(int));
	for(l=0;l<length;l++)
	{
		for(m=0;m<length;m++)
			matrix[Index2d(l,m,length,length)]=m0(l,m);
		vector[l]=v0[l];
	}
	double d;
    ludcmp(matrix,length,index,&d);
    // solve equations for RHS 
    lubksb(matrix,length,index,vector);
	CVector solution(length,false);
	solution.length=length;
	for(l=0;l<length;l++)
		solution[l]=(float) vector[l];
	free(vector);
	free(index);
	free(matrix);

	return solution;
}


float CMatrix::det(void) const
{
	if (m == NULL) 
		displayMessage(ERROR_MESSAGE,"CMatrix::inverse. matrix not allocated");	
	if (lm != ln)
		displayMessage(ERROR_MESSAGE,"CMatrix::det. not a square matrix");
	double *matrix,det;
	int *index,i,j;
	matrix=(double*) malloc(ln*ln*sizeof(double));
	index=(int*) malloc(ln*sizeof(int));
	for(i=0;i<ln;i++)
		for(j=0;j<ln;j++)
			matrix[Index2d(i,j,ln,ln)]=m[Index2d(i,j,ln,ln)];
	// do LU decomposition 
    ludcmp(matrix,ln,index,&det); // d=+-1
	for(j=0;j<3;j++) det*=matrix[Index2d(j,j,ln,ln)];
	free(index);
	free(matrix);

	return (float) det;
}


ostream& operator<<(ostream& s, const CMatrix& m1)
{
	s << "m=" << m1.lm << "  n=" << m1.ln << "\n";
	for(int i=0;i<m1.lm;i++)
	{
		for(int j=0;j<m1.ln;j++)
			s << " " << m1.m[i*m1.ln+j];
		s << "\n";
	}
	return s;
}

istream& operator>>(istream& s, CMatrix& m1)
{
	s >> m1.ln >> m1.lm;
	for(int i=0;i<m1.ln*m1.lm;i++) s >> m1.m[i];
	return s;
}




/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* ++              Solution of Linear Algebraic Equations                  ++ */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* used in ludcmp */
#define TINY_LUDCMP 1.0e-20

/* ++++++++++++++++++++++++++++ LU Decomposition ++++++++++++++++++++++++++++ */

void ludcmp(double *a, int n, int *indx, double *d)
/*******************************************************************************
LAST MODIFIED : 23 July 1997
SOURCE: Numerical Recipes in C

DESCRIPTION :
Given a a matrix a[0..n-1][0..n-1], this routine replaces it by the LU decomposition
of a rowwise permutation of itself. a and n are input. a is output (L=a_ij, i>j,
the diagonal unity elements are not stored, U=a_ij i<=j).
indx[0..n-1] is an output vector that records a row permutation effected by the partial 
pivoting; d is output as +-1 depending on whether the number of row interchanges was
even or odd, respectively. 
The routine is used in combination with lubksb to solve linear equations or invert
a matrix.
==============================================================================*/
{
  int i,imax,j,k;
  double big,dum,sum,temp;
  double *vv;                /* stores implicit scaling of each row */
  
  vv=(double*) malloc(n*sizeof(double));
  *d=1.0;                    /* no row interchnages yet */
  for(i=0;i<n;i++)           /* loop over rows to get the implicit */
  {                          /* scaling information */
    big=0.0;
    for(j=0;j<n;j++)
      if ((temp=fabs(a[Index2d(i,j,n,n)])) > big) big=temp;
    if (fabs(big) <= ZEROTOL) displayMessage(ERROR_MESSAGE,"ludcmp. Singular matrix"); 
    vv[i]=1.0/big;
  }
  for(j=0;j<n;j++)          /* this is the loop over columns of Crout's method */
  {
    for(i=0;i<j;i++)
    {
      sum=a[Index2d(i,j,n,n)];
      for(k=0;k<i;k++) sum -= a[Index2d(i,k,n,n)]*a[Index2d(k,j,n,n)];
      a[Index2d(i,j,n,n)]=sum;
    }
    big=0.0;                 /* initialize for the search for largest pivot element */
    for(i=j;i<n;i++)
    {
      sum=a[Index2d(i,j,n,n)];
      for(k=0;k<j;k++)
	    sum -= a[Index2d(i,k,n,n)]*a[Index2d(k,j,n,n)];
      a[Index2d(i,j,n,n)]=sum;
      if ( (dum=vv[i]*fabs(sum)) >= big)  /* is the figure of merit for the pivot */
      {                                   /* better than the best so far ? */
	big=dum;
	imax=i;
      }
    }
    if (j != imax)                        /* do we need to interchange rows ? */
    {                                      
      for(k=0;k<n;k++)                   /* Yes, do so .. */
      {
         dum=a[Index2d(imax,k,n,n)];
         a[Index2d(imax,k,n,n)]=a[Index2d(j,k,n,n)];
         a[Index2d(j,k,n,n)]=dum;
      }
      *d = -(*d);                         /* ... and change the parity of d */
      vv[imax]=vv[j];
    }
    indx[j]=imax;
    if (fabs(a[Index2d(j,j,n,n)]) <= ZEROTOL) a[Index2d(j,j,n,n)]=TINY_LUDCMP;
/*    if (fabs(a[Index2d(j,j,n,n)]) <= ZEROTOL) a[Index2d(j,j,n,n)]=(a[Index2d(j,j,n,n)] > 0.0 ? TINY_LUDCMP : -TINY_LUDCMP); */
                          /* If the pivot is zero the matrix is singular (at least    */
                          /* to the precision of the algorith). For some applications */
                          /* it may be desirable to substitute TINY for zero.         */
    if (j != n)                           /* now finally divide by the pivot elememt */
    {
      dum=1.0/(a[Index2d(j,j,n,n)]);
      for(i=j+1;i<n;i++) a[Index2d(i,j,n,n)] *= dum;
    } 
  }                                       /* go back for the next column for reduction */
  free(vv);
}


void lubksb(double *a, int n, int *indx, double b[])
/*******************************************************************************
LAST MODIFIED : 7 November 1997
SOURCE: Numerical Recipes in C (Modified)

DESCRIPTION :
Solves the set of n linear equations AX=B. Here a[0..n-1][0..n-1] is input, not as the
matrix A but rather as its LU decomposition, determined by the routine ludcmp.
indx[0..n-1] is input as the permutation vector returned by ludcmp. b[0..n-1] is input
as the right-hand side vector B, and returns with the solution vector X. A, n, and
indx are not modified by this routine and can be left in place for successive calls
with different right-hand sides b.
This routine takes into account the possibility that b will begin with many zero
elements, so it is efficient  for use in matrix inversion.
==============================================================================*/
{
  int i,ii=-1,ip,j;
  double sum;
  
  for(i=0;i<n;i++)     /* When ii is set to a non-negative value, it will become the */
  {                     /* index of the first nonvanishing element of b. We now   */
    ip=indx[i];         /* do the forward substitution, equation (2.3.6). The only*/
    sum=b[ip];          /* new wrinkle is to unscramble the permutation as we go. */
    b[ip]=b[i];
    if (ii>=0)
      for (j=ii;j<=i-1;j++) sum -= a[Index2d(i,j,n,n)]*b[j];
    else if (sum) ii=i;   /* A nonzero element was encountered, so from now on we */
    b[i]=sum;             /* have to do the sums in the loop above.               */
  }
  for(i=n-1;i>=0;i--)       /* Now we do the backsubstitution, equation (2.3.7).    */
  {
    sum=b[i];
    for(j=i+1;j<n;j++) sum -= a[Index2d(i,j,n,n)]*b[j];
    b[i]=sum/a[Index2d(i,i,n,n)];     /* Store a componenty of the solution vector X          */
  }                       /* All done!                                            */
}

