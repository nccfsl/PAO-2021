/*
https://docs.google.com/document/d/1Y5xDFGihGmSuK9b9jDkdXPt2CC3bjOFStH3Mcl77pNg/edit?usp=sharing

https://bit.ly/3pTcUDs
*/

/*
ESERCIZIO. 

Definire un template di classe dList<T> i cui oggetti rappresentano una struttura dati lista doppiamente concatenata (doubly linked list) per elementi di uno stesso tipo T. Il template dList<T> deve soddisfare i seguenti vincoli:

1. Gestione della memoria senza condivisione.

2. dList<T> rende disponibile un costruttore dList(unsigned int k, const T& t) che costruisce una 
lista contenente k nodi ed ognuno di questi nodi memorizza una copia di t.

3. dList<T> permette l'inserimento in testa ed in coda ad una lista in tempo O(1) (cioe` costante):
-- Deve essere disponibile un metodo void insertFront(const T&) con il seguente comportamento: dl.insertFront(t) inserisce l'elemento t in testa a dl in tempo O(1).
-- Deve essere disponibile un metodo void insertBack(const T&) con il seguente comportamento: dl.insertBack(t) inserisce l'elemento t in coda a dl in tempo O(1).

4. dList<T> rende disponibile un opportuno overloading di operator< che implementa l'ordinamento lessicografico (ad esempio, si ricorda che per l'ordinamento lessicografico tra stringhe abbiamo che "campana" < "cavolo" e che
"eccellente" < "ottimo").

5. dList<T> rende disponibile un tipo iteratore costante dList<T>::constiterator i cui oggetti permettono di iterare sugli elementi di una lista, e supporti il past-the-end propriamente.
*/

#include<iostream>

template <class T>
class dList
{
    private:
        class nodo
        {
        public:
            T info;
            nodo *prev, *next;
            nodo(const T &t, nodo *p = 0, nodo *n = 0) : info(t), prev(p), next(n) {}
        };
        nodo *last, *first;
        // lista vuota IFF first == nullptr == last
        
        /* static nodo* copia(nodo* n, nodo*& last, nodo* prev = nullptr) {
            if (n == nullptr) return last=nullptr;
            nodo* aux = new nodo(n->info, prev,nullptr);
            aux->next = copia(n->next, last, aux);
            if (n->next == nullptr) last=aux;
            return aux;
        } */
    
        static void distruggi(nodo *f)
        {
            if (f != nullptr)
            {
                distruggi(f->next);
                delete f;
            }
        }

        static nodo *copia(nodo *f, nodo *&last)
        {
            if (!f)
                return last = nullptr;
            nodo *first = new nodo(f->info, nullptr, nullptr);
            nodo *current = first;
            while (f->next != nullptr)
            {
                current = new nodo(f->next->info, current, nullptr);
                current->prev->next = current;
                f = f->next;
            }
            last = current;
            return first;
        }

    public:
        dList(const dList &d) : last(nullptr), first(copia(d.first, last)){};
        dList &operator=(const dList &d)
        {
            if (this != &d)
            {
                distruggi(first);
                first = copia(d.first, last);
            }
            return *this;
        }
        ~dList()
        {
            if (first)
                distruggi(first);
        }
        void insertFront(const T &t)
        {
            first = new nodo(t, nullptr, first);
            if (last == nullptr)
                last = first;
            else // la lista di invocazione era non vuota, quindi first->next !=0
                first->next->prev = first;
        }
        void insertBack(const T &t) {
            last = new nodo(t, last, nullptr);
            if (first == nullptr)
                first = last;
            else
                last->prev->next = last;
        }
        dList(unsigned int k, const T &t)
        {
            for (; k > 0; --k)
                insertFront(t);
        }
        bool operator<(const dList &l) const {
            if (first == nullptr && last == nullptr && l.first == nullptr && l.last == nullptr)
                return false;
            else if (first == nullptr && last == nullptr && l.first != nullptr)
                return true;
            else if (first != nullptr && l.first == nullptr && l.last == nullptr)
                return false;
            else {

            }
        }
        class const_iterator
        {
            friend class dList<T>;
            private:
                nodo *ptr; // ptr al nodo a cui si riferisce il const_iterator
                bool pte;  // true IFF il const_iterator e' past-the-end
                        // conversione nodo* => const_iterator
                const_iterator(nodo *p, bool b = false) : ptr(p), pte(b) {}

            public:
                const_iterator();
                const T &operator*() const {
                    return ptr->info;
                }
                const T *operator->() const {
                    return &(ptr->info);
                }
                const_iterator &operator++() {
                    if (ptr && !pte) {
                        if (ptr->next)
                            ptr = ptr->next;
                        else {
                            ptr = ptr + 1;
                            pte = true;
                        }
                    }
                    return *this;
                }
                const_iterator operator++(int) {
                    const_iterator aux = *this;
                    if (ptr && !pte) {
                        if (ptr->next)
                            ptr = ptr->next;
                        else {
                            ptr = ptr + 1;
                            pte = true;
                        }
                    }
                    return aux;
                }
                const_iterator &operator--() {
                    if (ptr) {
                        if (pte) {
                            ptr = ptr - 1;
                            pte = false;
                        }
                        else if (ptr->prev)
                            ptr = ptr->prev;
                    }
                    return *this;
                }
                const_iterator operator--(int) {
                    const_iterator aux = *this;
                    if (ptr) {
                        if (pte) {
                            ptr = ptr - 1;
                            pte = false;
                        }
                        else if (ptr->prev)
                            ptr = ptr->prev;
                    }
                    return aux;
                }
                bool operator==(const const_iterator &cit) const {
                    return ptr == cit.ptr;
                }
                bool operator!=(const const_iterator &cit) const {
                    return ptr != cit.ptr;
                }
        };
        const_iterator begin() const
        {
            return first; // uso il costruttore che fa conversione implicita [ nodo* => const_iterator ]
        }

        const_iterator end() const
        {
            if (first == nullptr)
                return const_iterator(nullptr, false);
            return const_iterator(last + 1, true); 
        }
};

    // esempio d'uso
    int main()
    {
        dList<int> x(4, 2), y(0, 0), z(6, 8);
        y = x;
        x.insertFront(-2);
        z.insertFront(3);
        y.insertFront(0);
        if (x < y)
            std::cout << "x < y" << std::endl;
        if (z < x)
            std::cout << "z < x" << std::endl;
        if (y < z)
            std::cout << "y < z" << std::endl;
        if (z < y)
            std::cout << "z < y" << std::endl;

        std::cout << "x= ";
        dList<int>::const_iterator j = --(x.end());
        for (; j != x.begin(); --j)
            std::cout << *j << ' ';
        std::cout << *j << std::endl
                  << "y= ";
        for (dList<int>::const_iterator k = y.begin(); k != y.end(); ++k)
            std::cout << *k << ' ';
        std::cout << std::endl
                  << "z= ";
        dList<int>::const_iterator i = z.begin();
        for (; i != z.end(); ++i)
            std::cout << *i << ' ' << std::endl;
    }
