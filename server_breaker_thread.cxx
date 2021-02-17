// This is the per-thread function that goes along with server_breaker.cxx

#include <vector>
#include <mutex>
#include <iostream>
#include "ee155_utils.hxx"
using namespace std;
// Fixed by the CPU microarchitecture.
static const int BYTES_PER_LINE=64;
extern std::mutex g_mut;	// For debug printing.

// The function that each thread executes.
// You must write this one yourself.
// For details on what it does, see the homework assignment .pdf.
// The parameters:
//	me: my thread id. Threads are numbered 0, 1, etc.
//	indices: a vector of line numbers. The lines are numbered [0,MAX_LINES)
//		They are used to index into g_mem[]. However, note that g_mem[]
//		is a vector of bytes, not of 64B lines. Thus, the actual index
//		into g_mem[] for line #i would be i*BYTES_PER_LINE.
//	g_mem: global memory, where the various cached lines live.
//	n_stores, n_loads: the number of times that a particular loop stores
//		or loads into its memory location.
void compute_thread (int me, const std::vector<int> &indices,
		     unsigned char g_mem[],
		     int n_stores, int n_loads, int n_loops)
		     {
    unsigned int data_val = 0;

    // main loop: loop through all lines as many times as requested.
    g_mut.lock();
    for (int loop=0; loop<n_loops; ++loop)
    {
	for (int s=0; s<n_stores; ++s)
	{
		for(int idx=0;idx < indices.size();++idx)
		{
			g_mem[indices[idx]*BYTES_PER_LINE + me] = ((idx+1)&0xFF);
		}
       	 //cout<<"store"<<s<<endl;
	 //... go through all of the lines, and do the stores.
        //g_mem[s*BYTES_PER_LINE + me] = s & 0xFF;
	}

	// Go through all of the lines again, and read the data back.
	for (int l=0; l<n_loads; ++l)
	{
	    //...
		for(int idx=0;idx < indices.size();++idx)
		{
			data_val = g_mem[indices[idx]*BYTES_PER_LINE + me];
		//if (data_val != ((idx+1) & 0xFF))
			//data_val = indices[idx];//l,indices[idx],((idx+1) & 0xFF)
			//cout<<"not same!"<<endl;
		}
	}
	//cout<<"loop:"<<loop<<endl;
    }
    g_mut.unlock();
}
