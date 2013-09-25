// Algebra.h: interface for the Algebra class.
// Copyright: (c) Burkhard Wuensche]
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALGEBRA_H__C5904AF0_31A2_11D2_B4A3_0000E8D74310__INCLUDED_)
#define AFX_ALGEBRA_H__C5904AF0_31A2_11D2_B4A3_0000E8D74310__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <iostream>
using namespace std;
#include <math.h>

// CVector class
class CVector {
	public:
	    // some good friends
	    friend class CMatrix;

		CVector(); // length = -1, no memory allocated
	    CVector(const int n, bool isInitialized=true);
	    virtual ~CVector();

		 // Copy constructor
	    CVector(const CVector& v);

	    // Assignment operator
	    CVector& operator=(const CVector& v1);
		CVector& operator=(float f);

	    // some other operators
		float* getArray() { return v; }
		void reset() { delete[] v; v=NULL; length=-1;}
		void setArray(float* vNew) {v=vNew;}
		void setSize(int n, bool isInitialized=true);
		int getSize() const {return length;};
		float& operator[](int i) const;
	    CVector& operator+=(const CVector& v1);
	    CVector& operator-=(const CVector& v1);
	    CVector& operator*=(float scalar);
	    CVector& operator/=(float scalar);
	    friend CVector operator+(const CVector& v1, const CVector& v2);
	    friend CVector operator-(const CVector& v1, const CVector& v2);
	    friend CVector operator*(float scalar, const CVector& v1);
	    friend CVector operator*(const CVector& v1, const CVector& v2);
		friend float dotProduct(const CVector& v1, const CVector& v2);
		
		// solve a linear system of equations
		friend CVector SolveLinearSystem(const CMatrix& m, const CVector& v);
		friend CVector LUSolve(const CMatrix& m, const CVector& v);
		friend CVector QRSolve(const CMatrix& m, const CVector& v);
		friend CVector CholeskySolve(const CMatrix& m, const CVector& v);

	    // I/O operator
	    friend ostream& operator<<(ostream& s, const CVector& v1);
	    friend istream& operator>>(istream& s, CVector& v1);

	    friend CVector operator*(const CMatrix& m1, const CVector& v1); // right multiplication
		friend CVector operator*(const CVector& v1, const CMatrix& m1); // left multiplication

	private:
  		int length;
	    float *v;
};


#define Index2d( i , j , n , m) ((i)*(m)+j)              /* Index [i,j] of a matrix. m is number of row elements */

class CMatrix {
	public:
	    // Constructors/ Destructor
		CMatrix(); // lengths = -1, no memory allocated
	    CMatrix(int m, int n, bool isInitialized=true); // m x n matrix
		virtual ~CMatrix();

	    CMatrix(const CMatrix& m1); // Copy constructor

	    // Assignment operator
	    CMatrix& operator=(const CMatrix& m1);
		CMatrix& operator=(float f);

		void setRow(int i, const CVector& v);
		void setColumn(int j, const CVector& v);
		CVector getRow(int i) const;
		CVector getColumn(int j) const;
		float& operator()(int i, int j) const;

		void setSize(int m, int n, bool isInitialized=true);
		int getSizeM() const {return lm;};
		int getSizeN() const {return ln;};

		// some other operators
	    CMatrix& operator+=(const CMatrix& m1);
	    CMatrix& operator-=(const CMatrix& m1);
	    CMatrix& operator*=(const CMatrix& m1);
	    CMatrix& operator*=(float scalar);
	    CMatrix& operator/=(float scalar);
	    friend CMatrix operator*(float scalar, const CMatrix& m1);
	    friend CVector operator*(const CMatrix& m1, const CVector& v1);
	    friend CVector operator*(const CVector& v1, const CMatrix& m1);
	    friend CMatrix operator+(const CMatrix& m1, const CMatrix& m2);
	    friend CMatrix operator*(const CMatrix& m1, const CMatrix& m2);

	    // I/O operator
	    friend ostream& operator<<(ostream& s, const CMatrix& m1);
	    friend istream& operator>>(istream& s, CMatrix& m1);

	    // calculation of the transposed CMatrix
	    CMatrix transpose(void) const;

		// solve a linear system of equations
		friend CVector SolveLinearSystem(const CMatrix& m, const CVector& v);
		friend CVector LUSolve(const CMatrix& m, const CVector& v);
		friend CVector QRSolve(const CMatrix& m, const CVector& v);
		friend CVector CholeskySolve(const CMatrix& m, const CVector& v);

		// calculation of eigenvectors and eigenvalues
		void diagonalize(CMatrix& eVectors, CVector& eValues) const;

		// calculation of the inverse CMatrix
	    CMatrix inverse(void) const;

	    // calculation of the determinante
	    float det(void) const;

	private:
 		int lm,ln;
	    float *m;
};

// more convienent way to use the transpose method
inline CMatrix transpose(CMatrix& m1) { return m1.transpose(); }

#endif // !defined(AFX_ALGEBRA_H__C5904AF0_31A2_11D2_B4A3_0000E8D74310__INCLUDED_)
