#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include <ctime>

std::mutex kitchen_mtx;

std::vector<std::string>orders;
std::vector<std::string>delivery;

constexpr int nOrders = 10;
bool flag = false;

std::string dishGenerator(){
    int n = rand()%5;
    if(n == 0){
        return "pizza";
    } else if(n == 1) {
        return "soup";
    } else if(n == 2) {
        return "steak";
    } else if(n == 3) {
        return "salad";
    } else{
        return "sushi";
    }
}

void orderGenerator(){
    while (!flag) {
        int waitTime = 5 + rand() % 6;
        std::this_thread::sleep_for(std::chrono::seconds(waitTime));
        std::string newOrder = dishGenerator();
        std::cout << "Waiter:: order accepted: " << newOrder <<std::endl;
        kitchen_mtx.lock();
        orders.push_back(newOrder);
        kitchen_mtx.unlock();
    }

}

void cooking(){
    int cookedOrders = 0;
    while (!flag){
        int waitTime = 5 + rand() % 11;
        if(!orders.empty()){
            std::cout<<"Kitchen:: order "<<orders[0]<<" is cooking"<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(waitTime));
            kitchen_mtx.lock();
            delivery.push_back(orders[0]);
            std::cout<<"Kitchen:: The order "<<orders[0]<<" has been sent to delivery!"<<std::endl;
            cookedOrders++;
            orders.erase(orders.begin());
            kitchen_mtx.unlock();
        }
    }
}

void delivering(){
    int deliveredOrders = 0;
    while(deliveredOrders < nOrders) {
        if (!delivery.empty()){
            std::cout << "Courier:: The order " << delivery[0] << " is delivering!" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(30));
            std::cout << "Courier:: The order " << delivery[0] << " is delivered!" << std::endl;
            kitchen_mtx.lock();
            delivery.erase(delivery.begin());
            deliveredOrders++;
            kitchen_mtx.unlock();

        }
    }
    flag = true;
}

int main() {
    srand(std::time(nullptr));
    std::thread waiter(orderGenerator);
    std::thread kitchen(cooking);
    std::thread courier(delivering);
    waiter.detach();
    kitchen.detach();
    courier.join();

 //   for(int i = 0; i<orders.size();i++){
 //       std::cout<<orders[i]<<std::endl;
 //   }
//    std::cout << orderGenerator() << std::endl;
    return 0;
}
