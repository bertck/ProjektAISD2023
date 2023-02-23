# Projekt Zaliczeniowy AiSD 2023

Temat: Implementacja wielomianów na bazie list powiązanych pojedynczo.

## Autor

Bartłomiej Liber

bartek.liber@student.uj.edu.pl

## Kompilacja i uruchamianie

Aby skompilować program wystarczy użyć "make" lub "make all".

```make
# Z użyciem "all"
make all
```

Program można uruchomić w terminalu ("./main.out") lub używając polecenia Make ("make run").

```make
# Używając Make
make run

# Bezpośrednio w terminalu
./main.out
```

Po zakończonej kompilacji można usunąć plik obiektowy (main.o) używając Make.

```make
make clean
```

# Omówienie kodu
## Struktura klasy Polynomial

Obiekt klasy Polynomial jest listą pojedyczno powiązaną wyrazów wielomianu posegregowanych rosnąco potęgami.
Każdy wyraz wielomianu jest niezerowy, zatem jego współczynnik nie jest równy zeru.
Każdy wyraz ma swoje pola współczynnika i potęgi oraz wskaźnik na następny wyraz wielomianu.

```cpp
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
```
## Dodawanie wyrazów do wielomianu

Funkcja addTerm() służy do dodawania wyrazów wielomianu.
Jeśli wyraz z daną potęgą już istnieje, to współczynniki są dodawane.
Jeśli po dodaniu współczynnik będzie wynosił zero, to wyraz będzie usunięty z wielomianu.
```cpp
template <typename T>
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
```

## Obliczanie wartości wielomianu

Do obliczania wartości wielomianu służy funkcja horner().
```cpp
template <typename T>
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
```

# Operatory działań matematycznych

## Dodawanie wielomianów

Dodając wielomiany tworzymy nowy wielomian za pomocą konstruktora kopiującego jeden z wielomianów i dodając do niego każdy wyraz drugiego wielomianu.

```cpp
template <typename T>
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
```

## Odejmowanie wielomianów

Odejmowanie wielomianów działa podobnie do dodawania z tą różnicą, że dodawane są wyrazy ze współczynnikami pomnożonymi przez -1.

```cpp
template <typename T>
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
```

## Mnożenie wielomianów

Mnożąc wielomiany tworzymy nowy wielomian. Dla każdego wyrazu pierwszego wielomianu przechodzimy po każdym wyrazie drugiego wielomianu i ich iloczyny dodajemy do wynikowego wielomianu.

```cpp
template <typename T>
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
```

# Operatory porównywania

Sprawdzane są w pierwszej kolejności stopnie wielomianów, następnie potęgi i na końcu współczynniki przy danych potęgach.

```cpp
template <typename T>
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
```