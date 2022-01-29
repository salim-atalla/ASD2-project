#include <iostream>
#include <vector>
#include <cmath>


#include "fint.h"

using namespace std;







    // retourner la liste des facteurs premièrs d'un entier
        void decomposition (int_t n, vector<int_t> &lf, vector<mult_t> &lm) 
        {
            if (lf.size() != 0 || lm.size() != 0) { cout << "Erreur: les vecteurs doivent être vides!" << endl; throw -1; }

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
  
            if (n < 1 || n > MAX_INT_T) { this->nb_couples = 0;  throw std::domain_error("Erreur: création à partir d'un nombre invalide!"); }
            else { 
                vector<int_t> f;
                vector<mult_t> m;
                decomposition (n, f, m);   

                int taille = f.size();
                this->nb_couples = taille;

                Couple couple;
                
                for (int i=0; i<taille; i++) {  

                    couple.fact = f[i];
                    couple.mult = m[i];
                    this->tab_couples.push_back(couple);
                }
            }
        }





    // constructeur à partir d'une liste de facteurs premiers lf
    // et d'une liste de multiplicité lm telles que lm et lf ont la
    // même taille et lm[i]>0 est la multiplicité de lf[i] pour tout i
    // exemple d'appel de ce constructeur :
    // fint f({2,5,11}, {1,2,2});
        fint::fint (const std::initializer_list<int_t>& lf,
                const std::initializer_list<mult_t>& lm) 
        {
            // vérifier que la liste des facteurs est pré ordonnée.
            int i = 0;
            while (i<lf.size() && lf.begin()[i] < lf.begin()[i+1]) { i++; }
            if (i != lf.size()-1) { cout << "Erreur: la liste des facteurs n'est pas ordonnée!" << endl; throw -1; }


            int taille = lf.size();
            this->nb_couples = taille;

            Couple couple;

            for (int i=0; i<taille; i++) { 
                
                couple.fact = lf.begin()[i];
                couple.mult = lm.begin()[i];
                this->tab_couples.push_back(couple);
            }
        }




 
    // destructeur
        fint::~fint() 
        { 
            this->tab_couples.clear();
        }





    // retourne la valeur décimale de this, throws std::overflow_error
        int_t fint::to_int() const 
        {
            int_t val = 1;
            int_t fVal, mVal;

            int taille = this->nb_couples;

            
            for (int i=0; i<taille; i++) { 
                fVal = this->tab_couples[i].fact;
                mVal = this->tab_couples[i].mult;

                for (int j=0; j<mVal; j++) {
                    val = val * fVal;
                    if (val > MAX_INT_T) { throw std::overflow_error("Erreur: dépassement de la limite"); }
                }
            }
            return val;
        } 




 
    // teste si this divise a
        bool fint::divides(const fint& a) const
        {
            if (a.nb_couples < this->nb_couples) { return false; }
            else{
                for (int i=0; i<this->nb_couples; i++) {
                    int j = 0;

                    while (j <= a.nb_couples 
                        && this->tab_couples[i].fact != a.tab_couples[j].fact) {

                        j++; 
                    }

                    if (j >= a.nb_couples  
                        || this->tab_couples[i].mult > a.tab_couples[j].mult) { 
                        
                        return false; 
                    }
                }
            }
            return true;
        } 





    // teste si this est premier
        bool fint::is_prime() const
        {
            // si primaire, alors il accepte la division par le 1 et lui-même.
            return this->nb_couples == 1 
                && this->tab_couples[0].mult == 1 
                && this->tab_couples[0].fact >= 2;
        }





    // comparaisons
        bool operator==(const fint& a, const fint& b){

            if (a.nb_couples == b.nb_couples) {  
            
                int i = a.nb_couples-1;
                while (a.tab_couples[i].fact == b.tab_couples[i].fact
                    && a.tab_couples[i].mult == b.tab_couples[i].mult
                    && i>=0) {

                    i--;
                }
                if (i < 0) { return true; }
            }
            return false;
        }


        bool operator!=(const fint& a, const fint& b){
            return !(a == b);
        }
   




    // retourne le plus petit commun multiple de a et b
        fint lcm(const fint& a, const fint& b)
        {
            vector<Couple> v_A, v_B, v_c;
            v_A = a.tab_couples;
            v_B = b.tab_couples;
            Couple c;
            int nb = 0;
            int i = 0, j = 0;


            while (v_A.size() > i || v_B.size() > j) {
                
                if (v_A[i].fact == v_B[j].fact) {
                    c.fact = v_A[i].fact;

                    if (v_A[i].mult > v_B[j].mult) {
                        c.mult = v_A[i].mult;
                    } else {
                        c.mult = v_B[j].mult;
                    } 
                    i++;
                    j++; 

                } else {
                    if (v_A[i].fact < v_B[j].fact) { 
                        c = v_A[i];
                        i++;
                    } else {
                        c = v_B[j];
                        j++;
                    }
                }
                v_c.push_back(c);
                nb++;
            }
            fint tmp (1);
            tmp.nb_couples = nb;
            tmp.tab_couples = v_c;
            return tmp;
        }






    // retourne le plus grand diviseur commun de a et b
        fint gcd(const fint& a, const fint& b)  
        {
            fint tmp = a;

            for (int i=0; i<a.nb_couples; i++) { 
                int j = 0;

                while (j<=a.nb_couples && a.tab_couples[i].fact != b.tab_couples[j].fact) { j++; }

                if (j < b.nb_couples) { 
                    if (a.tab_couples[i].mult > b.tab_couples[j].mult) {

                        tmp.tab_couples[i].mult = b.tab_couples[j].mult;
                    }
                } else { tmp.tab_couples[i].mult = 0; }
            }

            // nettoyage
            vector<Couple> v_c;
            Couple c;
            int nb = 0;

            for (int i=0; i<tmp.nb_couples; i++) { 
                if (tmp.tab_couples[i].mult != 0) {

                    c.fact = tmp.tab_couples[i].fact;
                    c.mult = tmp.tab_couples[i].mult;
                    v_c.push_back(c);
                    nb++;
                }
            }
            tmp.tab_couples = v_c;
            tmp.nb_couples = nb;

            return tmp;
        }





    // retourne a * b
        fint operator*(const fint& a, const fint& b) 
        {
            vector<Couple> v_A, v_B, v_c;
            v_A = a.tab_couples;
            v_B = b.tab_couples;
            Couple c;
            int nb = 0;
            int i = 0, j = 0;


            while (v_A.size() > i || v_B.size() > j) {
                
                if (v_A[i].fact == v_B[j].fact) {

                    c.fact = v_A[i].fact;
                    c.mult = v_A[i].mult + v_B[j].mult;
                    i++;
                    j++; 

                } else {
                    if (v_A[i].fact < v_B[j].fact) {
                        c = v_A[i];
                        i++;
                    } else {
                        c = v_B[j];
                        j++;
                    }
                }
                v_c.push_back(c);
                nb++;
            }
            fint tmp(1);
            tmp.nb_couples = nb;
            tmp.tab_couples = v_c;
            return tmp;
        }





    // retourne a / b si b divise a, throws std::domain_error sinon
       fint operator/(const fint& a, const fint& b)
        {
            if (a == b) { return fint (1); }
            else {
                int_t val_A = a.to_int(); 
                int_t val_B = 1;
                int_t f;
                mult_t m;

                for (int i=0; i<b.nb_couples; i++) {  

                    f = b.tab_couples[i].fact;
                    m = b.tab_couples[i].mult;
                    val_B = pow(f, m);
                    val_A = val_A / val_B;
                    if (val_A < 1) { throw std::domain_error("Erreur: division invalide!"); }
                }
                return fint (val_A); 
            }
        }



/*
    // Autre méthode pour effectuer la division:
    //===========================================
    
    // retourne a / b si b divise a, throws std::domain_error sinon
       fint operator/(const fint& a, const fint& b)
        {
            else if (a == b) { return fint (1); }

            fint tmp = a;

            if (b.divides(a)) { 
                int j = 0;
                for (int i=0; i<tmp.nb_couples; i++) { 
                    if (tmp.tab_couples[i].fact == b.tab_couples[i-j].fact) {
                        tmp.tab_couples[i].mult = tmp.tab_couples[i].mult - b.tab_couples[i-j].mult;
                    } else { j++; }
                }

                // nettoyage
                vector<Couple> v_c;
                Couple c;
                int nb = 0;
                for (int i=0; i<tmp.nb_couples; i++) {  
                    if (tmp.tab_couples[i].mult != 0) {
                        c.fact = tmp.tab_couples[i].fact;
                        c.mult = tmp.tab_couples[i].mult;
                        v_c.push_back(c);
                        nb++;
                    }
                }
                tmp.tab_couples = v_c;
                tmp.nb_couples = nb;
                

            } else {    // au pire b ne divise pas a

                int_t val_A = a.to_int(); 
                int_t val_B = 1;

                for (int i=0; i<b.nb_couples; i++) {    
                    val_B = pow(b.tab_couples[i].fact, b.tab_couples[i].mult);
                    val_A = val_A / val_B;
                    if (val_A == 0) { throw std::domain_error("Erreur: division invalide!"); }
                }
                tmp = fint (val_A); 
            }
            return tmp;
        }
*/




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
            if (n == 1) { return a; }

            fint tmp = a;

            for (int i=0; i<a.nb_couples; i++) {
                tmp.tab_couples[i].mult = tmp.tab_couples[i].mult * n;
            }
            return tmp;
        }





    // écriture de a sur un flot de sortie
        std::ostream& operator<<(std::ostream& os, const fint& a)
        {
            os << "[ ";
            for (int i=0; i<a.nb_couples; i++) { 
                os << "( " << a.tab_couples[i].fact << ", " << a.tab_couples[i].mult << " )";
            }
            os << " ]";
		    return os;
        }




