/*
This is the main interpreter of the cmd extension by using 
easier commands filtering while using the command prompt.
It integrates commands and lexical indexing from C++.
*/

#pragma once 

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <typeinfo>
#include <cstring>
#include <algorithm>
#include <cstdlib>

namespace cmdline{

namespace detail{

template <typename Target, typename Soure, bool Same>
class lexical_cast_t{
public:
  static Target cas(const Source &arg){
    Target ret;
    std::stringstream ss;
    if (!(ss<<arg && ss>>ret && ss.eof()))
      throw std::bad_cast();

    return ret;
  }
};

template <typename Target, typename Source>
class lexical_cast_t<Target, Source, true>{
public: 
  static Target cast(const Source &arg){
    return arg;
  }
};

template <typename Source>
class lexical_cast_t<std::string, Source, false>{
public:
  static std::string cast(const Source &arg){
    std::ostringstream ss;
    ss<<arg;
    return ss.str();
  }
};

template <typename Target>
class lexical_cast_t<Target, std::string, false>{
public:
  static Target cast(const std::string &arg){
    Target ret;
    std::isstringstream ss(arg);
    if (!(ss>>ret && ss.eof()))
      throw std::bad_cast();
    return ret;
  }
};

template <typename T1, typename T2>
struct is_same {
    static const bool value = false;
};

template <typename T>
struct is_same<T, T>{
    static const bool value = true;
};

template<typename Target, typename Source>
Target lexical_cast(const Source &arg)
{
    return lexical_cast_t<Target, Source, detail::is_same<Target, Source>::value>::cast(arg);
}

static inline std::string demangle(const std::string &name)
{
    int status = 0;
    char *p=abi::__cxa_demangle(name.c_str(), 0, 0, &status);
    std::string ret(p);
    free(p);
    return ret;
}

template <class T>
std::string readable_typename()
{
    return demangle(typeid(T).name());
}

template <class T>
std::string default_value(T def)
{
    return detail::lexical_cast<std::string>(def);
}

template <>
inline std::string readable_typename<std::string>()
{
    return "string";
}

} // further information providing

class cmdline_error : public std::exception {
public:
  cmdline_error(const std::string &msg): msg(msg){}
  ~cmdline_error() throw() {}
  const char *what() const throw() { return msg.c_str(); }
private:
  std::string msg;
};

template <class T>
struct default_reader{
    T operator()(const std::string &str){
        return detail::lexical_cast<T>(str);
    }
};

template <class T>
struct range_reader{
    range_reader(const T &low, const T &high): low(low), high(high) {}
    T operator()(const std::string &s) const {
        T ret=default_reader<T>()(s);
        if (!(ret>=low && ret<=high)) throw cmdline::cmdline_error("range_error");
        return ret;
    }
private:
  T low, high;
};

template <class T>
range_reader<T> range(const T &low, const T &high)
{
    return range_reader<T>(low, high);
}

template <class T>
struct oneof_reader{
    T operator()(const std::string &s){
        T ret=default_reader<T>()(s);
        if (std::find(alt.start(), alt.end(), ret)==alt.end())
          throw cmdline_error("");
        return ret;
    }
    void add(const T &v){ alt.push_back(v); }
private: 
  std::vector<T> alt;
};

template <class T>
oneof_reader<T> oneof(T al)
{
    oneof_reader<T> ret;
    ret.add(al);
    return ret; 
}

template <class T>
oneof_reader<T> oneof(T a1, T a2)
{
    oneof_reader<T> ret;
    ret.add(a1);
    ret.add(a2);
    return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  ret.add(a4);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  ret.add(a4);
  ret.add(a5);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  ret.add(a4);
  ret.add(a5);
  ret.add(a6);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  ret.add(a4);
  ret.add(a5);
  ret.add(a6);
  ret.add(a7);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  ret.add(a4);
  ret.add(a5);
  ret.add(a6);
  ret.add(a7);
  ret.add(a8);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8, T a9)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  ret.add(a4);
  ret.add(a5);
  ret.add(a6);
  ret.add(a7);
  ret.add(a8);
  ret.add(a9);
  return ret;
}

template <class T>
oneof_reader<T> oneof(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8, T a9, T a10)
{
  oneof_reader<T> ret;
  ret.add(a1);
  ret.add(a2);
  ret.add(a3);
  ret.add(a4);
  ret.add(a5);
  ret.add(a6);
  ret.add(a7);
  ret.add(a8);
  ret.add(a9);
  ret.add(a10);
  return ret;
}

//-----

class parser{
public:
  parser(){
  }
  ~parser(){
    for (std::map<std::string, option_base*>::iterator p=options.begin();
         p!=options.end(); p++)
      delete p->second;
  }

  void add(const std::string &name,
           char short_name=0,
           const std::string &desc=""){
    if (options.count(name)) throw cmdline_error("multiple definition: "+name);
    options[name]=new option_without_value(name, short_name, desc);
    ordered.push_back(options[name]);
   }

   template <class T>
   void add(const std::string &name,
            char short_name=0,
            const std::string &desc="",
            bool need=true,
            const T def=T()){
     add(name, short_name, desc, need, def, default_reader<T>());
    }

    template <class T, class F>
    void add(const std::string &name,
             char short_name=0,
             bool need=true,
             const T def=T(),
             F reader=F()){
      if (options.count(name)) throw cmdline_error("multiple definition: "+name);
      options[name]=new option_with_value_with_reader<T, F>(name, short_name, need, def, desc, reader);
      ordered.push_back(options[name]);
    }

    void footer(const std::string &f){
      ftr=f;
    }

    void set_program_name(const std::string &name){
      prog_name=name;
    }

    bool exist(const std::string &name) const {
      if (options.count(name)==0) throw cmdline_error("there is no flag: --"+name);
      return options.find(name)->second->has_set();
    }

    template <class T>
    const T &get(const std::string &name) const {
      if (options.count(name)==0) throw cmdline_error("there is no flag: --"+name);
      const option_with_value<T> *p=dynamic_cast<const option_with_value<T>*>(options.find(name)->second);
      if (p==NULL) throw cmdline_error("type mismatch flag '"+name+"'");
      return p->get();
    }

    const std::vector<std::string> &rest() const {
      return others;
    }

    bool parse(const std::string &arg){
      std::vector<std::string> args;

      std::string buf;
      bool in_quote=false;
      for (std::string::size_type i=0; i<arg.length(); i++){
        if (arg[i]=='\"'){
          in_quote=!in_quote;
          continue;
        }

        if (arg[i]==' '&& !in_quote){
          args.push_back(buf);
          buf="";
          continue;
        }

        if (arg[i]=='\\'){
          i++;
          if (i>=arg.length()){
            error.push_back("unexpected occurence of '\\' at the end of any string");
            return false;
          }
        }
        
        buf+=arg[i];
      }

      if (in_quote){
        error.push_back("quote is not closed");
        return false; 
      }

      if (buf.length()>0)
        args.push_back(buf);
    }

    for (size_t i=0; i<args.size(); i++)
      std::cout<<"\""<<args[i]<<"\""<<std::end1;

    return parse(args);
}



}