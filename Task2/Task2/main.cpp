#include<iostream>
#include<vector>
#include<iomanip>
#include<thread>
#include<chrono>

using namespace std;

void sum_part(const vector<int>& vec1, const vector<int>& vec2,  vector<int>& result, int start, int end) 
{
	for (int i = start; i < end; ++i) {
		result[i] = vec1[i] + vec2[i];
	
	}

}
double parallel_sum(const vector<int>& vec1, const vector<int>& vec2, vector<int>& result, int num_threads) 
{
	vector<thread>threads;

	int size = vec1.size();

	int chunk_size = size / num_threads;

	int remainder = size % num_threads;

	auto start_time = chrono::high_resolution_clock::now();
	int start = 0;

	for (int i = 0; i < num_threads; ++i) 
	{
		int end = start + chunk_size + (i < remainder ? 1 : 0);
		threads.emplace_back(sum_part, cref(vec1), cref(vec2), ref(result), start, end);
		start = end;
	
	
	
	}
	for (auto& t : threads) {
		t.join();
	
	}
	auto end_time = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = end_time - start_time;
	return elapsed.count();

}

int main() 
{
	unsigned int num_cores = thread::hardware_concurrency();
	cout << "Number of cores available: " << num_cores << endl;

	vector<int>sizes{ 1000,10000,100000,1000000 };

	vector<int>threads_count{ 2,4,8,16 };

	cout << setw(10) << "Elements: " << setw(10) << "Threads: " << setw(20) << "Time seconds:" << endl;
	cout << "_________________________________________________________________________________________" << endl;

	for (int size : sizes) {

		vector<int>vec1(size, 1);
		vector<int>vec2(size, 2);
		vector<int>result(size, 0);

		for (int num_threads : threads_count) {
			double elapsed_time = parallel_sum(vec1, vec2, result, num_threads);
			cout << setw(10) << size << setw(10) << num_threads << setw(20) << elapsed_time << endl;
		}
		return 0;
	}	
}