#include "matrix.hxx"

using namespace std;
#include <thread>

////////////////////////////////////////////////////////////////
// One thread, blocked. Loop order rB, kB, cB, r, k, c.
// This function is for you to write.
//
void Matrix::mpy1 (const Matrix &A, const Matrix &B, int BS) {
    int NBLK= this->N()/BS;
    assert (this->N() >= BS);
    for (int rB = 0; rB < NBLK; rB++)
    for (int kB = 0; kB < NBLK; kB++)
    for (int cB = 0; cB < NBLK; cB++)
	for (int r = 0; r < BS; r++)
	for (int k = 0; k < BS; k++) {
		int my_r = rB * BS + r;
		int my_k = kB * BS + k;
	for (int c = 0; c < BS; c++) {
		int my_c = cB * BS + c;
	if (my_k == 0)
		this->data[this->index(my_r, my_c)] = A(my_r, my_k) * B(my_k, my_c);
	else
		this->data[this->index(my_r, my_c)] += A(my_r, my_k) * B(my_k, my_c);
                        }
                    }
    //...
}

////////////////////////////////////////////////////////////////
// Multithreaded, blocked version.
//
// This function, th_func2(), does the per-thread work of multithreaded, blocked
// matrix multiplication.
static void th_func2 (Matrix &C, const Matrix &A, const Matrix &B, int me, int BS, int NBLK, int n_threads) 
{
 //   ...
	for (int i = 0; i < NBLK / n_threads; i++) {
		int rB = me + i * n_threads;
	for (int kB = 0; kB < NBLK; kB++) {
	for (int cB = 0; cB < NBLK; cB++) {
	for (int r = 0; r < BS; r++) {
	for (int k = 0; k < BS; k++) {
		int my_r = rB * BS + r;
		int my_k = kB * BS + k;
	for (int c = 0; c < BS; c++) {
		int my_c = cB * BS + c;
	if (my_k == 0)
		C(my_r, my_c) = A(my_r, my_k) * B(my_k, my_c);
	else
		C(my_r, my_c) += A(my_r, my_k) * B(my_k, my_c);
                        }
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////
// This function does multithreaded, blocked matrix multiplication. It is for
// you to write. The parameters:
//	A, B: the input matrices
//	BS: block size; i.e., you should use blocks of BSxBS.
//	n_procs: how many processors to use.
// You must store the output in (*this), which already has its .data array
// allocated (but not necessarily cleared).
// Note that you can find out the size of the A, B and (*this) matrices by
// either looking at the _N member variable, or calling Matrix.N().
void Matrix::mpy2 (const Matrix &A, const Matrix &B, int BS, int n_procs) 
{
   // ...
   int NBLK= this->N()/BS;
   assert (this->N() >= BS);
   vector <thread> threads;
   for (int i = 0; i < n_procs; ++i)
        threads.push_back(thread(th_func2, ref(*this), ref(A), ref(B), i, BS, NBLK, n_procs));
   for (auto &th : threads)
        th.join();
}

