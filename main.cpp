#include <iostream>
#include <climits>
#include <string>
#include <vector>
#include <sstream>

using byte = unsigned char;

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}


class IPAddress{
private:
    byte a,b,c,d;
    byte* array[4] = {&a, &b, &c, &d};
    friend std::ostream& operator<<(std::ostream& os, const IPAddress& ipAddress);
public:
    IPAddress(byte _a, byte _b, byte _c, byte _d){
        a = _a;
        b = _b;
        c = _c;
        d = _d;
    }

    IPAddress operator+(const IPAddress& rhs) const{
        byte new_a = (1u*a + rhs.a) % UCHAR_MAX;
        byte new_b = (1u*b + rhs.b) % UCHAR_MAX;
        byte new_c = (1u*c + rhs.c) % UCHAR_MAX;
        byte new_d = (1u*d + rhs.d) % UCHAR_MAX;
        return IPAddress(new_a, new_b, new_c, new_d);
    }

    IPAddress operator-(const IPAddress& rhs) const{
        byte new_a = (byte) (1u*a - rhs.a);
        byte new_b = (byte) (1u*b - rhs.b);
        byte new_c = (byte) (1u*c - rhs.c);
        byte new_d = (byte) (1u*d - rhs.d);

        return IPAddress(new_a, new_b, new_c, new_d);
    }

    bool operator==(const IPAddress& rhs) const{
        return (a == rhs.a) * (b == rhs.b) * (c == rhs.c) * (d == rhs.d);
    }

    bool operator<(const IPAddress& rhs) const{
        for(int i = 0; i < 4; i++){
            if (*(array[i]) > *(rhs.array[i])){
                return false;
            }
        }
        return true;
    }

    bool operator>(const IPAddress& rhs) const{
        for(int i = 0; i < 4; i++){
            if (*(array[i]) < *(rhs.array[i])){
                return false;
            }
        }
        return true;
    }

    bool belong_to_subnet(const IPAddress& subnet, const IPAddress& mask) const{
        byte new_a = a & mask.a;
        byte new_b = b & mask.b;
        byte new_c = c & mask.c;
        byte new_d = d & mask.d;
        return IPAddress(new_a, new_b, new_c, new_d) == subnet;
    }
};

IPAddress operator "" _ipAddr(const char* str, size_t size){
    std::vector<std::string> numbers = split(str, '.');
    return IPAddress(std::stoi(numbers[0]),std::stoi(numbers[1])
                     ,std::stoi(numbers[2]),std::stoi(numbers[3]));
}

std::ostream& operator<<(std::ostream& os, const IPAddress& ipAddress){
    std::string out = std::to_string(ipAddress.a) + '.' +
                      std::to_string(ipAddress.b) + '.' +
                      std::to_string(ipAddress.c) + '.' +
                      std::to_string(ipAddress.d);

    return os << out;
}

void help(){
    std::cout << "write exit for exit" << std::endl;
    std::cout << "write help for help" << std::endl;
    std::cout << "input format ipAdress1 ipAdress2 (ipAdress3) action" << std::endl;
    std::cout << "in input mode add - addition, sub - subtraction, less - ipAdress1 < ipAdress2, "
                 "greater - ipAdress1 > ipAdress2, eq for ipAdress1 == ipAdress2, "
                 "belong_to_subnet - ipAdress, subnet, mask, literal - show literal for 192 168 1 1" << std::endl;
}

int main() {
    std::string cmd;
    std::vector<std::string> out;
    std::stringstream buffer;
    int count = 0;
    help();
    while (true){
        getline(std::cin, cmd);
        if (cmd == "exit"){
            break;
        }
        else if (cmd == "help"){
            help();
        }
        else {
            count++;
            byte a1,b1,c1,d1;
            byte a2,b2,c2,d2;
            byte a3,b3,c3,d3;
            std::string action;
            std::vector<std::string> x = split(cmd, ' ');
            if (x.size() == 9){
                a1 = stod(x[0]);
                b1 = stod(x[1]);
                c1 = stod(x[2]);
                d1 = stod(x[3]);

                a2 = stod(x[4]);
                b2 = stod(x[5]);
                c2 = stod(x[6]);
                d2 = stod(x[7]);
                action = x[8];
            }
            else if (x.size() == 13){
                a1 = stod(x[0]);
                b1 = stod(x[1]);
                c1 = stod(x[2]);
                d1 = stod(x[3]);

                a2 = stod(x[4]);
                b2 = stod(x[5]);
                c2 = stod(x[6]);
                d2 = stod(x[7]);

                a3 = stod(x[8]);
                b3 = stod(x[9]);
                c3 = stod(x[10]);
                d3 = stod(x[11]);
                action = x[12];
            }

            IPAddress a = {a1, b1, c1, d1};
            IPAddress b = {a2, b2, c2, d2};
            IPAddress c = {a3, b3, c3, d3};

            if (action == "add"){
                buffer << "out " << count << ": " << (a + b) << std::endl;
            }
            else if (action == "sub"){
                buffer << "out " << count << ": " << (a - b) << std::endl;
            }
            else if (action == "less"){
                buffer << "out " << count << ": " << (a < b) << std::endl;
            }
            else if (action == "greater"){
                buffer << "out " << count << ": " << (a > b) << std::endl;
            }
            else if (action == "eq"){
                buffer << "out " << count << ": " << (a == b) << std::endl;
            }
            else if (action == "belong_to_subnet"){
                buffer << "out " << count << ": " << a.belong_to_subnet(b, c) << std::endl;
            }
            else if (action == "literal"){
                buffer << "out " << count << ": " << "192.168.1.1"_ipAddr << std::endl;
            }
        }
    }
    std::cout << buffer.str() << std::endl;
    return 0;
}
