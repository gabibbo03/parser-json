#include "include/json.hpp"

/**
 * codifica : 
 * 1 => null 1
 * 10 => true 2
 * 100 => string 4
 * 1000 => double 8
 * 10000 => list  16
 * 100000 => dictionary 32

**/

//DATA STRUCTURE 
struct json::impl{
   unsigned char type;
   std::string s;
   double num;
   bool e;
   
    struct Lcell
    {
     json j;
     Lcell* next;
     Lcell(const json& e,Lcell* next) : j{std::move(e)} , next{next} {};
    
    };
   
   using ls = Lcell*;
   ls Lhead,Ltail;
   void Lprepend(const json& e);
   void Lappend( const json& e);
   void Lpop_front();

    struct Dcell
    {
     std::pair<std::string,json> p;
     Dcell* next;
     Dcell(const std::pair<std::string,json>& e,Dcell* next) : p{e} , next{next} {};
    };
   
   using dic = Dcell*;
   dic Dhead,Dtail;
   
   void Dprepend(const std::pair<std::string,json>& e);
   void Dappend(const std::pair<std::string,json>& e);
   void Dpop_front();

};

//LIST FUNCTIONS

void json::impl::Lprepend(const json& e){
    
    if(!Lhead){
        //std::cout<<"prepend 0"<<std::endl;
        Lhead = new Lcell{e,nullptr};
        Ltail = Lhead;
    }else{
        //std::cout<<"prepend more than one"<<std::endl;
        ls elem = new Lcell{e,Lhead};
        Lhead = elem;
    }
}

void json::impl::Lappend(const json& e){

    if(!Lhead){
       //std::cout<<"append 0"<<std::endl;
        Lprepend(e);
    }else{
       //std::cout<<"append more than one"<<std::endl;
        Ltail->next = new Lcell{e,nullptr};
        Ltail = Ltail->next;
    }
}

void json::impl::Lpop_front(){
    
    ls pc = Lhead;
    Lhead = Lhead->next;
    delete pc;
}


//DICTIONARY FUNCTIONS

void json::impl::Dprepend(const std::pair<std::string,json>& e){
   
    if(!Dhead){
        Dhead = new Dcell{e,nullptr};
        Dtail = Dhead;
    }else{
        dic elem = new Dcell{e,Dhead};
        Dhead = elem;
    }
}

void json::impl::Dappend(const std::pair<std::string,json>& e){

    if(!Dhead){
        Dprepend(e);
    }else{
        Dtail->next = new Dcell{e,nullptr};
        Dtail = Dtail->next;
    }
}

void json::impl::Dpop_front(){
    
    dic pc = Dhead;
    Dhead = Dhead->next;
    delete pc;
}

//constructors and descructor

json::json(){
    //std::cout<<"costructor"<<std::endl;
    //std::cout<<"new di pimpl "<<std::endl;
    pimpl = nullptr;
    pimpl = new impl;
    pimpl->type = 1;
    pimpl->e = false;
    pimpl->num = 0.0f;
    pimpl->s = "";
    pimpl->Lhead = nullptr;
    pimpl->Ltail = nullptr;
    pimpl->Dhead = nullptr;
    pimpl->Dtail = nullptr;

} 

json::json(const json& j){
    //std::cout<<"copy constructor"<<std::endl;
    //std::cout<<"new di pimpl "<<std::endl;
    pimpl = nullptr;
    pimpl = new impl;
    pimpl->type = j.pimpl->type; // tipo
    pimpl->num = j.pimpl->num; // double
    pimpl->e = j.pimpl->e; // booleano
    pimpl->s = j.pimpl->s; //stringa
    pimpl->Lhead = nullptr;
    pimpl->Ltail = nullptr;
    pimpl->Dhead = nullptr;
    pimpl->Dtail = nullptr;
    impl::ls pc = j.pimpl->Lhead;
    while (pc)
    {
        pimpl->Lappend(pc->j);
        pc = pc->next;
    }
    
    impl::dic px = j.pimpl->Dhead; 
    while (px)
    {
        pimpl->Dappend(px->p);
        px = px->next;
    }

}

json::json(json&& j){
   //std::cout<<"move constructor"<<std::endl;

    pimpl = j.pimpl;
    j.pimpl = nullptr;
}

json::~json(){
    //std::cout<<"chiamo il distruttore"<<std::endl;
    if(pimpl){
       while (pimpl->Lhead) pimpl->Lpop_front();
       //std::cout<<"ho cancellato la lista"<<std::endl;
       while (pimpl->Dhead) pimpl->Dpop_front();
       //std::cout<<"ho cancellato il dizionario"<<std::endl;
    }
    delete pimpl;
}

//ASSIGNEMENT OPERATORS
json& json::operator=(const json& j){
  //std::cout<<"copy assignement"<<std::endl;
  if(this != &j){
     
    pimpl->type = j.pimpl->type;
    pimpl->e = j.pimpl->e;
    pimpl->num = j.pimpl->num;
    pimpl->s = j.pimpl->s;
    while(pimpl->Lhead) pimpl->Lpop_front();
    while(pimpl->Dhead) pimpl->Dpop_front();
    
    impl::ls pc = j.pimpl->Lhead;
    while (pc)
    {
        pimpl->Lappend(pc->j);
        pc = pc->next;
    }
    
    impl::dic px = j.pimpl->Dhead;
    while (px)
    {
        pimpl->Dappend(px->p);
        px = px->next;
    }
    
  } 
  return *(this);
}

json& json::operator=(json&& j){
   //std::cout<<"move assignement"<<std::endl;
   impl* tmp = pimpl;
   pimpl = j.pimpl;
   j.pimpl = tmp;
   
   return *this;
}

//IS_Type_Function

bool json::is_null()const {return pimpl->type == 1;}
bool json::is_bool()const {return pimpl->type == 2;}
bool json::is_string()const {return pimpl->type == 4;}
bool json::is_number()const {return pimpl->type == 8;}
bool json::is_list()const {return pimpl->type == 16;}
bool json::is_dictionary()const {return pimpl->type == 32;}

//dictionary_operators


json& json::operator[](const std::string& s){
   
   if(!(*this).is_dictionary()){
      json_exception expt;
      std::cout<<"[syntax - error] l'oggetto considerato non è un dizionario"<<std::endl;
      throw expt;
   }else{
     impl::dic px = pimpl->Dhead;
     while(px && px->p.first != s) px = px->next;
      
     if(px) return px->p.second;
     else{
        std::pair<std::string,json> p;
        p.first = s;
        p.second = json();
        pimpl->Dappend(p);
        impl::dic px = pimpl->Dhead;
        
        while (px->next) px = px->next;
        return px->p.second;
     }

   }
    
}

const json& json::operator[](const std::string& s) const{
   
   if(!(*this).is_dictionary()){
     json_exception e1;
     std::cout<<"[syntax - error] l'oggetto considerato non è un dizionario"<<std::endl;
     throw e1;
   }else{
     impl::dic px = pimpl->Dhead;
     while(px && px->p.first != s) px = px->next;
      
     if(px) return px->p.second;
       
     else{
        json_exception e2;
        std::cout<<"[data - error]la chiave ricercata non è presente nel dizionario"<<std::endl;
        throw e2;
     }
   }
}

//ITERATORS

//List_iterator
struct json::list_iterator
{
    public:
    using iterator_cat = std::forward_iterator_tag;
    using value_type = json;
    using pointer = json*;
    using reference = json&;

    list_iterator(impl::ls ptr);
    reference operator*() const;
    pointer operator->() const;

    list_iterator& operator++();
    list_iterator operator++(int);

    bool operator==(const list_iterator& i) const;
    bool operator!=(const list_iterator& i) const;

    operator bool();
    
    private:
    impl::ls ptr;

};

json::list_iterator::list_iterator(impl::ls ptr) : ptr(ptr) {}


json::list_iterator json::begin_list(){
    json_exception e;
    e.msg = "non è una lista";     
    if(pimpl->type != 16){
        throw e;
    }
    else {
        return{pimpl->Lhead};
    }
}

json::list_iterator json::end_list(){
    json_exception e;
    e.msg = "non è una lista";
    if(pimpl->type != 16) 
       throw e;
    else return {nullptr};
}

json::list_iterator::reference json::list_iterator::operator*() const{    
    return ptr->j;
}

json::list_iterator::pointer json::list_iterator::operator->() const{
    return &(ptr->j);
}

json::list_iterator& json::list_iterator::operator++(){
    ptr = ptr->next;
    return *this;
}

json::list_iterator json::list_iterator::operator++(int){
    list_iterator copy = *this;
    ptr = ptr->next;
    return copy;
}

bool json::list_iterator::operator==(const list_iterator& i) const{
    return ptr == i.ptr;
}

bool json::list_iterator::operator!=(const list_iterator& i) const{
  return !(ptr == i.ptr);
}

json::list_iterator::operator bool(){
  return ptr != nullptr;
}

//Const_List_iterator
struct json::const_list_iterator
{
    using iterator_cat = std::forward_iterator_tag;
    using value_type = const json;
    using pointer = const json*;
    using reference = const json&;

    const_list_iterator(impl::ls ptr);
    reference operator*() const;
    pointer operator->() const;
    
    bool operator==(const const_list_iterator& i) const;
    bool operator!=(const const_list_iterator& i) const;

    const_list_iterator& operator++();
    const_list_iterator operator++(int);
    operator bool();
    private:
    
    impl::ls ptr;

};

json::const_list_iterator::const_list_iterator(impl::ls ptr) : ptr(ptr) {} 


json::const_list_iterator json::begin_list() const{

   json_exception e;
   e.msg = "non è una lista";
    if(pimpl->type != 16) {
      throw e;
    }else{
        return {pimpl->Lhead};
    }
       
}
        
json::const_list_iterator json::end_list() const{
    
    json_exception e;
    e.msg = "non è una lista";
    if(pimpl->type != 16) 
      throw e;
    else{
       return {nullptr}; 
    } 
      
}

json::const_list_iterator::reference json::const_list_iterator::operator*() const{
    return ptr->j;
}

json::const_list_iterator::pointer json::const_list_iterator::operator->() const{
    return &(ptr->j);
}

json::const_list_iterator& json::const_list_iterator::operator++(){
    ptr = ptr->next;
    return *this;
}

json::const_list_iterator json::const_list_iterator::operator++(int){
    const_list_iterator copy = *this;
    ptr = ptr->next;
    return copy;
}

bool json::const_list_iterator::operator==(const const_list_iterator& i) const{
    return ptr == i.ptr;
}

bool json::const_list_iterator::operator!=(const const_list_iterator& i) const{
  return !(ptr == i.ptr);
}

json::const_list_iterator::operator bool(){
   
   return ptr != nullptr;  
} 

//dictionary_iterator

struct json::dictionary_iterator{
    
    using iterator_cat = std::forward_iterator_tag;
    using value_type = std::pair<std::string,json>;
    using pointer = std::pair<std::string,json>*;
    using reference = std::pair<std::string,json>&;
      
    dictionary_iterator(impl::dic ptr);
    reference operator*()const;
    pointer   operator->()const;

    dictionary_iterator& operator++();
    dictionary_iterator  operator++(int); 
    
    bool operator==(const dictionary_iterator& i) const;
    bool operator!=(const dictionary_iterator& i) const;
    operator bool();

    private:
    impl::dic ptr;
};

json::dictionary_iterator::dictionary_iterator(impl::dic ptr) : ptr(ptr) {}

json::dictionary_iterator json::begin_dictionary(){
   json_exception e;
   e.msg = "non è un dizionario";
   if(pimpl->type != 32)
      throw e;
   else return {pimpl->Dhead};
}

json::dictionary_iterator json::end_dictionary(){
    json_exception e;
    e.msg = "non è un dizionario";
    if(pimpl->type != 32)
      throw e;
    else return {nullptr};
}

json::dictionary_iterator::reference json::dictionary_iterator::operator*()const{
    return ptr->p;
}

json::dictionary_iterator::pointer json::dictionary_iterator::operator->()const{
    return &(ptr->p);
}

json::dictionary_iterator& json::dictionary_iterator::operator++(){
    ptr = ptr->next;
    return (*this);
}

json::dictionary_iterator  json::dictionary_iterator::operator++(int){
    dictionary_iterator copy = (*this);
    ptr = ptr->next;
    return copy;
}

bool json::dictionary_iterator::operator==(const dictionary_iterator& i) const{
    return ptr == i.ptr;
}

bool json::dictionary_iterator::operator!=(const dictionary_iterator& i) const{
    return !((*this) == i);
}

json::dictionary_iterator::operator bool(){
    return ptr == nullptr;
}

//const_dictionary_iterator

struct json::const_dictionary_iterator{
    
    using iterator_cat = std::output_iterator_tag;
    using value_type = const std::pair<std::string,json>;
    using pointer = const std::pair<std::string,json>*;
    using reference = const std::pair<std::string,json>&;
    
    const_dictionary_iterator(impl::dic ptr);
    reference operator*()const;
    pointer   operator->()const;

    const_dictionary_iterator& operator++();
    const_dictionary_iterator  operator++(int); 
    bool operator==(const const_dictionary_iterator& i) const;
    bool operator!=(const const_dictionary_iterator& i) const;
    operator bool();
    private:
    impl::dic ptr;
};

json::const_dictionary_iterator::const_dictionary_iterator(impl::dic ptr) : ptr(ptr) {}

json::const_dictionary_iterator json::begin_dictionary() const{
    json_exception e;
    e.msg = "non è un dizionario";
    if(pimpl->type != 32)
      throw e;
    else return {pimpl->Dhead};
}

json::const_dictionary_iterator json::end_dictionary() const{
    json_exception e;
    e.msg = "non è un dizionario";
    if(pimpl->type != 32)
      throw e;
    else return{nullptr};
}

json::const_dictionary_iterator::reference json::const_dictionary_iterator::operator*()const{
    return ptr->p;
}

json::const_dictionary_iterator::pointer  json::const_dictionary_iterator::operator->()const{
   return &(ptr->p);
}

json::const_dictionary_iterator& json::const_dictionary_iterator::operator++(){
   ptr = ptr->next;
   return (*this);
}

json::const_dictionary_iterator  json::const_dictionary_iterator::operator++(int){
   const_dictionary_iterator copy = *this;
   ptr = ptr->next;
   return copy;
}

bool json::const_dictionary_iterator::operator==(const const_dictionary_iterator& i) const{
   return ptr == i.ptr;
}

bool json::const_dictionary_iterator::operator!=(const const_dictionary_iterator& i) const{
   return !((*this) == i);
}

json::const_dictionary_iterator::operator bool(){
   return ptr == nullptr;
}

//get and set methods

double& json::get_number(){

   json_exception e;
   e.msg = "non è un numero";
   if(!(*this).is_number())
      throw e;
   else return pimpl->num;
}

const double& json::get_number() const{
    json_exception e;
    e.msg = "non è un numero";
    if(!(*this).is_number())
      throw e;
    else return pimpl->num;
}

bool& json::get_bool(){
    json_exception e;
    e.msg = "non è un booleano";
    if(!(*this).is_bool())
      throw e;
    else return pimpl->e;
}

bool const& json::get_bool() const{
    json_exception e;
    e.msg = "non è un booleano";
    if(!(*this).is_bool())
      throw e;
    else return pimpl->e;
}

std::string& json::get_string(){
   json_exception e;
   e.msg = "non è una stringa";
   if(!(*this).is_string())
     throw e;
   else return pimpl->s;
}

const std::string& json::get_string() const{
   json_exception e;
   e.msg = "non è una stringa";
   if(!(*this).is_string())
     throw e;
   else return pimpl->s;
}

void json::set_null(){
    
    if(!(*this).is_null()){
        pimpl->type = 1;
        pimpl->e = false;
        pimpl->num = 0.0f;
        pimpl->s = "";
        while(pimpl->Lhead) pimpl->Lpop_front();
        while(pimpl->Dhead) pimpl->Dpop_front();
    }
}

void json::set_bool(bool x){
    if((*this).is_bool()){
        pimpl->e = x;
    }else{
        pimpl->type = 2;
        pimpl->e = x;
        pimpl->num = 0.0f;
        pimpl->s = "";
        while(pimpl->Lhead) pimpl->Lpop_front();
        while(pimpl->Dhead) pimpl->Dpop_front();
    }
}

void json::set_string(const std::string& s){
    
    if((*this).is_string()){
        pimpl->s = s;
    }else{
        pimpl->type = 4;
        pimpl->e = false;
        pimpl->num = 0.0f;
        pimpl->s = s;
        while(pimpl->Lhead) pimpl->Lpop_front();
        while(pimpl->Dhead) pimpl->Dpop_front();
    }
}

void json::set_number(double x){
    if((*this).is_number()){
        pimpl->num = x;
    }else{
        pimpl->type = 8;
        pimpl->e = false;
        pimpl->num = x;
        pimpl->s = "";
        while(pimpl->Lhead) pimpl->Lpop_front();
        while(pimpl->Dhead) pimpl->Dpop_front();
    }
}

void json::set_list(){
    if((*this).is_list()){
        while(pimpl->Lhead) pimpl->Lpop_front();
    }else{
        pimpl->type = 16;
        pimpl->e = false;
        pimpl->num = 0.0f;
        pimpl->s = "";
        while(pimpl->Lhead) pimpl->Lpop_front();
        while(pimpl->Dhead) pimpl->Dpop_front();
    }
}

void json::set_dictionary(){
    if((*this).is_dictionary()){
        while(pimpl->Lhead) pimpl->Dpop_front();
    }else{
        pimpl->type = 32;
        pimpl->e = false;
        pimpl->num = 0.0f;
        pimpl->s = "";
        while(pimpl->Lhead) pimpl->Lpop_front();
        while(pimpl->Dhead) pimpl->Dpop_front();
    }
}

void json::push_front(const json& j){
    json_exception e;
    e.msg = "";
    if(!(*this).is_list()){
       std::cerr<<"[syntax - error] non è un lista"<<std::endl; 
       throw e;
    }else{
       pimpl->Lprepend(j);
    }
}

void json::push_back(const json& j){
   json_exception e;
    e.msg = "";
    if(!(*this).is_list()){
       std::cerr<<"[syntax - error] non è un lista "<<std::endl;
       throw e;
    }else{
       pimpl->Lappend(j);
    }
}

void json::insert(const std::pair<std::string,json>& p){
   json_exception e;
   e.msg = "";
   if(!(*this).is_dictionary()){
      std::cerr<<"non è un dizionario"<<std::endl;
      throw e;
   }else{
     pimpl->Dappend(p);
   }
}

std::ostream& operator<<(std::ostream& lhs, json const& rhs){
    
    if(!rhs.is_null()){

       if(rhs.is_number()){
          
          lhs<<rhs.get_number();

       }else if(rhs.is_string()){

          lhs<<char(34)<<rhs.get_string()<<char(34);
   
       }else if(rhs.is_bool()){
          
          lhs<<std::boolalpha<<rhs.get_bool();

       }else if(rhs.is_list()){

          lhs<<"[";
          for(auto i = rhs.begin_list(); i != rhs.end_list(); ++i){
             auto e = i;
             if(++e == rhs.end_list())
                lhs<<*i;
             else 
                lhs<<*i<<",";
          }
          lhs<<"]";
       }else if(rhs.is_dictionary()){
          lhs<<"{";
          for(auto i = rhs.begin_dictionary(); i != rhs.end_dictionary(); ++i){
             auto e = i;
             if(++e == rhs.end_dictionary())
                lhs<<char(34)<<(*i).first<<char(34)<<" : "<<(*i).second;
             else 
                lhs<<char(34)<<(*i).first<<char(34)<<" : "<<(*i).second<<" , ";
          }
          lhs<<"}";
       }

    }else{
        lhs<<"null";
    }

    return lhs;
}

std::string stack = "";

std::istream& read_string(std::string& s , bool& error , std::istream& is){

       char c = 0;
        is.get(c);


        if(c == '"'){
            s = "";
            return is;
        }else{
            do{   
                if(c == 92 && is.peek() == 34){
                    s+= c;
                    is.get(c);
                    s+=c;
                }else{
                    s+= c;
                }
           
            //std::cout<<s<<std::endl;
                is.get(c);
            
                if(is.tellg() == -1){
                    error = true;
                //std::cout<<"ho segnato un errore"<<std::endl;
                }

            }while(!error && c != '"');
        }

        return is;
}

std::istream& read_list( json& j , bool& err_par , bool& err_vir , std::istream& is){

      char c = 0;
      is>>c;
      if(c == ']'){
        return is;
      }else if(is.tellg() == -1){
        err_par = true;
        return is;
      }else{
        json app;
        is.putback(c);
        is>>app;
        is>>c;
        if(c == ']'){
            is.putback(c);
            j.push_back(app);
            return read_list(j,err_par,err_vir,is);
        }
        
        if(is.tellg() == -1){
            is.putback(c);
            return read_list(j,err_par,err_vir,is);
        }
        
        if(c == ','){
            j.push_back(app);
            return read_list(j,err_par,err_vir,is);
        }

        if( c != ','){
            err_vir = true;
            return is;
        }
      }
      return is;
}

std::istream& read_dictionary(json& j, bool& err_par , bool& err_vir ,bool& err_pnt , std::istream& is){
    
    json_exception e;
    char c = 0;
    is>>c;

    if(c == '}'){
        return is;
    }else if(is.tellg() == -1){
        err_par = true;
        return is;
    }else{
        std::pair<std::string,json> p;
        bool error = false;
        is.putback(c);
        is>>c;
        //std::cout<<"ho letto : "<<c<<std::endl;
        
        if(c == '"'){
           read_string(p.first,error,is);
           
           
           if(error){
             std::cout<<"[parse - error] carattere di chiusura stringa mancante"<<std::endl;
             throw e;
           }else{
             is>>c;
             if(c != ':'){
                std::cout<<"[parse - error] (:) mancante"<<std::endl;
                err_pnt = true;
                return is;
             }else{
                is>>p.second;
                is>>c;
                if(c == '}'){
                  is.putback(c);
                  j.insert(p);
                  return read_dictionary(j,err_par,err_vir,err_pnt,is);
                }
                if(is.tellg() == -1){
                   err_par = true;
                   return is;
                }
                if(c == ','){
                  j.insert(p);
                  return read_dictionary(j,err_par,err_vir,err_pnt,is);
                }else{
                  err_vir = true;
                  return is; 
                }
             }
           }
        }else{
            std::cout<<"[parse - error] carattere apertura stringa mancante"<<std::endl;
            throw e;
            return is;
        }
    }

    return is;

}

std::istream& operator>>(std::istream& lhs, json& rhs){
     
    char c = 0;

    //non leggo gli spazi
    lhs>>c;

    if(isdigit(c) || c == '-' || c == '+'){
        double n = 0.0f;
        lhs.putback(c);
        lhs>>n;
        rhs.set_number(n);
        lhs>>c;
        if(c == ','){
           if((stack.back() == '[' || stack.back() == '{')){
              lhs.putback(c);
              return lhs;
           }else{
              json_exception e;
              std::cout<<"[parse - error] errore di parsing del numero "<<std::endl;
              throw e;
              rhs.set_null();
           }
        }else if(c == ']'){
            if(stack.back() == '['){
                lhs.putback(c);
                return lhs;
            }else{
                json_exception e;
                std::cout<<"[parse - error] errore di parsing del numero "<<std::endl;
                throw e;
                rhs.set_null();
            }
        }else if(c == '}'){
            if( stack.back() == '{'){
                lhs.putback(c);
                return lhs;
            }else{
                json_exception e;
                std::cout<<"[parse - error] errore di parsing del numero "<<std::endl;
                throw e;
                rhs.set_null();
            }
        }else if(isalpha(c) || c == ':'){
            std::cout<<"[parse - error] errore di parsing del numero "<<std::endl;
            rhs.set_null();
        }
         
    }else if(c == 'n' || c == 't' || c == 'f'){
        //std::cout<<"ho letto un null o true/false"<<std::endl;
        std::string s = "";
        if( c == 'n' || c == 't'){
            lhs.putback(c);
            for( int i = 0; i < 4; i++){
                lhs>>c;
                s += c;
            }
            if(s == "null") rhs.set_null(); 
            else if(s == "true") rhs.set_bool(true);
            else{
                json_exception e;
                std::cout<<"errore di lettura del json : "<<s<<std::endl;
                throw e;
            }
        }else if( c == 'f'){
            lhs.putback(c);
            for( int i = 0; i < 5; i++){
                lhs>>c;
                s += c;
            }

        if(s == "false") rhs.set_bool(false);
        else {
            json_exception e;
            std::cout<<"errore di lettura del json : "<<s<<std::endl;
            throw e;
            }
        }
        
        lhs>>c;
        if(c == ','){
          if( stack.back() == '[' || stack.back() == '{'){
             lhs.putback(c);
             return lhs;
          }else{
            json_exception e;
             std::cout<<"[parse - error] errore di parsing del json "<<std::endl;
             rhs.set_null();
             throw e;
          }
        }else if(c == ']'){
            if( stack.back() == '['){
                lhs.putback(c);
                return lhs;
            }else{
                json_exception e;
                std::cout<<"[parse - error] errore di parsing del json "<<std::endl;
                rhs.set_null();
                throw e;
            }
        }else if(c == '}'){
            if( stack.back() == '{'){
                lhs.putback(c);
                return lhs;
            }else{
                json_exception e;
                std::cout<<"[parse - error] errore di parsing del json "<<std::endl;
                rhs.set_null();
                throw e;
            }
        }else if(isalpha(c) || c == ':'){
            json_exception e;
            std::cout<<"[parse - error] errore di parsing del json "<<std::endl;
            rhs.set_null();
            throw e;
        }
        
    }else if(c == '"'){

        std::string s = "";
        bool error = false;

        read_string(s,error,lhs);
        if(error){
            json_exception e;
            std::cout<<"[parse - error]errore di parsing stringa : "<<s<<std::endl;
            throw e;
        }else{
            //std::cout<<"stringa letta : "<<s<<std::endl;
            rhs.set_string(s);
        } 

    }else if( c == '['){
        stack += c;
        rhs.set_list();
        bool error_par = false;
        bool error_vir = false;
        read_list(rhs,error_par,error_vir,lhs);
         
        if(error_par){
            json_exception e;
            std::cout<<"[parse - error]errore nella lettura della lista (mancante parentesi di chiusura)"<<std::endl;
            rhs.set_null();
            throw e;
        }else if(error_vir){
            json_exception e;
            std::cout<<"[parse - error]errore nella lettura della lista (mancante virgola)"<<std::endl;
            rhs.set_null();
            throw e;
        }else{
           // std::cout<<"lista letta : "<<rhs<<std::endl;
            stack.pop_back();
        }
    
    }else if( c == '{'){
        stack += c;
        rhs.set_dictionary();
        bool error_par = false;
        bool error_vir = false;
        bool error_point = false;
        read_dictionary(rhs,error_par,error_vir,error_point,lhs);    
        if(error_par){
            json_exception e;
            std::cout<<"[parse - error]errore nella lettura del dizionario(mancante parentesi di chiusura)"<<std::endl;
            rhs.set_null();
            throw e;
        }else if(error_point){
            json_exception e;
            std::cout<<"[parse - error]errore nella lettura del dizionario (mancanti due punti)"<<std::endl;
            rhs.set_null();
            throw e;
        }else if(error_vir){
            json_exception e;
            std::cout<<"[parse - error]errore nella lettura del dizionario (mancante virgola)"<<std::endl;
            rhs.set_null();
            throw e;    
        }else{
            stack.pop_back();
        }

    }else if(!lhs.eof()){
        json_exception e;
        std::cout<<"[parse - error] ho letto : "<<c<<std::endl;
        throw e;
    }

    return lhs;
}

int main(){
   
 json j;
 std::cin>>j;
 std::cout<<j<<std::endl;
   
} 
