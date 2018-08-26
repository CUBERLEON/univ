#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>

using namespace std;

class string_set
{
public:
    string_set(size_t capacity)
        : m_data(capacity)
    {}

    void insert(const string& s)
    {
        auto& list = m_data[hash(s)];
        list.push_back(s);
    }

    bool contains(const string& s)
    {
        auto& list = m_data[hash(s)];
        return find(list.begin(), list.end(), s) != list.end();
    }

    bool erase(const string& s)
    {
        auto& list = m_data[hash(s)];
        auto it = find(list.begin(), list.end(), s);
        if (it != list.end())
        {
            list.erase(it);
            return true;
        }
        return false;
    }
private:
    vector<list<string>> m_data;

    constexpr static int MOD = 1e9 + 7;

    int hash(const string& s)
    {
        int result = 0;

        for (size_t i = 0; i < s.size(); ++i)
        {
            result = (result * 2 + s[i]) % MOD;
        }

        return result % m_data.size();
    }
};

int main()
{
    string_set s(1000);

    ifstream file("input.txt");

    string type;
    while (file >> type)
    {
        string value;
        if (type == "+")
        {
            file >> value;
            s.insert(value);
        }
        else if (type == "?")
        {
            file >> value;
            cout << "Contains '" << value << "'? = " << (s.contains(value) ? "OK" : "FAIL") << endl;
        }
        else if (type == "-")
        {
            file >> value;
            cout << "Erase '" << value << "' = " << (s.erase(value) ? "OK" : "FAIL") << endl;
        }
        else
        {
            break;
        }
    }

    file.close();

    system("pause");

    return 0;
}