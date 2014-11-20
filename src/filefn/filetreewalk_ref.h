
#ifndef RLF_FILETREEWALK_REF_H
#define RLF_FILETREEWALK_REF_H

#include "t_filename.h"
//#include "filetreewalk.h"

using rlf_filefn::t_filename;

namespace rlf_ftw {


   class tExcludeFolders  {
   public:
      std::string contains;
      tExcludeFolders( std::string const& s ) : contains( s ) {}
      tExcludeFolders& operator=( std::string const& s ) {
         contains = s;
         return *this;
      }
      tExcludeFolders() : contains() {}



   };


   class tIncludeFiles   {
   public:
      std::string last;
      tIncludeFiles( std::string const& s ) : last( s ) {}
      tIncludeFiles& operator=( std::string const& s ) {
         last = s;
         return *this;
      }
      tIncludeFiles() : last() {
      }
   };


   //class ftwReturn {
   //   std::vector <t_filename > _files;
   //   std::vector <t_filename > _folders;
   //public:
   //   ftwReturn(): _files(), _folders() {}
   //   std::vector <t_filename > const& files()const {
   //      return _files;
   //   }
   //   std::vector <t_filename > const& folders()const {
   //      return _folders;
   //   }
   //   void clear() {
   //      _files.clear();
   //      _folders.clear();
   //   }
   //   friend class ftw;
   //};

}

#endif // RLF_FILETREEWALK_REF_H
