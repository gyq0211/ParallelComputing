// Lab 1: Histogram generation 
// This is the "user" file that you fill in.

#include <vector>
#include <mutex>
#include <thread>
#include <pthread.h>
#include "ee155_utils.hxx"
using namespace std;
// How many buckets the histogram has.
extern const int N_BUCKETS;
static std::mutex g_mut;	// For printing with LOGM
static void th_func (int me, std::vector<int> &final_hist,const std::vector<int> &input_data, int n_threads);
//************************************************************************
// This is the function that you (mostly) write yourself. It spawns the thread
// function th_func() as many times as requested and waits for them to finish.
//************************************************************************
std::vector<int> compute_multithread
	(const std::vector<int> &input_data, int n_threads) 
{
	vector <thread> threads;
	vector <int> final_hist;
	final_hist = vector<int>(input_data.size(),0);
	for (int i=0;i<n_threads;++i)
		threads.push_back(thread(th_func,i,std::ref(final_hist),std::ref(input_data),n_threads));
	for (auto &th:threads)
		th.join();
	return final_hist;
}

//************************************************************************
// You write this function yourself also. It gets instantiated multiple times
// in different threads.
//************************************************************************
static void th_func (int me, std::vector<int> &final_hist,
		     const std::vector<int> &input_data, int n_threads) 
{
	for (int idx=me;idx<input_data.size();idx+=n_threads)
		{
			g_mut.lock();
			++final_hist[input_data[idx]];
			g_mut.unlock();
		}
}
