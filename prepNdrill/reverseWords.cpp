#include <iostream>

using namespace std;
void reverseWords1(string &str);
void SwapWords(string &str, int l, int r);
void SwapString(string &str);

int main()
{
    string str = "Hello how are you?";
    reverseWords1(str);
    cout << str << endl;
    return 0;
}

void reverseWords1(string &str)
{
    int i = 0;
    int j = 0;
    while (str[i] != str.back())
    {
        while (str[j] == ' ')
        {
            ++j;
            ++i;
        }

        while (str[i] != ' ' && str[i] != str.back())
        {
            ++i;
        }

        if (str[i] == ' ' || str[i] == str.back())
        {
            SwapWords(str, j, i - 1);
            j = i;
        }
    }

    SwapString(str);
}

void SwapWords(string &str, int l, int r)
{
    for (; l < r; ++l, --r)
    {
        swap(str[l], str[r]);
    }
}

void SwapString(string &str)
{
    for (int l = 0, r = str.size() - 1; l < r; ++l, --r)
    {
        swap(str[l], str[r]);
    }
}
