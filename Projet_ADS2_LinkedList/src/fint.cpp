#include <iostream>
#include <vector>
#include <cmath>

#include "fint.h"

#define null 0

using namespace std;






    // O(n*logn)
    // retourner la liste des facteurs premièrs d'un entier
        void decomposition (int_t n, vector<int_t> &lf, vector<mult_t> &lm) 
        {
            int_t i = 2;
            int_t prec = 0;
            int_t j = 0;
            bool checked = false;

            while (n>=2) {
                while (n%i==0) {
                    if (i!=prec) { lf.push_back(i); checked = true; }
                    n = n/i;
                    prec = i;
                    j++;
                }
                if (checked) { lm.push_back(j); checked = false; }
                j = 0;
                i++;
            }
        }





    // constructeur à partir d'un entier n >= 1
        fint::fint (int_t n) 
        {
            if (n < 1) { throw -1; }
            else { 
                vector<int_t> f;
                vector<mult_t> m;
                decomposition (n, f, m);    

                int_t taille = f.size();
                this->nb_couples = taille;
                maillon* courant = new maillon;
                premier = courant;
                premier->prec = nullptr;
                
                Couple c;
                
                for (int i=0; i<taille; i++) { 

                    c.fact = f[i];
                    c.mult = m[i];
                    courant->couple = c;
                    courant->suiv = new maillon;
                    courant->suiv->prec = courant;
                    courant = courant->suiv;
                }
                courant->suiv = nullptr;
                this->dernier = courant;
            }  
        }




    // O(logn)
    // constructeur à partir d'une liste de facteurs premiers lf
    // et d'une liste de multiplicité lm telles que lm et lf ont la
    // même taille et lm[i]>0 est la multiplicité de lf[i] pour tout i
    // exemple d'appel de ce constructeur :
    // fint f({2,5,11}, {1,2,2});
        fint::fint (const std::initializer_list<int_t>& lf,
                const std::initializer_list<mult_t>& lm) 
        {
            int_t taille = lf.size();
            this->nb_couples = taille;
            maillon* courant = new maillon;
            premier = courant;
            premier->prec = nullptr;

            Couple c;

            for (int i=0; i<taille; i++) {  
                
                c.fact = lf.begin()[i];
                c.mult = lm.begin()[i];
                courant->couple = c;
                courant->suiv = new maillon;
                courant->suiv->prec = courant;
                courant = courant->suiv;
            }
            courant->suiv = nullptr;
            this->dernier = courant;  
        }




 
    // destructeur
        fint::~fint() // segma fault pour gcd
        {
            while (this->premier->suiv != nullptr) {
                
                this->premier = this->premier->suiv;
                delete this->premier->prec;
            }
        }




    // O(logn)
    // retourne la valeur décimale de this, throws std::overflow_error
        int_t fint::to_int() const 
        {
            int_t val = 1;
            int_t taille = this->nb_couples;
            int_t fVal = 0;
            int_t mVal = 0;
            maillon* courant = this->premier;

            for (int i=0; i<taille; i++) {  
                fVal = courant->couple.fact;
                mVal = courant->couple.mult;

                for (int j=0; j<mVal; j++) {  
                    val = val * fVal;
                    if (val > MAX_INT_T) { throw std::overflow_error("Exception: overflow_error!"); }
                }
                courant = courant->suiv;
            }
            return val;
        } 




    // O(1)
    // teste si this divise a
        bool fint::divides(const fint& a) const
        { 
            if (this->nb_couples > a.nb_couples) { return false; }
            else {
                maillon* p_a = a.premier;
                maillon* p_b = this->premier;
                int j;

                for (int i=0; i<this->nb_couples; i++) {
                    j = 0;
                    while (j <= a.nb_couples && p_a->couple.fact != p_b->couple.fact) {
                        p_a = p_a->suiv;
                        j++;
                    }
                    if (j >= a.nb_couples 
                    || p_b->couple.mult > p_a->couple.mult) { return false; }
                    p_a = this->premier;
                    p_b = p_b->suiv;
                } 
            }
            return true;
        } 




    // O(logn)
    // teste si this est premier
        bool fint::is_prime() const
        {
            return this->nb_couples == 1 && this->premier->couple.mult == 1 && this->premier->couple.fact >=2;
        }




    // O(1)
    // comparaisons
        bool operator==(const fint& a, const fint& b){
            
            if (a.nb_couples != b.nb_couples) { return false; }
            else {

                maillon* p_a = a.premier;
                maillon* p_b = b.premier;

                while (p_a->suiv != nullptr 
                    && p_a->couple.fact == p_b->couple.fact
                    && p_a->couple.mult == p_b->couple.mult) {
                        p_a = p_a->suiv;
                        p_b = p_b->suiv;
                }
                if (p_a->suiv == nullptr) { return true; }
            }
            return false;
        }

    // O(1)
        bool operator!=(const fint& a, const fint& b){
            return !(a == b);
        }
   



    // O(n*logn)
    // retourne le plus petit commun multiple de a et b
        fint lcm(const fint& a, const fint& b)
        {
            maillon* p_a = a.premier;
            maillon* p_b = b.premier;

            maillon* premier = new maillon;
            premier->prec = nullptr;
            premier->suiv = nullptr;
            maillon* courant = premier;

        
            Couple c;
            int nb = 0;

            while (p_a->suiv != nullptr || p_b->suiv != nullptr) {
                
                if (p_a->couple.fact == p_b->couple.fact) {
                    c.fact = p_a->couple.fact;
                    if (p_a->couple.mult > p_b->couple.mult) {
                        c.mult = p_a->couple.mult;
                    } else {
                        c.mult = p_b->couple.mult;
                    }
                    p_a = p_a->suiv;
                    p_b = p_b->suiv;
                } else {
                    if (p_a->couple.fact < p_b->couple.fact) {
                      c = p_a->couple;  
                      p_a = p_a->suiv;
                    }else {
                        c = p_b->couple;  
                        p_b = p_b->suiv;
                    }
                }
                courant->couple = c;
                courant->suiv = new maillon;
                courant->suiv->prec = courant;
                courant = courant->suiv;
                courant->suiv = nullptr;
                nb++;
            }
            fint tmp (1);
            tmp.nb_couples = nb;
            tmp.premier = premier;
            tmp.dernier = courant;
            return tmp;
        }





    // retourne le plus grand diviseur commun de a et b
        fint gcd(const fint& a, const fint& b) 
        {
            fint tmp = a;

            maillon* p_a = a.premier;
            maillon* p_b = b.premier;

            for (int i=0; i<a.nb_couples; i++) {
                int j = 0;

                while (j<= b.nb_couples 
                    && p_a->couple.fact != p_b->couple.fact) {
                        p_b = p_b->suiv;
                        j++;
                }
                if (j < b.nb_couples) {
                    if (p_a->couple.mult > p_b->couple.mult) {
                        p_a->couple.mult = p_b->couple.mult;
                    }
                } else {
                   p_a->couple.mult = 0;
                }
                p_b = b.premier;
                p_a = p_a->suiv;
            }
            

            return tmp;
        }





    // retourne a * b
        fint operator*(const fint& a, const fint& b)
        {
            maillon* p_a = a.premier;
            maillon* p_b = b.premier;

            maillon* premier = new maillon;
            premier->prec = nullptr;
            premier->suiv = nullptr;
            maillon* courant = premier;

        
            Couple c;
            int nb = 0;

            while (p_a->suiv != nullptr || p_b->suiv != nullptr) {
                
                if (p_a->couple.fact == p_b->couple.fact) {
                    c.fact = p_a->couple.fact;
                    c.mult = p_a->couple.mult + p_b->couple.mult;
    
                    p_a = p_a->suiv;
                    p_b = p_b->suiv;
                } else {
                    if (p_a->couple.fact < p_b->couple.fact) {
                      c = p_a->couple;  
                      p_a = p_a->suiv;
                    }else {
                        c = p_b->couple;  
                        p_b = p_b->suiv;
                    }
                }
                courant->couple = c;
                courant->suiv = new maillon;
                courant->suiv->prec = courant;
                courant = courant->suiv;
                courant->suiv = nullptr;
                nb++;
            }
            fint tmp (1);
            tmp.nb_couples = nb;
            tmp.premier = premier;
            tmp.dernier = courant;
            return tmp;
        }





    // retourne a / b si b divise a, throws std::domain_error sinon
       fint operator/(const fint& a, const fint& b)
        {
            if (a.nb_couples < b.nb_couples) { throw -1; }
            else if (a == b) { return fint (1); }

            int_t val_A = a.to_int();
            int_t val_B = 1;

            maillon* p_b = b.premier;

            while (p_b->suiv != nullptr) {
                val_B = pow(p_b->couple.fact, p_b->couple.mult);
                val_A = val_A / val_B;
                p_b = p_b->suiv;
            }
            return fint (val_A);
        }


    // retourner a mod b
        fint operator%(const fint& a, const fint& b)
        {
            if (b.divides(a)) { 
                throw std::domain_error("La reste de division est 0,\n \t \t ce n'est pas possible de créer un fint pour le zéro!"); 
            }

            int_t val_A = a.to_int();
            int_t val_B = b.to_int(); 

            return fint (val_A % val_B);
        }





    // retourne a puissance n
        fint pow(const fint& a, unsigned int n)
        {
            fint tmp = a;
            maillon* p_a = tmp.premier;

            while (p_a->suiv != nullptr) {
                p_a->couple.mult = p_a->couple.mult * n;
                p_a = p_a->suiv;
            }
            return tmp;
        }





    // écriture de a sur un flot de sortie
        std::ostream& operator<<(std::ostream& os, const fint& a)
        {
            maillon* courant = a.premier;

            for (int i=0; i<a.nb_couples-1; i++) { 
                os << "( " << courant->couple.fact << ", " << courant->couple.mult << " ) ";
                courant = courant->suiv;
            } 
            os << "( " << courant->couple.fact << ", " << courant->couple.mult << " ) ";
		    return os;
        }




 



 
/********** Programme principal **********/
int main ()
{
    fint f1 ({2,3,5}, {1,2,1});
    fint f2 (2);

    cout << f1 << endl;
    cout << "f1 decimal = " << f1.to_int() << endl;
    cout << f2 << endl;
    cout << "f2 decimal = " << f2.to_int() << endl; 
     
    cout << "---------------------------" << endl;

    fint f3 = gcd(f1, f2);

    //cout << f2.divides(f1) << endl;
    //cout << "op == " << (f1 == f2) << endl;
    //cout << "op!= " << (f1 != f2) << endl;

    //cout << "lcm = " << lcm(f1, f2) << " = " << lcm(f1, f2).to_int() << endl;
    cout << "gcd = " << f3 << " = " << f3.to_int() << endl;
    //cout << "mult = " << (f1 * f2) << " = " << (f1 * f2).to_int() << endl;

    //cout << "op / =" << f3 << " = " <<  f3.to_int() << endl;
    //cout << "op % =" << f3 << " = " <<  f3.to_int() << endl;
    //cout << "pow = " << f3 << " = " << f3.to_int() << endl;

/*
    for (int i=1; i<100; i++) {
       fint f3 (i);
        if (f3.is_prime()) { cout << i << " " << endl; }
    }
*/
    return 0;
}