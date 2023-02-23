#include <iostream>
#include <bits/stdc++.h>
#include <stack>

/*

Implementacja wielomianów na bazie list powiązanych pojedynczo (szablon, współczynniki typu T).
Działania na wielomianach:
    -dodawanie (operator+),
    -odejmowanie (operator-),
    -mnożenie (operator*),
    -obliczanie wartości wielomianu,
    -porównywanie
        -operator==,
        -operator!=,
    -wyświetlanie.
Wielomiany są równe, gdy ich różnica jest wielomianem zerowym.
Klasa Term przechowuje jeden niezerowy wyraz wielomianu (c*x^n).
Klasa Poly przechowuje łącze do posortowanej listy powiązanej wyrazów i implementuje działania.
Przydają się funkcje
    -clear(),
    -is_zero(),
    -operator[] (odczyt współczynnika przy danej potędze x).

*/

template <typename T>
struct Term
{
    T wspolczynnik;
    int potega;
    Term *next;
    Term(int s = 0) : wspolczynnik(T()), potega(s), next(nullptr) {} // konstruktor domyslny
    Term(const T &item, const int &power, Term *ptr = nullptr) : wspolczynnik(item), potega(power), next(ptr) {}
    ~Term() {} // destruktor
};

template <typename T>
class Polynomial
{
    Term<T> *head, *tail;
    int stopien;

public:
    Polynomial() : head(nullptr), tail(nullptr), stopien(-1) {}
    ~Polynomial()
    {
        clear();
    }

    Polynomial(const Polynomial &other)
    {
        head = tail = nullptr;
        stopien = -1;
        Term<T> *otherValue = other.head;
        while (otherValue != nullptr)
        {
            if (head == nullptr)
            {
                head = tail = new Term<T>(otherValue->wspolczynnik, otherValue->potega);
                stopien = otherValue->potega;
            }
            else
            {
                tail->next = new Term<T>(otherValue->wspolczynnik, otherValue->potega);
                tail = tail->next;
                stopien = otherValue->potega;
            }
            otherValue = otherValue->next;
        }
    }

    Polynomial &operator=(const Polynomial &other)
    {
        if (!isZero())
        {
            clear();
        }
        if (!other.isZero())
        {
            Term<T> *otherValue = other.head;
            while (otherValue != nullptr)
            {
                if (head == nullptr)
                {
                    head = tail = new Term<T>(otherValue->wspolczynnik, otherValue->potega);
                    stopien = otherValue->potega;
                }
                else
                {
                    tail->next = new Term<T>(otherValue->wspolczynnik, otherValue->potega);
                    tail = tail->next;
                    stopien = otherValue->potega;
                }
                otherValue = otherValue->next;
            }
        }
        return *this;
    }

    bool isZero() const { return head == nullptr; }
    int size() const
    {
        int rozmiar = 0;
        Term<T> *current = head;
        while (current != nullptr)
        {
            rozmiar++;
            current = current->next;
        }
        return rozmiar;
    }

    void addTerm(const T &value, const int &power)
    {
        if (value == T())
            return;
        if (isZero())
        {
            push_front(value, power);
        }
        else if (power > stopien)
        {
            push_back(value, power);
        }
        else
        {
            Term<T> *currentNode = head;
            Term<T> *previousNode = nullptr;
            while (currentNode->potega < power && currentNode != nullptr)
            {
                previousNode = currentNode;
                currentNode = currentNode->next;
            }
            if (currentNode->potega == power)
            {
                currentNode->wspolczynnik += value;
                if (currentNode->wspolczynnik == 0)
                    erase(currentNode->potega);
                return;
            }
            if (currentNode == head)
            {
                push_front(value, power);
                return;
            }
            previousNode->next = new Term<T>(value, power, currentNode);
        }
    }

    void push_front(const T &value, const int &power)
    {
        if (isZero())
        {
            head = tail = new Term<T>(value, power);
            stopien = power;
        }
        else
        {
            head = new Term<T>(value, power, head);
        }
    }
    void push_back(const T &value, const int &power)
    {
        if (isZero())
        {
            head = tail = new Term<T>(value, power);
            stopien = power;
        }
        else
        {
            tail->next = new Term<T>(value, power, nullptr);
            tail = tail->next;
            stopien = power;
        }
    } // O(1), L.push_back(item)
    T &front() const { return head->wspolczynnik; }
    T &back() const { return tail->wspolczynnik; }

    void pop_front()
    {
        if (!isZero())
        {
            Term<T> *temp = head;
            if (head == tail)
            {
                head = tail = nullptr;
                stopien = -1;
            }
            else
            {
                head = head->next;
            }
            delete temp;
        }
    } // usuwa poczatek O(1)

    void pop_back()
    {
        if (!isZero())
        {
            Term<T> *lastNode = tail;
            if (head == tail)
            {
                head = tail = nullptr;
                stopien = -1;
            }
            else
            {
                Term<T> *previousNode = head;
                while (previousNode->next != tail)
                {
                    previousNode = previousNode->next;
                }
                stopien = previousNode->potega;
                tail = previousNode;
                tail->next = nullptr;
            }
            delete lastNode;
        }
    } // usuwa koniec O(n)

    void clear()
    {
        if (!isZero())
        {
            Term<T> *currentNode = head;
            Term<T> *nextNode = nullptr;
            while (currentNode != nullptr)
            {
                nextNode = currentNode->next;
                delete currentNode;
                currentNode = nextNode;
            }
            head = tail = nullptr;
            stopien = -1;
        }
    }
    void display()
    {
        std::stack<Term<T> *> stos;
        Term<T> *currentNode = head;
        Term<T> *node = nullptr;
        while (currentNode != nullptr)
        {
            stos.push(currentNode);
            currentNode = currentNode->next;
        }
        while (!stos.empty())
        {
            node = stos.top();
            if (node->wspolczynnik != 1 || node->wspolczynnik != -1)
                std::cout << node->wspolczynnik;
            else if (node->wspolczynnik == -1 && node->potega != 0)
                std::cout << "-";
            else
                std::cout << "- 1";
            if (node->potega != 0)
                std::cout << "x^" << node->potega;
            stos.pop();
            if (!stos.empty())
            {
                if (stos.top()->wspolczynnik > 0)
                    std::cout << " + ";
                else
                    std::cout << " ";
            }
        }
    }
    T &operator[](int power)
    {
        if (!isZero() && power >= 0 && power <= stopien)
        {
            Term<T> *currentNode = head;
            while (currentNode->potega != power && currentNode != nullptr)
                currentNode = currentNode->next;
            return currentNode->wspolczynnik;
        }
        return T();
    } // podstawienie L[pos]=item, odczyt L[pos]
    void erase(int power)
    {
        if (isZero())
            return;
        if (power == stopien)
            pop_back();
        if (power >= 0 && power < stopien)
        {
            Term<T> *currentNode = head;
            Term<T> *previousNode = nullptr;
            while (currentNode->potega != power && currentNode != nullptr)
            {
                previousNode = currentNode;
                currentNode = currentNode->next;
            }
            if (currentNode == head)
            {
                pop_front();
            }
            else if (currentNode != nullptr)
            {
                previousNode->next = previousNode->next->next;
                delete currentNode;
            }
        }
    }

    T horner(const T &x)
    {
        T wynik = T();
        T temp;
        Term<T> *currentNode = head;
        while (currentNode != nullptr)
        {
            temp = currentNode->wspolczynnik;
            for (int i = 0; i < currentNode->potega; i++)
            {
                temp *= x;
            }
            wynik += temp;
            currentNode = currentNode->next;
        }
        return wynik;
    }

    Polynomial operator+(const Polynomial &other)
    {
        Polynomial<T> wynik(other);
        Term<T> *currentNode = head;
        while (currentNode != nullptr)
        {
            wynik.addTerm(currentNode->wspolczynnik, currentNode->potega);
            currentNode = currentNode->next;
        }
        return wynik;
    }

    Polynomial operator-(const Polynomial &other)
    {
        Polynomial<T> wynik(*this);
        Term<T> *currentNode = other.head;
        while (currentNode != nullptr)
        {
            wynik.addTerm((currentNode->wspolczynnik * -1), currentNode->potega);
            currentNode = currentNode->next;
        }
        return wynik;
    }

    Polynomial operator*(const Polynomial &other)
    {
        Polynomial<T> wynik;
        Term<T> *currentNode = head;
        Term<T> *otherNode;
        T nowyWspolczynnik;
        int nowaPotega;
        while (currentNode != nullptr)
        {
            otherNode = other.head;
            while (otherNode != nullptr)
            {
                nowyWspolczynnik = ((currentNode->wspolczynnik) * (otherNode->wspolczynnik));
                nowaPotega = ((currentNode->potega) + (otherNode->potega));
                wynik.addTerm(nowyWspolczynnik, nowaPotega);
                otherNode = otherNode->next;
            }
            currentNode = currentNode->next;
        }
        return wynik;
    }

    bool operator==(const Polynomial &other)
    {
        if (stopien != other.stopien)
            return false;
        Term<T> *thisNode = head;
        Term<T> *otherNode = other.head;
        while (thisNode != nullptr && otherNode != nullptr)
        {
            if ((thisNode->potega != otherNode->potega) || (thisNode->wspolczynnik != otherNode->wspolczynnik))
                return false;
            thisNode = thisNode->next;
            otherNode = otherNode->next;
        }
        return true;
    }

    bool operator!=(const Polynomial &other)
    {
        if (stopien != other.stopien)
            return true;
        Term<T> *thisNode = head;
        Term<T> *otherNode = other.head;
        while (thisNode != nullptr && otherNode != nullptr)
        {
            if ((thisNode->potega != otherNode->potega) || (thisNode->wspolczynnik != otherNode->wspolczynnik))
                return true;
            thisNode = thisNode->next;
            otherNode = otherNode->next;
        }
        return false;
    }
};

int main()
{
    for (int i = 0; i < 150; i++)
        std::cout << "-";
    std::cout << std::endl
              << std::endl;
    for (int i = 0; i < 5; i++)
        std::cout << "\t";
    std::cout << "Implementacja wielomianów na bazie list powiązanych pojedynczo" << std::endl
              << std::endl;
    for (int i = 0; i < 150; i++)
        std::cout << "-";
    std::cout << std::endl
              << std::endl;

    //  Utworzenie wielomianów

    std::cout << "\tTworzenie oraz wyświetlanie wielomianów:\n\n";

    Polynomial<int> wielomian1;
    wielomian1.addTerm(2, 6);
    wielomian1.addTerm(-3, 4);
    wielomian1.addTerm(2, 1);
    Polynomial<int> wielomian2(wielomian1);
    wielomian2.addTerm(1, 4);
    wielomian2.addTerm(8, 2);
    wielomian2.addTerm(4, 1);
    wielomian2.addTerm(-1, 0);
    Polynomial<int> wielomian3;
    wielomian3 = wielomian2;
    wielomian3.addTerm(1, 6);
    wielomian3.addTerm(5, 4);
    wielomian3.addTerm(6, 2);
    //

    //  Wyświetlenie
    std::cout << "Pierwszy wielomian: \n\t";
    wielomian1.display();
    std::cout << std::endl
              << std::endl;
    std::cout << "Drugi wielomian: \n\t";
    wielomian2.display();
    std::cout << std::endl
              << std::endl;
    std::cout << "Trzeci wielomian: \n\t";
    wielomian3.display();
    std::cout << std::endl
              << std::endl;

    for (int i = 0; i < 150; i++)
        std::cout << "-";
    std::cout << std::endl
              << std::endl;

    //  Działania na wielomianach

    std::cout << "\tDziałania na wielomianach:\n\n";
    Polynomial<int> wielomian4; // Różnica
    wielomian4 = ((wielomian3 - wielomian2) - wielomian1);

    Polynomial<int> wielomian5; // Suma
    wielomian5 = ((wielomian3 + wielomian2) + wielomian1);

    Polynomial<int> wielomian6; // Iloczyn
    wielomian6 = (wielomian2 * wielomian3);
    //

    // Wyświetlenie wyników
    std::cout << "Różnica wielomianów: \n\t";
    wielomian4.display();
    std::cout << std::endl
              << std::endl;
    std::cout << "Suma wielomianów: \n\t";
    wielomian5.display();
    std::cout << std::endl
              << std::endl;
    std::cout << "Iloczyn wielomianów 2 i 3: \n\t";
    wielomian6.display();
    std::cout << std::endl
              << std::endl;

    for (int i = 0; i < 150; i++)
        std::cout << "-";

    // Obliczanie wartości wielomianu

    std::cout << std::endl
              << std::endl
              << "\tObliczanie wartości wielomianu:\n\nWielomian:\n\tW(x) = ";
    wielomian1.display();
    std::cout << "\n\nWartość wielomianu dla liczby 1:\tW(1) = " << wielomian1.horner(1)
              << "\nWartość wielomianu dla liczby 2:\tW(2) = " << wielomian1.horner(2)
              << std::endl
              << std::endl;

    for (int i = 0; i < 150; i++)
        std::cout << "-";
    std::cout << std::endl;

    // Porównywanie wielomianów

    bool a = (wielomian1 == wielomian1);
    bool b = (wielomian1 == wielomian2);
    bool c = (wielomian1 != wielomian1);
    bool d = (wielomian1 != wielomian2);

    std::cout << std::endl
              << "\tPorównywanie wielomianów:\n\nCzy wielomian1 jest równy samemu sobie? \t" << (a ? "Tak" : "Nie")
              << "\nCzy wielomian1 jest równy wielomianowi2?\t" << (b ? "Tak" : "Nie")
              << "\nCzy wielomian1 NIE jest równy samemu sobie?\t" << (c ? "Tak" : "Nie")
              << "\nCzy wielomian2 NIE jest równy wielomianowi2?\t" << (d ? "Tak" : "Nie")
              << std::endl
              << std::endl;

    for (int i = 0; i < 150; i++)
        std::cout << "-";
    std::cout << std::endl;

    return 0;
}
