/*
 * CoinSolver.h
 *
 *  Created on: May 20, 2014
 *      Author: morgan
 */

#ifndef COINSOLVER_H_
#define COINSOLVER_H_

#include <coin/CbcModel.hpp>
#include <coin/OsiClpSolverInterface.hpp>

#include <coin/CoinPackedVector.hpp>
#include <coin/CoinPackedVector.hpp>
namespace ttt {

class CoinSolver {

private:
	vnl_sparse_matrix<double> m_A;
	vnl_vector<double> m_b;
	vnl_vector<double> m_cost;
	vnl_vector<double> m_capacity;

	vnl_vector<double> m_x;
public:
	CoinSolver(const vnl_vector<double> & cost,
			const vnl_vector<double> & capacity,
			const vnl_sparse_matrix<double> & A, const vnl_vector<double> & b) {
		m_A = A;
		m_b = b;
		m_cost = cost;
		m_capacity = capacity;
	}
	vnl_vector<double> GetSolution() {
		return m_x;
	}
	void Solve() {
		OsiClpSolverInterface solver;

		int n_cols = m_cost.size();

		double * objective = m_cost.data_block();

		double * col_lb = new double[n_cols]; //the column lower bounds
//		double * col_ub = new double[n_cols];//the column upper bounds
		double * col_ub = m_capacity.data_block(); //the column upper bounds

		for (int i = 0; i < n_cols; i++) {
			col_lb[i] = 0.0;
			//col_ub[i] = 1;
		}

		int n_rows = m_b.size();
		CoinPackedMatrix * matrix = new CoinPackedMatrix(false, 0, 0);
		matrix->setDimensions(0, n_cols);
		//double * row_lb = new double[n_rows];//the row lower bounds
		//double * row_ub = new double[n_rows];//the row upper bounds
		double * row_lb = m_b.data_block();
		double * row_ub = m_b.data_block();

		for (int c = 0; c < n_rows; c++) {
			//row_lb[c]=-COIN_DBL_MAX;
			//row_lb[c]=
		}
		for (int r = 0; r < n_rows; r++) {
			CoinPackedVector row;

			vnl_sparse_matrix<double>::row aRow = m_A.get_row(r);

			for (unsigned int i = 0; i < aRow.size(); i++) {
				//std::cout << "I:" << i << " " << aRow[i].first << " " << aRow[i].second << std::endl;
				row.insert(aRow[i].first, aRow[i].second);
			}

			matrix->appendRow(row);
		}

		solver.loadProblem(*matrix, col_lb, col_ub, objective, row_lb, row_ub);
		//solver.writeLp("problem", "mps");

		for (int i = 0; i < n_cols; i++) {
			solver.setInteger(i);
		}

		// Pass the solver with the problem to be solved to CbcModel
		CbcModel model(solver);

		// Do complete search
		model.branchAndBound();

		m_x.clear();
		m_x.set_size(n_cols);
		/* Print the solution.  CbcModel clones the solver so we
		 need to get current copy from the CbcModel */
		int numberColumns = model.solver()->getNumCols();

		const double * solution = model.bestSolution();
		memcpy(m_x.data_block(), solution, sizeof(double) * n_cols);
		//	for (int iColumn=0;iColumn<numberColumns;iColumn++) {

		//    double value=solution[iColumn];
		//if (fabs(value)>1.0e-7&&model.solver()->isInteger(iColumn))
		//printf("%d has value %g\n",iColumn,value);

		//}
	}

};
}
#endif /* COINSOLVER_H_ */
