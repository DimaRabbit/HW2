#include<iostream>
#include<mutex>
#include<thread>
#include<chrono>

using namespace std;

mutex mtx;


int client_counter = 0;

bool continue_processing = true;


void client(int max_client) 
{
	while (client_counter < max_client) 
	{
	
		this_thread::sleep_for(chrono::seconds(1));
		lock_guard<mutex>lock(mtx);
		if (client_counter < max_client) {
			client_counter++;
			cout << "New lient add. Client in the queue: " << client_counter << endl;
		
		}
	}
}


void operator_window() 
{
	while (continue_processing || client_counter > 0) 
	{
		this_thread::sleep_for(chrono::seconds(2));
		lock_guard<mutex>lock(mtx);
		if(client_counter > 0)
		{
			client_counter--;
			cout << "The operator served the client. Left in the queue: " << client_counter << endl;
		}
		if(client_counter==0&& !continue_processing) {
			break;
		
		}
	
	}

}

int main() 
{
	int max_clients = 0;

	cout << "Enter number clients: ";
	cin >> max_clients;

	thread t1(client, max_clients);

	thread t2(operator_window);

	t1.join();
	continue_processing = false;
	t2.join();

	cout << "All clients are served. The program is completed!" << endl;

	return 0;
}