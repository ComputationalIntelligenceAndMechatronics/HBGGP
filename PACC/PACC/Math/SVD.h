//====================================
// Name        : svd.h
// Author      : Jim Cicon
// email       : jecdhf@mizzou.eduThis e-mail address is being protected from spam bots, you need JavaScript enabled to view it
// Version     :
// Copyright   : Copyright (C) 2008 by Jim Cicon
// Description :
//====================================
#ifndef SVD_H
#define SVD_H

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

class SVD
	{
	public:
		enum retval {SUCCESS, FAILURE};
		enum svd_mode {NORMAL, TRANSPOSE};
		
	public:   
		SVD(const gsl_matrix *);
		~SVD();
		
		SVD::retval kReduce(int k);
		gsl_matrix * computeReducedSDMatrix(void);
		gsl_matrix * computeReducedTSMatrix(void);
		gsl_matrix * reconstructMatrix(gsl_matrix * T = NULL, gsl_matrix * S = NULL, gsl_matrix * D = NULL);
		
		gsl_matrix * getT(void) {return m_T;};
		gsl_matrix * getD(void) {return m_D;};
		gsl_matrix * getS(void) {return m_S;};
		
		gsl_matrix * getKZS(void) {return m_kZeroS;};
		
		const gsl_vector * getSVar(void) {return m_varS;};
		
		gsl_matrix * getReducedT(void) {return m_reducedT;};
		gsl_matrix * getReducedD(void) {return m_reducedD;};
		gsl_matrix * getReducedS(void) {return m_reducedS;};
		int getK(void) {return m_k;};
		
		gsl_matrix * mapQuery(gsl_vector *);
		
		int getStatus(void) {return m_status;};
		
		gsl_matrix * matrixAbsVal(const gsl_matrix *);
		gsl_matrix * matrixInverse(const gsl_matrix *src);
		gsl_matrix * transpose(const gsl_matrix *);
		
	private:
		void computeSVD(const gsl_matrix *);
		
		void transposeSVD(void);
		
		gsl_matrix *m_T;
		gsl_matrix *m_S;
		gsl_matrix *m_D;
		
		gsl_matrix *m_kZeroS;
		
		gsl_matrix *m_reducedT;
		gsl_matrix *m_reducedS;
		gsl_matrix *m_reducedD;
		gsl_vector *m_varS;
		void freeReduced(void);
		
		int m_k;
		int m_status;
		
		void serializeVector(gsl_vector * vec);
		void serializeMatrix(gsl_matrix * mat);
	};

#endif
