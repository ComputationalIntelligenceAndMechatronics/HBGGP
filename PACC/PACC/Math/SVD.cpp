//=====================================
// Name        : svd.cpp
// Author      : Jim Cicon
// email       : jecdhf@mizzou.eduThis e-mail address is being protected from spam bots, you need JavaScript enabled to view it
// Version     :
// Copyright   : Copyright (C) 2008 by Jim Cicon
// Description :
//=====================================

#include <iostream>

#include "SVD.h"
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>

using namespace std;

SVD::SVD(const gsl_matrix * A)
{
    m_reducedT = NULL;
    m_reducedS = NULL;
    m_reducedD = NULL;
    m_varS = NULL;
    m_kZeroS = NULL;
    m_k = -1;
	
    gsl_matrix * B;
    bool transposed = false;
    if(A->size1 < A->size2)   
    {
        B = transpose(A);
        transposed = true;
    }
    else
    {
        B = gsl_matrix_alloc(A->size1, A->size2);
        gsl_matrix_memcpy(B, A);
    }
	
    computeSVD(B);
    gsl_matrix_free(B);
	
    if(true == transposed)
    {
        transposeSVD();
    }
	
    return;
}

SVD::~SVD()
{
    gsl_matrix_free(m_T);
    gsl_matrix_free(m_S);
    gsl_matrix_free(m_D);
    if(m_varS) gsl_vector_free(m_varS);
	
    freeReduced();
}

void SVD::computeSVD(const gsl_matrix * A)
{
    unsigned int i, offset;
    gsl_vector * S;
	
    unsigned int size1 = A->size1;
    unsigned int size2 = A->size2;
	
    m_T = gsl_matrix_alloc(size1, size2);   
	
    gsl_vector * work = gsl_vector_alloc(A->size1);
    i = 0; offset = 0;
	
    for(i=0; i < size2; i++)
    {
        gsl_matrix_get_col(work, A, i+offset);
        gsl_matrix_set_col(m_T, i, work);
    }
	
    // Create the S, T and D matrices
    S = gsl_vector_alloc(m_T->size2);
    m_D = gsl_matrix_alloc(m_T->size2, m_T->size2);
    gsl_vector * W = gsl_vector_alloc(m_T->size2);
	
    // Compute SVD
    m_status = gsl_linalg_SV_decomp (m_T, m_D, S, W);
	
    // build the S matrix
    m_S = gsl_matrix_calloc(S->size, S->size);
    for(i = 0; i < S->size; i++)
    {
        gsl_matrix_set(m_S, i, i, gsl_vector_get(S, i));
    }
	
    // Compute explained variances
    double sumOfSquares = 0;
    double value;
    m_varS = gsl_vector_calloc(S->size);
    for(i = 0; i < S->size; i++)
    {
        value = gsl_vector_get(S, i) * gsl_vector_get(S, i);
        sumOfSquares += value;
        gsl_vector_set(m_varS, i, value);
    }
    gsl_vector_scale(m_varS, 1/sumOfSquares);
	
    gsl_vector_free(S);
    gsl_vector_free(W);
    gsl_vector_free(work);
}

void SVD::transposeSVD(void)
{
    freeReduced(); // The reduced matrices are no longer valid
	
#if 0   
    gsl_matrix * T = transpose(m_D);   
    gsl_matrix * D = transpose(m_T);
    gsl_matrix_free(m_D);
    gsl_matrix_free(m_T);
#else
    gsl_matrix * T = m_D;   
    gsl_matrix * D = m_T;
#endif
	
    m_D = D;
    m_T = T;       
}

void SVD::freeReduced(void)
{
    if(m_reducedT) gsl_matrix_free(m_reducedT);
    if(m_reducedS) gsl_matrix_free(m_reducedS);
    if(m_reducedD) gsl_matrix_free(m_reducedD);       
    if(m_kZeroS) gsl_matrix_free(m_kZeroS);
	
    return;
}

// Reduce the actual dimensions of the S, T & D
// This is strictly not needed but it increases computations when reconstructing the reduced solution.
// A 'pureist' approach would simply set the k+1 to m diagonal elements of S to zero.
SVD::retval SVD::kReduce(int kReduced)
{
    gsl_vector * workT;
    gsl_vector * workD;
    double workS;
    int i;
	
    m_k = kReduced;
    if (0 >= m_k) m_k = m_T->size2;
	
    if(m_k > (signed int)m_T->size2)
    {
        cout << "SVD ERROR: k exceeds dimensionality" << endl;
        return SVD::FAILURE;
    }
	
    cout << "LSA using k = " << m_k << endl;
	
    freeReduced();
	
    m_reducedT = gsl_matrix_alloc(m_T->size1, m_k);
    m_reducedS = gsl_matrix_calloc(m_k, m_k);
    m_reducedD = gsl_matrix_alloc(m_D->size1, m_k);
    m_kZeroS = gsl_matrix_calloc(m_S->size1, m_S->size2);
	
    workT = gsl_vector_alloc(m_T->size1);
    workD = gsl_vector_alloc(m_D->size1);
	
    // reduce dimensions of all three matrices
    for(i = 0; i < m_k; i++)
    {
        gsl_matrix_get_col(workT, m_T, i);
        gsl_matrix_set_col(m_reducedT, i, workT);
		
        gsl_matrix_get_col(workD, m_D, i);
        gsl_matrix_set_col(m_reducedD, i, workD);
		
        workS = gsl_matrix_get(m_S, i, i);
        gsl_matrix_set(m_reducedS, i, i, workS);
		
        gsl_matrix_set(m_kZeroS, i, i, gsl_matrix_get(m_S, i, i));
    }
	
    gsl_vector_free(workT);
    gsl_vector_free(workD);
	
    return SVD::SUCCESS;
}

gsl_matrix * SVD::reconstructMatrix(gsl_matrix * T, gsl_matrix * S, gsl_matrix * D)
{
    if(NULL == T) T = m_reducedT;
    if(NULL == S) S = m_reducedS;
    if(NULL == D) D = m_reducedD;
	
    gsl_matrix * TS = gsl_matrix_alloc(T->size1, S->size2);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, T, S, 0.0, TS);
	
    gsl_matrix * TSDt = gsl_matrix_alloc(TS->size1, D->size1);
    gsl_blas_dgemm(CblasNoTrans, CblasTrans, 1.0, TS, D, 0.0, TSDt);
	
    gsl_matrix_free(TS);
	
    return TSDt;
}

// Caller responsible for freeing returned matrix
gsl_matrix * SVD::computeReducedSDMatrix(void)
{
    gsl_matrix * SD = gsl_matrix_alloc(m_reducedS->size1, m_reducedD->size1);
    gsl_blas_dgemm(CblasNoTrans, CblasTrans, 1.0, m_reducedS, m_reducedD, 0.0, SD);
	
    return SD;
}

// Caller responsible for freeing returned matrix
gsl_matrix * SVD::computeReducedTSMatrix(void)
{
    gsl_matrix * TS = gsl_matrix_alloc(m_reducedT->size1, m_reducedS->size1);
    gsl_blas_dgemm(CblasNoTrans, CblasTrans, 1.0, m_reducedT, m_reducedS, 0.0, TS);
	
    return TS;
}

// Return the inverse of a matrix
// Caller responsible to free inverted matrix
gsl_matrix * SVD::matrixInverse(const gsl_matrix *src)
{
    gsl_matrix *tmp, *dest;
    int signum;
	
    gsl_permutation *p = gsl_permutation_alloc(src->size1);
    tmp = gsl_matrix_alloc(src->size1, src->size2);
    dest = gsl_matrix_calloc(src->size1, src->size1);
    gsl_matrix_memcpy(tmp, src);
	
    gsl_linalg_LU_decomp(tmp, p, &signum);
    gsl_linalg_LU_invert(tmp, p, dest);
	
    gsl_permutation_free(p);
    gsl_matrix_free(tmp);
	
    return dest;
}

// Return the transpose of a matrix
// Caller responsible to free transposed matrix
gsl_matrix * SVD::transpose(const gsl_matrix * A)
{
    gsl_matrix * B = gsl_matrix_alloc(A->size2, A->size1);
    gsl_matrix_transpose_memcpy(B, A);
	
    return B;
}

// Return a matrix with every element positive
// Caller responsible to free matrix
gsl_matrix * SVD::matrixAbsVal(const gsl_matrix * A)
{
    gsl_matrix * B = gsl_matrix_alloc(A->size1, A->size2);
    gsl_matrix_memcpy(B, A);
	
    double x;
	
    for(unsigned int row = 0; row < A->size1; row++)
    {
        for(unsigned int col = 0; col < A->size2; col++)
        {
            x = gsl_matrix_get(B, row, col);
            if(0 > x) x *= -1;
            gsl_matrix_set(B, row, col, x);           
        }
    }
	
    return B;
}

void SVD::serializeVector(gsl_vector * vec)
{
    unsigned int i;
    double value;
	
    cout << "[";
    for(i = 0; i < vec->size ; i++)
    {
        if(i) cout << ", ";
        value = gsl_vector_get(vec, i);
        cout << value ;
    }
    cout << "]" << endl;
}

void SVD::serializeMatrix(gsl_matrix * mat)
{
    int row = mat->size1;
    int col = mat->size2;
    int i, j;
    float x;
    char buf[256];
	
    cout << "[" << endl;
    for (i = 0; i < row; i++)
    {
        cout << "     [";
        for(j = 0; j < col; j++)
        {
            if(j) cout << ", ";
            x = gsl_matrix_get(mat, i, j);
            sprintf(buf, "%7.2f", x);
            cout << buf;
        }
        cout << "]" << endl;
    }
    cout << "]" << endl;
}

//  See Grossman et al (2004), Information Retrieval, pg 72
gsl_matrix * SVD::mapQuery(gsl_vector * query)
{
    // copy the vector into a 1xN matrix
    gsl_matrix * mQuery = gsl_matrix_alloc(1, query->size);
    gsl_matrix_set_row(mQuery, 0, query);
	
    // Compute the QT part of the query
    gsl_matrix * QT = gsl_matrix_alloc(1, m_reducedT->size2);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, mQuery, m_reducedT, 0.0, QT);
	
    // Compute the QTS part of the query
    gsl_matrix * SI = matrixInverse(m_reducedS);
    gsl_matrix * QTS = gsl_matrix_alloc(QT->size1, SI->size2);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, QT, SI, 0.0, QTS);
	
    gsl_matrix_free(QT);
    gsl_matrix_free(mQuery);
    gsl_matrix_free(SI);
	
    return QTS;
}
