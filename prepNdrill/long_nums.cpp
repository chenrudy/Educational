#include <iostream>
#include <string>

using namespace std;
std::string Add(std::string a, std::string b);
void RoundEdges(const std::string &src, std::string &dest, int i);

int main()
{
    std::string x = "1546975465467451";
    std::string y = "5454112100024554747";
    std::cout << Add(x, y) << std::endl;
    return 0;
}

std::string Add(std::string a, std::string b)
{
    int ia = a.size() - 1;
    int ib = b.size() - 1;
    std::string result;
    int reminder = 0;

    for (; ia >= 0 && ib >= 0; --ia, --ib)
    {
        char sum = (a[ia] - 48) + (b[ib] - 48) + reminder;
        char c = (sum % 10) + 48;
        reminder = sum / 10;
        result.insert(result.begin(), c);
    }

    RoundEdges(a, result, ia);
    RoundEdges(b, result, ib);

    return result;
}

void RoundEdges(const std::string &src, std::string &dest, int i)
{
    while (i >= 0)
    {
        dest.insert(dest.begin(), src[i]);
        --i;
    }
}
