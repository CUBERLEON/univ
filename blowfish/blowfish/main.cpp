#include <iostream>
#include <string>
#include <chrono>

#include "blowfish.h"

#define ITERATIONS_CNT 1000000

using namespace std;

int main()
{
    CBlowFish blowfish;

    blowfish.Initialize((BYTE*)"23452345", 8);

    string text;
    cout << "Text: ";
    getline(cin, text);

    size_t text_len = text.size();
    size_t cipher_len = blowfish.GetOutputLength(text_len);

    string cipher;
    cipher.resize(cipher_len);
    string decoded_text;
    decoded_text.resize(text_len);

    auto start = chrono::system_clock::now();

    for (int i = 0; i < ITERATIONS_CNT; ++i)
    {
        blowfish.Encode((BYTE*)text.data(), (BYTE*)cipher.data(), text_len);
        blowfish.Decode((BYTE*)cipher.data(), (BYTE*)decoded_text.data(), cipher_len);
    }

    auto end = chrono::system_clock::now();
    cout << "Elapsed time: " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000. << " s" << endl;

    cout << "Cipher: \"" << cipher << "\"" << endl;
    cout << "Decoded text: \"" << decoded_text << "\"" << endl;

    system("pause");

    return 0;
}