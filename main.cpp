#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

typedef unsigned long long LLU;

// Bubble sort
// count sort
// radix sort (mai multe baze, infoarena)
// merge sort
// quicksort
// sortare nativa
// timpul masurat (clock inainte de apel, si dupa)
// pr caz 10^6 nr < 10^6... blubble sort timp, quicksort timp..
// verificam daca sortarea e corecta (ord crescatoare)
// rulat 10^7 numere
// toate testele intr-un fisier
// countsort sa zica ca nu se descurca pe testul ala cu 10^9
// la fel si bubble sort
// bubble sort 10^4 c/c++
// skiplist sort

void BubbleSort(LLU* v, int n)
{
    bool adr = true;
    if (n >= 10000)
    {
        cout << "Too many numbers for Bubble sort." << endl;
        return;
    }
    while(adr)
    {
        adr = false;

        for (int i = 0; i < n - 1; i++)
        {
            if (v[i] > v[i + 1])
            {
                LLU tmp = v[i];
                v[i] = v[i + 1];
                v[i + 1] = tmp;

                adr = true;
            }
        }
    }
}

void CountSort(LLU* v, int n)
{
    int ix = 0;

    if (n <= 0)
        return;

    LLU mx = v[0];
    for (int i = 0; i < n; i++)
        if (v[i] > mx)
            mx = v[i];

    if (mx >= 10000000)
    {
        cout << "Values too big for Count sort." << endl;
        return;
    }

    int* frv = new int[mx + 1];
    memset(frv, 0, sizeof(int) * (mx + 1));

    for (int i = 0; i < n; i++)
        frv[v[i]]++;

    for (LLU i = 0; i <= mx; i++)
        for (int j = 0; j < frv[i]; j++)
            v[ix++] = i;

    delete[] frv;
    frv = NULL;
}

void RadixSort(LLU* v, int n, int imp, int base)
{
    if (n <= 0 || imp <= 0)
        return;

    LLU** fv = new LLU*[base];
    int cnt = 0;

    for (int i = 0; i < base; i++)
    {
        fv[i] = new LLU[n + 1];
        memset(fv[i], 0, sizeof(LLU) * (n + 1));
    }

    for (int i = 0; i < n; i++)
    {
        int d = (v[i] / imp) % base;
        fv[d][++fv[d][0]] = v[i];
    }

    for (int i = 0; i < base; i++)
        RadixSort(&fv[i][1], fv[i][0], imp / base, base);


    for (int i = 0; i < base; i++)
        for (int j = 1; j <= fv[i][0]; j++)
            v[cnt++] = fv[i][j];

    for (int i = 0; i < base; i++)
    {
        delete[] fv[i];
        fv[i] = NULL;
    }

    delete[] fv;
    fv = NULL;
}

void MergeSort(LLU* v, int n)
{
    if (n <= 1)
        return;

    MergeSort(v, n / 2);
    MergeSort(v + (n / 2), n - (n / 2));

    LLU* aux = new LLU[n];

    int i = 0;
    int j = n / 2;
    int ix = 0;

    while(i < n / 2 && j < n)
    {
        if (v[i] < v[j])
            aux[ix++] = v[i++];
        else
            aux[ix++] = v[j++];
    }
    while (i < n / 2)
        aux[ix++] = v[i++];
    while (j < n)
        aux[ix++] = v[j++];

    memcpy(v, aux, sizeof(LLU) * n);

    delete[] aux;
    aux = NULL;
}

void QuickSort(LLU* v, int n, int pivotType)
{
    if (n <= 1)
        return;

    int i = 0;
    int j = n - 1;
    int pIx = rand() % n;

    if (pivotType == 0)
        pIx = 0;
    if (pivotType == 1)
        pIx = n - 1;
    int pivot = v[pIx];

    while (i <= j)
    {
        while(i < n && v[i] < pivot)
            i++;

        while(j >= 0 && v[j] > pivot)
            j--;

        if (i <= j)
        {
            int tmp = v[i];
            v[i] = v[j];
            v[j] = tmp;

            i++;
            j--;
        }
    }

    QuickSort(v, j + 1, pivotType);
    QuickSort(v + i, n - i, pivotType);
}

void Nativa(LLU* v, int n)
{
    sort(v, v + n);
}

void SiftDown(LLU* arr, int poz, int n)
{
    if (poz < 1 || poz > n)
        return;

    int left = poz * 2;
    int right = (poz * 2) + 1;

    if (left <= n)
    {
        int mn = poz;
        int mnVal = arr[poz];
        if (arr[left] < mnVal)
        {
            mn = left;
            mnVal = arr[left];
        }
        if (right <= n)
        {
            if (arr[right] < mnVal)
            {
                mn = right;
                mnVal = arr[right];
            }
        }

        if (mn != poz)
        {
            int tmp = arr[mn];
            arr[mn] = arr[poz];
            arr[poz] = tmp;

            SiftDown(arr, mn, n);
        }
    }
}

void HeapSort(LLU* v, int n)
{
    LLU* arr = new LLU[n];
    memcpy(arr, v, sizeof(LLU) * n);

    for (int i = (n / 2) + 1; i >= 1; i--)
        SiftDown(arr - 1, i, n);

    int length = n;
    for (int i = 0; i < n; i++)
    {
        int top = arr[0];
        v[i] = top;

        if (i != n - 1)
        {
            arr[0] = arr[length - 1];
            SiftDown(arr - 1, 1, length - 1);
        }
        length--;
    }

    delete[] arr;
    arr = NULL;
}

bool Verifica(LLU* v, int n)
{
    for (int i = 1; i < n; i++)
        if (v[i] < v[i - 1])
            return false;

    return true;
}

string OKString(bool v)
{
    return (v ? "OK" : "NOT OK");
}

int main()
{
    ifstream fin("input.txt");

    int n, k;
    LLU* v;

    fin >> k;
    for (int l = 0; l < k; l++)
    {
        int radixBase;
        fin >> n;
        fin >> radixBase;

        v = new LLU[n];

        for (int i = 0; i < n; i++)
            fin >> v[i];

        LLU* sorted = new LLU[n];

        cout << "TEST " << l + 1 << endl;

        memcpy(sorted, v, sizeof(LLU) * n);
        auto tm1 = chrono::high_resolution_clock::now();
        BubbleSort(sorted, n);
        auto tm2 = chrono::high_resolution_clock::now();
        cout << "Bubble sort: " << OKString(Verifica(sorted, n)) << endl;
        auto duration = chrono::duration_cast<chrono::milliseconds>(tm2 - tm1);
        cout << "Timp: " << duration.count() << " ms" << endl;

        memcpy(sorted, v, sizeof(LLU) * n);
        tm1 = chrono::high_resolution_clock::now();
        CountSort(sorted, n);
        tm2 = chrono::high_resolution_clock::now();
        cout << "Count sort: " << OKString(Verifica(sorted, n)) << endl;
        duration = chrono::duration_cast<chrono::milliseconds>(tm2 - tm1);
        cout << "Timp: " << duration.count() << " ms" << endl;

        memcpy(sorted, v, sizeof(LLU) * n);

        int imp = 1;
        LLU mx = 0;
        for (int i = 0; i < n; i++)
            mx = max(mx, sorted[i]);

        while (imp < mx)
            imp *= radixBase;

        if (imp > mx)
            imp /= radixBase;

        tm1 = chrono::high_resolution_clock::now();
        RadixSort(sorted, n, imp, radixBase);
        tm2 = chrono::high_resolution_clock::now();

        cout << "Radix sort: " << OKString(Verifica(sorted, n)) << endl;
        duration = chrono::duration_cast<chrono::milliseconds>(tm2 - tm1);
        cout << "Timp: " << duration.count() << " ms" << endl;

        memcpy(sorted, v, sizeof(LLU) * n);
        tm1 = chrono::high_resolution_clock::now();
        MergeSort(sorted, n);
        tm2 = chrono::high_resolution_clock::now();
        cout << "Merge sort: " << OKString(Verifica(sorted, n)) << endl;
        duration = chrono::duration_cast<chrono::milliseconds>(tm2 - tm1);
        cout << "Timp: " << duration.count() << " ms" << endl;

        memcpy(sorted, v, sizeof(LLU) * n);
        tm1 = chrono::high_resolution_clock::now();
        QuickSort(sorted, n, 2);
        tm2 = chrono::high_resolution_clock::now();
        cout << "Random Quick sort: " << OKString(Verifica(sorted, n)) << endl;
        duration = chrono::duration_cast<chrono::milliseconds>(tm2 - tm1);
        cout << "Timp: " << duration.count() << " ms" << endl;

        memcpy(sorted, v, sizeof(LLU) * n);
        tm1 = chrono::high_resolution_clock::now();
        QuickSort(sorted, n, 0);
        tm2 = chrono::high_resolution_clock::now();
        cout << "Left pivot Quick sort: " << OKString(Verifica(sorted, n)) << endl;
        duration = chrono::duration_cast<chrono::milliseconds>(tm2 - tm1);
        cout << "Timp: " << duration.count() << " ms" << endl;

        memcpy(sorted, v, sizeof(LLU) * n);
        tm1 = chrono::high_resolution_clock::now();
        QuickSort(sorted, n, 1);
        tm2 = chrono::high_resolution_clock::now();
        cout << "Right pivot Quick sort: " << OKString(Verifica(sorted, n)) << endl;
        duration = chrono::duration_cast<chrono::milliseconds>(tm2 - tm1);
        cout << "Timp: " << duration.count() << " ms" << endl;

        memcpy(sorted, v, sizeof(LLU) * n);
        tm1 = chrono::high_resolution_clock::now();
        HeapSort(sorted, n);
        tm2 = chrono::high_resolution_clock::now();
        cout << "Heap sort: " << OKString(Verifica(sorted, n)) << endl;
        duration = chrono::duration_cast<chrono::milliseconds>(tm2 - tm1);
        cout << "Timp: " << duration.count() << " ms" << endl;

        cout << endl;

        delete[] sorted;
        sorted = NULL;

        delete[] v;
        v = NULL;
    }

    return 0;
}
