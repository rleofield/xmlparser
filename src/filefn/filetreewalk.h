
#ifndef RLF_FILETREEWALK_H
#define RLF_FILETREEWALK_H


#include "t_filename.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>

#include "filetreewalk_ref.h"

//#if defined(__clang__) && __has_feature(cxx_noexcept) || defined(__GXX_EXPERIMENTAL_CXX0X__)   && __GNUC__ * 10 + __GNUC_MINOR__ >= 46   ||defined(_MSC_FULL_VER)   && _MSC_FULL_VER >= 180021114

// Is noexcept supported?
#if defined(__clang__) 	|| defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46 
  
#  define NOEXCEPT noexcept
#else
#  define NOEXCEPT
#endif

using rlf_filefn::t_filename;
using std::string;
using boost::filesystem::path;


namespace rlf_ftw {

   class tExcludeFolders;
   class tIncludeFiles;

   /*! bad_text_read
   \param [in] msg  Message
   */
   class bad_ftw: public  std::runtime_error {
   public:
      bad_ftw( string const& msg )
         :  std::runtime_error( msg ) { }
   };

   class ftw_impl;

   class ftw {
		 std::unique_ptr<ftw_impl> _impl;

      ftw( ftw const&);
      void operator=(ftw const&);

   public:
      ftw();
      ftw( std::string const& p );

      ~ftw();
			void path(std::string const& p) ;
      std::vector <t_filename > const& files()const;
      std::vector <t_filename > const& folders()const ;

      // set inlude/exclude pattern
      // file pattern is simply searched in name of file, no wildcards
      void set_include_files( tIncludeFiles const& i );
      void set_exclude_folders( tExcludeFolders const& i );

      // do the real scan
      void scan_folders() ;

   };



} //namespace


#endif // rlf_ftw
