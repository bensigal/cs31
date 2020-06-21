#include <iostream>

using namespace std;

class Dispenser {
public:
    Dispenser();
    int addWater(int num);
    bool dispenseWater();
private:
    int m_numGallons;
    int m_capacity;
};

Dispenser::Dispenser()
{
    m_numGallons = 0;
    m_capacity = 130;
}

int Dispenser::addWater(int num)
{
    if (num <= 0)
        return 0;
    if (m_numGallons + num > m_capacity)
        num = m_capacity - m_numGallons;
    m_numGallons += num;
    return num;
}

bool Dispenser::dispenseWater()
{
    if (m_numGallons == 0)
        return false;
    m_numGallons--;
    return true;
}

class VendingMachine
{
public:
    VendingMachine(double price, int startAmount);
    int addWater(int num);
    double purchase(double amt);
    double moneyRetained() const;
private:
    Dispenser m_dispenser;
    double    m_price;
    double    m_totalMoneyKept;
};

VendingMachine::VendingMachine(double price, int startAmount) {
    m_price = price;
    m_dispenser.addWater(startAmount);
    m_totalMoneyKept = 0;
}

int VendingMachine::addWater(int num) {
    return m_dispenser.addWater(num);
}

double VendingMachine::purchase(double amt) {

    double price = 0;

    //If there's enough money left for a gallon, try dispensing a gallon and adding it to the price.
    while (price + m_price < amt && m_dispenser.dispenseWater()) {
        price += m_price;
    }

    //Add amound paid to money kept
    m_totalMoneyKept += price;
    //Return amount given minus the price
    return amt - price;

}

double VendingMachine::moneyRetained() const {
    return m_totalMoneyKept;
}

VendingMachine* createVM() {
    VendingMachine vm(0.35, 130);
    vm.purchase(14);
    return &vm;
}

int main() {

    VendingMachine vm(3.50, 0);

    cout << vm.addWater(20) << endl;
    cout << vm.moneyRetained() << endl;
    cout << vm.purchase(100) << endl;
    cout << vm.moneyRetained() << endl;

    createVM();

}